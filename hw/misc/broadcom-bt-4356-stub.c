/*
 * Broadcom BCM4356 Bluetooth 4.1
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_BROADCOM_BT_4356 "broadcom-bt-4356"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} BroadcomBt4356State;

DECLARE_INSTANCE_CHECKER(BroadcomBt4356State, BROADCOM_BT_4356_STUB, TYPE_STUB_BROADCOM_BT_4356)

static uint64_t BROADCOM_BT_4356_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void BROADCOM_BT_4356_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps BROADCOM_BT_4356_ops = {
    .read=BROADCOM_BT_4356_r,.write=BROADCOM_BT_4356_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void BROADCOM_BT_4356_realize(PCIDevice *p, Error **e) {
    BroadcomBt4356State *s = BROADCOM_BT_4356_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0DC000) & 0xFF;
    pci_config_set_class(p->config, (0x0DC000) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x0A5C);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x6459);
    memory_region_init_io(&s->mmio,OBJECT(s),&BROADCOM_BT_4356_ops,s,"broadcom-bt-4356-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property BROADCOM_BT_4356_props[] = {
    DEFINE_PROP_STRING("device-name", BroadcomBt4356State, device_name),
};
static const VMStateDescription BROADCOM_BT_4356_vms = {
    .name="broadcom-bt-4356",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,BroadcomBt4356State),VMSTATE_END_OF_LIST()},
};
static void BROADCOM_BT_4356_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=BROADCOM_BT_4356_realize; pc->vendor_id=0x0A5C; pc->device_id=0x6459;
    pc->class_id=0x0DC000>>8; pc->revision=0x01;
    dc->desc="Broadcom BCM4356 Bluetooth 4.1"; dc->vmsd=&BROADCOM_BT_4356_vms; dc->hotpluggable=false;
    device_class_set_props(dc,BROADCOM_BT_4356_props);
}
static const TypeInfo BROADCOM_BT_4356_ti={
    .name=TYPE_STUB_BROADCOM_BT_4356,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(BroadcomBt4356State),.class_init=BROADCOM_BT_4356_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void BROADCOM_BT_4356_reg(void){type_register_static(&BROADCOM_BT_4356_ti);}
type_init(BROADCOM_BT_4356_reg)
