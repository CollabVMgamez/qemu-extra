/*
 * Nuvoton NPCT750 TPM 2.0
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_NUVOTON_TPM20 "nuvoton-tpm20"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} NuvotonTpm20State;

DECLARE_INSTANCE_CHECKER(NuvotonTpm20State, NUVOTON_TPM20_STUB, TYPE_STUB_NUVOTON_TPM20)

static uint64_t NUVOTON_TPM20_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0xFF;
    case 0x000C: return 0x05;
    case 0x0018: return 0x0090;
    default: return 0;
    }
}
static void NUVOTON_TPM20_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps NUVOTON_TPM20_ops = {
    .read=NUVOTON_TPM20_r,.write=NUVOTON_TPM20_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void NUVOTON_TPM20_realize(PCIDevice *p, Error **e) {
    NuvotonTpm20State *s = NUVOTON_TPM20_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x0C0500) & 0xFF;
    pci_config_set_class(p->config, (0x0C0500) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x1050);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x0750);
    memory_region_init_io(&s->mmio,OBJECT(s),&NUVOTON_TPM20_ops,s,"nuvoton-tpm20-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property NUVOTON_TPM20_props[] = {
    DEFINE_PROP_STRING("device-name", NuvotonTpm20State, device_name),
};
static const VMStateDescription NUVOTON_TPM20_vms = {
    .name="nuvoton-tpm20",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,NuvotonTpm20State),VMSTATE_END_OF_LIST()},
};
static void NUVOTON_TPM20_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=NUVOTON_TPM20_realize; pc->vendor_id=0x1050; pc->device_id=0x0750;
    pc->class_id=0x0C0500>>8; pc->revision=0x00;
    dc->desc="Nuvoton NPCT750 TPM 2.0"; dc->vmsd=&NUVOTON_TPM20_vms; dc->hotpluggable=false;
    device_class_set_props(dc,NUVOTON_TPM20_props);
}
static const TypeInfo NUVOTON_TPM20_ti={
    .name=TYPE_STUB_NUVOTON_TPM20,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(NuvotonTpm20State),.class_init=NUVOTON_TPM20_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void NUVOTON_TPM20_reg(void){type_register_static(&NUVOTON_TPM20_ti);}
type_init(NUVOTON_TPM20_reg)
