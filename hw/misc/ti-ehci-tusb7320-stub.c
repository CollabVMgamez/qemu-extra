/* Texas Instruments TUSB7320 USB 3.0 xHCI - USB Controller Identity Stub */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define TYPE_STUB_TI_EHCI_TUSB7320 "ti-ehci-tusb7320"
typedef struct { PCIDevice p; MemoryRegion m; } TiEhciTusb7320State;
DECLARE_INSTANCE_CHECKER(TiEhciTusb7320State, TI_EHCI_TUSB7320_STUB, TYPE_STUB_TI_EHCI_TUSB7320)
static uint64_t TI_EHCI_TUSB7320_r(void*o,hwaddr a,unsigned s){return a?0:1;}
static void TI_EHCI_TUSB7320_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps TI_EHCI_TUSB7320_ops={.read=TI_EHCI_TUSB7320_r,.write=TI_EHCI_TUSB7320_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void TI_EHCI_TUSB7320_realize(PCIDevice*p,Error**e){
    TiEhciTusb7320State*s=TI_EHCI_TUSB7320_STUB(p);
    pci_config_set_class(p->config,0x0C0320>>8);
    p->config[PCI_CLASS_PROG]=(0x0C0320)&0xFF;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,0x104C);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,0x8241);
    memory_region_init_io(&s->m,OBJECT(s),&TI_EHCI_TUSB7320_ops,s,"ti-ehci-tusb7320",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->m);
}
static const VMStateDescription TI_EHCI_TUSB7320_vms={.name="ti-ehci-tusb7320",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(p,TiEhciTusb7320State),VMSTATE_END_OF_LIST()}};
static void TI_EHCI_TUSB7320_ci(ObjectClass*k,const void*d){
    DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);
    pc->realize=TI_EHCI_TUSB7320_realize;pc->vendor_id=0x104C;pc->device_id=0x8241;
    pc->class_id=0x0C0320>>8;pc->revision=0x02;
    dc->desc="Texas Instruments TUSB7320 USB 3.0 xHCI";dc->vmsd=&TI_EHCI_TUSB7320_vms;dc->hotpluggable=false;
}
static const TypeInfo TI_EHCI_TUSB7320_ti={
    .name=TYPE_STUB_TI_EHCI_TUSB7320,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(TiEhciTusb7320State),.class_init=TI_EHCI_TUSB7320_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void TI_EHCI_TUSB7320_reg(void){type_register_static(&TI_EHCI_TUSB7320_ti);}
type_init(TI_EHCI_TUSB7320_reg)
