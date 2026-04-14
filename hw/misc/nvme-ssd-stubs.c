/*
 * Samsung SSD 970 EVO Plus 1TB (NVMe)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_970_EVO_PLUS "samsung-970-evo-plus"
OBJECT_DECLARE_SIMPLE_TYPE(Samsung970EvoPlusState, SAMSUNG_970_EVO_PLUS)

struct Samsung970EvoPlusState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t samsung_970_evo_plus_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void samsung_970_evo_plus_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps samsung_970_evo_plus_ops = {
    .read = samsung_970_evo_plus_r, .write = samsung_970_evo_plus_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void samsung_970_evo_plus_realize(PCIDevice *pdev, Error **errp) {
    Samsung970EvoPlusState *s = SAMSUNG_970_EVO_PLUS(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5197);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 43016);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_970_evo_plus_ops, s,
                          "samsung-970-evo-plus-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property samsung_970_evo_plus_props[] = {
    DEFINE_PROP_STRING("device-name", Samsung970EvoPlusState, device_name),
};

static const VMStateDescription vmstate_samsung_970_evo_plus = {
    .name = "samsung-970-evo-plus",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Samsung970EvoPlusState),
        VMSTATE_END_OF_LIST()
    },
};

static void samsung_970_evo_plus_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = samsung_970_evo_plus_realize;
    pc->vendor_id = 5197;
    pc->device_id = 43016;
    pc->class_id = 264;
    pc->revision = 1;
    dc->desc = "Samsung SSD 970 EVO Plus 1TB (NVMe)";
    dc->vmsd = &vmstate_samsung_970_evo_plus;
    device_class_set_props(dc, samsung_970_evo_plus_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo samsung_970_evo_plus_info = {
    .name = TYPE_SAMSUNG_970_EVO_PLUS,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Samsung970EvoPlusState),
    .class_init = samsung_970_evo_plus_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void samsung_970_evo_plus_reg(void) { type_register_static(&samsung_970_evo_plus_info); }
type_init(samsung_970_evo_plus_reg)
/*
 * Samsung SSD 980 PRO 1TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_980_PRO "samsung-980-pro"
OBJECT_DECLARE_SIMPLE_TYPE(Samsung980ProState, SAMSUNG_980_PRO)

struct Samsung980ProState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t samsung_980_pro_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void samsung_980_pro_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps samsung_980_pro_ops = {
    .read = samsung_980_pro_r, .write = samsung_980_pro_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void samsung_980_pro_realize(PCIDevice *pdev, Error **errp) {
    Samsung980ProState *s = SAMSUNG_980_PRO(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5197);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 43018);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_980_pro_ops, s,
                          "samsung-980-pro-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property samsung_980_pro_props[] = {
    DEFINE_PROP_STRING("device-name", Samsung980ProState, device_name),
};

static const VMStateDescription vmstate_samsung_980_pro = {
    .name = "samsung-980-pro",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Samsung980ProState),
        VMSTATE_END_OF_LIST()
    },
};

static void samsung_980_pro_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = samsung_980_pro_realize;
    pc->vendor_id = 5197;
    pc->device_id = 43018;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "Samsung SSD 980 PRO 1TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_samsung_980_pro;
    device_class_set_props(dc, samsung_980_pro_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo samsung_980_pro_info = {
    .name = TYPE_SAMSUNG_980_PRO,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Samsung980ProState),
    .class_init = samsung_980_pro_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void samsung_980_pro_reg(void) { type_register_static(&samsung_980_pro_info); }
type_init(samsung_980_pro_reg)
/*
 * Samsung SSD 990 PRO 2TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_990_PRO "samsung-990-pro"
OBJECT_DECLARE_SIMPLE_TYPE(Samsung990ProState, SAMSUNG_990_PRO)

struct Samsung990ProState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t samsung_990_pro_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void samsung_990_pro_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps samsung_990_pro_ops = {
    .read = samsung_990_pro_r, .write = samsung_990_pro_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void samsung_990_pro_realize(PCIDevice *pdev, Error **errp) {
    Samsung990ProState *s = SAMSUNG_990_PRO(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5197);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 43017);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_990_pro_ops, s,
                          "samsung-990-pro-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property samsung_990_pro_props[] = {
    DEFINE_PROP_STRING("device-name", Samsung990ProState, device_name),
};

static const VMStateDescription vmstate_samsung_990_pro = {
    .name = "samsung-990-pro",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Samsung990ProState),
        VMSTATE_END_OF_LIST()
    },
};

static void samsung_990_pro_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = samsung_990_pro_realize;
    pc->vendor_id = 5197;
    pc->device_id = 43017;
    pc->class_id = 264;
    pc->revision = 1;
    dc->desc = "Samsung SSD 990 PRO 2TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_samsung_990_pro;
    device_class_set_props(dc, samsung_990_pro_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo samsung_990_pro_info = {
    .name = TYPE_SAMSUNG_990_PRO,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Samsung990ProState),
    .class_init = samsung_990_pro_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void samsung_990_pro_reg(void) { type_register_static(&samsung_990_pro_info); }
type_init(samsung_990_pro_reg)
/*
 * Samsung PM9A1 1TB (NVMe OEM, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_PM9A1 "samsung-pm9a1"
OBJECT_DECLARE_SIMPLE_TYPE(SamsungPm9a1State, SAMSUNG_PM9A1)

struct SamsungPm9a1State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t samsung_pm9a1_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void samsung_pm9a1_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps samsung_pm9a1_ops = {
    .read = samsung_pm9a1_r, .write = samsung_pm9a1_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void samsung_pm9a1_realize(PCIDevice *pdev, Error **errp) {
    SamsungPm9a1State *s = SAMSUNG_PM9A1(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5197);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 43021);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_pm9a1_ops, s,
                          "samsung-pm9a1-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property samsung_pm9a1_props[] = {
    DEFINE_PROP_STRING("device-name", SamsungPm9a1State, device_name),
};

static const VMStateDescription vmstate_samsung_pm9a1 = {
    .name = "samsung-pm9a1",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SamsungPm9a1State),
        VMSTATE_END_OF_LIST()
    },
};

static void samsung_pm9a1_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = samsung_pm9a1_realize;
    pc->vendor_id = 5197;
    pc->device_id = 43021;
    pc->class_id = 264;
    pc->revision = 1;
    dc->desc = "Samsung PM9A1 1TB (NVMe OEM, PCIe 4.0)";
    dc->vmsd = &vmstate_samsung_pm9a1;
    device_class_set_props(dc, samsung_pm9a1_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo samsung_pm9a1_info = {
    .name = TYPE_SAMSUNG_PM9A1,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SamsungPm9a1State),
    .class_init = samsung_pm9a1_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void samsung_pm9a1_reg(void) { type_register_static(&samsung_pm9a1_info); }
type_init(samsung_pm9a1_reg)
/*
 * Samsung SSD 980 1TB (NVMe, PCIe 3.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_980 "samsung-980"
OBJECT_DECLARE_SIMPLE_TYPE(Samsung980State, SAMSUNG_980)

struct Samsung980State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t samsung_980_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void samsung_980_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps samsung_980_ops = {
    .read = samsung_980_r, .write = samsung_980_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void samsung_980_realize(PCIDevice *pdev, Error **errp) {
    Samsung980State *s = SAMSUNG_980(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 5197);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 43014);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_980_ops, s,
                          "samsung-980-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property samsung_980_props[] = {
    DEFINE_PROP_STRING("device-name", Samsung980State, device_name),
};

static const VMStateDescription vmstate_samsung_980 = {
    .name = "samsung-980",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Samsung980State),
        VMSTATE_END_OF_LIST()
    },
};

static void samsung_980_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = samsung_980_realize;
    pc->vendor_id = 5197;
    pc->device_id = 43014;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "Samsung SSD 980 1TB (NVMe, PCIe 3.0)";
    dc->vmsd = &vmstate_samsung_980;
    device_class_set_props(dc, samsung_980_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo samsung_980_info = {
    .name = TYPE_SAMSUNG_980,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Samsung980State),
    .class_init = samsung_980_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void samsung_980_reg(void) { type_register_static(&samsung_980_info); }
type_init(samsung_980_reg)
/*
 * WD Black SN770 1TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_WD_BLACK_SN770 "wd-black-sn770"
OBJECT_DECLARE_SIMPLE_TYPE(WdBlackSn770State, WD_BLACK_SN770)

struct WdBlackSn770State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t wd_black_sn770_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void wd_black_sn770_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps wd_black_sn770_ops = {
    .read = wd_black_sn770_r, .write = wd_black_sn770_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void wd_black_sn770_realize(PCIDevice *pdev, Error **errp) {
    WdBlackSn770State *s = WD_BLACK_SN770(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 6987);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 4868);
    memory_region_init_io(&s->mmio, OBJECT(s), &wd_black_sn770_ops, s,
                          "wd-black-sn770-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property wd_black_sn770_props[] = {
    DEFINE_PROP_STRING("device-name", WdBlackSn770State, device_name),
};

static const VMStateDescription vmstate_wd_black_sn770 = {
    .name = "wd-black-sn770",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, WdBlackSn770State),
        VMSTATE_END_OF_LIST()
    },
};

static void wd_black_sn770_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = wd_black_sn770_realize;
    pc->vendor_id = 6987;
    pc->device_id = 4868;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "WD Black SN770 1TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_wd_black_sn770;
    device_class_set_props(dc, wd_black_sn770_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo wd_black_sn770_info = {
    .name = TYPE_WD_BLACK_SN770,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(WdBlackSn770State),
    .class_init = wd_black_sn770_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void wd_black_sn770_reg(void) { type_register_static(&wd_black_sn770_info); }
type_init(wd_black_sn770_reg)
/*
 * WD Black SN850X 2TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_WD_BLACK_SN850X "wd-black-sn850x"
OBJECT_DECLARE_SIMPLE_TYPE(WdBlackSn850xState, WD_BLACK_SN850X)

struct WdBlackSn850xState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t wd_black_sn850x_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void wd_black_sn850x_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps wd_black_sn850x_ops = {
    .read = wd_black_sn850x_r, .write = wd_black_sn850x_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void wd_black_sn850x_realize(PCIDevice *pdev, Error **errp) {
    WdBlackSn850xState *s = WD_BLACK_SN850X(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 6987);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 4934);
    memory_region_init_io(&s->mmio, OBJECT(s), &wd_black_sn850x_ops, s,
                          "wd-black-sn850x-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property wd_black_sn850x_props[] = {
    DEFINE_PROP_STRING("device-name", WdBlackSn850xState, device_name),
};

static const VMStateDescription vmstate_wd_black_sn850x = {
    .name = "wd-black-sn850x",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, WdBlackSn850xState),
        VMSTATE_END_OF_LIST()
    },
};

static void wd_black_sn850x_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = wd_black_sn850x_realize;
    pc->vendor_id = 6987;
    pc->device_id = 4934;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "WD Black SN850X 2TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_wd_black_sn850x;
    device_class_set_props(dc, wd_black_sn850x_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo wd_black_sn850x_info = {
    .name = TYPE_WD_BLACK_SN850X,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(WdBlackSn850xState),
    .class_init = wd_black_sn850x_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void wd_black_sn850x_reg(void) { type_register_static(&wd_black_sn850x_info); }
type_init(wd_black_sn850x_reg)
/*
 * WD Blue SN570 1TB (NVMe, PCIe 3.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_WD_BLUE_SN570 "wd-blue-sn570"
OBJECT_DECLARE_SIMPLE_TYPE(WdBlueSn570State, WD_BLUE_SN570)

struct WdBlueSn570State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t wd_blue_sn570_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void wd_blue_sn570_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps wd_blue_sn570_ops = {
    .read = wd_blue_sn570_r, .write = wd_blue_sn570_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void wd_blue_sn570_realize(PCIDevice *pdev, Error **errp) {
    WdBlueSn570State *s = WD_BLUE_SN570(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 6987);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 4873);
    memory_region_init_io(&s->mmio, OBJECT(s), &wd_blue_sn570_ops, s,
                          "wd-blue-sn570-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property wd_blue_sn570_props[] = {
    DEFINE_PROP_STRING("device-name", WdBlueSn570State, device_name),
};

static const VMStateDescription vmstate_wd_blue_sn570 = {
    .name = "wd-blue-sn570",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, WdBlueSn570State),
        VMSTATE_END_OF_LIST()
    },
};

static void wd_blue_sn570_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = wd_blue_sn570_realize;
    pc->vendor_id = 6987;
    pc->device_id = 4873;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "WD Blue SN570 1TB (NVMe, PCIe 3.0)";
    dc->vmsd = &vmstate_wd_blue_sn570;
    device_class_set_props(dc, wd_blue_sn570_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo wd_blue_sn570_info = {
    .name = TYPE_WD_BLUE_SN570,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(WdBlueSn570State),
    .class_init = wd_blue_sn570_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void wd_blue_sn570_reg(void) { type_register_static(&wd_blue_sn570_info); }
type_init(wd_blue_sn570_reg)
/*
 * SK Hynix Platinum P41 2TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SK_HYNIX_P41_PLATINUM "sk-hynix-p41-platinum"
OBJECT_DECLARE_SIMPLE_TYPE(SkHynixP41PlatinumState, SK_HYNIX_P41_PLATINUM)

struct SkHynixP41PlatinumState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t sk_hynix_p41_platinum_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void sk_hynix_p41_platinum_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps sk_hynix_p41_platinum_ops = {
    .read = sk_hynix_p41_platinum_r, .write = sk_hynix_p41_platinum_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void sk_hynix_p41_platinum_realize(PCIDevice *pdev, Error **errp) {
    SkHynixP41PlatinumState *s = SK_HYNIX_P41_PLATINUM(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 7260);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 132);
    memory_region_init_io(&s->mmio, OBJECT(s), &sk_hynix_p41_platinum_ops, s,
                          "sk-hynix-p41-platinum-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property sk_hynix_p41_platinum_props[] = {
    DEFINE_PROP_STRING("device-name", SkHynixP41PlatinumState, device_name),
};

static const VMStateDescription vmstate_sk_hynix_p41_platinum = {
    .name = "sk-hynix-p41-platinum",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SkHynixP41PlatinumState),
        VMSTATE_END_OF_LIST()
    },
};

static void sk_hynix_p41_platinum_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = sk_hynix_p41_platinum_realize;
    pc->vendor_id = 7260;
    pc->device_id = 132;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "SK Hynix Platinum P41 2TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_sk_hynix_p41_platinum;
    device_class_set_props(dc, sk_hynix_p41_platinum_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo sk_hynix_p41_platinum_info = {
    .name = TYPE_SK_HYNIX_P41_PLATINUM,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SkHynixP41PlatinumState),
    .class_init = sk_hynix_p41_platinum_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void sk_hynix_p41_platinum_reg(void) { type_register_static(&sk_hynix_p41_platinum_info); }
type_init(sk_hynix_p41_platinum_reg)
/*
 * SK Hynix Gold P31 1TB (NVMe, PCIe 3.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SK_HYNIX_GOLD_P31 "sk-hynix-gold-p31"
OBJECT_DECLARE_SIMPLE_TYPE(SkHynixGoldP31State, SK_HYNIX_GOLD_P31)

struct SkHynixGoldP31State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t sk_hynix_gold_p31_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void sk_hynix_gold_p31_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps sk_hynix_gold_p31_ops = {
    .read = sk_hynix_gold_p31_r, .write = sk_hynix_gold_p31_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void sk_hynix_gold_p31_realize(PCIDevice *pdev, Error **errp) {
    SkHynixGoldP31State *s = SK_HYNIX_GOLD_P31(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 7260);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 104);
    memory_region_init_io(&s->mmio, OBJECT(s), &sk_hynix_gold_p31_ops, s,
                          "sk-hynix-gold-p31-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property sk_hynix_gold_p31_props[] = {
    DEFINE_PROP_STRING("device-name", SkHynixGoldP31State, device_name),
};

static const VMStateDescription vmstate_sk_hynix_gold_p31 = {
    .name = "sk-hynix-gold-p31",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SkHynixGoldP31State),
        VMSTATE_END_OF_LIST()
    },
};

static void sk_hynix_gold_p31_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = sk_hynix_gold_p31_realize;
    pc->vendor_id = 7260;
    pc->device_id = 104;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "SK Hynix Gold P31 1TB (NVMe, PCIe 3.0)";
    dc->vmsd = &vmstate_sk_hynix_gold_p31;
    device_class_set_props(dc, sk_hynix_gold_p31_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo sk_hynix_gold_p31_info = {
    .name = TYPE_SK_HYNIX_GOLD_P31,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SkHynixGoldP31State),
    .class_init = sk_hynix_gold_p31_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void sk_hynix_gold_p31_reg(void) { type_register_static(&sk_hynix_gold_p31_info); }
type_init(sk_hynix_gold_p31_reg)
/*
 * Crucial T700 2TB (NVMe, PCIe 5.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CRUCIAL_T700 "crucial-t700"
OBJECT_DECLARE_SIMPLE_TYPE(CrucialT700State, CRUCIAL_T700)

struct CrucialT700State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t crucial_t700_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void crucial_t700_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps crucial_t700_ops = {
    .read = crucial_t700_r, .write = crucial_t700_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void crucial_t700_realize(PCIDevice *pdev, Error **errp) {
    CrucialT700State *s = CRUCIAL_T700(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 49321);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 21505);
    memory_region_init_io(&s->mmio, OBJECT(s), &crucial_t700_ops, s,
                          "crucial-t700-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property crucial_t700_props[] = {
    DEFINE_PROP_STRING("device-name", CrucialT700State, device_name),
};

static const VMStateDescription vmstate_crucial_t700 = {
    .name = "crucial-t700",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CrucialT700State),
        VMSTATE_END_OF_LIST()
    },
};

static void crucial_t700_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = crucial_t700_realize;
    pc->vendor_id = 49321;
    pc->device_id = 21505;
    pc->class_id = 264;
    pc->revision = 1;
    dc->desc = "Crucial T700 2TB (NVMe, PCIe 5.0)";
    dc->vmsd = &vmstate_crucial_t700;
    device_class_set_props(dc, crucial_t700_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo crucial_t700_info = {
    .name = TYPE_CRUCIAL_T700,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CrucialT700State),
    .class_init = crucial_t700_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void crucial_t700_reg(void) { type_register_static(&crucial_t700_info); }
type_init(crucial_t700_reg)
/*
 * Crucial P5 Plus 1TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CRUCIAL_P5_PLUS "crucial-p5-plus"
OBJECT_DECLARE_SIMPLE_TYPE(CrucialP5PlusState, CRUCIAL_P5_PLUS)

struct CrucialP5PlusState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t crucial_p5_plus_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void crucial_p5_plus_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps crucial_p5_plus_ops = {
    .read = crucial_p5_plus_r, .write = crucial_p5_plus_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void crucial_p5_plus_realize(PCIDevice *pdev, Error **errp) {
    CrucialP5PlusState *s = CRUCIAL_P5_PLUS(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 49321);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 20504);
    memory_region_init_io(&s->mmio, OBJECT(s), &crucial_p5_plus_ops, s,
                          "crucial-p5-plus-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property crucial_p5_plus_props[] = {
    DEFINE_PROP_STRING("device-name", CrucialP5PlusState, device_name),
};

static const VMStateDescription vmstate_crucial_p5_plus = {
    .name = "crucial-p5-plus",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CrucialP5PlusState),
        VMSTATE_END_OF_LIST()
    },
};

static void crucial_p5_plus_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = crucial_p5_plus_realize;
    pc->vendor_id = 49321;
    pc->device_id = 20504;
    pc->class_id = 264;
    pc->revision = 1;
    dc->desc = "Crucial P5 Plus 1TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_crucial_p5_plus;
    device_class_set_props(dc, crucial_p5_plus_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo crucial_p5_plus_info = {
    .name = TYPE_CRUCIAL_P5_PLUS,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CrucialP5PlusState),
    .class_init = crucial_p5_plus_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void crucial_p5_plus_reg(void) { type_register_static(&crucial_p5_plus_info); }
type_init(crucial_p5_plus_reg)
/*
 * Intel SSD 670p 1TB (NVMe, PCIe 3.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_670P "intel-670p"
OBJECT_DECLARE_SIMPLE_TYPE(Intel670pState, INTEL_670P)

struct Intel670pState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_670p_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_670p_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_670p_ops = {
    .read = intel_670p_r, .write = intel_670p_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_670p_realize(PCIDevice *pdev, Error **errp) {
    Intel670pState *s = INTEL_670P(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 61861);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_670p_ops, s,
                          "intel-670p-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_670p_props[] = {
    DEFINE_PROP_STRING("device-name", Intel670pState, device_name),
};

static const VMStateDescription vmstate_intel_670p = {
    .name = "intel-670p",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Intel670pState),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_670p_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_670p_realize;
    pc->vendor_id = 32902;
    pc->device_id = 61861;
    pc->class_id = 264;
    pc->revision = 3;
    dc->desc = "Intel SSD 670p 1TB (NVMe, PCIe 3.0)";
    dc->vmsd = &vmstate_intel_670p;
    device_class_set_props(dc, intel_670p_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_670p_info = {
    .name = TYPE_INTEL_670P,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Intel670pState),
    .class_init = intel_670p_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_670p_reg(void) { type_register_static(&intel_670p_info); }
type_init(intel_670p_reg)
/*
 * Intel SSD 760p 512GB (NVMe, PCIe 3.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_760P "intel-760p"
OBJECT_DECLARE_SIMPLE_TYPE(Intel760pState, INTEL_760P)

struct Intel760pState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_760p_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_760p_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_760p_ops = {
    .read = intel_760p_r, .write = intel_760p_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_760p_realize(PCIDevice *pdev, Error **errp) {
    Intel760pState *s = INTEL_760P(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 61864);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_760p_ops, s,
                          "intel-760p-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_760p_props[] = {
    DEFINE_PROP_STRING("device-name", Intel760pState, device_name),
};

static const VMStateDescription vmstate_intel_760p = {
    .name = "intel-760p",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Intel760pState),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_760p_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_760p_realize;
    pc->vendor_id = 32902;
    pc->device_id = 61864;
    pc->class_id = 264;
    pc->revision = 2;
    dc->desc = "Intel SSD 760p 512GB (NVMe, PCIe 3.0)";
    dc->vmsd = &vmstate_intel_760p;
    device_class_set_props(dc, intel_760p_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_760p_info = {
    .name = TYPE_INTEL_760P,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Intel760pState),
    .class_init = intel_760p_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_760p_reg(void) { type_register_static(&intel_760p_info); }
type_init(intel_760p_reg)
/*
 * Kingston FURY Renegade 2TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_KINGSTON_FURY_RENEGADE "kingston-fury-renegade"
OBJECT_DECLARE_SIMPLE_TYPE(KingstonFuryRenegadeState, KINGSTON_FURY_RENEGADE)

struct KingstonFuryRenegadeState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t kingston_fury_renegade_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void kingston_fury_renegade_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps kingston_fury_renegade_ops = {
    .read = kingston_fury_renegade_r, .write = kingston_fury_renegade_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void kingston_fury_renegade_realize(PCIDevice *pdev, Error **errp) {
    KingstonFuryRenegadeState *s = KINGSTON_FURY_RENEGADE(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 9798);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 20498);
    memory_region_init_io(&s->mmio, OBJECT(s), &kingston_fury_renegade_ops, s,
                          "kingston-fury-renegade-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property kingston_fury_renegade_props[] = {
    DEFINE_PROP_STRING("device-name", KingstonFuryRenegadeState, device_name),
};

static const VMStateDescription vmstate_kingston_fury_renegade = {
    .name = "kingston-fury-renegade",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, KingstonFuryRenegadeState),
        VMSTATE_END_OF_LIST()
    },
};

static void kingston_fury_renegade_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = kingston_fury_renegade_realize;
    pc->vendor_id = 9798;
    pc->device_id = 20498;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "Kingston FURY Renegade 2TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_kingston_fury_renegade;
    device_class_set_props(dc, kingston_fury_renegade_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo kingston_fury_renegade_info = {
    .name = TYPE_KINGSTON_FURY_RENEGADE,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(KingstonFuryRenegadeState),
    .class_init = kingston_fury_renegade_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void kingston_fury_renegade_reg(void) { type_register_static(&kingston_fury_renegade_info); }
type_init(kingston_fury_renegade_reg)
/*
 * Seagate FireCuda 530 2TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SEAGATE_FIRECUDA_530 "seagate-firecuda-530"
OBJECT_DECLARE_SIMPLE_TYPE(SeagateFirecuda530State, SEAGATE_FIRECUDA_530)

struct SeagateFirecuda530State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t seagate_firecuda_530_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void seagate_firecuda_530_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps seagate_firecuda_530_ops = {
    .read = seagate_firecuda_530_r, .write = seagate_firecuda_530_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void seagate_firecuda_530_realize(PCIDevice *pdev, Error **errp) {
    SeagateFirecuda530State *s = SEAGATE_FIRECUDA_530(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 6966);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 15);
    memory_region_init_io(&s->mmio, OBJECT(s), &seagate_firecuda_530_ops, s,
                          "seagate-firecuda-530-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property seagate_firecuda_530_props[] = {
    DEFINE_PROP_STRING("device-name", SeagateFirecuda530State, device_name),
};

static const VMStateDescription vmstate_seagate_firecuda_530 = {
    .name = "seagate-firecuda-530",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SeagateFirecuda530State),
        VMSTATE_END_OF_LIST()
    },
};

static void seagate_firecuda_530_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = seagate_firecuda_530_realize;
    pc->vendor_id = 6966;
    pc->device_id = 15;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "Seagate FireCuda 530 2TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_seagate_firecuda_530;
    device_class_set_props(dc, seagate_firecuda_530_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo seagate_firecuda_530_info = {
    .name = TYPE_SEAGATE_FIRECUDA_530,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SeagateFirecuda530State),
    .class_init = seagate_firecuda_530_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void seagate_firecuda_530_reg(void) { type_register_static(&seagate_firecuda_530_info); }
type_init(seagate_firecuda_530_reg)
/*
 * Corsair MP600 PRO 2TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CORSAIR_MP600_PRO "corsair-mp600-pro"
OBJECT_DECLARE_SIMPLE_TYPE(CorsairMp600ProState, CORSAIR_MP600_PRO)

struct CorsairMp600ProState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t corsair_mp600_pro_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void corsair_mp600_pro_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps corsair_mp600_pro_ops = {
    .read = corsair_mp600_pro_r, .write = corsair_mp600_pro_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void corsair_mp600_pro_realize(PCIDevice *pdev, Error **errp) {
    CorsairMp600ProState *s = CORSAIR_MP600_PRO(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 7260);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 5945);
    memory_region_init_io(&s->mmio, OBJECT(s), &corsair_mp600_pro_ops, s,
                          "corsair-mp600-pro-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property corsair_mp600_pro_props[] = {
    DEFINE_PROP_STRING("device-name", CorsairMp600ProState, device_name),
};

static const VMStateDescription vmstate_corsair_mp600_pro = {
    .name = "corsair-mp600-pro",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CorsairMp600ProState),
        VMSTATE_END_OF_LIST()
    },
};

static void corsair_mp600_pro_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = corsair_mp600_pro_realize;
    pc->vendor_id = 7260;
    pc->device_id = 5945;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "Corsair MP600 PRO 2TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_corsair_mp600_pro;
    device_class_set_props(dc, corsair_mp600_pro_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo corsair_mp600_pro_info = {
    .name = TYPE_CORSAIR_MP600_PRO,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CorsairMp600ProState),
    .class_init = corsair_mp600_pro_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void corsair_mp600_pro_reg(void) { type_register_static(&corsair_mp600_pro_info); }
type_init(corsair_mp600_pro_reg)
/*
 * Sabrent Rocket 4 Plus 2TB (NVMe, PCIe 4.0)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SABRENT_ROCKET_4_PLUS "sabrent-rocket-4-plus"
OBJECT_DECLARE_SIMPLE_TYPE(SabrentRocket4PlusState, SABRENT_ROCKET_4_PLUS)

struct SabrentRocket4PlusState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t sabrent_rocket_4_plus_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void sabrent_rocket_4_plus_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps sabrent_rocket_4_plus_ops = {
    .read = sabrent_rocket_4_plus_r, .write = sabrent_rocket_4_plus_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void sabrent_rocket_4_plus_realize(PCIDevice *pdev, Error **errp) {
    SabrentRocket4PlusState *s = SABRENT_ROCKET_4_PLUS(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 6535);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 20502);
    memory_region_init_io(&s->mmio, OBJECT(s), &sabrent_rocket_4_plus_ops, s,
                          "sabrent-rocket-4-plus-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property sabrent_rocket_4_plus_props[] = {
    DEFINE_PROP_STRING("device-name", SabrentRocket4PlusState, device_name),
};

static const VMStateDescription vmstate_sabrent_rocket_4_plus = {
    .name = "sabrent-rocket-4-plus",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SabrentRocket4PlusState),
        VMSTATE_END_OF_LIST()
    },
};

static void sabrent_rocket_4_plus_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = sabrent_rocket_4_plus_realize;
    pc->vendor_id = 6535;
    pc->device_id = 20502;
    pc->class_id = 264;
    pc->revision = 0;
    dc->desc = "Sabrent Rocket 4 Plus 2TB (NVMe, PCIe 4.0)";
    dc->vmsd = &vmstate_sabrent_rocket_4_plus;
    device_class_set_props(dc, sabrent_rocket_4_plus_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo sabrent_rocket_4_plus_info = {
    .name = TYPE_SABRENT_ROCKET_4_PLUS,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SabrentRocket4PlusState),
    .class_init = sabrent_rocket_4_plus_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void sabrent_rocket_4_plus_reg(void) { type_register_static(&sabrent_rocket_4_plus_info); }
type_init(sabrent_rocket_4_plus_reg)
