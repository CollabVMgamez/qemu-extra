/*
 * LG WH16NS40 BD-R Writer
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_LG_DVD_WRITER "lg-dvd-writer"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} LgDvdWriterState;

DECLARE_INSTANCE_CHECKER(LgDvdWriterState, LG_DVD_WRITER_STUB, TYPE_STUB_LG_DVD_WRITER)

static uint64_t LG_DVD_WRITER_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void LG_DVD_WRITER_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps LG_DVD_WRITER_ops = {
    .read=LG_DVD_WRITER_r,.write=LG_DVD_WRITER_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void LG_DVD_WRITER_realize(PCIDevice *p, Error **e) {
    LgDvdWriterState *s = LG_DVD_WRITER_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x010601) & 0xFF;
    pci_config_set_class(p->config, (0x010601) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x13FD);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x3960);
    memory_region_init_io(&s->mmio,OBJECT(s),&LG_DVD_WRITER_ops,s,"lg-dvd-writer-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property LG_DVD_WRITER_props[] = {
    DEFINE_PROP_STRING("device-name", LgDvdWriterState, device_name),
};
static const VMStateDescription LG_DVD_WRITER_vms = {
    .name="lg-dvd-writer",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,LgDvdWriterState),VMSTATE_END_OF_LIST()},
};
static void LG_DVD_WRITER_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=LG_DVD_WRITER_realize; pc->vendor_id=0x13FD; pc->device_id=0x3960;
    pc->class_id=0x010601>>8; pc->revision=0x00;
    dc->desc="LG WH16NS40 BD-R Writer"; dc->vmsd=&LG_DVD_WRITER_vms; dc->hotpluggable=false;
    device_class_set_props(dc,LG_DVD_WRITER_props);
}
static const TypeInfo LG_DVD_WRITER_ti={
    .name=TYPE_STUB_LG_DVD_WRITER,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(LgDvdWriterState),.class_init=LG_DVD_WRITER_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void LG_DVD_WRITER_reg(void){type_register_static(&LG_DVD_WRITER_ti);}
type_init(LG_DVD_WRITER_reg)
