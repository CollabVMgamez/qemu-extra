/*
 * NEC/Renesas uPD720200 USB 3.0 Host Controller
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_NEC_USB3 "nec-usb3"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} NecUsb3State;

DECLARE_INSTANCE_CHECKER(NecUsb3State, NEC_USB3_STUB, TYPE_STUB_NEC_USB3)

static uint64_t NEC_USB3_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void NEC_USB3_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps NEC_USB3_ops = {
    .read=NEC_USB3_r,.write=NEC_USB3_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void NEC_USB3_realize(PCIDevice *p, Error **e) {
    NecUsb3State *s = NEC_USB3_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0C0330) & 0xFF;
    pci_config_set_class(p->config, (0x0C0330) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x1033);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x0194);
    memory_region_init_io(&s->mmio,OBJECT(s),&NEC_USB3_ops,s,"nec-usb3-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property NEC_USB3_props[] = {
    DEFINE_PROP_STRING("device-name", NecUsb3State, device_name),
};
static const VMStateDescription NEC_USB3_vms = {
    .name="nec-usb3",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,NecUsb3State),VMSTATE_END_OF_LIST()},
};
static void NEC_USB3_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=NEC_USB3_realize; pc->vendor_id=0x1033; pc->device_id=0x0194;
    pc->class_id=0x0C0330>>8; pc->revision=0xA1;
    dc->desc="NEC/Renesas uPD720200 USB 3.0 Host Controller"; dc->vmsd=&NEC_USB3_vms; dc->hotpluggable=false;
    device_class_set_props(dc,NEC_USB3_props);
}
static const TypeInfo NEC_USB3_ti={
    .name=TYPE_STUB_NEC_USB3,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(NecUsb3State),.class_init=NEC_USB3_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void NEC_USB3_reg(void){type_register_static(&NEC_USB3_ti);}
type_init(NEC_USB3_reg)
