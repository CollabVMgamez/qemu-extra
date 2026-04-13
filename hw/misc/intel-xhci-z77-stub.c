/*
 * Intel 7 Series xHCI USB 3.0 Controller
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_INTEL_XHCI_Z77 "intel-xhci-z77"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} IntelXhciZ77State;

DECLARE_INSTANCE_CHECKER(IntelXhciZ77State, INTEL_XHCI_Z77_STUB, TYPE_STUB_INTEL_XHCI_Z77)

static uint64_t INTEL_XHCI_Z77_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void INTEL_XHCI_Z77_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps INTEL_XHCI_Z77_ops = {
    .read=INTEL_XHCI_Z77_r,.write=INTEL_XHCI_Z77_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void INTEL_XHCI_Z77_realize(PCIDevice *p, Error **e) {
    IntelXhciZ77State *s = INTEL_XHCI_Z77_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0C0330) & 0xFF;
    pci_config_set_class(p->config, (0x0C0330) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x1E31);
    memory_region_init_io(&s->mmio,OBJECT(s),&INTEL_XHCI_Z77_ops,s,"intel-xhci-z77-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property INTEL_XHCI_Z77_props[] = {
    DEFINE_PROP_STRING("device-name", IntelXhciZ77State, device_name),
};
static const VMStateDescription INTEL_XHCI_Z77_vms = {
    .name="intel-xhci-z77",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelXhciZ77State),VMSTATE_END_OF_LIST()},
};
static void INTEL_XHCI_Z77_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_XHCI_Z77_realize; pc->vendor_id=0x8086; pc->device_id=0x1E31;
    pc->class_id=0x0C0330>>8; pc->revision=0x04;
    dc->desc="Intel 7 Series xHCI USB 3.0 Controller"; dc->vmsd=&INTEL_XHCI_Z77_vms; dc->hotpluggable=false;
    device_class_set_props(dc,INTEL_XHCI_Z77_props);
}
static const TypeInfo INTEL_XHCI_Z77_ti={
    .name=TYPE_STUB_INTEL_XHCI_Z77,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelXhciZ77State),.class_init=INTEL_XHCI_Z77_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_XHCI_Z77_reg(void){type_register_static(&INTEL_XHCI_Z77_ti);}
type_init(INTEL_XHCI_Z77_reg)
