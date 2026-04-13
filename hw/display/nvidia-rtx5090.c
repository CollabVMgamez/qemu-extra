/*
 * QEMU - NVIDIA GeForce RTX 5090 stub device (Blackwell GB202)
 *
 * Visible in GPU-Z:
 *   Name        : GeForce RTX 5090
 *   GPU         : GB202
 *   Revision    : A1
 *   VRAM        : 32768 MB GDDR7
 *   GPU Clock   : 2017-2407 MHz fluctuating (base/boost range)
 *   Mem Clock   : 1750 MHz (GDDR7 effective 28 GT/s)
 *   Bus         : PCIe x16 5.0
 *
 * PCI identifiers:
 *   Vendor  : 0x10DE  (NVIDIA Corporation)
 *   Device  : 0x2685  (GB202 [GeForce RTX 5090])
 *   Subsys  : 0x10DE:0x17F9
 *   Class   : 0x0300
 *   Rev     : 0xA1
 *
 * Usage:
 *   -device nvidia-rtx5090
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/log.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/pci/pci_ids.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

/* PCI IDs */
#define NVIDIA_VENDOR_ID         0x10DE
#define RTX5090_DEVICE_ID        0x2685   /* GB202 [GeForce RTX 5090] */
#define RTX5090_SUBSYS_VENDOR    0x10DE
#define RTX5090_SUBSYS_DEVICE    0x17F9   /* RTX 5090 reference */
#define RTX5090_REVISION         0xA1     /* GB202-A1 */
#define RTX5090_CLASS            0x0300   /* VGA compatible */

/*
 * BAR sizes — BAR1 capped at 256MB for 32-bit Windows compatibility.
 * Real RTX 5090 has 32GB BAR1 but that requires 64-bit addressing.
 */
#define NV_BAR0_SIZE  (16  * MiB)
#define NV_BAR1_SIZE  (256 * MiB)
#define NV_BAR3_SIZE  (32  * MiB)
#define NV_BAR5_SIZE  (128 * KiB)

/* MMIO register offsets (shared NV register map) */
#define NV_PMC_BOOT_0        0x000000
#define NV_PMC_BOOT_1        0x000004
#define NV_PMC_INTR_0        0x000100
#define NV_PMC_INTR_EN_0     0x000140
#define NV_PBUS_INTR_0       0x001100
#define NV_PFIFO_INTR_0      0x002100
#define NV_PTIMER_INTR_0     0x009100
#define NV_PTIMER_INTR_EN_0  0x009140
#define NV_PTIMER_NUMERATOR  0x009200
#define NV_PTIMER_DENOMINATOR 0x009210
#define NV_PTIMER_TIME_0     0x009400
#define NV_PTIMER_TIME_1     0x009410
#define NV_PFB_BOOT_0        0x100800
#define NV_PFB_PARTS         0x100200
#define NV_PFB_REFCTRL       0x100210
#define NV_PFB_MEM_PARTS     0x11020C
#define NV_PGRAPH_STATUS     0x400700
#define NV_PDISP_INTR_0      0x610020

/*
 * GB202 PMC_BOOT_0:
 *   [31:24] = 0x19  (Blackwell architecture)
 *   [23:20] = 0x2   (GB202 implementation)
 *   [7:0]   = 0xA1  (revision)
 */
#define GB202_PMC_BOOT_0     0x192000A1

/*
 * RTX 5090 clock range:
 *   Base:  2017 MHz
 *   Boost: 2407 MHz
 */
#define RTX5090_CLOCK_BASE_MHZ   2017
#define RTX5090_CLOCK_BOOST_MHZ  2407

/*
 * VRAM: 32GB GDDR7
 *   NV_PFB_BOOT_0 type field for GDDR7: value 0x04 (beyond GDDR5=0x03)
 *   GPU-Z may show "GDDR7" or "GDDR6X" depending on version;
 *   we use the GDDR7 type code reported by GB202.
 *   8 partitions × 4GB = 32GB
 */
#define GB202_PFB_BOOT_0     0x00000014   /* GDDR7, 128-bit per partition */
#define GB202_PFB_PARTS      0x00000008   /* 8 partitions = 32GB */
#define GB202_PFB_REFCTRL    0x800006D9   /* GDDR7 ~1750 MHz ref clock */

/* PCIe cap offsets */
#define MSI_CAP_OFFSET   0x50
#define PCIE_CAP_OFFSET  0x60

#define TYPE_NVIDIA_RTX5090 "nvidia-rtx5090"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaRTX5090State, NVIDIA_RTX5090)

struct NvidiaRTX5090State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en;
    uint32_t pfifo_intr_en;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    uint32_t gpu_count;
    char *gpu_name;
    char *board_partner;
};

/* Fluctuating clock between base and boost */
static uint32_t rtx5090_get_clock(NvidiaRTX5090State *s)
{
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        uint32_t r = RTX5090_CLOCK_BOOST_MHZ - RTX5090_CLOCK_BASE_MHZ;
        s->clock_mhz = RTX5090_CLOCK_BASE_MHZ + (s->clock_mhz % (r + 1));
    }
    return s->clock_mhz;
}

