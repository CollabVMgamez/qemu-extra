/*
 * Infineon SLB9670 TPM 2.0 (SPI)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INFINEON_SLB9670_TPM2 "infineon-slb9670-tpm2"
OBJECT_DECLARE_SIMPLE_TYPE(InfineonSlb9670Tpm2State, INFINEON_SLB9670_TPM2)

struct InfineonSlb9670Tpm2State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t infineon_slb9670_tpm2_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void infineon_slb9670_tpm2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps infineon_slb9670_tpm2_ops = {
    .read = infineon_slb9670_tpm2_r, .write = infineon_slb9670_tpm2_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void infineon_slb9670_tpm2_realize(PCIDevice *pdev, Error **errp) {
    InfineonSlb9670Tpm2State *s = INFINEON_SLB9670_TPM2(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 7359);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 12330);
    memory_region_init_io(&s->mmio, OBJECT(s), &infineon_slb9670_tpm2_ops, s,
                          "infineon-slb9670-tpm2-mmio", 4096);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property infineon_slb9670_tpm2_props[] = {
    DEFINE_PROP_STRING("device-name", InfineonSlb9670Tpm2State, device_name),
};

static const VMStateDescription vmstate_infineon_slb9670_tpm2 = {
    .name = "infineon-slb9670-tpm2",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, InfineonSlb9670Tpm2State),
        VMSTATE_END_OF_LIST()
    },
};

static void infineon_slb9670_tpm2_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = infineon_slb9670_tpm2_realize;
    pc->vendor_id = 7359;
    pc->device_id = 12330;
    pc->class_id = 3072;
    pc->revision = 1;
    dc->desc = "Infineon SLB9670 TPM 2.0 (SPI)";
    dc->vmsd = &vmstate_infineon_slb9670_tpm2;
    device_class_set_props(dc, infineon_slb9670_tpm2_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo infineon_slb9670_tpm2_info = {
    .name = TYPE_INFINEON_SLB9670_TPM2,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(InfineonSlb9670Tpm2State),
    .class_init = infineon_slb9670_tpm2_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void infineon_slb9670_tpm2_reg(void) { type_register_static(&infineon_slb9670_tpm2_info); }
type_init(infineon_slb9670_tpm2_reg)
/*
 * Nuvoton NPCT75x TPM 2.0 (SPI)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_NUVOTON_NPCT75X_TPM2 "nuvoton-npct75x-tpm2"
OBJECT_DECLARE_SIMPLE_TYPE(NuvotonNpct75xTpm2State, NUVOTON_NPCT75X_TPM2)

struct NuvotonNpct75xTpm2State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t nuvoton_npct75x_tpm2_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void nuvoton_npct75x_tpm2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps nuvoton_npct75x_tpm2_ops = {
    .read = nuvoton_npct75x_tpm2_r, .write = nuvoton_npct75x_tpm2_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void nuvoton_npct75x_tpm2_realize(PCIDevice *pdev, Error **errp) {
    NuvotonNpct75xTpm2State *s = NUVOTON_NPCT75X_TPM2(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4176);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 1281);
    memory_region_init_io(&s->mmio, OBJECT(s), &nuvoton_npct75x_tpm2_ops, s,
                          "nuvoton-npct75x-tpm2-mmio", 4096);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property nuvoton_npct75x_tpm2_props[] = {
    DEFINE_PROP_STRING("device-name", NuvotonNpct75xTpm2State, device_name),
};

static const VMStateDescription vmstate_nuvoton_npct75x_tpm2 = {
    .name = "nuvoton-npct75x-tpm2",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, NuvotonNpct75xTpm2State),
        VMSTATE_END_OF_LIST()
    },
};

static void nuvoton_npct75x_tpm2_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = nuvoton_npct75x_tpm2_realize;
    pc->vendor_id = 4176;
    pc->device_id = 1281;
    pc->class_id = 3072;
    pc->revision = 6;
    dc->desc = "Nuvoton NPCT75x TPM 2.0 (SPI)";
    dc->vmsd = &vmstate_nuvoton_npct75x_tpm2;
    device_class_set_props(dc, nuvoton_npct75x_tpm2_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo nuvoton_npct75x_tpm2_info = {
    .name = TYPE_NUVOTON_NPCT75X_TPM2,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NuvotonNpct75xTpm2State),
    .class_init = nuvoton_npct75x_tpm2_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void nuvoton_npct75x_tpm2_reg(void) { type_register_static(&nuvoton_npct75x_tpm2_info); }
type_init(nuvoton_npct75x_tpm2_reg)
/*
 * STMicro ST33HTPH TPM 2.0 (SPI)
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_STMICRO_ST33HTPM2 "stmicro-st33htpm2"
OBJECT_DECLARE_SIMPLE_TYPE(StmicroSt33htpm2State, STMICRO_ST33HTPM2)

struct StmicroSt33htpm2State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t stmicro_st33htpm2_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void stmicro_st33htpm2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps stmicro_st33htpm2_ops = {
    .read = stmicro_st33htpm2_r, .write = stmicro_st33htpm2_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void stmicro_st33htpm2_realize(PCIDevice *pdev, Error **errp) {
    StmicroSt33htpm2State *s = STMICRO_ST33HTPM2(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 4170);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 52235);
    memory_region_init_io(&s->mmio, OBJECT(s), &stmicro_st33htpm2_ops, s,
                          "stmicro-st33htpm2-mmio", 4096);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property stmicro_st33htpm2_props[] = {
    DEFINE_PROP_STRING("device-name", StmicroSt33htpm2State, device_name),
};

static const VMStateDescription vmstate_stmicro_st33htpm2 = {
    .name = "stmicro-st33htpm2",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, StmicroSt33htpm2State),
        VMSTATE_END_OF_LIST()
    },
};

static void stmicro_st33htpm2_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = stmicro_st33htpm2_realize;
    pc->vendor_id = 4170;
    pc->device_id = 52235;
    pc->class_id = 3072;
    pc->revision = 3;
    dc->desc = "STMicro ST33HTPH TPM 2.0 (SPI)";
    dc->vmsd = &vmstate_stmicro_st33htpm2;
    device_class_set_props(dc, stmicro_st33htpm2_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo stmicro_st33htpm2_info = {
    .name = TYPE_STMICRO_ST33HTPM2,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(StmicroSt33htpm2State),
    .class_init = stmicro_st33htpm2_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void stmicro_st33htpm2_reg(void) { type_register_static(&stmicro_st33htpm2_info); }
type_init(stmicro_st33htpm2_reg)
/*
 * Intel PTT (fTPM) TPM 2.0
 * Hardware identity stub — correct PCI IDs for Device Manager/AIDA64.
 */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_PTT_TPM2 "intel-ptt-tpm2"
