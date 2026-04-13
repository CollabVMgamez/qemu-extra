/*
 * Infineon SLB 9670 TPM 2.0
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_INFINEON_TPM20 "infineon-tpm20"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} InfineonTpm20State;

DECLARE_INSTANCE_CHECKER(InfineonTpm20State, INFINEON_TPM20_STUB, TYPE_STUB_INFINEON_TPM20)

static uint64_t INFINEON_TPM20_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0xFF;
    case 0x000C: return 0x05;
    case 0x0018: return 0x0090;
    default: return 0;
    }
}
static void INFINEON_TPM20_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps INFINEON_TPM20_ops = {
    .read=INFINEON_TPM20_r,.write=INFINEON_TPM20_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void INFINEON_TPM20_realize(PCIDevice *p, Error **e) {
    InfineonTpm20State *s = INFINEON_TPM20_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0C0500) & 0xFF;
    pci_config_set_class(p->config, (0x0C0500) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x15D1);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x001B);
    memory_region_init_io(&s->mmio,OBJECT(s),&INFINEON_TPM20_ops,s,"infineon-tpm20-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property INFINEON_TPM20_props[] = {
    DEFINE_PROP_STRING("device-name", InfineonTpm20State, device_name),
};
static const VMStateDescription INFINEON_TPM20_vms = {
    .name="infineon-tpm20",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,InfineonTpm20State),VMSTATE_END_OF_LIST()},
};
static void INFINEON_TPM20_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=INFINEON_TPM20_realize; pc->vendor_id=0x15D1; pc->device_id=0x001B;
    pc->class_id=0x0C0500>>8; pc->revision=0x00;
    dc->desc="Infineon SLB 9670 TPM 2.0"; dc->vmsd=&INFINEON_TPM20_vms; dc->hotpluggable=false;
    device_class_set_props(dc,INFINEON_TPM20_props);
}
static const TypeInfo INFINEON_TPM20_ti={
    .name=TYPE_STUB_INFINEON_TPM20,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(InfineonTpm20State),.class_init=INFINEON_TPM20_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INFINEON_TPM20_reg(void){type_register_static(&INFINEON_TPM20_ti);}
type_init(INFINEON_TPM20_reg)
