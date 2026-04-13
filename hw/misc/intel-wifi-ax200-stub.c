/*
 * Intel Wi-Fi 6 AX200 802.11ax
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_INTEL_WIFI_AX200 "intel-wifi-ax200"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} IntelWifiAx200State;

DECLARE_INSTANCE_CHECKER(IntelWifiAx200State, INTEL_WIFI_AX200_STUB, TYPE_STUB_INTEL_WIFI_AX200)

static uint64_t INTEL_WIFI_AX200_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void INTEL_WIFI_AX200_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps INTEL_WIFI_AX200_ops = {
    .read=INTEL_WIFI_AX200_r,.write=INTEL_WIFI_AX200_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void INTEL_WIFI_AX200_realize(PCIDevice *p, Error **e) {
    IntelWifiAx200State *s = INTEL_WIFI_AX200_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x028000) & 0xFF;
    pci_config_set_class(p->config, (0x028000) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x2723);
    memory_region_init_io(&s->mmio,OBJECT(s),&INTEL_WIFI_AX200_ops,s,"intel-wifi-ax200-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property INTEL_WIFI_AX200_props[] = {
    DEFINE_PROP_STRING("device-name", IntelWifiAx200State, device_name),
};
static const VMStateDescription INTEL_WIFI_AX200_vms = {
    .name="intel-wifi-ax200",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelWifiAx200State),VMSTATE_END_OF_LIST()},
};
static void INTEL_WIFI_AX200_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_WIFI_AX200_realize; pc->vendor_id=0x8086; pc->device_id=0x2723;
    pc->class_id=0x028000>>8; pc->revision=0x1A;
    dc->desc="Intel Wi-Fi 6 AX200 802.11ax"; dc->vmsd=&INTEL_WIFI_AX200_vms; dc->hotpluggable=false;
    device_class_set_props(dc,INTEL_WIFI_AX200_props);
}
static const TypeInfo INTEL_WIFI_AX200_ti={
    .name=TYPE_STUB_INTEL_WIFI_AX200,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelWifiAx200State),.class_init=INTEL_WIFI_AX200_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_WIFI_AX200_reg(void){type_register_static(&INTEL_WIFI_AX200_ti);}
type_init(INTEL_WIFI_AX200_reg)
