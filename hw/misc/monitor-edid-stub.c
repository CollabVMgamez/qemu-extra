/*
 * Monitor EDID stub — exposes a configurable EDID via a DDC/CI PCI device.
 *
 * Real monitors expose EDID via I2C on the DDC2B bus (address 0x50).
 * This stub creates a PCI device that:
 *   1. Generates a proper 128-byte EDID block using QEMU's existing generator
 *   2. Exposes it via a small MMIO region (DDC-like register interface)
 *   3. Allows overriding all fields via device properties
 *
 * Tools that read EDID (MonInfo, Custom Resolution Utility, PowerStrip,
 * Windows Device Manager "Monitor" entries) will see the configured monitor.
 *
 * Usage:
 *   -device monitor-edid
 *   -device monitor-edid,vendor=SAM,name="Samsung U28E590",xres=3840,yres=2160
 *   -device monitor-edid,vendor=DEL,name="Dell P2419H",xres=1920,yres=1080,refresh_rate=60
 *
 * Common vendor IDs (UEFI PNP):
 *   SAM = Samsung,  DEL = Dell,    LGE = LG,       ACI = ASUS
 *   ACR = Acer,     HPN = HP,      BNQ = BenQ,     AOC = AOC
 *   PHL = Philips,  MSI = MSI,     NVD = NVIDIA    (for G-Sync monitors)
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/display/edid.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#include <string.h>

#define TYPE_MONITOR_EDID "monitor-edid"
OBJECT_DECLARE_SIMPLE_TYPE(MonitorEDIDState, MONITOR_EDID)

#define EDID_SIZE 256  /* standard EDID block */

/* DDC/CI register offsets in our MMIO region */
#define DDC_REG_CAPABILITIES  0x00  /* 32-bit: EDID size */
#define DDC_REG_EDID_DATA     0x04  /* EDID bytes follow from here */
#define DDC_REG_STATUS        0x01  /* ready status */

struct MonitorEDIDState {
    PCIDevice parent_obj;
    MemoryRegion mmio;

    /* EDID configuration properties */
    char    *vendor;       /* 3-char PNP manufacturer ID, e.g. "SAM" */
    char    *name;         /* Monitor name, max 13 chars */
    char    *serial;       /* Serial string */
    uint32_t xres;         /* Preferred horizontal resolution */
    uint32_t yres;         /* Preferred vertical resolution */
    uint32_t xmax;         /* Maximum horizontal resolution */
    uint32_t ymax;         /* Maximum vertical resolution */
    uint32_t refresh_rate; /* Preferred refresh rate (Hz) */
    uint32_t width_mm;     /* Physical width in mm */
    uint32_t height_mm;    /* Physical height in mm */

    /* Generated EDID block */
    uint8_t edid[EDID_SIZE];
};

static void monitor_edid_generate(MonitorEDIDState *s)
{
    qemu_edid_info info = {
        .vendor       = s->vendor ? s->vendor : "SAM",
        .name         = s->name ? s->name : "Samsung U28E590D",
        .serial       = s->serial ? s->serial : NULL,
        .width_mm     = s->width_mm  ? s->width_mm  : 620,
        .height_mm    = s->height_mm ? s->height_mm : 340,
        .prefx        = s->xres ? s->xres : 3840,
        .prefy        = s->yres ? s->yres : 2160,
        .maxx         = s->xmax ? s->xmax : 3840,
        .maxy         = s->ymax ? s->ymax : 2160,
        .refresh_rate = s->refresh_rate ? s->refresh_rate : 60,
    };
    qemu_edid_generate(s->edid, sizeof(s->edid), &info);
}

static uint64_t monitor_edid_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    MonitorEDIDState *s = opaque;

    if (addr == 0) {
        /* Capabilities: EDID present (bit 0), size */
        return 0x00000101 | (EDID_SIZE << 8);
    }
    if (addr >= 4 && addr < 4 + EDID_SIZE) {
        /* EDID data — return 4 bytes at a time */
        uint32_t off = addr - 4;
        if (off + 3 < EDID_SIZE) {
            return ((uint32_t)s->edid[off + 3] << 24) |
                   ((uint32_t)s->edid[off + 2] << 16) |
                   ((uint32_t)s->edid[off + 1] <<  8) |
                   ((uint32_t)s->edid[off]);
        }
    }
    return 0;
}

static void monitor_edid_mmio_write(void *opaque, hwaddr addr,
                                     uint64_t val, unsigned size)
{
    /* DDC is read-only from the host side */
}

static const MemoryRegionOps monitor_edid_ops = {
    .read  = monitor_edid_mmio_read,
    .write = monitor_edid_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void monitor_edid_realize(PCIDevice *pdev, Error **errp)
{
    MonitorEDIDState *s = MONITOR_EDID(pdev);

    /* Generate the EDID from our properties */
    monitor_edid_generate(s);

    /* PCI class: 0x0380 = Display controller, Other */
    pci_config_set_class(pdev->config, 0x0380);
    pdev->config[PCI_REVISION_ID] = 0x01;

    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 0x1AE0);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0x0001);

    /* BAR0: DDC/CI register region (4KB) */
    memory_region_init_io(&s->mmio, OBJECT(s), &monitor_edid_ops, s,
                          "monitor-edid-ddc", 4 * KiB);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->mmio);
}

static const Property monitor_edid_props[] = {
    DEFINE_PROP_STRING("vendor",       MonitorEDIDState, vendor),
    DEFINE_PROP_STRING("name",         MonitorEDIDState, name),
    DEFINE_PROP_STRING("serial",       MonitorEDIDState, serial),
    DEFINE_PROP_UINT32("xres",         MonitorEDIDState, xres,   3840),
    DEFINE_PROP_UINT32("yres",         MonitorEDIDState, yres,   2160),
    DEFINE_PROP_UINT32("xmax",         MonitorEDIDState, xmax,   3840),
    DEFINE_PROP_UINT32("ymax",         MonitorEDIDState, ymax,   2160),
    DEFINE_PROP_UINT32("refresh_rate", MonitorEDIDState, refresh_rate, 60),
    DEFINE_PROP_UINT32("width_mm",     MonitorEDIDState, width_mm,  620),
    DEFINE_PROP_UINT32("height_mm",    MonitorEDIDState, height_mm, 340),
};

static const VMStateDescription vmstate_monitor_edid = {
    .name = "monitor-edid", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, MonitorEDIDState),
        VMSTATE_UINT8_ARRAY(edid, MonitorEDIDState, EDID_SIZE),
        VMSTATE_END_OF_LIST()
    },
};

static void monitor_edid_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass    *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k  = PCI_DEVICE_CLASS(klass);

    k->realize   = monitor_edid_realize;
    k->vendor_id = 0x1AE0;   /* Google (virtio-compatible) */
    k->device_id = 0x1000;
    k->class_id  = 0x0380;
    k->revision  = 0x01;

    dc->desc         = "Monitor EDID stub (configurable vendor/resolution)";
    dc->vmsd         = &vmstate_monitor_edid;
    dc->hotpluggable = false;
    device_class_set_props(dc, monitor_edid_props);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo monitor_edid_info = {
    .name          = TYPE_MONITOR_EDID,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(MonitorEDIDState),
    .class_init    = monitor_edid_class_init,
    .interfaces    = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};

static void monitor_edid_register_types(void)
{
    type_register_static(&monitor_edid_info);
}
type_init(monitor_edid_register_types)
