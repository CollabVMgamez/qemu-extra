/* Intel ICH5 OHCI USB 1.1 Controller - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_INTEL_OHCI_ICH5 "intel-ohci-ich5"
typedef struct { PCIDevice p; MemoryRegion m; } IntelOhciIch5State;
DECLARE_INSTANCE_CHECKER(IntelOhciIch5State, INTEL_OHCI_ICH5_STUB, TYPE_STUB_INTEL_OHCI_ICH5)
static uint64_t INTEL_OHCI_ICH5_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void INTEL_OHCI_ICH5_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps INTEL_OHCI_ICH5_ops={.read=INTEL_OHCI_ICH5_r,.write=INTEL_OHCI_ICH5_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void INTEL_OHCI_ICH5_realize(PCIDevice*p,Error**e){
    IntelOhciIch5State*s=INTEL_OHCI_ICH5_STUB(p);
    pci_config_set_class(p->config,0x0C0310>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0310)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x24D2);
    memory_region_init_io(&s->m,OBJECT(s),&INTEL_OHCI_ICH5_ops,s,"intel-ohci-ich5",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription INTEL_OHCI_ICH5_vms={.name="intel-ohci-ich5",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,IntelOhciIch5State),VMSTATE_END_OF_LIST()}};
static void INTEL_OHCI_ICH5_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_OHCI_ICH5_realize;pc->vendor_id=0x8086;pc->device_id=0x24D2;
    pc->class_id=0x0C0310>>8;pc->revision=0x02;
    dc->desc="Intel ICH5 OHCI USB 1.1 Controller";dc->vmsd=&INTEL_OHCI_ICH5_vms;dc->hotpluggable=false;
}
static const TypeInfo INTEL_OHCI_ICH5_ti={
    .name=TYPE_STUB_INTEL_OHCI_ICH5,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelOhciIch5State),.class_init=INTEL_OHCI_ICH5_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_OHCI_ICH5_reg(void){type_register_static(&INTEL_OHCI_ICH5_ti);}
type_init(INTEL_OHCI_ICH5_reg)
