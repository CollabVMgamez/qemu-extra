/*
 * NVIDIA GeForce GTX 580 (GF110, Fermi)
 * Auto-generated stub — see nvidia-gtx750ti.c for full documentation.
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define GPU_VENDOR_ID    0x10DE
#define GPU_DEVICE_ID    0x1080
#define GPU_SUBSYS_VID   0x10DE
#define GPU_SUBSYS_DID   0x0854
#define GPU_REVISION     0xA1
#define GPU_CLASS        0x0300

#define NV_BAR0_SIZE (16  * MiB)
#define NV_BAR1_SIZE (256 * MiB)
#define NV_BAR3_SIZE (32  * MiB)
#define NV_BAR5_SIZE (128 * KiB)

#define NV_PMC_BOOT_0       0x000000
#define NV_PMC_BOOT_1       0x000004
#define NV_PMC_INTR_0       0x000100
#define NV_PMC_INTR_EN_0    0x000140
#define NV_PBUS_INTR_0      0x001100
#define NV_PFIFO_INTR_0     0x002100
#define NV_PTIMER_NUMERATOR  0x009200
#define NV_PTIMER_DENOMINATOR 0x009210
#define NV_PTIMER_TIME_0    0x009400
#define NV_PTIMER_TIME_1    0x009410
#define NV_PFB_BOOT_0       0x100800
#define NV_PFB_PARTS        0x100200
#define NV_PFB_REFCTRL      0x100210
#define NV_PFB_MEM_PARTS    0x11020C
#define NV_PGRAPH_STATUS    0x400700
#define NV_PDISP_INTR_0     0x610020

#define GPU_PMC_BOOT_0   0x0C8000A1
#define GPU_PFB_BOOT_0   0x00000013
#define GPU_PFB_PARTS    0x00000006
#define GPU_PFB_REFCTRL  0x00000053
#define GPU_CLK_BASE     772
#define GPU_CLK_BOOST    772
#define GPU_PCIE_GEN     2

#define TYPE_NVIDIA_GTX580 "nvidia-gtx580"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaGtx580State, NVIDIA_GTX580)

struct NvidiaGtx580State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en, pfifo_intr_en;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
};

static uint32_t gpu_get_clock(NvidiaGtx580State *s)
{
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        uint32_t r = GPU_CLK_BOOST - GPU_CLK_BASE;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (r + 1));
    }
    return s->clock_mhz;
}

static uint64_t gpu_bar0_read(void *opaque, hwaddr addr, unsigned size)
{
    NvidiaGtx580State *s = opaque;
    uint32_t mhz; uint64_t ns;
    switch (addr & ~3ULL) {
    case NV_PMC_BOOT_0:         return GPU_PMC_BOOT_0;
    case NV_PMC_BOOT_1:         return 0x00000001;
    case NV_PMC_INTR_0:
    case NV_PMC_INTR_EN_0:
    case NV_PBUS_INTR_0:
    case NV_PFIFO_INTR_0:       return 0;
    case NV_PTIMER_NUMERATOR:   return 31;
    case NV_PTIMER_DENOMINATOR: return 3;
    case NV_PTIMER_TIME_0:
        mhz = gpu_get_clock(s); ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return (uint32_t)((ns * mhz / 1000ULL) & 0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1:
        mhz = gpu_get_clock(s); ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return (uint32_t)(((ns * mhz / 1000ULL) >> 32) & 0xFFFFFFFFULL);
    case NV_PFB_BOOT_0:         return GPU_PFB_BOOT_0;
    case NV_PFB_PARTS:
    case NV_PFB_MEM_PARTS:      return GPU_PFB_PARTS;
    case NV_PFB_REFCTRL:        return GPU_PFB_REFCTRL;
    case NV_PGRAPH_STATUS:
    case NV_PDISP_INTR_0:       return 0;
    default:                    return 0;
    }
}
static void gpu_bar0_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    NvidiaGtx580State *s = opaque;
    switch (addr & ~3ULL) {
    case NV_PMC_INTR_EN_0: s->intr_en       = (uint32_t)val; break;
    case NV_PFIFO_INTR_0:  s->pfifo_intr_en = (uint32_t)val; break;
    default: break;
    }
}
static const MemoryRegionOps bar0_ops = {
    .read = gpu_bar0_read, .write = gpu_bar0_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};
static uint64_t stub_r(void *o, hwaddr a, unsigned s) { return 0; }
static void     stub_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps bar1_ops = {
    .read = stub_r, .write = stub_w, .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 8 },
};
static const MemoryRegionOps bar35_ops = {
    .read = stub_r, .write = stub_w, .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

#define MSI_CAP  0x50
#define PCIE_CAP 0x60
static void gpu_setup_caps(PCIDevice *pdev)
{
    uint8_t *c = pdev->config;
    c[PCI_CAPABILITY_LIST] = MSI_CAP;
    c[PCI_STATUS] |= PCI_STATUS_CAP_LIST;
    c[MSI_CAP+0]=0x05; c[MSI_CAP+1]=PCIE_CAP; c[MSI_CAP+2]=0x01; c[MSI_CAP+3]=0x00;
    c[PCIE_CAP+0]=0x10; c[PCIE_CAP+1]=0x00;
    pci_set_word(c+PCIE_CAP+2,  0x0002);
    pci_set_long(c+PCIE_CAP+4,  0x00008000 | GPU_PCIE_GEN);
    pci_set_word(c+PCIE_CAP+8,  0x0000);
    pci_set_word(c+PCIE_CAP+10, 0x0000);
    pci_set_long(c+PCIE_CAP+12, (0x10<<4) | GPU_PCIE_GEN);
    pci_set_word(c+PCIE_CAP+16, 0x0000);
    pci_set_word(c+PCIE_CAP+18, (0x10<<4) | GPU_PCIE_GEN | (1<<13));
}

static void gpu_realize(PCIDevice *pdev, Error **errp)
{
    NvidiaGtx580State *s = (NvidiaGtx580State *)pdev;
    s->clock_mhz = GPU_CLK_BASE; s->clock_last_ns = 0;
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config+PCI_SUBSYSTEM_VENDOR_ID, GPU_SUBSYS_VID);
    pci_set_word(pdev->config+PCI_SUBSYSTEM_ID,        GPU_SUBSYS_DID);
    gpu_setup_caps(pdev);
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0_ops,  s, "nvidia-gtx580-mmio", NV_BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar0);
    memory_region_init_io(&s->bar1, OBJECT(s), &bar1_ops,  s, "nvidia-gtx580-vram", NV_BAR1_SIZE);
    pci_register_bar(pdev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar1);
    memory_region_init_io(&s->bar3, OBJECT(s), &bar35_ops, s, "nvidia-gtx580-ramin",NV_BAR3_SIZE);
    pci_register_bar(pdev, 3, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar3);
    memory_region_init_io(&s->bar5, OBJECT(s), &bar35_ops, s, "nvidia-gtx580-vgaio",NV_BAR5_SIZE);
    pci_register_bar(pdev, 5, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->bar5);
}

static const VMStateDescription vmstate_nvidia_gtx580 = {
    .name = "nvidia-gtx580", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, NvidiaGtx580State),
        VMSTATE_UINT32(intr_en,       NvidiaGtx580State),
        VMSTATE_UINT32(pfifo_intr_en, NvidiaGtx580State),
        VMSTATE_UINT32(clock_mhz,     NvidiaGtx580State),
        VMSTATE_UINT64(clock_last_ns, NvidiaGtx580State),
        VMSTATE_END_OF_LIST()
    },
};

static void gpu_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass    *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k  = PCI_DEVICE_CLASS(klass);
    k->realize   = gpu_realize;
    k->vendor_id = GPU_VENDOR_ID;
    k->device_id = 0x1080;
    k->class_id  = GPU_CLASS;
    k->revision  = GPU_REVISION;
    dc->desc         = "NVIDIA GeForce GTX 580 (GF110, Fermi)";
    dc->vmsd         = &vmstate_nvidia_gtx580;
    dc->hotpluggable = false;
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo nvidia_gtx580_info = {
    .name          = TYPE_NVIDIA_GTX580,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NvidiaGtx580State),
    .class_init    = gpu_class_init,
    .interfaces    = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};
static void nvidia_gtx580_register(void) { type_register_static(&nvidia_gtx580_info); }
type_init(nvidia_gtx580_register)
