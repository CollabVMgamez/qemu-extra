/*
 * Broadcom BCM4352 802.11ac WiFi
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_BROADCOM_WIFI_4352 "broadcom-wifi-4352"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} BroadcomWifi4352State;

DECLARE_INSTANCE_CHECKER(BroadcomWifi4352State, BROADCOM_WIFI_4352_STUB, TYPE_STUB_BROADCOM_WIFI_4352)

static uint64_t BROADCOM_WIFI_4352_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void BROADCOM_WIFI_4352_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps BROADCOM_WIFI_4352_ops = {
    .read=BROADCOM_WIFI_4352_r,.write=BROADCOM_WIFI_4352_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void BROADCOM_WIFI_4352_realize(PCIDevice *p, Error **e) {
    BroadcomWifi4352State *s = BROADCOM_WIFI_4352_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x028000) & 0xFF;
    pci_config_set_class(p->config, (0x028000) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x14E4);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x43B1);
    memory_region_init_io(&s->mmio,OBJECT(s),&BROADCOM_WIFI_4352_ops,s,"broadcom-wifi-4352-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property BROADCOM_WIFI_4352_props[] = {
    DEFINE_PROP_STRING("device-name", BroadcomWifi4352State, device_name),
};
static const VMStateDescription BROADCOM_WIFI_4352_vms = {
    .name="broadcom-wifi-4352",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,BroadcomWifi4352State),VMSTATE_END_OF_LIST()},
};
static void BROADCOM_WIFI_4352_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=BROADCOM_WIFI_4352_realize; pc->vendor_id=0x14E4; pc->device_id=0x43B1;
    pc->class_id=0x028000>>8; pc->revision=0x03;
    dc->desc="Broadcom BCM4352 802.11ac WiFi"; dc->vmsd=&BROADCOM_WIFI_4352_vms; dc->hotpluggable=false;
    device_class_set_props(dc,BROADCOM_WIFI_4352_props);
}
static const TypeInfo BROADCOM_WIFI_4352_ti={
    .name=TYPE_STUB_BROADCOM_WIFI_4352,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(BroadcomWifi4352State),.class_init=BROADCOM_WIFI_4352_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void BROADCOM_WIFI_4352_reg(void){type_register_static(&BROADCOM_WIFI_4352_ti);}
type_init(BROADCOM_WIFI_4352_reg)
