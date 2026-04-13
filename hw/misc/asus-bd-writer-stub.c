/*
 * ASUS BW-16D1HT Blu-ray Writer
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_ASUS_BD_WRITER "asus-bd-writer"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} AsusBdWriterState;

DECLARE_INSTANCE_CHECKER(AsusBdWriterState, ASUS_BD_WRITER_STUB, TYPE_STUB_ASUS_BD_WRITER)

static uint64_t ASUS_BD_WRITER_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void ASUS_BD_WRITER_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps ASUS_BD_WRITER_ops = {
    .read=ASUS_BD_WRITER_r,.write=ASUS_BD_WRITER_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void ASUS_BD_WRITER_realize(PCIDevice *p, Error **e) {
    AsusBdWriterState *s = ASUS_BD_WRITER_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x010601) & 0xFF;
    pci_config_set_class(p->config, (0x010601) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x13FD);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x3940);
    memory_region_init_io(&s->mmio,OBJECT(s),&ASUS_BD_WRITER_ops,s,"asus-bd-writer-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property ASUS_BD_WRITER_props[] = {
    DEFINE_PROP_STRING("device-name", AsusBdWriterState, device_name),
};
static const VMStateDescription ASUS_BD_WRITER_vms = {
    .name="asus-bd-writer",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AsusBdWriterState),VMSTATE_END_OF_LIST()},
};
static void ASUS_BD_WRITER_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=ASUS_BD_WRITER_realize; pc->vendor_id=0x13FD; pc->device_id=0x3940;
    pc->class_id=0x010601>>8; pc->revision=0x00;
    dc->desc="ASUS BW-16D1HT Blu-ray Writer"; dc->vmsd=&ASUS_BD_WRITER_vms; dc->hotpluggable=false;
    device_class_set_props(dc,ASUS_BD_WRITER_props);
}
static const TypeInfo ASUS_BD_WRITER_ti={
    .name=TYPE_STUB_ASUS_BD_WRITER,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(AsusBdWriterState),.class_init=ASUS_BD_WRITER_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void ASUS_BD_WRITER_reg(void){type_register_static(&ASUS_BD_WRITER_ti);}
type_init(ASUS_BD_WRITER_reg)
