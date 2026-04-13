/* Etron EJ168 USB 3.0 xHCI - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_ETRON_XHCI_EJ168 "etron-xhci-ej168"
typedef struct { PCIDevice p; MemoryRegion m; } EtronXhciEj168State;
DECLARE_INSTANCE_CHECKER(EtronXhciEj168State, ETRON_XHCI_EJ168_STUB, TYPE_STUB_ETRON_XHCI_EJ168)
static uint64_t ETRON_XHCI_EJ168_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void ETRON_XHCI_EJ168_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps ETRON_XHCI_EJ168_ops={.read=ETRON_XHCI_EJ168_r,.write=ETRON_XHCI_EJ168_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void ETRON_XHCI_EJ168_realize(PCIDevice*p,Error**e){
    EtronXhciEj168State*s=ETRON_XHCI_EJ168_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x1B6F);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x7023);
    memory_region_init_io(&s->m,OBJECT(s),&ETRON_XHCI_EJ168_ops,s,"etron-xhci-ej168",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription ETRON_XHCI_EJ168_vms={.name="etron-xhci-ej168",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,EtronXhciEj168State),VMSTATE_END_OF_LIST()}};
static void ETRON_XHCI_EJ168_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=ETRON_XHCI_EJ168_realize;pc->vendor_id=0x1B6F;pc->device_id=0x7023;
    pc->class_id=0x0C0330>>8;pc->revision=0x01;
    dc->desc="Etron EJ168 USB 3.0 xHCI";dc->vmsd=&ETRON_XHCI_EJ168_vms;dc->hotpluggable=false;
}
static const TypeInfo ETRON_XHCI_EJ168_ti={
    .name=TYPE_STUB_ETRON_XHCI_EJ168,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(EtronXhciEj168State),.class_init=ETRON_XHCI_EJ168_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void ETRON_XHCI_EJ168_reg(void){type_register_static(&ETRON_XHCI_EJ168_ti);}
type_init(ETRON_XHCI_EJ168_reg)
