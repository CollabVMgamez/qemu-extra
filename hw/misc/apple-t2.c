/*
 * Apple T2 Security Chip (PCI identity stub)
 *
 * The Apple T2 SoC appears in 2016-2020 Intel Macs as a PCI device
 * handling Secure Enclave, SSD controller, image signal processor,
 * audio codec and Thunderbolt security.  This stub presents the
 * correct PCI vendor/device IDs so Device Manager / AIDA64 can
 * identify the chip.
 *
 * Properties:
 *   chip-name — display name (default: "Apple T2 Security Chip")
 *
 * Usage:
 *   -device apple-t2
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

#define APPLE_VID  0x106B
#define T2_DID     0x1801
#define T2_SVID    0x106B
#define T2_SSID    0x1801
#define T2_REV     0x21
#define T2_CLASS   0x0C00
#define T2_BAR0_SZ (64 * 1024)

#define TYPE_APPLE_T2 "apple-t2"
OBJECT_DECLARE_SIMPLE_TYPE(AppleT2State, APPLE_T2)

struct AppleT2State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *chip_name;
};

static uint64_t t2_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    switch (addr & ~3ULL) {
    case 0x0000: return 0x00000001;
    case 0x0004: return 0x80000000;
    case 0x0010: return 0x00000200;
    default: return 0;
    }
}

static void t2_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps t2_mmio_ops = {
    .read = t2_mmio_read,
    .write = t2_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void apple_t2_realize(PCIDevice *pdev, Error **errp)
{
    AppleT2State *s = APPLE_T2(pdev);

    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, T2_SVID);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, T2_SSID);

    memory_region_init_io(&s->mmio, OBJECT(s), &t2_mmio_ops, s,
                          "apple-t2-mmio", T2_BAR0_SZ);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property apple_t2_props[] = {
    DEFINE_PROP_STRING("chip-name", AppleT2State, chip_name),
};

static const VMStateDescription vmstate_apple_t2 = {
    .name = "apple-t2",
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, AppleT2State),
        VMSTATE_END_OF_LIST()
    },
};

static void apple_t2_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(klass);

    pc->realize = apple_t2_realize;
    pc->vendor_id = APPLE_VID;
    pc->device_id = T2_DID;
    pc->class_id = T2_CLASS;
    pc->revision = T2_REV;
    dc->desc = "Apple T2 Security Chip";
    dc->vmsd = &vmstate_apple_t2;
    device_class_set_props(dc, apple_t2_props);
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo apple_t2_info = {
    .name = TYPE_APPLE_T2,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(AppleT2State),
    .class_init = apple_t2_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void apple_t2_register(void)
{
    type_register_static(&apple_t2_info);
}
type_init(apple_t2_register)
