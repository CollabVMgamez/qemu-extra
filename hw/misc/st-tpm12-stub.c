/*
 * STMicroelectronics TPM 1.2
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_ST_TPM12 "st-tpm12"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} StTpm12State;

DECLARE_INSTANCE_CHECKER(StTpm12State, ST_TPM12_STUB, TYPE_STUB_ST_TPM12)

static uint64_t ST_TPM12_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0xFF;
    case 0x000C: return 0x05;
    case 0x0018: return 0x0090;
    default: return 0;
    }
}
static void ST_TPM12_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps ST_TPM12_ops = {
    .read=ST_TPM12_r,.write=ST_TPM12_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void ST_TPM12_realize(PCIDevice *p, Error **e) {
    StTpm12State *s = ST_TPM12_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0C0500) & 0xFF;
    pci_config_set_class(p->config, (0x0C0500) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x104A);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x0701);
    memory_region_init_io(&s->mmio,OBJECT(s),&ST_TPM12_ops,s,"st-tpm12-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property ST_TPM12_props[] = {
    DEFINE_PROP_STRING("device-name", StTpm12State, device_name),
};
static const VMStateDescription ST_TPM12_vms = {
    .name="st-tpm12",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,StTpm12State),VMSTATE_END_OF_LIST()},
};
static void ST_TPM12_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=ST_TPM12_realize; pc->vendor_id=0x104A; pc->device_id=0x0701;
    pc->class_id=0x0C0500>>8; pc->revision=0x00;
    dc->desc="STMicroelectronics TPM 1.2"; dc->vmsd=&ST_TPM12_vms; dc->hotpluggable=false;
    device_class_set_props(dc,ST_TPM12_props);
}
static const TypeInfo ST_TPM12_ti={
    .name=TYPE_STUB_ST_TPM12,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(StTpm12State),.class_init=ST_TPM12_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void ST_TPM12_reg(void){type_register_static(&ST_TPM12_ti);}
type_init(ST_TPM12_reg)
