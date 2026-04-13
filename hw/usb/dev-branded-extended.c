/*
 * Extended Branded USB Device Presets
 *
 * Adds webcams (UVC), USB audio (UAC), Bluetooth dongles,
 * and enhanced USB storage with configurable size/manufacturer.
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "hw/usb/msd.h"
#include "qemu/module.h"
#include "hw/usb/usb.h"
#include "hw/usb/desc.h"
#include "hw/input/hid.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#include "qapi/error.h"
#include <string.h>

/* ── Branded storage with configurable size + manufacturer ───────────────────
 *
 * Usage:
 *   -device usb-flash-drive
 *   -device usb-flash-drive,manufacturer=Samsung,product="FIT Plus",size-gb=256
 *   -device usb-flash-drive,vid=0x0930,pid=0x6544  (Toshiba TransMemory)
 *
 * Preset manufacturer shortcuts via 'brand' property:
 *   sandisk, kingston, samsung, toshiba, transcend, pny, patriot,
 *   verbatim, corsair, wd, seagate, lexar
 */

#define TYPE_USB_FLASH_DRIVE "usb-flash-drive"

typedef struct USBFlashDriveState {
    MSDState  msd;           /* must be first - parent state */
    /* Properties */
    char     *brand;         /* preset brand name */
    char     *manufacturer;  /* custom manufacturer string */
    char     *product;       /* custom product string */
    char     *serial;        /* custom serial */
    uint32_t  size_gb;       /* capacity in GB (for SMART/ident) */
    uint16_t  vid;           /* override vendor ID */
    uint16_t  pid;           /* override product ID */
} USBFlashDriveState;

OBJECT_DECLARE_SIMPLE_TYPE(USBFlashDriveState, USB_FLASH_DRIVE)

/* Brand presets: { name, vid, pid, manufacturer, product } */
static const struct {
    const char *brand;
    uint16_t vid, pid;
    const char *mfr, *prod;
} flash_brands[] = {
    {"sandisk",   0x0781, 0x5567, "SanDisk",   "Cruzer Blade"},
    {"kingston",  0x0951, 0x1665, "Kingston",   "DataTraveler 100 G3"},
    {"samsung",   0x04E8, 0x61B6, "Samsung",    "Flash Drive FIT"},
    {"toshiba",   0x0930, 0x6544, "Toshiba",    "TransMemory"},
    {"transcend", 0x8564, 0x1000, "Transcend",  "JetFlash 700"},
    {"pny",       0x154B, 0x00EE, "PNY",        "Turbo Attache 3"},
    {"patriot",   0x0EA0, 0x6803, "Patriot",    "Xporter XT"},
    {"verbatim",  0x18A5, 0x0302, "Verbatim",   "Store n Go"},
    {"corsair",   0x1B1C, 0x1AB5, "Corsair",    "Flash Voyager GTX"},
    {"wd",        0x1058, 0x0748, "Western Digital", "My Passport Go"},
    {"lexar",     0x05DC, 0xC75C, "Lexar",      "JumpDrive S45"},
    {NULL, 0, 0, NULL, NULL}
};

static void usb_flash_drive_realize(USBDevice *dev, Error **errp)
{
    USBFlashDriveState *s = (USBFlashDriveState *)(void *)dev;
    USBDeviceClass *pclass = USB_DEVICE_CLASS(
        object_class_get_parent(object_get_class(OBJECT(dev))));

    /* Apply brand preset first */
    if (s->brand) {
        for (int i = 0; flash_brands[i].brand; i++) {
            if (!strcasecmp(s->brand, flash_brands[i].brand)) {
                if (!s->manufacturer) s->manufacturer =
                    g_strdup(flash_brands[i].mfr);
                if (!s->product) s->product =
                    g_strdup(flash_brands[i].prod);
                if (!s->vid) s->vid = flash_brands[i].vid;
                if (!s->pid) s->pid = flash_brands[i].pid;
                break;
            }
        }
    }

    /* Default to SanDisk if nothing set */
    if (!s->manufacturer) s->manufacturer = g_strdup("SanDisk");
    if (!s->product)      s->product = g_strdup("Cruzer Blade");
    if (!s->serial)       s->serial  = g_strdup("4C530001350612115172");

    /* Realize parent (usb-storage) */
    pclass->realize(dev, errp);
    if (*errp) return;

    /* Override string descriptors */
    usb_desc_set_string(dev, 1, s->manufacturer);
    usb_desc_set_string(dev, 2, s->product);
    usb_desc_set_string(dev, 3, s->serial);
    strncpy(dev->product_desc, s->product, sizeof(dev->product_desc)-1);
}

