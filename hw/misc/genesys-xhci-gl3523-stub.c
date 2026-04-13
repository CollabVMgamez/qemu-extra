/* Genesys Logic GL3523 USB 3.1 Hub+xHCI - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_GENESYS_XHCI_GL3523 "genesys-xhci-gl3523"
typedef struct { PCIDevice p; MemoryRegion m; } GenesysXhciGl3523State;
DECLARE_INSTANCE_CHECKER(GenesysXhciGl3523State, GENESYS_XHCI_GL3523_STUB, TYPE_STUB_GENESYS_XHCI_GL3523)
static uint64_t GENESYS_XHCI_GL3523_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void GENESYS_XHCI_GL3523_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps GENESYS_XHCI_GL3523_ops={.read=GENESYS_XHCI_GL3523_r,.write=GENESYS_XHCI_GL3523_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void GENESYS_XHCI_GL3523_realize(PCIDevice*p,Error**e){
    GenesysXhciGl3523State*s=GENESYS_XHCI_GL3523_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x05E3);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x0626);
    memory_region_init_io(&s->m,OBJECT(s),&GENESYS_XHCI_GL3523_ops,s,"genesys-xhci-gl3523",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription GENESYS_XHCI_GL3523_vms={.name="genesys-xhci-gl3523",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,GenesysXhciGl3523State),VMSTATE_END_OF_LIST()}};
static void GENESYS_XHCI_GL3523_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=GENESYS_XHCI_GL3523_realize;pc->vendor_id=0x05E3;pc->device_id=0x0626;
    pc->class_id=0x0C0330>>8;pc->revision=0x00;
    dc->desc="Genesys Logic GL3523 USB 3.1 Hub+xHCI";dc->vmsd=&GENESYS_XHCI_GL3523_vms;dc->hotpluggable=false;
}
static const TypeInfo GENESYS_XHCI_GL3523_ti={
    .name=TYPE_STUB_GENESYS_XHCI_GL3523,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(GenesysXhciGl3523State),.class_init=GENESYS_XHCI_GL3523_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void GENESYS_XHCI_GL3523_reg(void){type_register_static(&GENESYS_XHCI_GL3523_ti);}
type_init(GENESYS_XHCI_GL3523_reg)
