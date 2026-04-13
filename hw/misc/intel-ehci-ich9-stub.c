/*
 * Intel ICH9 EHCI USB 2.0 Controller
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_INTEL_EHCI_ICH9 "intel-ehci-ich9"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} IntelEhciIch9State;

DECLARE_INSTANCE_CHECKER(IntelEhciIch9State, INTEL_EHCI_ICH9_STUB, TYPE_STUB_INTEL_EHCI_ICH9)

static uint64_t INTEL_EHCI_ICH9_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void INTEL_EHCI_ICH9_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps INTEL_EHCI_ICH9_ops = {
    .read=INTEL_EHCI_ICH9_r,.write=INTEL_EHCI_ICH9_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void INTEL_EHCI_ICH9_realize(PCIDevice *p, Error **e) {
    IntelEhciIch9State *s = INTEL_EHCI_ICH9_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0C0320) & 0xFF;
    pci_config_set_class(p->config, (0x0C0320) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x293A);
    memory_region_init_io(&s->mmio,OBJECT(s),&INTEL_EHCI_ICH9_ops,s,"intel-ehci-ich9-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property INTEL_EHCI_ICH9_props[] = {
    DEFINE_PROP_STRING("device-name", IntelEhciIch9State, device_name),
};
static const VMStateDescription INTEL_EHCI_ICH9_vms = {
    .name="intel-ehci-ich9",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelEhciIch9State),VMSTATE_END_OF_LIST()},
};
static void INTEL_EHCI_ICH9_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_EHCI_ICH9_realize; pc->vendor_id=0x8086; pc->device_id=0x293A;
    pc->class_id=0x0C0320>>8; pc->revision=0x03;
    dc->desc="Intel ICH9 EHCI USB 2.0 Controller"; dc->vmsd=&INTEL_EHCI_ICH9_vms; dc->hotpluggable=false;
    device_class_set_props(dc,INTEL_EHCI_ICH9_props);
}
static const TypeInfo INTEL_EHCI_ICH9_ti={
    .name=TYPE_STUB_INTEL_EHCI_ICH9,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelEhciIch9State),.class_init=INTEL_EHCI_ICH9_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_EHCI_ICH9_reg(void){type_register_static(&INTEL_EHCI_ICH9_ti);}
type_init(INTEL_EHCI_ICH9_reg)
