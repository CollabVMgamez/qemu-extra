/*
 * QEMU - NVIDIA GeForce GTX 750 Ti stub device (Maxwell GM107)
 *
 * Visible in GPU-Z:
 *   Name        : GeForce GTX 750 Ti
 *   GPU         : GM107
 *   Revision    : A2
 *   VRAM        : 2048 MB (type selectable: DDR1/DDR2/DDR3/DDR4/GDDR5)
 *   GPU Clock   : 1020-1085 MHz fluctuating
 *   Bus         : PCIe x16 2.0
 *
 * QEMU property "vram-type" selects memory type reported to GPU-Z:
 *   0 = GDDR5  (default, correct for real GTX 750 Ti)
 *   1 = DDR1
 *   2 = DDR2
 *   3 = DDR3
 *   4 = DDR4
 *
 * Usage:
 *   -device nvidia-gtx750ti                   (GDDR5, default)
 *   -device nvidia-gtx750ti,vram-type=3       (DDR3)
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
#define NVIDIA_VENDOR_ID        0x10DE
#define GTX750TI_DEVICE_ID      0x1380
#define GTX750TI_SUBSYS_VENDOR  0x10DE
#define GTX750TI_SUBSYS_DEVICE  0x1073
#define GTX750TI_REVISION       0xA2
#define GTX750TI_CLASS          0x0300

/* BAR sizes — BAR1 capped at 256MB for 32-bit Windows */
#define NV_BAR0_SIZE  (16  * MiB)
#define NV_BAR1_SIZE  (256 * MiB)
#define NV_BAR3_SIZE  (32  * MiB)
#define NV_BAR5_SIZE  (128 * KiB)

/* MMIO register offsets */
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

/* GM107 chip ID */
#define GM107_PMC_BOOT_0     0x118000A2

/* GPU clock range */
#define GTX750TI_CLOCK_BASE_MHZ   1020
#define GTX750TI_CLOCK_BOOST_MHZ  1085

/* vram-type property values */
#define VRAM_TYPE_GDDR5  0
#define VRAM_TYPE_DDR1   1
#define VRAM_TYPE_DDR2   2
#define VRAM_TYPE_DDR3   3
#define VRAM_TYPE_DDR4   4

/*
 * Per-type NV_PFB_BOOT_0 and NV_PFB_REFCTRL values.
 * GPU-Z reads PFB_BOOT_0[1:0] for memory type, then uses REFCTRL
 * upper byte to distinguish subtypes (GDDR5 vs DDR4 vs DDR3 etc).
 */
static const struct {
    uint32_t pfb_boot0;
    uint32_t refctrl;
} vram_type_regs[] = {
    [VRAM_TYPE_GDDR5] = { 0x00000013, 0x80000053 },
    [VRAM_TYPE_DDR1]  = { 0x00000010, 0x00000019 },
    [VRAM_TYPE_DDR2]  = { 0x00000011, 0x0000002A },
    [VRAM_TYPE_DDR3]  = { 0x00000012, 0x00000043 },
    [VRAM_TYPE_DDR4]  = { 0x00000013, 0x40000053 },
};

/* PCIe cap offsets */
#define MSI_CAP_OFFSET   0x50
#define PCIE_CAP_OFFSET  0x60

#define TYPE_NVIDIA_GTX750TI "nvidia-gtx750ti"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaGTX750TiState, NVIDIA_GTX750TI)

struct NvidiaGTX750TiState {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en;
    uint32_t pfifo_intr_en;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    uint32_t vram_type;
    uint32_t gpu_count;
};

static uint32_t gtx750ti_get_clock(NvidiaGTX750TiState *s)
{
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        uint32_t r = GTX750TI_CLOCK_BOOST_MHZ - GTX750TI_CLOCK_BASE_MHZ;
        s->clock_mhz = GTX750TI_CLOCK_BASE_MHZ + (s->clock_mhz % (r + 1));
    }
    return s->clock_mhz;
}

