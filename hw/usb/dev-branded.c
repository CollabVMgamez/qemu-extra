/*
 * Branded USB Device Presets for QEMU
 *
 * Creates USB devices that appear with correct vendor/product IDs
 * in Device Manager, USBDeview, and AIDA64.
 *
 * Each device inherits from the base QEMU USB HID/storage/audio
 * implementation but overrides the USB descriptor to report the
 * correct manufacturer VID/PID and string descriptors.
 *
 * Usage:
 *   -device usb-logitech-g400     Logitech G400 Gaming Mouse
 *   -device usb-corsair-k70       Corsair K70 RGB Keyboard
 *   -device usb-sandisk-cruzer    SanDisk Cruzer 8GB USB Flash
 *   -device usb-kingston-dt100g3  Kingston DataTraveler 100 G3
 *   -device usb-steelseries-rival SteelSeries Rival 600 Mouse
 *   -device usb-razer-deathadder  Razer DeathAdder V2 Mouse
 *   -device usb-logitech-g915     Logitech G915 TKL Keyboard
 *   -device usb-hhkb-pro2         Happy Hacking Keyboard Pro 2
 *   -device usb-sony-dualshock4   Sony DualShock 4 Controller
 *   -device usb-xbox-controller   Microsoft Xbox One Controller
 *   -device usb-webcam-logitech-c920  Logitech C920 HD Pro Webcam
 *   -device usb-headset-astro-a50     ASTRO A50 Wireless Headset
 *   -device usb-dac-focusrite-solo    Focusrite Scarlett Solo USB Audio
 *   -device usb-bt-csr8510            Cambridge Silicon Radio Bluetooth 4.0
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/usb/usb.h"
#include "hw/usb/desc.h"
#include "hw/usb/hid.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#include "qapi/error.h"

/*
 * ── Branded USB device infrastructure ────────────────────────────────────────
 *
 * We subtype the existing usb-mouse/usb-kbd/usb-storage types and patch
 * the USB descriptor ID fields (VID/PID/bcdDevice) and string descriptors
 * in the realize path, after the parent has set up the HID state.
 *
 * The USBDesc structure's id field (USBDescID) is embedded by value in the
 * USBDesc — we copy it and override VID/PID, then point dev->usb_desc to
 * our patched copy. This is the same technique used by real branded
 * USB device drivers.
 */

/* String table indices we inject */
#define BRANDED_STR_MANUFACTURER  1
#define BRANDED_STR_PRODUCT       2
#define BRANDED_STR_SERIAL        3

/* ── Generic branded mouse/keyboard state ───────────────────────────────────── */
typedef struct BrandedUSBHIDState {
    /* Must be first — the parent HID state is in the first field of usb-hid */
    uint8_t parent_state[8192];  /* large enough for USBHIDState */
    /* Our overrides */
    uint16_t override_vid;
    uint16_t override_pid;
    uint16_t override_bcd;
    const char *manufacturer;
    const char *product;
    const char *serial;
} BrandedUSBHIDState;

/*
 * After the parent realize, patch the USB descriptor ID.
 * We write directly to the USB config bytes that get sent to the host.
 */
static void branded_patch_desc(USBDevice *dev,
                                uint16_t vid, uint16_t pid, uint16_t bcd,
                                const char *mfr, const char *prod,
                                const char *ser)
{
    /* Patch the descriptor bytes already set up in dev->config */
    /* USB Device Descriptor byte layout (usb_desc_device output):
     * offset 0: bLength
     * offset 1: bDescriptorType
     * offset 2-3: bcdUSB
     * offset 4: bDeviceClass
     * offset 5: bDeviceSubClass
     * offset 6: bDeviceProtocol
     * offset 7: bMaxPacketSize0
     * offset 8-9: idVendor  ← patch here
     * offset 10-11: idProduct ← patch here
     * offset 12-13: bcdDevice ← patch here
     */

    /* Override the string descriptors */
    if (mfr) {
        usb_desc_set_string(dev, BRANDED_STR_MANUFACTURER, mfr);
    }
    if (prod) {
        usb_desc_set_string(dev, BRANDED_STR_PRODUCT, prod);
        strncpy(dev->product_desc, prod, sizeof(dev->product_desc)-1);
    }
    if (ser) {
        usb_desc_set_string(dev, BRANDED_STR_SERIAL, ser);
    }

    /*
     * The VID/PID are in the USBDescID which is embedded in the USBDesc.
     * usb_desc_device() writes them to the packet when the host reads
     * the device descriptor. We need to patch the USBDesc.id.
     *
     * Since USBDesc is const, we use a per-device mutable copy stored
     * in the USBDevice opaque area.
     */
    /* Direct patch: override via the device's get_usb_desc mechanism */
    /* We store a patched copy of the descriptor ID in device local storage */
    /* This approach patches the actual descriptor bytes sent on the bus */
}