static uint64_t rtx5090_bar0_read(void *opaque, hwaddr addr, unsigned size)
{
    NvidiaRTX5090State *s = opaque;
    uint32_t mhz;
    uint64_t ns;

    switch (addr & ~3ULL) {
    case NV_PMC_BOOT_0:           return GB202_PMC_BOOT_0;
    case NV_PMC_BOOT_1:           return 0x00000001;
    case NV_PMC_INTR_0:
    case NV_PMC_INTR_EN_0:
    case NV_PBUS_INTR_0:
    case NV_PFIFO_INTR_0:
    case NV_PTIMER_INTR_0:
    case NV_PTIMER_INTR_EN_0:     return 0;
    case NV_PTIMER_NUMERATOR:     return 31;
    case NV_PTIMER_DENOMINATOR:   return 3;
    case NV_PTIMER_TIME_0:
        mhz = rtx5090_get_clock(s);
        ns  = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return (uint32_t)((ns * mhz / 1000ULL) & 0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1:
        mhz = rtx5090_get_clock(s);
        ns  = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return (uint32_t)(((ns * mhz / 1000ULL) >> 32) & 0xFFFFFFFFULL);
    case NV_PFB_BOOT_0:           return GB202_PFB_BOOT_0;
    case NV_PFB_PARTS:
    case NV_PFB_MEM_PARTS:        return GB202_PFB_PARTS;
    case NV_PFB_REFCTRL:          return GB202_PFB_REFCTRL;
    case NV_PGRAPH_STATUS:
    case NV_PDISP_INTR_0:         return 0;

    /* Temperature: GPU-Z reads NV_THERM_I2CS_SENSOR at 0x020050
     * bits[23:16] = raw ADC, formula: celsius = raw - 120
     * 60C idle: raw=180=0xB4 -> return 0x00B40000
     * We fluctuate 55-72C using the same LCG as the clock */
    case 0x020050: /* NV_THERM_I2CS_SENSOR */
    case 0x020400: /* NV_THERM_TEMP */
    case 0x020440: /* NV_THERM_TEMP_1 */ {
        uint32_t mhz2 = rtx5090_get_clock(s);
        uint32_t temp_c = 55 + ((mhz2 - RTX5090_CLOCK_BASE_MHZ) * 17) /
                          (RTX5090_CLOCK_BOOST_MHZ - RTX5090_CLOCK_BASE_MHZ + 1);
        uint32_t noise = (mhz2 * 6364136223846793005ULL + 1442695040888963407ULL) & 3;
        temp_c += noise;
        uint32_t raw = temp_c + 120;
        return (raw << 16);
    }
    case 0x132020: /* NV_CLK_MCLK_PLL_COEFF */ {
        uint32_t mhz3 = rtx5090_get_clock(s);
        uint32_t M = 2, P = 0;
        uint32_t N = (1750 * M) / 27;
        N += (mhz3 & 1);
        return (P << 16) | (N << 8) | M;
    }
    case 0x1373f0:
    case 0x00410C: return 1750;
    case 0x070090: {
        uint32_t mhz4 = rtx5090_get_clock(s);
        return 30 + ((mhz4 - RTX5090_CLOCK_BASE_MHZ) * 15) /
               (RTX5090_CLOCK_BOOST_MHZ - RTX5090_CLOCK_BASE_MHZ + 1);
    }
    default:                      return 0;
    }
}

static void rtx5090_bar0_write(void *opaque, hwaddr addr,
                               uint64_t val, unsigned size)
{
    NvidiaRTX5090State *s = opaque;
    switch (addr & ~3ULL) {
    case NV_PMC_INTR_EN_0: s->intr_en       = (uint32_t)val; break;
    case NV_PFIFO_INTR_0:  s->pfifo_intr_en = (uint32_t)val; break;
    default: break;
    }
}

static const MemoryRegionOps rtx5090_bar0_ops = {
    .read = rtx5090_bar0_read, .write = rtx5090_bar0_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static uint64_t stub_read(void *o, hwaddr a, unsigned s) { return 0; }
static void     stub_write(void *o, hwaddr a, uint64_t v, unsigned s) {}

static const MemoryRegionOps bar1_ops = {
    .read = stub_read, .write = stub_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 8 },
};
static const MemoryRegionOps bar35_ops = {
    .read = stub_read, .write = stub_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

/*
 * PCIe x16 Gen5 capability block.
 * GPU-Z reads Link Capabilities and Link Status to show bus interface.
 * Gen5 = link speed value 0x5 (32 GT/s), x16 width = 0x10.
 */
static void rtx5090_setup_caps(PCIDevice *pdev)
{
    uint8_t *c = pdev->config;

    c[PCI_CAPABILITY_LIST] = MSI_CAP_OFFSET;
    c[PCI_STATUS] |= PCI_STATUS_CAP_LIST;

    /* MSI 32-bit at 0x50 */
    c[MSI_CAP_OFFSET+0] = 0x05;
    c[MSI_CAP_OFFSET+1] = PCIE_CAP_OFFSET;
    c[MSI_CAP_OFFSET+2] = 0x01;
    c[MSI_CAP_OFFSET+3] = 0x00;

    /* PCIe Endpoint v2 at 0x60 */
    c[PCIE_CAP_OFFSET+0] = 0x10;
    c[PCIE_CAP_OFFSET+1] = 0x00;
    pci_set_word(c + PCIE_CAP_OFFSET + 2,  0x0002); /* v2, endpoint */
    pci_set_long(c + PCIE_CAP_OFFSET + 4,  0x00008005); /* dev caps: Gen5 */
    pci_set_word(c + PCIE_CAP_OFFSET + 8,  0x0000);
    pci_set_word(c + PCIE_CAP_OFFSET + 10, 0x0000);

    /*
     * Link Capabilities:
     *   [3:0]  = 0x5 (Gen5, 32 GT/s)
     *   [9:4]  = 0x10 (x16 max width)
     */
    pci_set_long(c + PCIE_CAP_OFFSET + 12,
                 (0x10 << 4) | 0x05);

    pci_set_word(c + PCIE_CAP_OFFSET + 16, 0x0000);

    /*
     * Link Status:
     *   [3:0]  = 0x5 (current speed Gen5)
     *   [9:4]  = 0x10 (negotiated width x16)
     *   [13]   = 1 (slot clock config)
     */
    pci_set_word(c + PCIE_CAP_OFFSET + 18,
                 (0x10 << 4) | 0x05 | (1 << 13));
}

static void rtx5090_realize(PCIDevice *pdev, Error **errp)
{
    NvidiaRTX5090State *s = NVIDIA_RTX5090(pdev);

    s->clock_mhz    = RTX5090_CLOCK_BASE_MHZ;
    s->clock_last_ns = 0;

    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, RTX5090_SUBSYS_VENDOR);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID,        RTX5090_SUBSYS_DEVICE);

    rtx5090_setup_caps(pdev);

    memory_region_init_io(&s->bar0, OBJECT(s), &rtx5090_bar0_ops, s,
                          "rtx5090-mmio", NV_BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar0);

    memory_region_init_io(&s->bar1, OBJECT(s), &bar1_ops, s,
                          "rtx5090-vram", NV_BAR1_SIZE);
    pci_register_bar(pdev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32 |
                     PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar1);

    memory_region_init_io(&s->bar3, OBJECT(s), &bar35_ops, s,
                          "rtx5090-ramin", NV_BAR3_SIZE);
    pci_register_bar(pdev, 3, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar3);

    memory_region_init_io(&s->bar5, OBJECT(s), &bar35_ops, s,
                          "rtx5090-vgaio", NV_BAR5_SIZE);
    pci_register_bar(pdev, 5, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->bar5);
}

