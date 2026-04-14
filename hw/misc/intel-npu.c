/*
 * QEMU - Intel NPU (Neural Processing Unit) stub device
 *
 * Emulates Intel NPU devices visible as PCI devices on Meteor Lake,
 * Lunar Lake, and Arrow Lake platforms.
 *
 * PCI identifiers:
 *   Meteor Lake NPU  : 0x8086:0x7D1D (VPU, class 0x1200)
 *   Lunar Lake NPU   : 0x8086:0x643E
 *   Arrow Lake NPU   : 0x8086:0x7D45
 *   Panther Lake NPU : 0x8086:0xB05B
 *
 * Usage:
 *   -device intel-npu
 *   -device intel-npu,npu-gen=lunarlake
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

#define TYPE_INTEL_NPU "intel-npu"
OBJECT_DECLARE_SIMPLE_TYPE(IntelNPUState, INTEL_NPU)

static const struct {
    const char *name;
    uint16_t device_id;
    const char *desc;
} intel_npu_table[] = {
    { "meteorlake",  0x7D1D, "Intel NPU (Meteor Lake, MTL-P)" },
    { "lunarlake",   0x643E, "Intel NPU 4 (Lunar Lake, LNL)" },
    { "arrowlake",   0x7D45, "Intel NPU (Arrow Lake, ARL)" },
    { "pantherlake", 0xB05B, "Intel NPU 5 (Panther Lake, PTL)" },
    { NULL, 0, NULL }
};

struct IntelNPUState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *npu_gen;
};

static uint64_t intel_npu_read(void *opaque, hwaddr addr, unsigned size)
{
    /* Return device-ready status on capability register */
    if (addr == 0) return 0x00000001;
    return 0;
}
static void intel_npu_write(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps intel_npu_ops = {
    .read=intel_npu_read, .write=intel_npu_write,
    .endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};

static void intel_npu_realize(PCIDevice *pdev, Error **errp)
{
    IntelNPUState *s = INTEL_NPU(pdev);
    uint16_t devid = 0x7D1D; /* default: Meteor Lake */

    if (s->npu_gen) {
        for (int i = 0; intel_npu_table[i].name; i++) {
            if (!strcmp(s->npu_gen, intel_npu_table[i].name)) {
                devid = intel_npu_table[i].device_id;
                break;
            }
        }
    }

    pci_config_set_device_id(pdev->config, devid);
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 0x8086);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, devid);

    memory_region_init_io(&s->mmio, OBJECT(s), &intel_npu_ops, s,
                          "intel-npu-mmio", 4 * KiB);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->mmio);
}

static const Property intel_npu_props[] = {
    DEFINE_PROP_STRING("npu-gen", IntelNPUState, npu_gen),
};
static const VMStateDescription vmstate_intel_npu = {
    .name="intel-npu",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelNPUState),VMSTATE_END_OF_LIST()},
};
static void intel_npu_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize   = intel_npu_realize;
    k->vendor_id = 0x8086; /* Intel */
    k->device_id = 0x7D1D; /* Meteor Lake NPU default */
    k->class_id  = 0x1200; /* Processing Accelerator */
    k->revision  = 0x00;
    dc->desc         = "Intel NPU (Neural Processing Unit) stub";
    dc->vmsd         = &vmstate_intel_npu;
    dc->hotpluggable = false;
    device_class_set_props(dc, intel_npu_props);
}
static const TypeInfo intel_npu_info = {
    .name=TYPE_INTEL_NPU,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(IntelNPUState),.class_init=intel_npu_class_init,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void intel_npu_register(void){type_register_static(&intel_npu_info);}
type_init(intel_npu_register)
