/*
 * Cerebras WSE-1 (Wafer Scale Engine 1, 1.2 trillion transistors, 18GB SRAM,
 * 400,000 AI cores, PCIe gen3 x16, 2019)
 * QEMU stub device for P4 fork.
 *
 * Cerebras does not publish official PCI IDs; this uses a plausible
 * allocation within the PCIe "Processing Accelerator" class (0x1200).
 * Vendor ID 0x1DA2 is assigned to Cerebras Systems.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define WSE_VENDOR_ID   0x1DA2
#define WSE_DEVICE_ID   0x0001
#define WSE_SUBSYS_VID  0x1DA2
#define WSE_SUBSYS_DID  0x0001
#define WSE_REVISION    0x01
/* PCI class 0x1200 = Processing Accelerator */
#define WSE_CLASS       0x1200
#define WSE_BAR0_SIZE   (16*MiB)
#define WSE_BAR2_SIZE   (256*MiB)
/* WSE-1: 1.05 GHz fabric clock, 18 GB on-chip SRAM (not HBM) */
#define WSE_CLK_BASE    1050
#define WSE_CLK_BOOST   1050
#define WSE_MEM_MHZ     1050  /* on-chip SRAM, same domain */
#define WSE_PCIE_GEN    3
#define TYPE_CEREBRAS_WSE1 "cerebras-wse-1"
OBJECT_DECLARE_SIMPLE_TYPE(CerebrasWse1State, CEREBRAS_WSE1)
struct CerebrasWse1State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar2;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    uint32_t chip_count;
    char *chip_name;
    char *board_partner;
};
static uint32_t wse_clk(CerebrasWse1State *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = WSE_CLK_BASE + (s->clock_mhz % (WSE_CLK_BOOST - WSE_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    CerebrasWse1State *s = opaque; uint32_t m; uint64_t n;
    switch (addr & ~3ULL) {
    /* Chip identification */
    case 0x000000: return 0xCE010001; /* Cerebras WSE gen1 */
    case 0x000004: return 0x00000001; /* chip present */
    /* Core count: 400,000 AI cores */
    case 0x000008: return 400000;
    /* SRAM: 18 GB expressed in KiB */
    case 0x00000C: return (uint32_t)(18ULL * 1024 * 1024);
    /* Fabric clock */
    case 0x000100: m=wse_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)((n*m/1000ULL)&0xFFFFFFFFULL);
    case 0x000104: m=wse_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFFULL);
    /* Temperature in millidegrees: WSE runs warm, 50-70C */
    case 0x000200: {
        uint64_t ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t seed = (uint32_t)(ns >> 28) * 1664525u + 1013904223u;
        return (50000 + (seed % 20000));
    }
    /* Memory clock (on-chip SRAM) */
    case 0x000300: return WSE_MEM_MHZ;
    /* Power: ~15 kW for the full wafer, we report in milliwatts */
    case 0x000400: {
        uint64_t ns2 = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t seed2 = (uint32_t)(ns2 >> 28) * 1664525u + 1013904223u;
        return (14000000u + (seed2 % 2000000u));
    }
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
static void wse_caps(PCIDevice *p) {
    uint8_t *c=p->config;
    c[PCI_CAPABILITY_LIST]=MC; c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;
    c[MC]=0x05; c[MC+1]=PC; c[MC+2]=0x01; c[MC+3]=0;
    c[PC]=0x10; c[PC+1]=0;
    pci_set_word(c+PC+2, 0x0002);
    pci_set_long(c+PC+4, 0x00008000|WSE_PCIE_GEN);
    pci_set_word(c+PC+8, 0); pci_set_word(c+PC+10, 0);
    pci_set_long(c+PC+12, (0x10<<4)|WSE_PCIE_GEN);
    pci_set_word(c+PC+16, 0);
    pci_set_word(c+PC+18, (0x10<<4)|WSE_PCIE_GEN|(1<<13));
}
static void wse_realize(PCIDevice *p, Error **e) {
    CerebrasWse1State *s = (CerebrasWse1State *)p;
    s->clock_mhz = WSE_CLK_BASE; s->clock_last_ns = 0;
    p->config[PCI_CLASS_PROG] = 0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, WSE_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, WSE_SUBSYS_DID);
    wse_caps(p);
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0ops, s, "cerebras-wse-1-mmio", WSE_BAR0_SIZE);
    pci_register_bar(p, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar0);
    memory_region_init_io(&s->bar2, OBJECT(s), &bar2ops, s, "cerebras-wse-1-sram", WSE_BAR2_SIZE);
    pci_register_bar(p, 2, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar2);
}
static const VMStateDescription vms_cerebras_wse1 = {
    .name="cerebras-wse-1",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,CerebrasWse1State),VMSTATE_UINT32(clock_mhz,CerebrasWse1State),VMSTATE_UINT64(clock_last_ns,CerebrasWse1State),VMSTATE_END_OF_LIST()}
};
static const Property wse_props[] = {
    DEFINE_PROP_STRING("chip-name", CerebrasWse1State, chip_name),
    DEFINE_PROP_UINT32("chip-count", CerebrasWse1State, chip_count, 1),
    DEFINE_PROP_STRING("board-partner", CerebrasWse1State, board_partner),
};
static void wse_class_init(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k); PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = wse_realize; pc->vendor_id = WSE_VENDOR_ID; pc->device_id = WSE_DEVICE_ID;
    pc->class_id = WSE_CLASS; pc->revision = WSE_REVISION;
    dc->desc = "Cerebras WSE-1 (Wafer Scale Engine, 400K AI cores, 18GB SRAM, 2019)";
    dc->vmsd = &vms_cerebras_wse1; dc->hotpluggable = false;
    device_class_set_props(dc, wse_props);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}
static const TypeInfo ti_cerebras_wse1 = {
    .name=TYPE_CEREBRAS_WSE1,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(CerebrasWse1State),.class_init=wse_class_init,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}
};
static void reg_cerebras_wse1(void) { type_register_static(&ti_cerebras_wse1); }
type_init(reg_cerebras_wse1)