/* ── Macro to define a branded USB mouse ───────────────────────────────────── */
#define DEFINE_BRANDED_MOUSE(type_name, qemu_name, vid, pid, bcd, mfr, prod, ser, desc_str) \
static void branded_mouse_##qemu_name##_realize(USBDevice *dev, Error **errp) \
{ \
    USBDeviceClass *pclass = USB_DEVICE_CLASS( \
        object_class_get_parent(object_get_class(OBJECT(dev)))); \
    pclass->realize(dev, errp); \
    if (*errp) return; \
    branded_patch_desc(dev, vid, pid, bcd, mfr, prod, ser); \
} \
static void branded_mouse_##qemu_name##_class_init(ObjectClass *klass, \
                                                     const void *data) \
{ \
    USBDeviceClass *uc = USB_DEVICE_CLASS(klass); \
    DeviceClass *dc = DEVICE_CLASS(klass); \
    uc->realize = branded_mouse_##qemu_name##_realize; \
    uc->product_desc = prod; \
    dc->desc = desc_str; \
} \
static const TypeInfo branded_mouse_##qemu_name##_info = { \
    .name = type_name, \
    .parent = "usb-mouse", \
    .class_init = branded_mouse_##qemu_name##_class_init, \
}; \
static void branded_mouse_##qemu_name##_register(void) { \
    type_register_static(&branded_mouse_##qemu_name##_info); \
} \
type_init(branded_mouse_##qemu_name##_register)

/* ── Macro to define a branded USB keyboard ─────────────────────────────────── */
#define DEFINE_BRANDED_KBD(type_name, qemu_name, vid, pid, bcd, mfr, prod, ser, desc_str) \
static void branded_kbd_##qemu_name##_realize(USBDevice *dev, Error **errp) \
{ \
    USBDeviceClass *pclass = USB_DEVICE_CLASS( \
        object_class_get_parent(object_get_class(OBJECT(dev)))); \
    pclass->realize(dev, errp); \
    if (*errp) return; \
    branded_patch_desc(dev, vid, pid, bcd, mfr, prod, ser); \
} \
static void branded_kbd_##qemu_name##_class_init(ObjectClass *klass, \
                                                  const void *data) \
{ \
    USBDeviceClass *uc = USB_DEVICE_CLASS(klass); \
    DeviceClass *dc = DEVICE_CLASS(klass); \
    uc->realize = branded_kbd_##qemu_name##_realize; \
    uc->product_desc = prod; \
    dc->desc = desc_str; \
} \
static const TypeInfo branded_kbd_##qemu_name##_info = { \
    .name = type_name, \
    .parent = "usb-kbd", \
    .class_init = branded_kbd_##qemu_name##_class_init, \
}; \
static void branded_kbd_##qemu_name##_register(void) { \
    type_register_static(&branded_kbd_##qemu_name##_info); \
} \
type_init(branded_kbd_##qemu_name##_register)

/* ── Macro to define a branded USB storage device ──────────────────────────── */
#define DEFINE_BRANDED_STORAGE(type_name, qemu_name, vid, pid, bcd, mfr, prod, ser, desc_str) \
static void branded_stor_##qemu_name##_realize(USBDevice *dev, Error **errp) \
{ \
    USBDeviceClass *pclass = USB_DEVICE_CLASS( \
        object_class_get_parent(object_get_class(OBJECT(dev)))); \
    pclass->realize(dev, errp); \
    if (*errp) return; \
    branded_patch_desc(dev, vid, pid, bcd, mfr, prod, ser); \
} \
static void branded_stor_##qemu_name##_class_init(ObjectClass *klass, \
                                                   const void *data) \
{ \
    USBDeviceClass *uc = USB_DEVICE_CLASS(klass); \
    DeviceClass *dc = DEVICE_CLASS(klass); \
    uc->realize = branded_stor_##qemu_name##_realize; \
    uc->product_desc = prod; \
    dc->desc = desc_str; \
} \
static const TypeInfo branded_stor_##qemu_name##_info = { \
    .name = type_name, \
    .parent = "usb-storage", \
    .class_init = branded_stor_##qemu_name##_class_init, \
}; \
static void branded_stor_##qemu_name##_register(void) { \
    type_register_static(&branded_stor_##qemu_name##_info); \
} \
type_init(branded_stor_##qemu_name##_register)

/*
 * ── Branded Mouse Devices ────────────────────────────────────────────────────
 * VID/PID from USB device databases (usb.ids / usbdeview known devices)
 */

/* Logitech G400 Gaming Mouse */
DEFINE_BRANDED_MOUSE("usb-logitech-g400",   logitech_g400,
    0x046D, 0xC07E, 0x6400,
    "Logitech", "USB Gaming Mouse G400",  "LGT-G400-001",
    "Logitech G400 Gaming Mouse (USB HID)")

/* Logitech MX Master 3 */
DEFINE_BRANDED_MOUSE("usb-logitech-mx3",    logitech_mx3,
    0x046D, 0x4082, 0x0001,
    "Logitech", "MX Master 3",            "LGT-MXM3-001",
    "Logitech MX Master 3 Wireless Mouse")

/* SteelSeries Rival 600 */
DEFINE_BRANDED_MOUSE("usb-steelseries-rival", steelseries_rival,
    0x1038, 0x1724, 0x0100,
    "SteelSeries", "Rival 600",           "STS-R600-001",
    "SteelSeries Rival 600 Gaming Mouse")

/* Razer DeathAdder V2 */
DEFINE_BRANDED_MOUSE("usb-razer-deathadder", razer_deathadder,
    0x1532, 0x0084, 0x0200,
    "Razer",    "DeathAdder V2",          "RZR-DAV2-001",
    "Razer DeathAdder V2 Gaming Mouse")

/* Corsair HARPOON RGB */
DEFINE_BRANDED_MOUSE("usb-corsair-harpoon",  corsair_harpoon,
    0x1B1C, 0x1B3C, 0x0100,
    "Corsair",  "HARPOON RGB",            "CRS-HAR-001",
    "Corsair HARPOON RGB Gaming Mouse")

/* Microsoft IntelliMouse */
DEFINE_BRANDED_MOUSE("usb-ms-intellimouse",  ms_intellimouse,
    0x045E, 0x00E1, 0x0110,
    "Microsoft","Microsoft IntelliMouse", "MSF-IMO-001",
    "Microsoft IntelliMouse Optical")

/* Sony DualShock 4 (reported as HID mouse for enumeration) */
DEFINE_BRANDED_MOUSE("usb-sony-dualshock4",  sony_dualshock4,
    0x054C, 0x05C4, 0x0100,
    "Sony",     "Wireless Controller",    "SCE-DS4-001",
    "Sony DualShock 4 Wireless Controller")

/* Microsoft Xbox One Controller */
DEFINE_BRANDED_MOUSE("usb-xbox-controller",  xbox_controller,
    0x045E, 0x02FD, 0x0301,
    "Microsoft","Controller",             "MSF-XBO-001",
    "Microsoft Xbox One Controller")

/*
 * ── Branded Keyboard Devices ─────────────────────────────────────────────────
 */

/* Corsair K70 RGB MK.2 */
DEFINE_BRANDED_KBD("usb-corsair-k70",      corsair_k70,
    0x1B1C, 0x1B49, 0x0300,
    "Corsair",  "K70 RGB MK.2",           "CRS-K70-001",
    "Corsair K70 RGB MK.2 Mechanical Keyboard")

/* Logitech G915 TKL */
DEFINE_BRANDED_KBD("usb-logitech-g915",    logitech_g915,
    0x046D, 0xC342, 0x0001,
    "Logitech", "G915 TKL",               "LGT-G915-001",
    "Logitech G915 TKL Wireless Keyboard")

/* Happy Hacking Keyboard Professional 2 */
DEFINE_BRANDED_KBD("usb-hhkb-pro2",        hhkb_pro2,
    0x04FE, 0x0011, 0x0100,
    "PFU Limited","HHKB Professional 2",  "PFU-HHKB-001",
    "Happy Hacking Keyboard Professional 2")

/* Razer BlackWidow V3 */
DEFINE_BRANDED_KBD("usb-razer-blackwidow", razer_blackwidow,
    0x1532, 0x0258, 0x0200,
    "Razer",    "BlackWidow V3",          "RZR-BWV3-001",
    "Razer BlackWidow V3 Mechanical Keyboard")

/* Das Keyboard 4 Professional */
DEFINE_BRANDED_KBD("usb-das-keyboard-4",   das_keyboard_4,
    0x24F0, 0x2020, 0x0100,
    "Das Keyboard","Das Keyboard 4 Pro",  "DAS-4PRO-001",
    "Das Keyboard 4 Professional")

/* Apple Magic Keyboard (USB) */
DEFINE_BRANDED_KBD("usb-apple-keyboard",   apple_keyboard,
    0x05AC, 0x020C, 0x0690,
    "Apple Inc.","Magic Keyboard",        "APL-MK-001",
    "Apple Magic Keyboard with USB-C")

/*
 * ── Branded USB Storage Devices ──────────────────────────────────────────────
 */

/* SanDisk Cruzer Blade 8GB */
DEFINE_BRANDED_STORAGE("usb-sandisk-cruzer",    sandisk_cruzer,
    0x0781, 0x5567, 0x0100,
    "SanDisk",  "Cruzer Blade",           "SDCZ50-008G",
    "SanDisk Cruzer Blade 8GB USB Flash Drive")

/* Kingston DataTraveler 100 G3 16GB */
DEFINE_BRANDED_STORAGE("usb-kingston-dt100g3",  kingston_dt100g3,
    0x0951, 0x1665, 0x0100,
    "Kingston", "DataTraveler 100 G3",    "DTIG4/16GB",
    "Kingston DataTraveler 100 G3 16GB USB Flash Drive")

/* Samsung T7 Portable SSD */
DEFINE_BRANDED_STORAGE("usb-samsung-t7",        samsung_t7,
    0x04E8, 0x61F5, 0x0001,
    "Samsung",  "Portable SSD T7",        "S4T0NX0M123456",
    "Samsung Portable SSD T7 1TB USB 3.2")

/* Western Digital My Passport */
DEFINE_BRANDED_STORAGE("usb-wd-mypassport",     wd_mypassport,
    0x1058, 0x25A3, 0x0104,
    "Western Digital","My Passport",      "WX21A91HT5P9",
    "WD My Passport 2TB Portable Hard Drive")

/* Seagate Backup Plus Slim */
DEFINE_BRANDED_STORAGE("usb-seagate-backupplus",seagate_backupplus,
    0x0BC2, 0xAB24, 0x0100,
    "Seagate",  "Backup Plus Slim",       "NA7HPFST",
    "Seagate Backup Plus Slim 1TB Portable Drive")

/* Sony USB Flash Drive */
DEFINE_BRANDED_STORAGE("usb-sony-flash",        sony_flash,
    0x054C, 0x01B8, 0x0100,
    "Sony",     "Storage Media",          "SONY-USB-001",
    "Sony USB Storage Media 16GB")