static const VMStateDescription vmstate_rtx5090 = {
    .name = "nvidia-rtx5090", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, NvidiaRTX5090State),
        VMSTATE_UINT32(intr_en,        NvidiaRTX5090State),
        VMSTATE_UINT32(pfifo_intr_en,  NvidiaRTX5090State),
        VMSTATE_UINT32(clock_mhz,      NvidiaRTX5090State),
        VMSTATE_UINT32(gpu_count,       NvidiaRTX5090State            ),
        VMSTATE_UINT64(clock_last_ns,  NvidiaRTX5090State),
        VMSTATE_END_OF_LIST()
    },
};

static const Property gpu_multi_props_NvidiaRTX5090State[] = {
    DEFINE_PROP_STRING("gpu-name", NvidiaRTX5090State, gpu_name),
    DEFINE_PROP_UINT32("gpu-count", NvidiaRTX5090State, gpu_count, 1),
    DEFINE_PROP_STRING("board-partner", NvidiaRTX5090State, board_partner),
};
static void rtx5090_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass    *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k  = PCI_DEVICE_CLASS(klass);

    k->realize   = rtx5090_realize;
    k->vendor_id = NVIDIA_VENDOR_ID;
    k->device_id = RTX5090_DEVICE_ID;
    k->class_id  = RTX5090_CLASS;
    k->revision  = RTX5090_REVISION;

    dc->desc         = "NVIDIA GeForce RTX 5090 (GB202, Blackwell)";
    dc->vmsd         = &vmstate_rtx5090;
    dc->hotpluggable = false;
    device_class_set_props(dc, gpu_multi_props_NvidiaRTX5090State);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo rtx5090_info = {
    .name          = TYPE_NVIDIA_RTX5090,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NvidiaRTX5090State),
    .class_init    = rtx5090_class_init,
    .interfaces    = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};

static void rtx5090_register_types(void)
{ type_register_static(&rtx5090_info); }
type_init(rtx5090_register_types)
