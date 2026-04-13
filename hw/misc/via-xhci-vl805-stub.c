/* VIA/VL805 USB 3.0 xHCI Host Controller - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_VIA_XHCI_VL805 "via-xhci-vl805"
typedef struct { PCIDevice p; MemoryRegion m; } ViaXhciVl805State;
DECLARE_INSTANCE_CHECKER(ViaXhciVl805State, VIA_XHCI_VL805_STUB, TYPE_STUB_VIA_XHCI_VL805)
static uint64_t VIA_XHCI_VL805_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void VIA_XHCI_VL805_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps VIA_XHCI_VL805_ops={.read=VIA_XHCI_VL805_r,.write=VIA_XHCI_VL805_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void VIA_XHCI_VL805_realize(PCIDevice*p,Error**e){
    ViaXhciVl805State*s=VIA_XHCI_VL805_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x1106);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x3483);
    memory_region_init_io(&s->m,OBJECT(s),&VIA_XHCI_VL805_ops,s,"via-xhci-vl805",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription VIA_XHCI_VL805_vms={.name="via-xhci-vl805",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,ViaXhciVl805State),VMSTATE_END_OF_LIST()}};
static void VIA_XHCI_VL805_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=VIA_XHCI_VL805_realize;pc->vendor_id=0x1106;pc->device_id=0x3483;
    pc->class_id=0x0C0330>>8;pc->revision=0x01;
    dc->desc="VIA/VL805 USB 3.0 xHCI Host Controller";dc->vmsd=&VIA_XHCI_VL805_vms;dc->hotpluggable=false;
}
static const TypeInfo VIA_XHCI_VL805_ti={
    .name=TYPE_STUB_VIA_XHCI_VL805,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(ViaXhciVl805State),.class_init=VIA_XHCI_VL805_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void VIA_XHCI_VL805_reg(void){type_register_static(&VIA_XHCI_VL805_ti);}
type_init(VIA_XHCI_VL805_reg)