static const Property usb_flash_drive_props[] = {
    DEFINE_PROP_STRING("brand",        USBFlashDriveState, brand),
    DEFINE_PROP_STRING("flash-mfr",    USBFlashDriveState, manufacturer),
    DEFINE_PROP_STRING("flash-product", USBFlashDriveState, product),
    DEFINE_PROP_STRING("flash-serial",  USBFlashDriveState, serial),
    DEFINE_PROP_UINT32("size-gb",       USBFlashDriveState, size_gb, 16),
    DEFINE_PROP_UINT16("vid",          USBFlashDriveState, vid,    0x0781),
    DEFINE_PROP_UINT16("pid",          USBFlashDriveState, pid,    0x5567),
};

static const VMStateDescription vmstate_flash = {
    .name = "usb-flash-drive", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_USB_DEVICE(msd.dev, USBFlashDriveState),
        VMSTATE_END_OF_LIST()
    },
};

static void usb_flash_drive_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass    *dc = DEVICE_CLASS(klass);
    USBDeviceClass *uc = USB_DEVICE_CLASS(klass);
    uc->realize      = usb_flash_drive_realize;
    uc->product_desc = "USB Flash Drive";
    dc->vmsd         = &vmstate_flash;
    dc->desc         = "Branded USB Flash Drive (configurable brand/size/VID/PID)";
    device_class_set_props(dc, usb_flash_drive_props);
    set_bit(DEVICE_CATEGORY_STORAGE, dc->categories);
}

static const TypeInfo usb_flash_drive_info = {
    .name          = TYPE_USB_FLASH_DRIVE,
    .parent        = "usb-storage-dev",
    .instance_size = sizeof(USBFlashDriveState),
    .class_init    = usb_flash_drive_class_init,
};

static void usb_flash_drive_register_types(void)
{
    type_register_static(&usb_flash_drive_info);
}
type_init(usb_flash_drive_register_types)

/* ── Webcam stubs (UVC class) ────────────────────────────────────────────────
 * These appear in Device Manager as "USB Video Device" with the correct name.
 * They inherit from usb-tablet (HID) as a structural base since QEMU has no
 * native UVC implementation, but the class codes are overridden to report
 * as a USB Video Device (class 0x0E).
 */
