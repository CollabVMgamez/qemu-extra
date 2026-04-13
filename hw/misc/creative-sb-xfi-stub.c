/*
 * Creative Sound Blaster X-Fi (CA0110)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_CREATIVE_SB_XFI "creative-sb-xfi"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} CreativeSbXfiState;

DECLARE_INSTANCE_CHECKER(CreativeSbXfiState, CREATIVE_SB_XFI_STUB, TYPE_STUB_CREATIVE_SB_XFI)

static uint64_t CREATIVE_SB_XFI_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0008: return 0x00000001;
    case 0x0018: return 0x00000001;
    default: return 0;
    }
}
static void CREATIVE_SB_XFI_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps CREATIVE_SB_XFI_ops = {
    .read=CREATIVE_SB_XFI_r,.write=CREATIVE_SB_XFI_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void CREATIVE_SB_XFI_realize(PCIDevice *p, Error **e) {
    CreativeSbXfiState *s = CREATIVE_SB_XFI_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x040100) & 0xFF;
    pci_config_set_class(p->config, (0x040100) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x1102);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x0005);
    memory_region_init_io(&s->mmio,OBJECT(s),&CREATIVE_SB_XFI_ops,s,"creative-sb-xfi-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property CREATIVE_SB_XFI_props[] = {
    DEFINE_PROP_STRING("device-name", CreativeSbXfiState, device_name),
};
static const VMStateDescription CREATIVE_SB_XFI_vms = {
    .name="creative-sb-xfi",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,CreativeSbXfiState),VMSTATE_END_OF_LIST()},
};
static void CREATIVE_SB_XFI_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=CREATIVE_SB_XFI_realize; pc->vendor_id=0x1102; pc->device_id=0x0005;
    pc->class_id=0x040100>>8; pc->revision=0x00;
    dc->desc="Creative Sound Blaster X-Fi (CA0110)"; dc->vmsd=&CREATIVE_SB_XFI_vms; dc->hotpluggable=false;
    device_class_set_props(dc,CREATIVE_SB_XFI_props);
}
static const TypeInfo CREATIVE_SB_XFI_ti={
    .name=TYPE_STUB_CREATIVE_SB_XFI,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(CreativeSbXfiState),.class_init=CREATIVE_SB_XFI_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void CREATIVE_SB_XFI_reg(void){type_register_static(&CREATIVE_SB_XFI_ti);}
type_init(CREATIVE_SB_XFI_reg)
