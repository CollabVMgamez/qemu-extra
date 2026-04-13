/*
 * VIA VT6212L USB 2.0 EHCI Host Controller
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_VIA_USB2 "via-usb2"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} ViaUsb2State;

DECLARE_INSTANCE_CHECKER(ViaUsb2State, VIA_USB2_STUB, TYPE_STUB_VIA_USB2)

static uint64_t VIA_USB2_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void VIA_USB2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps VIA_USB2_ops = {
    .read=VIA_USB2_r,.write=VIA_USB2_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void VIA_USB2_realize(PCIDevice *p, Error **e) {
    ViaUsb2State *s = VIA_USB2_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0C0320) & 0xFF;
    pci_config_set_class(p->config, (0x0C0320) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x1106);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x3038);
    memory_region_init_io(&s->mmio,OBJECT(s),&VIA_USB2_ops,s,"via-usb2-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property VIA_USB2_props[] = {
    DEFINE_PROP_STRING("device-name", ViaUsb2State, device_name),
};
static const VMStateDescription VIA_USB2_vms = {
    .name="via-usb2",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,ViaUsb2State),VMSTATE_END_OF_LIST()},
};
static void VIA_USB2_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=VIA_USB2_realize; pc->vendor_id=0x1106; pc->device_id=0x3038;
    pc->class_id=0x0C0320>>8; pc->revision=0x86;
    dc->desc="VIA VT6212L USB 2.0 EHCI Host Controller"; dc->vmsd=&VIA_USB2_vms; dc->hotpluggable=false;
    device_class_set_props(dc,VIA_USB2_props);
}
static const TypeInfo VIA_USB2_ti={
    .name=TYPE_STUB_VIA_USB2,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(ViaUsb2State),.class_init=VIA_USB2_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void VIA_USB2_reg(void){type_register_static(&VIA_USB2_ti);}
type_init(VIA_USB2_reg)
