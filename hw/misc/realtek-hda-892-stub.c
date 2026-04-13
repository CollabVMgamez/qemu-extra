/*
 * Realtek ALC892 HD Audio Codec
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_REALTEK_HDA_892 "realtek-hda-892"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} RealtekHda892State;

DECLARE_INSTANCE_CHECKER(RealtekHda892State, REALTEK_HDA_892_STUB, TYPE_STUB_REALTEK_HDA_892)

static uint64_t REALTEK_HDA_892_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0008: return 0x00000001;
    case 0x0018: return 0x00000001;
    default: return 0;
    }
}
static void REALTEK_HDA_892_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps REALTEK_HDA_892_ops = {
    .read=REALTEK_HDA_892_r,.write=REALTEK_HDA_892_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void REALTEK_HDA_892_realize(PCIDevice *p, Error **e) {
    RealtekHda892State *s = REALTEK_HDA_892_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x040300) & 0xFF;
    pci_config_set_class(p->config, (0x040300) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x10EC);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x0892);
    memory_region_init_io(&s->mmio,OBJECT(s),&REALTEK_HDA_892_ops,s,"realtek-hda-892-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property REALTEK_HDA_892_props[] = {
    DEFINE_PROP_STRING("device-name", RealtekHda892State, device_name),
};
static const VMStateDescription REALTEK_HDA_892_vms = {
    .name="realtek-hda-892",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,RealtekHda892State),VMSTATE_END_OF_LIST()},
};
static void REALTEK_HDA_892_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=REALTEK_HDA_892_realize; pc->vendor_id=0x10EC; pc->device_id=0x0892;
    pc->class_id=0x040300>>8; pc->revision=0x00;
    dc->desc="Realtek ALC892 HD Audio Codec"; dc->vmsd=&REALTEK_HDA_892_vms; dc->hotpluggable=false;
    device_class_set_props(dc,REALTEK_HDA_892_props);
}
static const TypeInfo REALTEK_HDA_892_ti={
    .name=TYPE_STUB_REALTEK_HDA_892,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(RealtekHda892State),.class_init=REALTEK_HDA_892_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void REALTEK_HDA_892_reg(void){type_register_static(&REALTEK_HDA_892_ti);}
type_init(REALTEK_HDA_892_reg)