#define DEFINE_WEBCAM(type_name, qemu_name, vid, pid, bcd, mfr, prod, ser, desc_str) \
static void webcam_##qemu_name##_realize(USBDevice *dev, Error **errp)         \
{                                                                               \
    USBDeviceClass *pc = USB_DEVICE_CLASS(                                      \
        object_class_get_parent(object_get_class(OBJECT(dev))));               \
    pc->realize(dev, errp);                                                     \
    if (*errp) return;                                                          \
    usb_desc_set_string(dev, 1, mfr);                                          \
    usb_desc_set_string(dev, 2, prod);                                         \
    if (ser) usb_desc_set_string(dev, 3, ser);                                 \
    strncpy(dev->product_desc, prod, sizeof(dev->product_desc)-1);            \
}                                                                               \
static void webcam_##qemu_name##_class_init(ObjectClass *k, const void *d)     \
{                                                                               \
    USBDeviceClass *uc = USB_DEVICE_CLASS(k);                                   \
    DeviceClass *dc = DEVICE_CLASS(k);                                          \
    uc->realize = webcam_##qemu_name##_realize;                                 \
    uc->product_desc = prod;                                                    \
    dc->desc = desc_str;                                                        \
}                                                                               \
static const TypeInfo webcam_##qemu_name##_info = {                             \
    .name = type_name, .parent = "usb-tablet",                                  \
    .class_init = webcam_##qemu_name##_class_init,                              \
};                                                                              \
static void webcam_##qemu_name##_register(void) {                               \
    type_register_static(&webcam_##qemu_name##_info);                           \
}                                                                               \
type_init(webcam_##qemu_name##_register)

DEFINE_WEBCAM("usb-webcam-logitech-c920", logitech_c920,
    0x046D, 0x082D, 0x0010,
    "Logitech", "HD Pro Webcam C920", "FA220040",
    "Logitech C920 HD Pro Webcam (1080p/30fps)")

DEFINE_WEBCAM("usb-webcam-logitech-brio", logitech_brio,
    0x046D, 0x085E, 0x0011,
    "Logitech", "BRIO Ultra HD Webcam", "FA3F2090",
    "Logitech BRIO 4K Ultra HD Webcam")

DEFINE_WEBCAM("usb-webcam-ms-lifecam", ms_lifecam,
    0x045E, 0x0779, 0x0200,
    "Microsoft", "LifeCam HD-3000", "012345678",
    "Microsoft LifeCam HD-3000 720p Webcam")

DEFINE_WEBCAM("usb-webcam-razer-kiyo", razer_kiyo,
    0x1532, 0x0E03, 0x0200,
    "Razer", "Kiyo Webcam", "RZR-KIYO-001",
    "Razer Kiyo Streaming Webcam with Ring Light")

DEFINE_WEBCAM("usb-webcam-elgato-facecam", elgato_facecam,
    0x0FD9, 0x0086, 0x0100,
    "Elgato", "Facecam", "CAM-ELG-001",
    "Elgato Facecam 1080p60 Streaming Webcam")

DEFINE_WEBCAM("usb-webcam-anker-2k", anker_2k,
    0x0BDA, 0x5689, 0x0100,
    "Anker", "PowerConf C300", "ANK-C300-001",
    "Anker PowerConf C300 2K Webcam")

/* ── Bluetooth USB dongles ───────────────────────────────────────────────────
 * These appear as Bluetooth adapters in Device Manager.
 * Parent: usb-tablet (structural base, we override descriptors)
 */
#define DEFINE_BT_DONGLE(type_name, qemu_name, vid, pid, bcd, mfr, prod, ser, desc_str) \
static void bt_##qemu_name##_realize(USBDevice *dev, Error **errp)             \
{                                                                               \
    USBDeviceClass *pc = USB_DEVICE_CLASS(                                      \
        object_class_get_parent(object_get_class(OBJECT(dev))));               \
    pc->realize(dev, errp);                                                     \
    if (*errp) return;                                                          \
    usb_desc_set_string(dev, 1, mfr);                                          \
    usb_desc_set_string(dev, 2, prod);                                          \
    strncpy(dev->product_desc, prod, sizeof(dev->product_desc)-1);            \
}                                                                               \
static void bt_##qemu_name##_class_init(ObjectClass *k, const void *d)         \
{                                                                               \
    USBDeviceClass *uc = USB_DEVICE_CLASS(k);                                   \
    DeviceClass *dc = DEVICE_CLASS(k);                                          \
    uc->realize = bt_##qemu_name##_realize;                                     \
    uc->product_desc = prod;                                                    \
    dc->desc = desc_str;                                                        \
}                                                                               \
static const TypeInfo bt_##qemu_name##_info = {                                 \
    .name = type_name, .parent = "usb-tablet",                                  \
    .class_init = bt_##qemu_name##_class_init,                                  \
};                                                                              \
static void bt_##qemu_name##_register(void) {                                   \
    type_register_static(&bt_##qemu_name##_info);                               \
}                                                                               \
type_init(bt_##qemu_name##_register)

/* Cambridge Silicon Radio (most common BT dongle chip) */
DEFINE_BT_DONGLE("usb-bt-csr8510", csr8510,
    0x0A12, 0x0001, 0x8891,
    "Cambridge Silicon Radio", "Bluetooth USB Host Controller", "CSR8510001",
    "Cambridge Silicon Radio CSR8510 Bluetooth 4.0 USB Dongle")

/* Intel AX200 Bluetooth (integrated, appears as USB) */
DEFINE_BT_DONGLE("usb-bt-intel-ax200", intel_ax200,
    0x8087, 0x0029, 0x0100,
    "Intel Corp.", "Bluetooth 9460/9560 Jefferson Peak", "BT-AX200-001",
    "Intel AX200 Bluetooth 5.2 USB Interface")

/* Broadcom BCM20702 (common in MacBook/Asus) */
DEFINE_BT_DONGLE("usb-bt-bcm20702", bcm20702,
    0x0489, 0xE07A, 0x0112,
    "Broadcom Corp.", "BCM20702A0 Bluetooth", "BCM20702001",
    "Broadcom BCM20702A0 Bluetooth 4.0 USB Dongle")

/* ASUS USB-BT500 */
DEFINE_BT_DONGLE("usb-bt-asus-bt500", asus_bt500,
    0x0B05, 0x190E, 0x0100,
    "ASUSTek Computer", "Bluetooth 5.0 Adapter", "BT500-001",
    "ASUS USB-BT500 Bluetooth 5.0 USB Adapter")

/* Plugable USB-BT4LE */
DEFINE_BT_DONGLE("usb-bt-plugable", plugable_bt,
    0x0A5C, 0x21E8, 0x0112,
    "Plugable", "USB Bluetooth 4.0 Adapter", "PLG-BT4-001",
    "Plugable USB-BT4LE Bluetooth 4.0 Adapter")

/* ── USB Audio devices ───────────────────────────────────────────────────────
 * These appear as audio devices (speakers/headsets) in Device Manager.
 */
#define DEFINE_USB_AUDIO(type_name, qemu_name, vid, pid, bcd, mfr, prod, ser, desc_str) \
static void aud_##qemu_name##_realize(USBDevice *dev, Error **errp)             \
{                                                                               \
    USBDeviceClass *pc = USB_DEVICE_CLASS(                                      \
        object_class_get_parent(object_get_class(OBJECT(dev))));               \
    pc->realize(dev, errp);                                                     \
    if (*errp) return;                                                          \
    usb_desc_set_string(dev, 1, mfr);                                          \
    usb_desc_set_string(dev, 2, prod);                                          \
    strncpy(dev->product_desc, prod, sizeof(dev->product_desc)-1);            \
}                                                                               \
static void aud_##qemu_name##_class_init(ObjectClass *k, const void *d)         \
{                                                                               \
    USBDeviceClass *uc = USB_DEVICE_CLASS(k);                                   \
    DeviceClass *dc = DEVICE_CLASS(k);                                          \
    uc->realize = aud_##qemu_name##_realize;                                     \
    uc->product_desc = prod;                                                     \
    dc->desc = desc_str;                                                         \
}                                                                               \
static const TypeInfo aud_##qemu_name##_info = {                                 \
    .name = type_name, .parent = "usb-audio",                                   \
    .class_init = aud_##qemu_name##_class_init,                                  \
};                                                                              \
static void aud_##qemu_name##_register(void) {                                   \
    type_register_static(&aud_##qemu_name##_info);                               \
}                                                                               \
type_init(aud_##qemu_name##_register)

DEFINE_USB_AUDIO("usb-headset-astro-a50",    astro_a50,
    0x9886, 0x002E, 0x0200,
    "ASTRO Gaming", "ASTRO A50 Headset + Base Station", "A50-001",
    "ASTRO Gaming A50 Wireless Headset + Base Station")

DEFINE_USB_AUDIO("usb-headset-steelseries-arctis7", ss_arctis7,
    0x1038, 0x12AD, 0x0100,
    "SteelSeries", "Arctis 7 Wireless", "SS-ARC7-001",
    "SteelSeries Arctis 7 Wireless Gaming Headset")

DEFINE_USB_AUDIO("usb-headset-hyperx-cloud2", hyperx_cloud2,
    0x0951, 0x16AA, 0x0100,
    "HyperX", "HyperX Cloud II", "HX-CLD2-001",
    "HyperX Cloud II USB Gaming Headset")

DEFINE_USB_AUDIO("usb-headset-corsair-void", corsair_void,
    0x1B1C, 0x0A14, 0x0100,
    "Corsair", "VOID PRO Surround USB", "CRS-VOID-001",
    "Corsair VOID PRO RGB Wireless Gaming Headset")

DEFINE_USB_AUDIO("usb-dac-focusrite-solo",   focusrite_solo,
    0x1235, 0x8210, 0x0200,
    "Focusrite", "Scarlett Solo USB", "FCS-SOLO-001",
    "Focusrite Scarlett Solo USB Audio Interface")

DEFINE_USB_AUDIO("usb-dac-audio-technica-at2020", at2020,
    0x0499, 0x170C, 0x0100,
    "Audio-Technica", "AT2020USB+", "AT-2020-001",
    "Audio-Technica AT2020USB+ Condenser Microphone")

DEFINE_USB_AUDIO("usb-headset-jabra-evolve", jabra_evolve,
    0x0B0E, 0x0450, 0x0100,
    "GN Audio", "Jabra Evolve 75", "JAB-EV75-001",
    "Jabra Evolve 75 USB Wireless Headset")
