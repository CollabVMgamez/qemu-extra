/*
 * Google Cloud TPU v5e (16GB HBM per chip, PCIe gen4 x8, 2023, edge/inference)
 * QEMU stub device for P4 fork.
 *
 * Vendor: 0x1AE0 (Google)
 * Device: 0x0063 (Cloud TPU v5e)
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TPU_VENDOR_ID   0x1AE0
#define TPU_DEVICE_ID   0x0063
#define TPU_SUBSYS_VID  0x1AE0
#define TPU_SUBSYS_DID  0x0063
#define TPU_REVISION    0x01
#define TPU_CLASS       0x1200
#define TPU_BAR0_SIZE   (16*MiB)
#define TPU_BAR2_SIZE   (256*MiB)
#define TPU_CLK_BASE    1100
#define TPU_CLK_BOOST   1100
#define TPU_MEM_MHZ     1600
#define TPU_PCIE_GEN    4
#define TYPE_GOOGLE_TPU_V5E "google-tpu-v5e"
OBJECT_DECLARE_SIMPLE_TYPE(GoogleTpuV5eState, GOOGLE_TPU_V5E)
struct GoogleTpuV5eState {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar2;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    uint32_t chip_count;
    char *chip_name;
    char *board_partner;
};
static uint32_t tpu_clk(GoogleTpuV5eState *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = TPU_CLK_BASE + (s->clock_mhz % (TPU_CLK_BOOST - TPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    GoogleTpuV5eState *s = opaque; uint32_t m; uint64_t n;
    switch (addr & ~3ULL) {
    case 0x000000: return 0x00057E01; /* 5e variant */
    case 0x000004: return 0x00000001;
    case 0x000100: m=tpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)((n*m/1000ULL)&0xFFFFFFFFULL);
    case 0x000104: m=tpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFFULL);
    case 0x000200: {
        uint64_t ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t seed = (uint32_t)(ns >> 28) * 1664525u + 1013904223u;
        return (40000 + (seed % 10000));
    }
    case 0x000300: return TPU_MEM_MHZ;
    default: return 0;
    }
}
static void bar0_w(void *opaque, hwaddr addr, uint64_t val, unsigned size) {}
static const MemoryRegionOps bar0ops = {.read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t bar2_r(void *o, hwaddr a, unsigned s) { return 0; }
static void bar2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps bar2ops = {.read=bar2_r,.write=bar2_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
#define MC 0x50
#define PC 0x60
static void tpu_caps(PCIDevice *p) {
    uint8_t *c=p->config;
    c[PCI_CAPABILITY_LIST]=MC; c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;
    c[MC]=0x05; c[MC+1]=PC; c[MC+2]=0x01; c[MC+3]=0;
    c[PC]=0x10; c[PC+1]=0;
    pci_set_word(c+PC+2, 0x0002);
    pci_set_long(c+PC+4, 0x00008000|TPU_PCIE_GEN);
    pci_set_word(c+PC+8, 0); pci_set_word(c+PC+10, 0);
    pci_set_long(c+PC+12, (0x10<<4)|TPU_PCIE_GEN);
    pci_set_word(c+PC+16, 0);
    pci_set_word(c+PC+18, (0x10<<4)|TPU_PCIE_GEN|(1<<13));
}
static void tpu_realize(PCIDevice *p, Error **e) {
    GoogleTpuV5eState *s = (GoogleTpuV5eState *)p;
    s->clock_mhz = TPU_CLK_BASE; s->clock_last_ns = 0;
    p->config[PCI_CLASS_PROG] = 0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, TPU_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, TPU_SUBSYS_DID);
    tpu_caps(p);
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0ops, s, "google-tpu-v5e-mmio", TPU_BAR0_SIZE);
    pci_register_bar(p, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar0);
    memory_region_init_io(&s->bar2, OBJECT(s), &bar2ops, s, "google-tpu-v5e-hbm", TPU_BAR2_SIZE);
    pci_register_bar(p, 2, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar2);
}
static const VMStateDescription vms_google_tpu_v5e = {
    .name="google-tpu-v5e",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,GoogleTpuV5eState),VMSTATE_UINT32(clock_mhz,GoogleTpuV5eState),VMSTATE_UINT64(clock_last_ns,GoogleTpuV5eState),VMSTATE_END_OF_LIST()}
};
static const Property tpu_props[] = {
    DEFINE_PROP_STRING("chip-name", GoogleTpuV5eState, chip_name),
    DEFINE_PROP_UINT32("chip-count", GoogleTpuV5eState, chip_count, 1),
    DEFINE_PROP_STRING("board-partner", GoogleTpuV5eState, board_partner),
};
static void tpu_class_init(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k); PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = tpu_realize; pc->vendor_id = TPU_VENDOR_ID; pc->device_id = TPU_DEVICE_ID;
    pc->class_id = TPU_CLASS; pc->revision = TPU_REVISION;
    dc->desc = "Google Cloud TPU v5e (16GB HBM per chip, PCIe gen4, inference, 2023)";
    dc->vmsd = &vms_google_tpu_v5e; dc->hotpluggable = false;
    device_class_set_props(dc, tpu_props);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}
static const TypeInfo ti_google_tpu_v5e = {
    .name=TYPE_GOOGLE_TPU_V5E,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(GoogleTpuV5eState),.class_init=tpu_class_init,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}
};
static void reg_google_tpu_v5e(void) { type_register_static(&ti_google_tpu_v5e); }
type_init(reg_google_tpu_v5e)
