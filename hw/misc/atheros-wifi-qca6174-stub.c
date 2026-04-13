/*
 * Qualcomm Atheros QCA6174 802.11ac
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_ATHEROS_WIFI_QCA6174 "atheros-wifi-qca6174"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} AtherosWifiQca6174State;

DECLARE_INSTANCE_CHECKER(AtherosWifiQca6174State, ATHEROS_WIFI_QCA6174_STUB, TYPE_STUB_ATHEROS_WIFI_QCA6174)

static uint64_t ATHEROS_WIFI_QCA6174_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void ATHEROS_WIFI_QCA6174_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps ATHEROS_WIFI_QCA6174_ops = {
    .read=ATHEROS_WIFI_QCA6174_r,.write=ATHEROS_WIFI_QCA6174_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void ATHEROS_WIFI_QCA6174_realize(PCIDevice *p, Error **e) {
    AtherosWifiQca6174State *s = ATHEROS_WIFI_QCA6174_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x028000) & 0xFF;
    pci_config_set_class(p->config, (0x028000) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x168C);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x003E);
    memory_region_init_io(&s->mmio,OBJECT(s),&ATHEROS_WIFI_QCA6174_ops,s,"atheros-wifi-qca6174-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property ATHEROS_WIFI_QCA6174_props[] = {
    DEFINE_PROP_STRING("device-name", AtherosWifiQca6174State, device_name),
};
static const VMStateDescription ATHEROS_WIFI_QCA6174_vms = {
    .name="atheros-wifi-qca6174",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AtherosWifiQca6174State),VMSTATE_END_OF_LIST()},
};
static void ATHEROS_WIFI_QCA6174_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=ATHEROS_WIFI_QCA6174_realize; pc->vendor_id=0x168C; pc->device_id=0x003E;
    pc->class_id=0x028000>>8; pc->revision=0x32;
    dc->desc="Qualcomm Atheros QCA6174 802.11ac"; dc->vmsd=&ATHEROS_WIFI_QCA6174_vms; dc->hotpluggable=false;
    device_class_set_props(dc,ATHEROS_WIFI_QCA6174_props);
}
static const TypeInfo ATHEROS_WIFI_QCA6174_ti={
    .name=TYPE_STUB_ATHEROS_WIFI_QCA6174,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(AtherosWifiQca6174State),.class_init=ATHEROS_WIFI_QCA6174_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void ATHEROS_WIFI_QCA6174_reg(void){type_register_static(&ATHEROS_WIFI_QCA6174_ti);}
type_init(ATHEROS_WIFI_QCA6174_reg)
