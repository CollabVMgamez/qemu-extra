/*
 * Intel Management Engine (ME) stub device
 * Appears as PCI communication controller (class 0x0780).
 * Required for some BIOS/UEFI firmware to not complain about missing ME.
 * Does not implement any actual ME functionality.
 *
 * Usage: -device intel-me
 *        -device intel-me,me-gen=skylake   (default)
 *        Valid me-gen values: ich8 ich9 p67 z77 haswell skylake alderlake
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/units.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_ME "intel-me"
OBJECT_DECLARE_SIMPLE_TYPE(IntelMEState, INTEL_ME)

/* ME device IDs per generation */
static const struct { const char *name; uint16_t devid; } me_gen_table[] = {
    { "ich8",      0x2974 },
    { "ich9",      0x2944 },
    { "p67",       0x1C3A },
    { "z77",       0x1E3A },
    { "haswell",   0x8C3A },
    { "skylake",   0xA13A },  /* default */
    { "alderlake", 0x7AE8 },
    { "raptorlake",0x7F70 },
    { NULL, 0 }
};

struct IntelMEState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *me_gen;  /* generation string */
};

static uint64_t intel_me_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    /* ME firmware status register at offset 0 — return "ME ready" */
    if (addr == 0) return 0x00001111; /* FWSTS1: ME in normal mode */
    if (addr == 4) return 0x00000000; /* FWSTS2 */
    return 0;
}
static void intel_me_mmio_write(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_me_ops = {
    .read=intel_me_mmio_read, .write=intel_me_mmio_write,
    .endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};

static void intel_me_realize(PCIDevice *pdev, Error **errp)
{
    IntelMEState *s = INTEL_ME(pdev);
    uint16_t devid = 0xA13A; /* skylake default */

    if (s->me_gen) {
        for (int i = 0; me_gen_table[i].name; i++) {
            if (strcmp(s->me_gen, me_gen_table[i].name) == 0) {
                devid = me_gen_table[i].devid;
                break;
            }
        }
    }

    /* Patch device ID based on generation */
    pci_config_set_device_id(pdev->config, devid);
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 0x8086);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, devid);

    memory_region_init_io(&s->mmio, OBJECT(s), &intel_me_ops, s,
                          "intel-me-mmio", 4 * KiB);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->mmio);
}

static const Property intel_me_props[] = {
    DEFINE_PROP_STRING("me-gen", IntelMEState, me_gen),
};
static const VMStateDescription vmstate_intel_me = {
    .name="intel-me",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelMEState),VMSTATE_END_OF_LIST()},
};
static void intel_me_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize    = intel_me_realize;
    k->vendor_id  = 0x8086;
    k->device_id  = 0xA13A;   /* Skylake ME, overridden in realize */
    k->class_id   = 0x0780;   /* Communication Controller: Other */
    k->revision   = 0x01;
    dc->desc         = "Intel Management Engine (ME) stub";
    dc->vmsd         = &vmstate_intel_me;
    dc->hotpluggable = false;
    device_class_set_props(dc, intel_me_props);
}
static const TypeInfo intel_me_info = {
    .name=TYPE_INTEL_ME,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelMEState),.class_init=intel_me_class_init,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void intel_me_register(void){type_register_static(&intel_me_info);}
type_init(intel_me_register)
