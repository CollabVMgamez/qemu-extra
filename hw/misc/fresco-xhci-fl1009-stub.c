/* Fresco Logic FL1009 USB 3.1 xHCI - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_FRESCO_XHCI_FL1009 "fresco-xhci-fl1009"
typedef struct { PCIDevice p; MemoryRegion m; } FrescoXhciFl1009State;
DECLARE_INSTANCE_CHECKER(FrescoXhciFl1009State, FRESCO_XHCI_FL1009_STUB, TYPE_STUB_FRESCO_XHCI_FL1009)
static uint64_t FRESCO_XHCI_FL1009_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void FRESCO_XHCI_FL1009_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps FRESCO_XHCI_FL1009_ops={.read=FRESCO_XHCI_FL1009_r,.write=FRESCO_XHCI_FL1009_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void FRESCO_XHCI_FL1009_realize(PCIDevice*p,Error**e){
    FrescoXhciFl1009State*s=FRESCO_XHCI_FL1009_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x1B73);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x1009);
    memory_region_init_io(&s->m,OBJECT(s),&FRESCO_XHCI_FL1009_ops,s,"fresco-xhci-fl1009",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription FRESCO_XHCI_FL1009_vms={.name="fresco-xhci-fl1009",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,FrescoXhciFl1009State),VMSTATE_END_OF_LIST()}};
static void FRESCO_XHCI_FL1009_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=FRESCO_XHCI_FL1009_realize;pc->vendor_id=0x1B73;pc->device_id=0x1009;
    pc->class_id=0x0C0330>>8;pc->revision=0x01;
    dc->desc="Fresco Logic FL1009 USB 3.1 xHCI";dc->vmsd=&FRESCO_XHCI_FL1009_vms;dc->hotpluggable=false;
}
static const TypeInfo FRESCO_XHCI_FL1009_ti={
    .name=TYPE_STUB_FRESCO_XHCI_FL1009,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(FrescoXhciFl1009State),.class_init=FRESCO_XHCI_FL1009_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void FRESCO_XHCI_FL1009_reg(void){type_register_static(&FRESCO_XHCI_FL1009_ti);}
type_init(FRESCO_XHCI_FL1009_reg)
