/*
 * Realtek RTL8111 Gigabit Ethernet
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_REALTEK_GBE_8111 "realtek-gbe-8111"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} RealtekGbe8111State;

DECLARE_INSTANCE_CHECKER(RealtekGbe8111State, REALTEK_GBE_8111_STUB, TYPE_STUB_REALTEK_GBE_8111)

static uint64_t REALTEK_GBE_8111_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void REALTEK_GBE_8111_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps REALTEK_GBE_8111_ops = {
    .read=REALTEK_GBE_8111_r,.write=REALTEK_GBE_8111_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void REALTEK_GBE_8111_realize(PCIDevice *p, Error **e) {
    RealtekGbe8111State *s = REALTEK_GBE_8111_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x020000) & 0xFF;
    pci_config_set_class(p->config, (0x020000) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x10EC);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x8168);
    memory_region_init_io(&s->mmio,OBJECT(s),&REALTEK_GBE_8111_ops,s,"realtek-gbe-8111-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property REALTEK_GBE_8111_props[] = {
    DEFINE_PROP_STRING("device-name", RealtekGbe8111State, device_name),
};
static const VMStateDescription REALTEK_GBE_8111_vms = {
    .name="realtek-gbe-8111",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,RealtekGbe8111State),VMSTATE_END_OF_LIST()},
};
static void REALTEK_GBE_8111_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=REALTEK_GBE_8111_realize; pc->vendor_id=0x10EC; pc->device_id=0x8168;
    pc->class_id=0x020000>>8; pc->revision=0x15;
    dc->desc="Realtek RTL8111 Gigabit Ethernet"; dc->vmsd=&REALTEK_GBE_8111_vms; dc->hotpluggable=false;
    device_class_set_props(dc,REALTEK_GBE_8111_props);
}
static const TypeInfo REALTEK_GBE_8111_ti={
    .name=TYPE_STUB_REALTEK_GBE_8111,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(RealtekGbe8111State),.class_init=REALTEK_GBE_8111_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void REALTEK_GBE_8111_reg(void){type_register_static(&REALTEK_GBE_8111_ti);}
type_init(REALTEK_GBE_8111_reg)
