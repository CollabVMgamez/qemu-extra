/*
 * Intel I210-AT Gigabit Ethernet
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_I210 "intel-i210"
OBJECT_DECLARE_SIMPLE_TYPE(IntelI210State, INTEL_I210)

struct IntelI210State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_i210_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_i210_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_i210_ops = {
    .read = intel_i210_r, .write = intel_i210_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_i210_realize(PCIDevice *pdev, Error **errp) {
    IntelI210State *s = INTEL_I210(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_i210_ops, s,
                          "intel-i210-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_i210_props[] = {
    DEFINE_PROP_STRING("device-name", IntelI210State, device_name),
};

static const VMStateDescription vmstate_intel_i210 = {
    .name = "intel-i210",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelI210State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_i210_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_i210_realize;
    pc->vendor_id = 32902;
    pc->device_id = 5427;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Intel I210-AT Gigabit Ethernet";
    dc->vmsd = &vmstate_intel_i210;
    device_class_set_props(dc, intel_i210_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_i210_info = {
    .name = TYPE_INTEL_I210,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelI210State),
    .class_init = intel_i210_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_i210_reg(void) { type_register_static(&intel_i210_info); }
type_init(intel_i210_reg)
/*
 * Intel I226-V 2.5GbE Ethernet
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_I226_V "intel-i226-v"
OBJECT_DECLARE_SIMPLE_TYPE(IntelI226VState, INTEL_I226_V)

struct IntelI226VState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_i226_v_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_i226_v_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_i226_v_ops = {
    .read = intel_i226_v_r, .write = intel_i226_v_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_i226_v_realize(PCIDevice *pdev, Error **errp) {
    IntelI226VState *s = INTEL_I226_V(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_i226_v_ops, s,
                          "intel-i226-v-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_i226_v_props[] = {
    DEFINE_PROP_STRING("device-name", IntelI226VState, device_name),
};

static const VMStateDescription vmstate_intel_i226_v = {
    .name = "intel-i226-v",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelI226VState),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_i226_v_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_i226_v_realize;
    pc->vendor_id = 32902;
    pc->device_id = 4699;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Intel I226-V 2.5GbE Ethernet";
    dc->vmsd = &vmstate_intel_i226_v;
    device_class_set_props(dc, intel_i226_v_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_i226_v_info = {
    .name = TYPE_INTEL_I226_V,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelI226VState),
    .class_init = intel_i226_v_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_i226_v_reg(void) { type_register_static(&intel_i226_v_info); }
type_init(intel_i226_v_reg)
/*
 * Intel I226-LM 2.5GbE Ethernet (Mobile)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_I226_LM "intel-i226-lm"
OBJECT_DECLARE_SIMPLE_TYPE(IntelI226LmState, INTEL_I226_LM)

struct IntelI226LmState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_i226_lm_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_i226_lm_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_i226_lm_ops = {
    .read = intel_i226_lm_r, .write = intel_i226_lm_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_i226_lm_realize(PCIDevice *pdev, Error **errp) {
    IntelI226LmState *s = INTEL_I226_LM(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_i226_lm_ops, s,
                          "intel-i226-lm-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_i226_lm_props[] = {
    DEFINE_PROP_STRING("device-name", IntelI226LmState, device_name),
};

static const VMStateDescription vmstate_intel_i226_lm = {
    .name = "intel-i226-lm",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelI226LmState),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_i226_lm_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_i226_lm_realize;
    pc->vendor_id = 32902;
    pc->device_id = 4700;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Intel I226-LM 2.5GbE Ethernet (Mobile)";
    dc->vmsd = &vmstate_intel_i226_lm;
    device_class_set_props(dc, intel_i226_lm_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_i226_lm_info = {
    .name = TYPE_INTEL_I226_LM,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelI226LmState),
    .class_init = intel_i226_lm_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_i226_lm_reg(void) { type_register_static(&intel_i226_lm_info); }
type_init(intel_i226_lm_reg)
/*
 * Intel X710-DA2 10GbE SFP+
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_X710 "intel-x710"
OBJECT_DECLARE_SIMPLE_TYPE(IntelX710State, INTEL_X710)

struct IntelX710State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_x710_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_x710_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_x710_ops = {
    .read = intel_x710_r, .write = intel_x710_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_x710_realize(PCIDevice *pdev, Error **errp) {
    IntelX710State *s = INTEL_X710(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_x710_ops, s,
                          "intel-x710-mmio", 131072);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_x710_props[] = {
    DEFINE_PROP_STRING("device-name", IntelX710State, device_name),
};

static const VMStateDescription vmstate_intel_x710 = {
    .name = "intel-x710",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelX710State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_x710_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_x710_realize;
    pc->vendor_id = 32902;
    pc->device_id = 5490;
    pc->class_id = 512;
    pc->revision = 2;
    dc->desc = "Intel X710-DA2 10GbE SFP+";
    dc->vmsd = &vmstate_intel_x710;
    device_class_set_props(dc, intel_x710_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_x710_info = {
    .name = TYPE_INTEL_X710,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelX710State),
    .class_init = intel_x710_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_x710_reg(void) { type_register_static(&intel_x710_info); }
type_init(intel_x710_reg)
/*
 * Intel X550-T2 10GbE RJ45
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_X550_T2 "intel-x550-t2"
OBJECT_DECLARE_SIMPLE_TYPE(IntelX550T2State, INTEL_X550_T2)

struct IntelX550T2State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_x550_t2_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_x550_t2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_x550_t2_ops = {
    .read = intel_x550_t2_r, .write = intel_x550_t2_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_x550_t2_realize(PCIDevice *pdev, Error **errp) {
    IntelX550T2State *s = INTEL_X550_T2(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_x550_t2_ops, s,
                          "intel-x550-t2-mmio", 131072);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_x550_t2_props[] = {
    DEFINE_PROP_STRING("device-name", IntelX550T2State, device_name),
};

static const VMStateDescription vmstate_intel_x550_t2 = {
    .name = "intel-x550-t2",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelX550T2State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_x550_t2_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_x550_t2_realize;
    pc->vendor_id = 32902;
    pc->device_id = 5475;
    pc->class_id = 512;
    pc->revision = 1;
    dc->desc = "Intel X550-T2 10GbE RJ45";
    dc->vmsd = &vmstate_intel_x550_t2;
    device_class_set_props(dc, intel_x550_t2_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_x550_t2_info = {
    .name = TYPE_INTEL_X550_T2,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelX550T2State),
    .class_init = intel_x550_t2_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_x550_t2_reg(void) { type_register_static(&intel_x550_t2_info); }
type_init(intel_x550_t2_reg)
/*
 * Intel E810-CQDA2 100GbE QSFP28
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_E810_CQDA2 "intel-e810-cqda2"
OBJECT_DECLARE_SIMPLE_TYPE(IntelE810Cqda2State, INTEL_E810_CQDA2)

struct IntelE810Cqda2State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_e810_cqda2_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_e810_cqda2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_e810_cqda2_ops = {
    .read = intel_e810_cqda2_r, .write = intel_e810_cqda2_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_e810_cqda2_realize(PCIDevice *pdev, Error **errp) {
    IntelE810Cqda2State *s = INTEL_E810_CQDA2(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_e810_cqda2_ops, s,
                          "intel-e810-cqda2-mmio", 262144);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_e810_cqda2_props[] = {
    DEFINE_PROP_STRING("device-name", IntelE810Cqda2State, device_name),
};

static const VMStateDescription vmstate_intel_e810_cqda2 = {
    .name = "intel-e810-cqda2",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelE810Cqda2State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_e810_cqda2_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_e810_cqda2_realize;
    pc->vendor_id = 32902;
    pc->device_id = 5522;
    pc->class_id = 512;
    pc->revision = 2;
    dc->desc = "Intel E810-CQDA2 100GbE QSFP28";
    dc->vmsd = &vmstate_intel_e810_cqda2;
    device_class_set_props(dc, intel_e810_cqda2_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_e810_cqda2_info = {
    .name = TYPE_INTEL_E810_CQDA2,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelE810Cqda2State),
    .class_init = intel_e810_cqda2_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_e810_cqda2_reg(void) { type_register_static(&intel_e810_cqda2_info); }
type_init(intel_e810_cqda2_reg)
/*
 * Realtek RTL8125 2.5GbE Ethernet
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_REALTEK_RTL8125_2G5 "realtek-rtl8125-2g5"
OBJECT_DECLARE_SIMPLE_TYPE(RealtekRtl81252g5State, REALTEK_RTL8125_2G5)

struct RealtekRtl81252g5State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t realtek_rtl8125_2g5_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void realtek_rtl8125_2g5_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps realtek_rtl8125_2g5_ops = {
    .read = realtek_rtl8125_2g5_r, .write = realtek_rtl8125_2g5_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void realtek_rtl8125_2g5_realize(PCIDevice *pdev, Error **errp) {
    RealtekRtl81252g5State *s = REALTEK_RTL8125_2G5(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4332);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 33061);
    memory_region_init_io(&s->mmio, OBJECT(s), &realtek_rtl8125_2g5_ops, s,
                          "realtek-rtl8125-2g5-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property realtek_rtl8125_2g5_props[] = {
    DEFINE_PROP_STRING("device-name", RealtekRtl81252g5State, device_name),
};

static const VMStateDescription vmstate_realtek_rtl8125_2g5 = {
    .name = "realtek-rtl8125-2g5",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, RealtekRtl81252g5State),
        VMSTATE_END_OF_LIST()
    },
};

static void realtek_rtl8125_2g5_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = realtek_rtl8125_2g5_realize;
    pc->vendor_id = 4332;
    pc->device_id = 33061;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Realtek RTL8125 2.5GbE Ethernet";
    dc->vmsd = &vmstate_realtek_rtl8125_2g5;
    device_class_set_props(dc, realtek_rtl8125_2g5_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo realtek_rtl8125_2g5_info = {
    .name = TYPE_REALTEK_RTL8125_2G5,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RealtekRtl81252g5State),
    .class_init = realtek_rtl8125_2g5_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void realtek_rtl8125_2g5_reg(void) { type_register_static(&realtek_rtl8125_2g5_info); }
type_init(realtek_rtl8125_2g5_reg)
/*
 * Realtek RTL8117 PCIe GbE Controller
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_REALTEK_RTL8117 "realtek-rtl8117"
OBJECT_DECLARE_SIMPLE_TYPE(RealtekRtl8117State, REALTEK_RTL8117)

struct RealtekRtl8117State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t realtek_rtl8117_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void realtek_rtl8117_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps realtek_rtl8117_ops = {
    .read = realtek_rtl8117_r, .write = realtek_rtl8117_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void realtek_rtl8117_realize(PCIDevice *pdev, Error **errp) {
    RealtekRtl8117State *s = REALTEK_RTL8117(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4332);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 33047);
    memory_region_init_io(&s->mmio, OBJECT(s), &realtek_rtl8117_ops, s,
                          "realtek-rtl8117-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property realtek_rtl8117_props[] = {
    DEFINE_PROP_STRING("device-name", RealtekRtl8117State, device_name),
};

static const VMStateDescription vmstate_realtek_rtl8117 = {
    .name = "realtek-rtl8117",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, RealtekRtl8117State),
        VMSTATE_END_OF_LIST()
    },
};

static void realtek_rtl8117_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = realtek_rtl8117_realize;
    pc->vendor_id = 4332;
    pc->device_id = 33047;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Realtek RTL8117 PCIe GbE Controller";
    dc->vmsd = &vmstate_realtek_rtl8117;
    device_class_set_props(dc, realtek_rtl8117_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo realtek_rtl8117_info = {
    .name = TYPE_REALTEK_RTL8117,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RealtekRtl8117State),
    .class_init = realtek_rtl8117_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void realtek_rtl8117_reg(void) { type_register_static(&realtek_rtl8117_info); }
type_init(realtek_rtl8117_reg)
/*
 * Broadcom BCM5720 Dual-Port GbE
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_BROADCOM_BCM5720 "broadcom-bcm5720"
OBJECT_DECLARE_SIMPLE_TYPE(BroadcomBcm5720State, BROADCOM_BCM5720)

struct BroadcomBcm5720State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t broadcom_bcm5720_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void broadcom_bcm5720_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps broadcom_bcm5720_ops = {
    .read = broadcom_bcm5720_r, .write = broadcom_bcm5720_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void broadcom_bcm5720_realize(PCIDevice *pdev, Error **errp) {
    BroadcomBcm5720State *s = BROADCOM_BCM5720(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5348);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 5727);
    memory_region_init_io(&s->mmio, OBJECT(s), &broadcom_bcm5720_ops, s,
                          "broadcom-bcm5720-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property broadcom_bcm5720_props[] = {
    DEFINE_PROP_STRING("device-name", BroadcomBcm5720State, device_name),
};

static const VMStateDescription vmstate_broadcom_bcm5720 = {
    .name = "broadcom-bcm5720",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, BroadcomBcm5720State),
        VMSTATE_END_OF_LIST()
    },
};

static void broadcom_bcm5720_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = broadcom_bcm5720_realize;
    pc->vendor_id = 5348;
    pc->device_id = 5727;
    pc->class_id = 512;
    pc->revision = 1;
    dc->desc = "Broadcom BCM5720 Dual-Port GbE";
    dc->vmsd = &vmstate_broadcom_bcm5720;
    device_class_set_props(dc, broadcom_bcm5720_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo broadcom_bcm5720_info = {
    .name = TYPE_BROADCOM_BCM5720,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(BroadcomBcm5720State),
    .class_init = broadcom_bcm5720_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void broadcom_bcm5720_reg(void) { type_register_static(&broadcom_bcm5720_info); }
type_init(broadcom_bcm5720_reg)
/*
 * Broadcom BCM57416 Dual-Port 10GbE
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_BROADCOM_BCM57416 "broadcom-bcm57416"
OBJECT_DECLARE_SIMPLE_TYPE(BroadcomBcm57416State, BROADCOM_BCM57416)

struct BroadcomBcm57416State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t broadcom_bcm57416_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void broadcom_bcm57416_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps broadcom_bcm57416_ops = {
    .read = broadcom_bcm57416_r, .write = broadcom_bcm57416_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void broadcom_bcm57416_realize(PCIDevice *pdev, Error **errp) {
    BroadcomBcm57416State *s = BROADCOM_BCM57416(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5348);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 5652);
    memory_region_init_io(&s->mmio, OBJECT(s), &broadcom_bcm57416_ops, s,
                          "broadcom-bcm57416-mmio", 131072);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property broadcom_bcm57416_props[] = {
    DEFINE_PROP_STRING("device-name", BroadcomBcm57416State, device_name),
};

static const VMStateDescription vmstate_broadcom_bcm57416 = {
    .name = "broadcom-bcm57416",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, BroadcomBcm57416State),
        VMSTATE_END_OF_LIST()
    },
};

static void broadcom_bcm57416_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = broadcom_bcm57416_realize;
    pc->vendor_id = 5348;
    pc->device_id = 5652;
    pc->class_id = 512;
    pc->revision = 1;
    dc->desc = "Broadcom BCM57416 Dual-Port 10GbE";
    dc->vmsd = &vmstate_broadcom_bcm57416;
    device_class_set_props(dc, broadcom_bcm57416_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo broadcom_bcm57416_info = {
    .name = TYPE_BROADCOM_BCM57416,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(BroadcomBcm57416State),
    .class_init = broadcom_bcm57416_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void broadcom_bcm57416_reg(void) { type_register_static(&broadcom_bcm57416_info); }
type_init(broadcom_bcm57416_reg)
/*
 * Mellanox ConnectX-5 EX 25GbE
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_MELLANOX_CONNECTX5 "mellanox-connectx5"
OBJECT_DECLARE_SIMPLE_TYPE(MellanoxConnectx5State, MELLANOX_CONNECTX5)

struct MellanoxConnectx5State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t mellanox_connectx5_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void mellanox_connectx5_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps mellanox_connectx5_ops = {
    .read = mellanox_connectx5_r, .write = mellanox_connectx5_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void mellanox_connectx5_realize(PCIDevice *pdev, Error **errp) {
    MellanoxConnectx5State *s = MELLANOX_CONNECTX5(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5555);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 7);
    memory_region_init_io(&s->mmio, OBJECT(s), &mellanox_connectx5_ops, s,
                          "mellanox-connectx5-mmio", 262144);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property mellanox_connectx5_props[] = {
    DEFINE_PROP_STRING("device-name", MellanoxConnectx5State, device_name),
};

static const VMStateDescription vmstate_mellanox_connectx5 = {
    .name = "mellanox-connectx5",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, MellanoxConnectx5State),
        VMSTATE_END_OF_LIST()
    },
};

static void mellanox_connectx5_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = mellanox_connectx5_realize;
    pc->vendor_id = 5555;
    pc->device_id = 4121;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Mellanox ConnectX-5 EX 25GbE";
    dc->vmsd = &vmstate_mellanox_connectx5;
    device_class_set_props(dc, mellanox_connectx5_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo mellanox_connectx5_info = {
    .name = TYPE_MELLANOX_CONNECTX5,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(MellanoxConnectx5State),
    .class_init = mellanox_connectx5_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void mellanox_connectx5_reg(void) { type_register_static(&mellanox_connectx5_info); }
type_init(mellanox_connectx5_reg)
/*
 * Mellanox ConnectX-6 Dx 25GbE
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_MELLANOX_CONNECTX6_DX "mellanox-connectx6-dx"
OBJECT_DECLARE_SIMPLE_TYPE(MellanoxConnectx6DxState, MELLANOX_CONNECTX6_DX)

struct MellanoxConnectx6DxState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t mellanox_connectx6_dx_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void mellanox_connectx6_dx_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps mellanox_connectx6_dx_ops = {
    .read = mellanox_connectx6_dx_r, .write = mellanox_connectx6_dx_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void mellanox_connectx6_dx_realize(PCIDevice *pdev, Error **errp) {
    MellanoxConnectx6DxState *s = MELLANOX_CONNECTX6_DX(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5555);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 7);
    memory_region_init_io(&s->mmio, OBJECT(s), &mellanox_connectx6_dx_ops, s,
                          "mellanox-connectx6-dx-mmio", 262144);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property mellanox_connectx6_dx_props[] = {
    DEFINE_PROP_STRING("device-name", MellanoxConnectx6DxState, device_name),
};

static const VMStateDescription vmstate_mellanox_connectx6_dx = {
    .name = "mellanox-connectx6-dx",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, MellanoxConnectx6DxState),
        VMSTATE_END_OF_LIST()
    },
};

static void mellanox_connectx6_dx_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = mellanox_connectx6_dx_realize;
    pc->vendor_id = 5555;
    pc->device_id = 4125;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Mellanox ConnectX-6 Dx 25GbE";
    dc->vmsd = &vmstate_mellanox_connectx6_dx;
    device_class_set_props(dc, mellanox_connectx6_dx_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo mellanox_connectx6_dx_info = {
    .name = TYPE_MELLANOX_CONNECTX6_DX,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(MellanoxConnectx6DxState),
    .class_init = mellanox_connectx6_dx_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void mellanox_connectx6_dx_reg(void) { type_register_static(&mellanox_connectx6_dx_info); }
type_init(mellanox_connectx6_dx_reg)
/*
 * Qualcomm FastConnect 6900 WiFi 6E
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_QUALCOMM_FASTCONNECT_6900 "qualcomm-fastconnect-6900"
OBJECT_DECLARE_SIMPLE_TYPE(QualcommFastconnect6900State, QUALCOMM_FASTCONNECT_6900)

struct QualcommFastconnect6900State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t qualcomm_fastconnect_6900_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void qualcomm_fastconnect_6900_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps qualcomm_fastconnect_6900_ops = {
    .read = qualcomm_fastconnect_6900_r, .write = qualcomm_fastconnect_6900_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void qualcomm_fastconnect_6900_realize(PCIDevice *pdev, Error **errp) {
    QualcommFastconnect6900State *s = QUALCOMM_FASTCONNECT_6900(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 6091);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 4353);
    memory_region_init_io(&s->mmio, OBJECT(s), &qualcomm_fastconnect_6900_ops, s,
                          "qualcomm-fastconnect-6900-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property qualcomm_fastconnect_6900_props[] = {
    DEFINE_PROP_STRING("device-name", QualcommFastconnect6900State, device_name),
};

static const VMStateDescription vmstate_qualcomm_fastconnect_6900 = {
    .name = "qualcomm-fastconnect-6900",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, QualcommFastconnect6900State),
        VMSTATE_END_OF_LIST()
    },
};

static void qualcomm_fastconnect_6900_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = qualcomm_fastconnect_6900_realize;
    pc->vendor_id = 6091;
    pc->device_id = 4353;
    pc->class_id = 512;
    pc->revision = 1;
    dc->desc = "Qualcomm FastConnect 6900 WiFi 6E";
    dc->vmsd = &vmstate_qualcomm_fastconnect_6900;
    device_class_set_props(dc, qualcomm_fastconnect_6900_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo qualcomm_fastconnect_6900_info = {
    .name = TYPE_QUALCOMM_FASTCONNECT_6900,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(QualcommFastconnect6900State),
    .class_init = qualcomm_fastconnect_6900_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void qualcomm_fastconnect_6900_reg(void) { type_register_static(&qualcomm_fastconnect_6900_info); }
type_init(qualcomm_fastconnect_6900_reg)
/*
 * MediaTek MT7921K WiFi 6E
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_MEDIATEK_MT7921_WIFI "mediatek-mt7921-wifi"
OBJECT_DECLARE_SIMPLE_TYPE(MediatekMt7921WifiState, MEDIATEK_MT7921_WIFI)

struct MediatekMt7921WifiState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t mediatek_mt7921_wifi_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void mediatek_mt7921_wifi_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps mediatek_mt7921_wifi_ops = {
    .read = mediatek_mt7921_wifi_r, .write = mediatek_mt7921_wifi_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void mediatek_mt7921_wifi_realize(PCIDevice *pdev, Error **errp) {
    MediatekMt7921WifiState *s = MEDIATEK_MT7921_WIFI(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5315);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 1544);
    memory_region_init_io(&s->mmio, OBJECT(s), &mediatek_mt7921_wifi_ops, s,
                          "mediatek-mt7921-wifi-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property mediatek_mt7921_wifi_props[] = {
    DEFINE_PROP_STRING("device-name", MediatekMt7921WifiState, device_name),
};

static const VMStateDescription vmstate_mediatek_mt7921_wifi = {
    .name = "mediatek-mt7921-wifi",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, MediatekMt7921WifiState),
        VMSTATE_END_OF_LIST()
    },
};

static void mediatek_mt7921_wifi_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = mediatek_mt7921_wifi_realize;
    pc->vendor_id = 5315;
    pc->device_id = 1544;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "MediaTek MT7921K WiFi 6E";
    dc->vmsd = &vmstate_mediatek_mt7921_wifi;
    device_class_set_props(dc, mediatek_mt7921_wifi_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo mediatek_mt7921_wifi_info = {
    .name = TYPE_MEDIATEK_MT7921_WIFI,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(MediatekMt7921WifiState),
    .class_init = mediatek_mt7921_wifi_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void mediatek_mt7921_wifi_reg(void) { type_register_static(&mediatek_mt7921_wifi_info); }
type_init(mediatek_mt7921_wifi_reg)
/*
 * Intel AX210 WiFi 6E (PCIe)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_AX210_WIFI "intel-ax210-wifi"
OBJECT_DECLARE_SIMPLE_TYPE(IntelAx210WifiState, INTEL_AX210_WIFI)

struct IntelAx210WifiState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_ax210_wifi_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_ax210_wifi_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_ax210_wifi_ops = {
    .read = intel_ax210_wifi_r, .write = intel_ax210_wifi_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_ax210_wifi_realize(PCIDevice *pdev, Error **errp) {
    IntelAx210WifiState *s = INTEL_AX210_WIFI(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 36);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_ax210_wifi_ops, s,
                          "intel-ax210-wifi-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_ax210_wifi_props[] = {
    DEFINE_PROP_STRING("device-name", IntelAx210WifiState, device_name),
};

static const VMStateDescription vmstate_intel_ax210_wifi = {
    .name = "intel-ax210-wifi",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelAx210WifiState),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_ax210_wifi_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_ax210_wifi_realize;
    pc->vendor_id = 32902;
    pc->device_id = 10021;
    pc->class_id = 512;
    pc->revision = 26;
    dc->desc = "Intel AX210 WiFi 6E (PCIe)";
    dc->vmsd = &vmstate_intel_ax210_wifi;
    device_class_set_props(dc, intel_ax210_wifi_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_ax210_wifi_info = {
    .name = TYPE_INTEL_AX210_WIFI,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelAx210WifiState),
    .class_init = intel_ax210_wifi_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_ax210_wifi_reg(void) { type_register_static(&intel_ax210_wifi_info); }
type_init(intel_ax210_wifi_reg)
/*
 * Intel BE200 WiFi 7 (PCIe)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_BE200_WIFI "intel-be200-wifi"
OBJECT_DECLARE_SIMPLE_TYPE(IntelBe200WifiState, INTEL_BE200_WIFI)

struct IntelBe200WifiState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_be200_wifi_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_be200_wifi_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_be200_wifi_ops = {
    .read = intel_be200_wifi_r, .write = intel_be200_wifi_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_be200_wifi_realize(PCIDevice *pdev, Error **errp) {
    IntelBe200WifiState *s = INTEL_BE200_WIFI(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 36);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_be200_wifi_ops, s,
                          "intel-be200-wifi-mmio", 65536);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_be200_wifi_props[] = {
    DEFINE_PROP_STRING("device-name", IntelBe200WifiState, device_name),
};

static const VMStateDescription vmstate_intel_be200_wifi = {
    .name = "intel-be200-wifi",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelBe200WifiState),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_be200_wifi_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_be200_wifi_realize;
    pc->vendor_id = 32902;
    pc->device_id = 58432;
    pc->class_id = 512;
    pc->revision = 0;
    dc->desc = "Intel BE200 WiFi 7 (PCIe)";
    dc->vmsd = &vmstate_intel_be200_wifi;
    device_class_set_props(dc, intel_be200_wifi_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_be200_wifi_info = {
    .name = TYPE_INTEL_BE200_WIFI,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelBe200WifiState),
    .class_init = intel_be200_wifi_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_be200_wifi_reg(void) { type_register_static(&intel_be200_wifi_info); }
type_init(intel_be200_wifi_reg)