static uint64_t gtx750ti_bar0_read(void *opaque, hwaddr addr, unsigned size)
{
    NvidiaGTX750TiState *s = opaque;
    uint32_t type = (s->vram_type <= VRAM_TYPE_DDR4) ? s->vram_type : VRAM_TYPE_GDDR5;
    uint32_t mhz;
    uint64_t ns;

    switch (addr & ~3ULL) {
    case NV_PMC_BOOT_0:          return GM107_PMC_BOOT_0;
    case NV_PMC_BOOT_1:          return 0x00000001;
    case NV_PMC_INTR_0:
    case NV_PMC_INTR_EN_0:
    case NV_PBUS_INTR_0:
    case NV_PFIFO_INTR_0:
    case NV_PTIMER_INTR_0:
    case NV_PTIMER_INTR_EN_0:    return 0;
    case NV_PTIMER_NUMERATOR:    return 31;
    case NV_PTIMER_DENOMINATOR:  return 3;
    case NV_PTIMER_TIME_0:
        mhz = gtx750ti_get_clock(s);
        ns  = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return (uint32_t)((ns * mhz / 1000ULL) & 0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1:
        mhz = gtx750ti_get_clock(s);
        ns  = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return (uint32_t)(((ns * mhz / 1000ULL) >> 32) & 0xFFFFFFFFULL);
    case NV_PFB_BOOT_0:          return vram_type_regs[type].pfb_boot0;
    case NV_PFB_PARTS:
    case NV_PFB_MEM_PARTS:       return 4; /* 4 x 512MB = 2048MB */
    case NV_PFB_REFCTRL:         return vram_type_regs[type].refctrl;
    case NV_PGRAPH_STATUS:
    case NV_PDISP_INTR_0:        return 0;

    /* Temperature: bits[23:16] = raw ADC, celsius = raw - 120 */
    case 0x020050: /* NV_THERM_I2CS_SENSOR */
    case 0x020400: /* NV_THERM_TEMP */
    case 0x020440: /* NV_THERM_TEMP_1 */ {
        uint32_t tmhz = gtx750ti_get_clock(s);
        uint32_t temp_c = 55 + ((tmhz - GTX750TI_CLOCK_BASE_MHZ) * 17) /
                          (GTX750TI_CLOCK_BOOST_MHZ - GTX750TI_CLOCK_BASE_MHZ + 1);
        temp_c += (tmhz & 3);  /* ±3C noise */
        return ((temp_c + 120) << 16);
    }
    /* Memory clock PLL: mem_mhz = (27 * N) / (M * (1<<P)) */
    case 0x132020: /* NV_CLK_MCLK_PLL_COEFF */ {
        uint32_t tmhz2 = gtx750ti_get_clock(s);
        /* GTX 750 Ti GDDR5: ~1350 MHz -> N=100, M=2, P=0 */
        uint32_t N = 100 + (tmhz2 & 1);  /* slight fluctuation */
        return (0 << 16) | (N << 8) | 2;
    }
    case 0x1373f0: /* NV_CLK_MCLK_OUT */
    case 0x00410C: /* NV_PERF_MEM_CLK */
        return 1350;
    /* Fan: 30-45% based on load */
    case 0x070090: /* NV_PFAN_PERCENT */ {
        uint32_t tmhz3 = gtx750ti_get_clock(s);
        return 30 + ((tmhz3 - GTX750TI_CLOCK_BASE_MHZ) * 15) /
               (GTX750TI_CLOCK_BOOST_MHZ - GTX750TI_CLOCK_BASE_MHZ + 1);
    }
    default:                     return 0;
    }
}

static void gtx750ti_bar0_write(void *opaque, hwaddr addr,
                                uint64_t val, unsigned size)
{
    NvidiaGTX750TiState *s = opaque;
    switch (addr & ~3ULL) {
    case NV_PMC_INTR_EN_0: s->intr_en       = (uint32_t)val; break;
    case NV_PFIFO_INTR_0:  s->pfifo_intr_en = (uint32_t)val; break;
    default: break;
    }
}

static const MemoryRegionOps gtx750ti_bar0_ops = {
    .read = gtx750ti_bar0_read, .write = gtx750ti_bar0_write,
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

static void gtx750ti_setup_caps(PCIDevice *pdev)
{
    uint8_t *c = pdev->config;
    c[PCI_CAPABILITY_LIST] = MSI_CAP_OFFSET;
    c[PCI_STATUS] |= PCI_STATUS_CAP_LIST;

    /* MSI 32-bit at 0x50 */
    c[MSI_CAP_OFFSET+0]=0x05; c[MSI_CAP_OFFSET+1]=PCIE_CAP_OFFSET;
    c[MSI_CAP_OFFSET+2]=0x01; c[MSI_CAP_OFFSET+3]=0x00;

    /* PCIe Endpoint v2 at 0x60 */
    c[PCIE_CAP_OFFSET+0]=0x10; c[PCIE_CAP_OFFSET+1]=0x00;
    pci_set_word(c+PCIE_CAP_OFFSET+2,  0x0002);
    pci_set_long(c+PCIE_CAP_OFFSET+4,  0x00008002);
    pci_set_word(c+PCIE_CAP_OFFSET+8,  0x0000);
    pci_set_word(c+PCIE_CAP_OFFSET+10, 0x0000);
    /* Link Caps: x16 max, Gen2 */
    pci_set_long(c+PCIE_CAP_OFFSET+12, (0x10<<4)|0x02);
    /* Link Ctrl: 0 */
    pci_set_word(c+PCIE_CAP_OFFSET+16, 0x0000);
    /* Link Status: negotiated x16, Gen2, slot clock */
    pci_set_word(c+PCIE_CAP_OFFSET+18, (0x10<<4)|0x02|(1<<13));
}

static void gtx750ti_realize(PCIDevice *pdev, Error **errp)
{
    NvidiaGTX750TiState *s = NVIDIA_GTX750TI(pdev);
    s->clock_mhz = GTX750TI_CLOCK_BASE_MHZ;
    s->clock_last_ns = 0;
    if (s->vram_type > VRAM_TYPE_DDR4) s->vram_type = VRAM_TYPE_GDDR5;

    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config+PCI_SUBSYSTEM_VENDOR_ID, GTX750TI_SUBSYS_VENDOR);
    pci_set_word(pdev->config+PCI_SUBSYSTEM_ID,        GTX750TI_SUBSYS_DEVICE);
    gtx750ti_setup_caps(pdev);

    memory_region_init_io(&s->bar0, OBJECT(s), &gtx750ti_bar0_ops, s, "gtx750ti-mmio", NV_BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar0);

    memory_region_init_io(&s->bar1, OBJECT(s), &bar1_ops, s, "gtx750ti-vram", NV_BAR1_SIZE);
    pci_register_bar(pdev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar1);

    memory_region_init_io(&s->bar3, OBJECT(s), &bar35_ops, s, "gtx750ti-ramin", NV_BAR3_SIZE);
    pci_register_bar(pdev, 3, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar3);

    memory_region_init_io(&s->bar5, OBJECT(s), &bar35_ops, s, "gtx750ti-vgaio", NV_BAR5_SIZE);
    pci_register_bar(pdev, 5, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->bar5);
}

static const Property gtx750ti_properties[] = {
    DEFINE_PROP_UINT32("vram-type", NvidiaGTX750TiState, vram_type, VRAM_TYPE_GDDR5),
};

static const VMStateDescription vmstate_gtx750ti = {
    .name = "nvidia-gtx750ti", .version_id = 2, .minimum_version_id = 2,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, NvidiaGTX750TiState),
        VMSTATE_UINT32(intr_en,       NvidiaGTX750TiState),
        VMSTATE_UINT32(pfifo_intr_en, NvidiaGTX750TiState),
        VMSTATE_UINT32(clock_mhz,     NvidiaGTX750TiState),
        VMSTATE_UINT32(gpu_count,       NvidiaGTX750TiState           ),
        VMSTATE_UINT64(clock_last_ns, NvidiaGTX750TiState),
        VMSTATE_UINT32(vram_type,     NvidiaGTX750TiState),
        VMSTATE_END_OF_LIST()
    },
};

static const Property gpu_multi_props_NvidiaGTX750TiState[] = {
    DEFINE_PROP_UINT32("gpu-count", NvidiaGTX750TiState, gpu_count, 1),
};
static void gtx750ti_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass    *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k  = PCI_DEVICE_CLASS(klass);
    k->realize   = gtx750ti_realize;
    k->vendor_id = NVIDIA_VENDOR_ID;
    k->device_id = GTX750TI_DEVICE_ID;
    k->class_id  = GTX750TI_CLASS;
    k->revision  = GTX750TI_REVISION;
    dc->desc         = "NVIDIA GeForce GTX 750 Ti (GM107, Maxwell)";
    dc->vmsd         = &vmstate_gtx750ti;
    dc->hotpluggable = false;
    device_class_set_props(dc, gpu_multi_props_NvidiaGTX750TiState);
    device_class_set_props(dc, gtx750ti_properties);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo gtx750ti_info = {
    .name          = TYPE_NVIDIA_GTX750TI,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NvidiaGTX750TiState),
    .class_init    = gtx750ti_class_init,
    .interfaces    = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};

static void gtx750ti_register_types(void)
{ type_register_static(&gtx750ti_info); }
type_init(gtx750ti_register_types)
