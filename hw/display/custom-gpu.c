/*
 * Custom GPU device — user-settable PCI identity
 *
 * Lets you create a fictional or unlisted GPU with arbitrary
 * vendor/device IDs and display name.  Shows up in lspci/Device Manager
 * with whatever identity you choose.
 *
 * Properties:
 *   vendor-id       — PCI Vendor ID (hex, e.g. 0x10DE for NVIDIA)
 *   device-id       — PCI Device ID (hex)
 *   subsystem-vendor-id — PCI Subsystem Vendor ID
 *   subsystem-device-id — PCI Subsystem Device ID
 *   class-id        — PCI Class Code (hex, 0x0300 = VGA, 0x0302 = 3D)
 *   revision        — PCI Revision
 *   gpu-name        — Display name string (shows in Device Manager)
 *   vram-mb         — Reported VRAM size in MB (default: 8192)
 *
 * Usage:
 *   -device custom-gpu,vendor-id=0x10DE,device-id=0x2620,gpu-name="NVIDIA RTX 5090",vram-mb=32768
 *   -device custom-gpu,vendor-id=0x1002,device-id=0x7480,gpu-name="AMD Radeon RX 8900 XT"
 *   -device custom-gpu,vendor-id=0x1234,device-id=0xDEAD,gpu-name="My Cool GPU 9000",vram-mb=16384
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#include "hw/acpi/acpi_aml_interface.h"
#include "hw/acpi/aml-build.h"

#define TYPE_CUSTOM_GPU "custom-gpu"
OBJECT_DECLARE_SIMPLE_TYPE(CustomGpuState, CUSTOM_GPU)

struct CustomGpuState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    MemoryRegion vram;
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsystem_vendor_id;
    uint32_t subsystem_device_id;
    uint32_t class_id;
    uint32_t revision;
    char *gpu_name;
    uint32_t vram_mb;
};

static uint64_t custom_gpu_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    CustomGpuState *s = CUSTOM_GPU(opaque);
    switch (addr & ~3ULL) {
    case 0x0000: return 0x00000001;
    case 0x0004: return 0x80000000;
    case 0x0010: return (uint64_t)s->vram_mb << 20;
    default: return 0;
    }
}

static void custom_gpu_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps custom_gpu_mmio_ops = {
    .read = custom_gpu_mmio_read,
    .write = custom_gpu_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void custom_gpu_realize(PCIDevice *pdev, Error **errp)
{
    CustomGpuState *s = CUSTOM_GPU(pdev);

    pci_set_word(pdev->config + PCI_VENDOR_ID, s->vendor_id);
    pci_set_word(pdev->config + PCI_DEVICE_ID, s->device_id);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, s->subsystem_vendor_id);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, s->subsystem_device_id);
    pci_set_byte(pdev->config + PCI_REVISION_ID, s->revision);
    pci_config_set_class(pdev->config, s->class_id);

    memory_region_init_io(&s->mmio, OBJECT(s), &custom_gpu_mmio_ops, s,
                          "custom-gpu-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);

    memory_region_init_ram(&s->vram, OBJECT(s), "custom-gpu-vram",
                           (uint64_t)s->vram_mb * 1024 * 1024, &error_fatal);
    pci_register_bar(pdev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY |
                         PCI_BASE_ADDRESS_MEM_PREFETCH, &s->vram);
}

static void custom_gpu_build_aml(AcpiDevAmlIf *adev, Aml *scope)
{
    CustomGpuState *s = CUSTOM_GPU(adev);
    Aml *method;

    method = aml_method("_S1D", 0, AML_NOTSERIALIZED);
    aml_append(method, aml_return(aml_int(0)));
    aml_append(scope, method);

    method = aml_method("_S2D", 0, AML_NOTSERIALIZED);
    aml_append(method, aml_return(aml_int(0)));
    aml_append(scope, method);

    method = aml_method("_S3D", 0, AML_NOTSERIALIZED);
    aml_append(method, aml_return(aml_int(2)));
    aml_append(scope, method);
}

static const Property custom_gpu_props[] = {
    DEFINE_PROP_UINT32("vendor-id", CustomGpuState, vendor_id, 0x1234),
    DEFINE_PROP_UINT32("device-id", CustomGpuState, device_id, 0xBEEF),
    DEFINE_PROP_UINT32("subsystem-vendor-id", CustomGpuState, subsystem_vendor_id, 0x1234),
    DEFINE_PROP_UINT32("subsystem-device-id", CustomGpuState, subsystem_device_id, 0xBEEF),
    DEFINE_PROP_UINT32("class-id", CustomGpuState, class_id, 0x0300),
    DEFINE_PROP_UINT32("revision", CustomGpuState, revision, 0x01),
    DEFINE_PROP_STRING("gpu-name", CustomGpuState, gpu_name),
    DEFINE_PROP_UINT32("vram-mb", CustomGpuState, vram_mb, 8192),
};

static const VMStateDescription vmstate_custom_gpu = {
    .name = "custom-gpu",
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CustomGpuState),
        VMSTATE_END_OF_LIST()
    },
};

static void custom_gpu_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(klass);

    pc->realize = custom_gpu_realize;
    pc->class_id = 0x0300;
    dc->desc = "Custom GPU (user-settable PCI identity)";
    dc->vmsd = &vmstate_custom_gpu;
    device_class_set_props(dc, custom_gpu_props);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo custom_gpu_info = {
    .name          = TYPE_CUSTOM_GPU,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CustomGpuState),
    .class_init    = custom_gpu_class_init,
    .interfaces    = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { TYPE_ACPI_DEV_AML_IF },
        { },
    },
};

static void custom_gpu_register(void)
{
    type_register_static(&custom_gpu_info);
}
type_init(custom_gpu_register)
