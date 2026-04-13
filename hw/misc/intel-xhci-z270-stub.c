/* Intel Z270 xHCI USB 3.1 Controller - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_INTEL_XHCI_Z270 "intel-xhci-z270"
typedef struct { PCIDevice p; MemoryRegion m; } IntelXhciZ270State;
DECLARE_INSTANCE_CHECKER(IntelXhciZ270State, INTEL_XHCI_Z270_STUB, TYPE_STUB_INTEL_XHCI_Z270)
static uint64_t INTEL_XHCI_Z270_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void INTEL_XHCI_Z270_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps INTEL_XHCI_Z270_ops={.read=INTEL_XHCI_Z270_r,.write=INTEL_XHCI_Z270_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void INTEL_XHCI_Z270_realize(PCIDevice*p,Error**e){
    IntelXhciZ270State*s=INTEL_XHCI_Z270_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0xA2AF);
    memory_region_init_io(&s->m,OBJECT(s),&INTEL_XHCI_Z270_ops,s,"intel-xhci-z270",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription INTEL_XHCI_Z270_vms={.name="intel-xhci-z270",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,IntelXhciZ270State),VMSTATE_END_OF_LIST()}};
static void INTEL_XHCI_Z270_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_XHCI_Z270_realize;pc->vendor_id=0x8086;pc->device_id=0xA2AF;
    pc->class_id=0x0C0330>>8;pc->revision=0x31;
    dc->desc="Intel Z270 xHCI USB 3.1 Controller";dc->vmsd=&INTEL_XHCI_Z270_vms;dc->hotpluggable=false;
}
static const TypeInfo INTEL_XHCI_Z270_ti={
    .name=TYPE_STUB_INTEL_XHCI_Z270,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelXhciZ270State),.class_init=INTEL_XHCI_Z270_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_XHCI_Z270_reg(void){type_register_static(&INTEL_XHCI_Z270_ti);}
type_init(INTEL_XHCI_Z270_reg)
