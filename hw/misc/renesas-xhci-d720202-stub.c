/* Renesas uPD720202 USB 3.0 xHCI - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_RENESAS_XHCI_D720202 "renesas-xhci-d720202"
typedef struct { PCIDevice p; MemoryRegion m; } RenesasXhciD720202State;
DECLARE_INSTANCE_CHECKER(RenesasXhciD720202State, RENESAS_XHCI_D720202_STUB, TYPE_STUB_RENESAS_XHCI_D720202)
static uint64_t RENESAS_XHCI_D720202_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void RENESAS_XHCI_D720202_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps RENESAS_XHCI_D720202_ops={.read=RENESAS_XHCI_D720202_r,.write=RENESAS_XHCI_D720202_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void RENESAS_XHCI_D720202_realize(PCIDevice*p,Error**e){
    RenesasXhciD720202State*s=RENESAS_XHCI_D720202_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x1912);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x0015);
    memory_region_init_io(&s->m,OBJECT(s),&RENESAS_XHCI_D720202_ops,s,"renesas-xhci-d720202",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription RENESAS_XHCI_D720202_vms={.name="renesas-xhci-d720202",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,RenesasXhciD720202State),VMSTATE_END_OF_LIST()}};
static void RENESAS_XHCI_D720202_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=RENESAS_XHCI_D720202_realize;pc->vendor_id=0x1912;pc->device_id=0x0015;
    pc->class_id=0x0C0330>>8;pc->revision=0x04;
    dc->desc="Renesas uPD720202 USB 3.0 xHCI";dc->vmsd=&RENESAS_XHCI_D720202_vms;dc->hotpluggable=false;
}
static const TypeInfo RENESAS_XHCI_D720202_ti={
    .name=TYPE_STUB_RENESAS_XHCI_D720202,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(RenesasXhciD720202State),.class_init=RENESAS_XHCI_D720202_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void RENESAS_XHCI_D720202_reg(void){type_register_static(&RENESAS_XHCI_D720202_ti);}
type_init(RENESAS_XHCI_D720202_reg)
