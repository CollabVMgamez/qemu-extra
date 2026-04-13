/*
 * Intel I219-V Gigabit Ethernet
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STUB_INTEL_I219V "intel-i219v"

typedef struct {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
} IntelI219vState;

DECLARE_INSTANCE_CHECKER(IntelI219vState, INTEL_I219V_STUB, TYPE_STUB_INTEL_I219V)

static uint64_t INTEL_I219V_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void INTEL_I219V_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps INTEL_I219V_ops = {
    .read=INTEL_I219V_r,.write=INTEL_I219V_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};
static void INTEL_I219V_realize(PCIDevice *p, Error **e) {
    IntelI219vState *s = INTEL_I219V_STUB(p);
    p->config[PCI_CLASS_PROG] = (0x020000) & 0xFF;
    pci_config_set_class(p->config, (0x020000) >> 8);
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, 0x8086);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID, 0x15BC);
    memory_region_init_io(&s->mmio,OBJECT(s),&INTEL_I219V_ops,s,"intel-i219v-mmio",4*KiB);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->mmio);
}
static const Property INTEL_I219V_props[] = {
    DEFINE_PROP_STRING("device-name", IntelI219vState, device_name),
};
static const VMStateDescription INTEL_I219V_vms = {
    .name="intel-i219v",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelI219vState),VMSTATE_END_OF_LIST()},
};
static void INTEL_I219V_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=INTEL_I219V_realize; pc->vendor_id=0x8086; pc->device_id=0x15BC;
    pc->class_id=0x020000>>8; pc->revision=0x21;
    dc->desc="Intel I219-V Gigabit Ethernet"; dc->vmsd=&INTEL_I219V_vms; dc->hotpluggable=false;
    device_class_set_props(dc,INTEL_I219V_props);
}
static const TypeInfo INTEL_I219V_ti={
    .name=TYPE_STUB_INTEL_I219V,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelI219vState),.class_init=INTEL_I219V_ci,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void INTEL_I219V_reg(void){type_register_static(&INTEL_I219V_ti);}
type_init(INTEL_I219V_reg)
