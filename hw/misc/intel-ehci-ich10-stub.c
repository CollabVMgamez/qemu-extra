/* Intel ICH10 EHCI USB 2.0 Controller - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_INTEL_EHCI_ICH10 "intel-ehci-ich10"
typedef struct { PCIDevice p; MemoryRegion m; } IntelEhciIch10State;
DECLARE_INSTANCE_CHECKER(IntelEhciIch10State, INTEL_EHCI_ICH10_STUB, TYPE_STUB_INTEL_EHCI_ICH10)
static uint64_t INTEL_EHCI_ICH10_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void INTEL_EHCI_ICH10_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps INTEL_EHCI_ICH10_ops={.read=INTEL_EHCI_ICH10_r,.write=INTEL_EHCI_ICH10_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void INTEL_EHCI_ICH10_realize(PCIDevice*p,Error**e){
    IntelEhciIch10State*s=INTEL_EHCI_ICH10_STUB(p);
    pci_config_set_class(p->config,0x0C0320>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0320)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x3A3A);
    memory_region_init_io(&s->m,OBJECT(s),&INTEL_EHCI_ICH10_ops,s,"intel-ehci-ich10",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription INTEL_EHCI_ICH10_vms={.name="intel-ehci-ich10",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,IntelEhciIch10State),VMSTATE_END_OF_LIST()}};
static void INTEL_EHCI_ICH10_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_EHCI_ICH10_realize;pc->vendor_id=0x8086;pc->device_id=0x3A3A;
    pc->class_id=0x0C0320>>8;pc->revision=0x02;
    dc->desc="Intel ICH10 EHCI USB 2.0 Controller";dc->vmsd=&INTEL_EHCI_ICH10_vms;dc->hotpluggable=false;
}
static const TypeInfo INTEL_EHCI_ICH10_ti={
    .name=TYPE_STUB_INTEL_EHCI_ICH10,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelEhciIch10State),.class_init=INTEL_EHCI_ICH10_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_EHCI_ICH10_reg(void){type_register_static(&INTEL_EHCI_ICH10_ti);}
type_init(INTEL_EHCI_ICH10_reg)