OBJECT_DECLARE_SIMPLE_TYPE(IntelPttTpm2State, INTEL_PTT_TPM2)

struct IntelPttTpm2State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *device_name;
};

static uint64_t intel_ptt_tpm2_r(void *o, hwaddr a, unsigned s) {
    switch (a & ~3ULL) {
    case 0x0000: return 0x00000001;
    default: return 0;
    }
}
static void intel_ptt_tpm2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_ptt_tpm2_ops = {
    .read = intel_ptt_tpm2_r, .write = intel_ptt_tpm2_w,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void intel_ptt_tpm2_realize(PCIDevice *pdev, Error **errp) {
    IntelPttTpm2State *s = INTEL_PTT_TPM2(pdev);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 32902);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 41242);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_ptt_tpm2_ops, s,
                          "intel-ptt-tpm2-mmio", 4096);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property intel_ptt_tpm2_props[] = {
    DEFINE_PROP_STRING("device-name", IntelPttTpm2State, device_name),
};

static const VMStateDescription vmstate_intel_ptt_tpm2 = {
    .name = "intel-ptt-tpm2",
    .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelPttTpm2State),
        VMSTATE_END_OF_LIST()
    },
};

static void intel_ptt_tpm2_ci(ObjectClass *k, const void *d) {
    DeviceClass *dc = DEVICE_CLASS(k);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(k);
    pc->realize = intel_ptt_tpm2_realize;
    pc->vendor_id = 32902;
    pc->device_id = 41242;
    pc->class_id = 3072;
    pc->revision = 0;
    dc->desc = "Intel PTT (fTPM) TPM 2.0";
    dc->vmsd = &vmstate_intel_ptt_tpm2;
    device_class_set_props(dc, intel_ptt_tpm2_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo intel_ptt_tpm2_info = {
    .name = TYPE_INTEL_PTT_TPM2,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelPttTpm2State),
    .class_init = intel_ptt_tpm2_ci,
    .interfaces = (InterfaceInfo[]) { { INTERFACE_CONVENTIONAL_PCI_DEVICE }, {} },
};

static void intel_ptt_tpm2_reg(void) { type_register_static(&intel_ptt_tpm2_info); }
type_init(intel_ptt_tpm2_reg)
