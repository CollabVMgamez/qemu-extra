/* Fresco Logic FL1100 USB 3.0 xHCI - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_FRESCO_XHCI_FL1100 "fresco-xhci-fl1100"
typedef struct { PCIDevice p; MemoryRegion m; } FrescoXhciFl1100State;
DECLARE_INSTANCE_CHECKER(FrescoXhciFl1100State, FRESCO_XHCI_FL1100_STUB, TYPE_STUB_FRESCO_XHCI_FL1100)
static uint64_t FRESCO_XHCI_FL1100_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void FRESCO_XHCI_FL1100_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps FRESCO_XHCI_FL1100_ops={.read=FRESCO_XHCI_FL1100_r,.write=FRESCO_XHCI_FL1100_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void FRESCO_XHCI_FL1100_realize(PCIDevice*p,Error**e){
    FrescoXhciFl1100State*s=FRESCO_XHCI_FL1100_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x1B73);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x1100);
    memory_region_init_io(&s->m,OBJECT(s),&FRESCO_XHCI_FL1100_ops,s,"fresco-xhci-fl1100",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription FRESCO_XHCI_FL1100_vms={.name="fresco-xhci-fl1100",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,FrescoXhciFl1100State),VMSTATE_END_OF_LIST()}};
static void FRESCO_XHCI_FL1100_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=FRESCO_XHCI_FL1100_realize;pc->vendor_id=0x1B73;pc->device_id=0x1100;
    pc->class_id=0x0C0330>>8;pc->revision=0x10;
    dc->desc="Fresco Logic FL1100 USB 3.0 xHCI";dc->vmsd=&FRESCO_XHCI_FL1100_vms;dc->hotpluggable=false;
}
static const TypeInfo FRESCO_XHCI_FL1100_ti={
    .name=TYPE_STUB_FRESCO_XHCI_FL1100,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(FrescoXhciFl1100State),.class_init=FRESCO_XHCI_FL1100_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void FRESCO_XHCI_FL1100_reg(void){type_register_static(&FRESCO_XHCI_FL1100_ti);}
type_init(FRESCO_XHCI_FL1100_reg)
