/*
 * Realtek ALC4080 HD Audio (USB-C)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_REALTEK_HDA_4080 "realtek-hda-4080"
OBJECT_DECLARE_SIMPLE_TYPE(RealtekHda4080State, REALTEK_HDA_4080)

struct RealtekHda4080State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t realtek_hda_4080_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void realtek_hda_4080_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps realtek_hda_4080_ops = {
    .read = realtek_hda_4080_r, .write = realtek_hda_4080_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void realtek_hda_4080_realize(PCIDevice *pdev, Error **errp) {
    RealtekHda4080State *s = REALTEK_HDA_4080(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4332);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 4640);
    memory_region_init_io(&s->mmio, OBJECT(s), &realtek_hda_4080_ops, s,
                          "realtek-hda-4080-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property realtek_hda_4080_props[] = {
    DEFINE_PROP_STRING("device-name", RealtekHda4080State, device_name),
};

static const VMStateDescription vmstate_realtek_hda_4080 = {
    .name = "realtek-hda-4080",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, RealtekHda4080State),
        VMSTATE_END_OF_LIST()
    },
};

static void realtek_hda_4080_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = realtek_hda_4080_realize;
    pc->vendor_id = 4332;
    pc->device_id = 4640;
    pc->class_id = 1027;
    pc->revision = 2;
    dc->desc = "Realtek ALC4080 HD Audio (USB-C)";
    dc->vmsd = &vmstate_realtek_hda_4080;
    device_class_set_props(dc, realtek_hda_4080_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo realtek_hda_4080_info = {
    .name = TYPE_REALTEK_HDA_4080,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RealtekHda4080State),
    .class_init = realtek_hda_4080_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void realtek_hda_4080_reg(void) { type_register_static(&realtek_hda_4080_info); }
type_init(realtek_hda_4080_reg)
/*
 * Realtek ALC897 HD Audio 7.1ch
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_REALTEK_HDA_897 "realtek-hda-897"
OBJECT_DECLARE_SIMPLE_TYPE(RealtekHda897State, REALTEK_HDA_897)

struct RealtekHda897State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t realtek_hda_897_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void realtek_hda_897_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps realtek_hda_897_ops = {
    .read = realtek_hda_897_r, .write = realtek_hda_897_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void realtek_hda_897_realize(PCIDevice *pdev, Error **errp) {
    RealtekHda897State *s = REALTEK_HDA_897(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4332);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 2199);
    memory_region_init_io(&s->mmio, OBJECT(s), &realtek_hda_897_ops, s,
                          "realtek-hda-897-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property realtek_hda_897_props[] = {
    DEFINE_PROP_STRING("device-name", RealtekHda897State, device_name),
};

static const VMStateDescription vmstate_realtek_hda_897 = {
    .name = "realtek-hda-897",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, RealtekHda897State),
        VMSTATE_END_OF_LIST()
    },
};

static void realtek_hda_897_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = realtek_hda_897_realize;
    pc->vendor_id = 4332;
    pc->device_id = 2199;
    pc->class_id = 1027;
    pc->revision = 4;
    dc->desc = "Realtek ALC897 HD Audio 7.1ch";
    dc->vmsd = &vmstate_realtek_hda_897;
    device_class_set_props(dc, realtek_hda_897_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo realtek_hda_897_info = {
    .name = TYPE_REALTEK_HDA_897,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RealtekHda897State),
    .class_init = realtek_hda_897_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void realtek_hda_897_reg(void) { type_register_static(&realtek_hda_897_info); }
type_init(realtek_hda_897_reg)
/*
 * Realtek ALC1220B HD Audio (MB)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_REALTEK_HDA_1220B "realtek-hda-1220b"
OBJECT_DECLARE_SIMPLE_TYPE(RealtekHda1220bState, REALTEK_HDA_1220B)

struct RealtekHda1220bState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t realtek_hda_1220b_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void realtek_hda_1220b_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps realtek_hda_1220b_ops = {
    .read = realtek_hda_1220b_r, .write = realtek_hda_1220b_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void realtek_hda_1220b_realize(PCIDevice *pdev, Error **errp) {
    RealtekHda1220bState *s = REALTEK_HDA_1220B(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5218);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 31337);
    memory_region_init_io(&s->mmio, OBJECT(s), &realtek_hda_1220b_ops, s,
                          "realtek-hda-1220b-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property realtek_hda_1220b_props[] = {
    DEFINE_PROP_STRING("device-name", RealtekHda1220bState, device_name),
};

static const VMStateDescription vmstate_realtek_hda_1220b = {
    .name = "realtek-hda-1220b",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, RealtekHda1220bState),
        VMSTATE_END_OF_LIST()
    },
};

static void realtek_hda_1220b_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = realtek_hda_1220b_realize;
    pc->vendor_id = 4332;
    pc->device_id = 4640;
    pc->class_id = 1027;
    pc->revision = 1;
    dc->desc = "Realtek ALC1220B HD Audio (MB)";
    dc->vmsd = &vmstate_realtek_hda_1220b;
    device_class_set_props(dc, realtek_hda_1220b_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo realtek_hda_1220b_info = {
    .name = TYPE_REALTEK_HDA_1220B,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RealtekHda1220bState),
    .class_init = realtek_hda_1220b_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void realtek_hda_1220b_reg(void) { type_register_static(&realtek_hda_1220b_info); }
type_init(realtek_hda_1220b_reg)
/*
 * Creative Sound BlasterX AE-5
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CREATIVE_SB_AE5 "creative-sb-ae5"
OBJECT_DECLARE_SIMPLE_TYPE(CreativeSbAe5State, CREATIVE_SB_AE5)

struct CreativeSbAe5State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t creative_sb_ae5_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void creative_sb_ae5_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps creative_sb_ae5_ops = {
    .read = creative_sb_ae5_r, .write = creative_sb_ae5_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void creative_sb_ae5_realize(PCIDevice *pdev, Error **errp) {
    CreativeSbAe5State *s = CREATIVE_SB_AE5(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4354);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 16);
    memory_region_init_io(&s->mmio, OBJECT(s), &creative_sb_ae5_ops, s,
                          "creative-sb-ae5-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property creative_sb_ae5_props[] = {
    DEFINE_PROP_STRING("device-name", CreativeSbAe5State, device_name),
};

static const VMStateDescription vmstate_creative_sb_ae5 = {
    .name = "creative-sb-ae5",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CreativeSbAe5State),
        VMSTATE_END_OF_LIST()
    },
};

static void creative_sb_ae5_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = creative_sb_ae5_realize;
    pc->vendor_id = 4354;
    pc->device_id = 16;
    pc->class_id = 1027;
    pc->revision = 1;
    dc->desc = "Creative Sound BlasterX AE-5";
    dc->vmsd = &vmstate_creative_sb_ae5;
    device_class_set_props(dc, creative_sb_ae5_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo creative_sb_ae5_info = {
    .name = TYPE_CREATIVE_SB_AE5,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CreativeSbAe5State),
    .class_init = creative_sb_ae5_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void creative_sb_ae5_reg(void) { type_register_static(&creative_sb_ae5_info); }
type_init(creative_sb_ae5_reg)
/*
 * Creative Sound Blaster AE-9
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CREATIVE_SB_AE9 "creative-sb-ae9"
OBJECT_DECLARE_SIMPLE_TYPE(CreativeSbAe9State, CREATIVE_SB_AE9)

struct CreativeSbAe9State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t creative_sb_ae9_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void creative_sb_ae9_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps creative_sb_ae9_ops = {
    .read = creative_sb_ae9_r, .write = creative_sb_ae9_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void creative_sb_ae9_realize(PCIDevice *pdev, Error **errp) {
    CreativeSbAe9State *s = CREATIVE_SB_AE9(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4354);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 18);
    memory_region_init_io(&s->mmio, OBJECT(s), &creative_sb_ae9_ops, s,
                          "creative-sb-ae9-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property creative_sb_ae9_props[] = {
    DEFINE_PROP_STRING("device-name", CreativeSbAe9State, device_name),
};

static const VMStateDescription vmstate_creative_sb_ae9 = {
    .name = "creative-sb-ae9",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CreativeSbAe9State),
        VMSTATE_END_OF_LIST()
    },
};

static void creative_sb_ae9_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = creative_sb_ae9_realize;
    pc->vendor_id = 4354;
    pc->device_id = 18;
    pc->class_id = 1027;
    pc->revision = 1;
    dc->desc = "Creative Sound Blaster AE-9";
    dc->vmsd = &vmstate_creative_sb_ae9;
    device_class_set_props(dc, creative_sb_ae9_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo creative_sb_ae9_info = {
    .name = TYPE_CREATIVE_SB_AE9,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CreativeSbAe9State),
    .class_init = creative_sb_ae9_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void creative_sb_ae9_reg(void) { type_register_static(&creative_sb_ae9_info); }
type_init(creative_sb_ae9_reg)
