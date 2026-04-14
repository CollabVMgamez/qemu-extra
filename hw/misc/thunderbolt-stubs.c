/*
 * Intel JHL7540 Thunderbolt 3
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_JHL7540_TB3 "intel-jhl7540-tb3"
OBJECT_DECLARE_SIMPLE_TYPE(IntelJhl7540Tb3State, INTEL_JHL7540_TB3)

struct IntelJhl7540Tb3State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_jhl7540_tb3_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_jhl7540_tb3_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_jhl7540_tb3_ops = {
    .read = intel_jhl7540_tb3_r, .write = intel_jhl7540_tb3_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_jhl7540_tb3_realize(PCIDevice *pdev, Error **errp) {
    IntelJhl7540Tb3State *s = INTEL_JHL7540_TB3(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 8738);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_jhl7540_tb3_ops, s,
                          "intel-jhl7540-tb3-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_jhl7540_tb3_props[] = {
    DEFINE_PROP_STRING("device-name", IntelJhl7540Tb3State, device_name),
};

static const VMStateDescription vmstate_intel_jhl7540_tb3 = {
    .name = "intel-jhl7540-tb3",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelJhl7540Tb3State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_jhl7540_tb3_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_jhl7540_tb3_realize;
    pc->vendor_id = 32902;
    pc->device_id = 5610;
    pc->class_id = 65280;
    pc->revision = 3;
    dc->desc = "Intel JHL7540 Thunderbolt 3";
    dc->vmsd = &vmstate_intel_jhl7540_tb3;
    device_class_set_props(dc, intel_jhl7540_tb3_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_jhl7540_tb3_info = {
    .name = TYPE_INTEL_JHL7540_TB3,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelJhl7540Tb3State),
    .class_init = intel_jhl7540_tb3_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_jhl7540_tb3_reg(void) { type_register_static(&intel_jhl7540_tb3_info); }
type_init(intel_jhl7540_tb3_reg)
/*
 * Intel JHL8540 Thunderbolt 4
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_JHL8540_TB4 "intel-jhl8540-tb4"
OBJECT_DECLARE_SIMPLE_TYPE(IntelJhl8540Tb4State, INTEL_JHL8540_TB4)

struct IntelJhl8540Tb4State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_jhl8540_tb4_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_jhl8540_tb4_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_jhl8540_tb4_ops = {
    .read = intel_jhl8540_tb4_r, .write = intel_jhl8540_tb4_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_jhl8540_tb4_realize(PCIDevice *pdev, Error **errp) {
    IntelJhl8540Tb4State *s = INTEL_JHL8540_TB4(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 8738);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_jhl8540_tb4_ops, s,
                          "intel-jhl8540-tb4-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_jhl8540_tb4_props[] = {
    DEFINE_PROP_STRING("device-name", IntelJhl8540Tb4State, device_name),
};

static const VMStateDescription vmstate_intel_jhl8540_tb4 = {
    .name = "intel-jhl8540-tb4",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelJhl8540Tb4State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_jhl8540_tb4_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_jhl8540_tb4_realize;
    pc->vendor_id = 32902;
    pc->device_id = 5616;
    pc->class_id = 65280;
    pc->revision = 0;
    dc->desc = "Intel JHL8540 Thunderbolt 4";
    dc->vmsd = &vmstate_intel_jhl8540_tb4;
    device_class_set_props(dc, intel_jhl8540_tb4_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_jhl8540_tb4_info = {
    .name = TYPE_INTEL_JHL8540_TB4,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelJhl8540Tb4State),
    .class_init = intel_jhl8540_tb4_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_jhl8540_tb4_reg(void) { type_register_static(&intel_jhl8540_tb4_info); }
type_init(intel_jhl8540_tb4_reg)
/*
 * Intel Maple Ridge Thunderbolt 4
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_MAPLE_RIDGE_TB4 "intel-maple-ridge-tb4"
OBJECT_DECLARE_SIMPLE_TYPE(IntelMapleRidgeTb4State, INTEL_MAPLE_RIDGE_TB4)

struct IntelMapleRidgeTb4State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_maple_ridge_tb4_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_maple_ridge_tb4_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_maple_ridge_tb4_ops = {
    .read = intel_maple_ridge_tb4_r, .write = intel_maple_ridge_tb4_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_maple_ridge_tb4_realize(PCIDevice *pdev, Error **errp) {
    IntelMapleRidgeTb4State *s = INTEL_MAPLE_RIDGE_TB4(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 8738);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_maple_ridge_tb4_ops, s,
                          "intel-maple-ridge-tb4-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_maple_ridge_tb4_props[] = {
    DEFINE_PROP_STRING("device-name", IntelMapleRidgeTb4State, device_name),
};

static const VMStateDescription vmstate_intel_maple_ridge_tb4 = {
    .name = "intel-maple-ridge-tb4",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelMapleRidgeTb4State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_maple_ridge_tb4_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_maple_ridge_tb4_realize;
    pc->vendor_id = 32902;
    pc->device_id = 4403;
    pc->class_id = 65280;
    pc->revision = 0;
    dc->desc = "Intel Maple Ridge Thunderbolt 4";
    dc->vmsd = &vmstate_intel_maple_ridge_tb4;
    device_class_set_props(dc, intel_maple_ridge_tb4_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_maple_ridge_tb4_info = {
    .name = TYPE_INTEL_MAPLE_RIDGE_TB4,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelMapleRidgeTb4State),
    .class_init = intel_maple_ridge_tb4_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_maple_ridge_tb4_reg(void) { type_register_static(&intel_maple_ridge_tb4_info); }
type_init(intel_maple_ridge_tb4_reg)
/*
 * ASMedia ASM4242 Thunderbolt 4
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_ASMEDIA_ASM4242_TB4 "asmedia-asm4242-tb4"
OBJECT_DECLARE_SIMPLE_TYPE(AsmediaAsm4242Tb4State, ASMEDIA_ASM4242_TB4)

struct AsmediaAsm4242Tb4State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t asmedia_asm4242_tb4_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void asmedia_asm4242_tb4_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps asmedia_asm4242_tb4_ops = {
    .read = asmedia_asm4242_tb4_r, .write = asmedia_asm4242_tb4_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void asmedia_asm4242_tb4_realize(PCIDevice *pdev, Error **errp) {
    AsmediaAsm4242Tb4State *s = ASMEDIA_ASM4242_TB4(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 6945);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 16962);
    memory_region_init_io(&s->mmio, OBJECT(s), &asmedia_asm4242_tb4_ops, s,
                          "asmedia-asm4242-tb4-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property asmedia_asm4242_tb4_props[] = {
    DEFINE_PROP_STRING("device-name", AsmediaAsm4242Tb4State, device_name),
};

static const VMStateDescription vmstate_asmedia_asm4242_tb4 = {
    .name = "asmedia-asm4242-tb4",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, AsmediaAsm4242Tb4State),
        VMSTATE_END_OF_LIST()
    },
};

static void asmedia_asm4242_tb4_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = asmedia_asm4242_tb4_realize;
    pc->vendor_id = 6945;
    pc->device_id = 16962;
    pc->class_id = 65280;
    pc->revision = 0;
    dc->desc = "ASMedia ASM4242 Thunderbolt 4";
    dc->vmsd = &vmstate_asmedia_asm4242_tb4;
    device_class_set_props(dc, asmedia_asm4242_tb4_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo asmedia_asm4242_tb4_info = {
    .name = TYPE_ASMEDIA_ASM4242_TB4,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(AsmediaAsm4242Tb4State),
    .class_init = asmedia_asm4242_tb4_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void asmedia_asm4242_tb4_reg(void) { type_register_static(&asmedia_asm4242_tb4_info); }
type_init(asmedia_asm4242_tb4_reg)
