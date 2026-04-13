/*
 * Creative Sound Blaster Live! (EMU10K1)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_CREATIVE_SB_LIVE "creative-sb-live"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} CreativeSbLiveState;

DECLARE_INSTANCE_CHECKER(CreativeSbLiveState, CREATIVE_SB_LIVE_STUB, TYPE_STUB_CREATIVE_SB_LIVE)

static uint64_t CREATIVE_SB_LIVE_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0008: return 0x00000001;
    case 0x0018: return 0x00000001;
    default: return 0;
    }
}
static void CREATIVE_SB_LIVE_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps CREATIVE_SB_LIVE_ops = {
    .read=CREATIVE_SB_LIVE_r,.write=CREATIVE_SB_LIVE_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void CREATIVE_SB_LIVE_realize(PCIDevice *p, Error **e) {
    CreativeSbLiveState *s = CREATIVE_SB_LIVE_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x040100) & 0xFF;
    pci_config_set_class(p->config, (0x040100) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x1102);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x0002);
    memory_region_init_io(&s->mmio,OBJECT(s),&CREATIVE_SB_LIVE_ops,s,"creative-sb-live-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property CREATIVE_SB_LIVE_props[] = {
    DEFINE_PROP_STRING("device-name", CreativeSbLiveState, device_name),
};
static const VMStateDescription CREATIVE_SB_LIVE_vms = {
    .name="creative-sb-live",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,CreativeSbLiveState),VMSTATE_END_OF_LIST()},
};
static void CREATIVE_SB_LIVE_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=CREATIVE_SB_LIVE_realize; pc->vendor_id=0x1102; pc->device_id=0x0002;
    pc->class_id=0x040100>>8; pc->revision=0x07;
    dc->desc="Creative Sound Blaster Live! (EMU10K1)"; dc->vmsd=&CREATIVE_SB_LIVE_vms; dc->hotpluggable=false;
    device_class_set_props(dc,CREATIVE_SB_LIVE_props);
}
static const TypeInfo CREATIVE_SB_LIVE_ti={
    .name=TYPE_STUB_CREATIVE_SB_LIVE,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(CreativeSbLiveState),.class_init=CREATIVE_SB_LIVE_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void CREATIVE_SB_LIVE_reg(void){type_register_static(&CREATIVE_SB_LIVE_ti);}
type_init(CREATIVE_SB_LIVE_reg)
