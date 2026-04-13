/* Intel ICH7 EHCI USB 2.0 Controller - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_INTEL_EHCI_ICH7 "intel-ehci-ich7"
typedef struct { PCIDevice p; MemoryRegion m; } IntelEhciIch7State;
DECLARE_INSTANCE_CHECKER(IntelEhciIch7State, INTEL_EHCI_ICH7_STUB, TYPE_STUB_INTEL_EHCI_ICH7)
static uint64_t INTEL_EHCI_ICH7_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void INTEL_EHCI_ICH7_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps INTEL_EHCI_ICH7_ops={.read=INTEL_EHCI_ICH7_r,.write=INTEL_EHCI_ICH7_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void INTEL_EHCI_ICH7_realize(PCIDevice*p,Error**e){
    IntelEhciIch7State*s=INTEL_EHCI_ICH7_STUB(p);
    pci_config_set_class(p->config,0x0C0320>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0320)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x27CC);
    memory_region_init_io(&s->m,OBJECT(s),&INTEL_EHCI_ICH7_ops,s,"intel-ehci-ich7",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription INTEL_EHCI_ICH7_vms={.name="intel-ehci-ich7",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,IntelEhciIch7State),VMSTATE_END_OF_LIST()}};
static void INTEL_EHCI_ICH7_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_EHCI_ICH7_realize;pc->vendor_id=0x8086;pc->device_id=0x27CC;
    pc->class_id=0x0C0320>>8;pc->revision=0x01;
    dc->desc="Intel ICH7 EHCI USB 2.0 Controller";dc->vmsd=&INTEL_EHCI_ICH7_vms;dc->hotpluggable=false;
}
static const TypeInfo INTEL_EHCI_ICH7_ti={
    .name=TYPE_STUB_INTEL_EHCI_ICH7,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelEhciIch7State),.class_init=INTEL_EHCI_ICH7_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_EHCI_ICH7_reg(void){type_register_static(&INTEL_EHCI_ICH7_ti);}
type_init(INTEL_EHCI_ICH7_reg)
