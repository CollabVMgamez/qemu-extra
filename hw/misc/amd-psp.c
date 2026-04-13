/*
 * AMD Platform Security Processor (PSP) / AMD Secure Processor stub device
 * Appears as PCI encryption controller (class 0x1080) on modern AMD platforms.
 * AMD PSP PCI device IDs:
 *   Ryzen 1000-2000 (Zen/Zen+):  0x1456
 *   Ryzen 3000 (Zen2):           0x1486
 *   Ryzen 5000 (Zen3):           0x1649
 *   Ryzen 7000 (Zen4):           0x14EF
 *
 * Usage: -device amd-psp
 *        -device amd-psp,psp-gen=zen4
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

#define TYPE_AMD_PSP "amd-psp"
OBJECT_DECLARE_SIMPLE_TYPE(AmdPSPState, AMD_PSP)

static const struct { const char *name; uint16_t devid; } psp_gen_table[] = {
    { "zen",   0x1456 },
    { "zen+",  0x1468 },
    { "zen2",  0x1486 },
    { "zen3",  0x1649 },
    { "zen4",  0x14EF },  /* default */
    { "zen5",  0x14F0 },
    { NULL, 0 }
};

struct AmdPSPState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *psp_gen;
};

static uint64_t amd_psp_read(void *opaque, hwaddr addr, unsigned size)
{
    /* PSP capability register — return "PSP present and ready" */
    if (addr == 0) return 0x00000001;  /* PSP enabled */
    return 0;
}
static void amd_psp_write(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps amd_psp_ops = {
    .read=amd_psp_read,.write=amd_psp_write,
    .endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};

static void amd_psp_realize(PCIDevice *pdev, Error **errp)
{
    AmdPSPState *s = AMD_PSP(pdev);
    uint16_t devid = 0x14EF; /* zen4 default */

    if (s->psp_gen) {
        for (int i = 0; psp_gen_table[i].name; i++) {
            if (strcmp(s->psp_gen, psp_gen_table[i].name) == 0) {
                devid = psp_gen_table[i].devid;
                break;
            }
        }
    }

    pci_config_set_device_id(pdev->config, devid);
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 0x1022);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, devid);

    memory_region_init_io(&s->mmio, OBJECT(s), &amd_psp_ops, s,
                          "amd-psp-mmio", 4 * KiB);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->mmio);
}

static const Property amd_psp_props[] = {
    DEFINE_PROP_STRING("psp-gen", AmdPSPState, psp_gen),
};
static const VMStateDescription vmstate_amd_psp = {
    .name="amd-psp",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AmdPSPState),VMSTATE_END_OF_LIST()},
};
static void amd_psp_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize    = amd_psp_realize;
    k->vendor_id  = 0x1022;
    k->device_id  = 0x14EF;
    k->class_id   = 0x1080;   /* Encryption Controller */
    k->revision   = 0x00;
    dc->desc         = "AMD Platform Security Processor (PSP) stub";
    dc->vmsd         = &vmstate_amd_psp;
    dc->hotpluggable = false;
    device_class_set_props(dc, amd_psp_props);
}
static const TypeInfo amd_psp_info = {
    .name=TYPE_AMD_PSP,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(AmdPSPState),.class_init=amd_psp_class_init,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void amd_psp_register(void){type_register_static(&amd_psp_info);}
type_init(amd_psp_register)
