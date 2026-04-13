/* AMD B450 xHCI USB 3.1 Controller (Matisse) - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_AMD_XHCI_B450 "amd-xhci-b450"
typedef struct { PCIDevice p; MemoryRegion m; } AmdXhciB450State;
DECLARE_INSTANCE_CHECKER(AmdXhciB450State, AMD_XHCI_B450_STUB, TYPE_STUB_AMD_XHCI_B450)
static uint64_t AMD_XHCI_B450_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void AMD_XHCI_B450_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps AMD_XHCI_B450_ops={.read=AMD_XHCI_B450_r,.write=AMD_XHCI_B450_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void AMD_XHCI_B450_realize(PCIDevice*p,Error**e){
    AmdXhciB450State*s=AMD_XHCI_B450_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x1022);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x43BB);
    memory_region_init_io(&s->m,OBJECT(s),&AMD_XHCI_B450_ops,s,"amd-xhci-b450",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription AMD_XHCI_B450_vms={.name="amd-xhci-b450",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,AmdXhciB450State),VMSTATE_END_OF_LIST()}};
static void AMD_XHCI_B450_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=AMD_XHCI_B450_realize;pc->vendor_id=0x1022;pc->device_id=0x43BB;
    pc->class_id=0x0C0330>>8;pc->revision=0x02;
    dc->desc="AMD B450 xHCI USB 3.1 Controller (Matisse)";dc->vmsd=&AMD_XHCI_B450_vms;dc->hotpluggable=false;
}
static const TypeInfo AMD_XHCI_B450_ti={
    .name=TYPE_STUB_AMD_XHCI_B450,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(AmdXhciB450State),.class_init=AMD_XHCI_B450_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void AMD_XHCI_B450_reg(void){type_register_static(&AMD_XHCI_B450_ti);}
type_init(AMD_XHCI_B450_reg)
