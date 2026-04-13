/* AMD B550 xHCI USB 3.2 Gen2x2 Controller - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_AMD_XHCI_B550 "amd-xhci-b550"
typedef struct { PCIDevice p; MemoryRegion m; } AmdXhciB550State;
DECLARE_INSTANCE_CHECKER(AmdXhciB550State, AMD_XHCI_B550_STUB, TYPE_STUB_AMD_XHCI_B550)
static uint64_t AMD_XHCI_B550_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void AMD_XHCI_B550_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps AMD_XHCI_B550_ops={.read=AMD_XHCI_B550_r,.write=AMD_XHCI_B550_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void AMD_XHCI_B550_realize(PCIDevice*p,Error**e){
    AmdXhciB550State*s=AMD_XHCI_B550_STUB(p);
    pci_config_set_class(p->config,0x0C0330>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0330)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x1022);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x43EE);
    memory_region_init_io(&s->m,OBJECT(s),&AMD_XHCI_B550_ops,s,"amd-xhci-b550",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription AMD_XHCI_B550_vms={.name="amd-xhci-b550",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,AmdXhciB550State),VMSTATE_END_OF_LIST()}};
static void AMD_XHCI_B550_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=AMD_XHCI_B550_realize;pc->vendor_id=0x1022;pc->device_id=0x43EE;
    pc->class_id=0x0C0330>>8;pc->revision=0x01;
    dc->desc="AMD B550 xHCI USB 3.2 Gen2x2 Controller";dc->vmsd=&AMD_XHCI_B550_vms;dc->hotpluggable=false;
}
static const TypeInfo AMD_XHCI_B550_ti={
    .name=TYPE_STUB_AMD_XHCI_B550,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(AmdXhciB550State),.class_init=AMD_XHCI_B550_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void AMD_XHCI_B550_reg(void){type_register_static(&AMD_XHCI_B550_ti);}
type_init(AMD_XHCI_B550_reg)
