/*
 * QEMU - AMD NPU (XDNA Neural Processing Unit) stub device
 *
 * Emulates AMD NPU/XDNA devices found in:
 *   Ryzen 7040 (Phoenix) : NPU (XDNA 1.0)
 *   Ryzen 8040 (Hawk Point): NPU (XDNA 1.1)
 *   Ryzen AI 300 (Strix Point): NPU 4 (XDNA 2.0)
 *   EPYC 9005 (Turin):  AI Engine (datacenter variant)
 *
 * PCI identifiers (AMD class 0x1200 = Processing Accelerator):
 *   Phoenix NPU    : 0x1022:0x1502
 *   Hawk Point NPU : 0x1022:0x17F0
 *   Strix Point NPU: 0x1022:0x17F1
 *
 * Usage:
 *   -device amd-npu
 *   -device amd-npu,npu-gen=strixpoint
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

#define TYPE_AMD_NPU "amd-npu"
OBJECT_DECLARE_SIMPLE_TYPE(AmdNPUState, AMD_NPU)

static const struct {
    const char *name;
    uint16_t device_id;
    const char *desc;
} amd_npu_table[] = {
    { "phoenix",    0x1502, "AMD XDNA NPU (Phoenix, Ryzen 7040)" },
    { "hawkpoint",  0x17F0, "AMD XDNA NPU (Hawk Point, Ryzen 8040)" },
    { "strixpoint", 0x17F1, "AMD XDNA 2 NPU 4 (Strix Point, Ryzen AI 300)" },
    { "krackan",    0x17F2, "AMD XDNA 2 NPU (Krackan Point)" },
    { "sienapoint", 0x14EA, "AMD XDNA NPU (Siena Point, Ryzen 8040HX)" },
    { NULL, 0, NULL }
};

struct AmdNPUState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *npu_gen;
};

static uint64_t amd_npu_read(void *opaque, hwaddr addr, unsigned size)
{
    if (addr == 0) return 0x00000001; /* NPU ready */
    return 0;
}
static void amd_npu_write(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps amd_npu_ops = {
    .read=amd_npu_read, .write=amd_npu_write,
    .endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};

static void amd_npu_realize(PCIDevice *pdev, Error **errp)
{
    AmdNPUState *s = AMD_NPU(pdev);
    uint16_t devid = 0x1502; /* default: Phoenix */

    if (s->npu_gen) {
        for (int i = 0; amd_npu_table[i].name; i++) {
            if (!strcmp(s->npu_gen, amd_npu_table[i].name)) {
                devid = amd_npu_table[i].device_id;
                break;
            }
        }
    }

    pci_config_set_device_id(pdev->config, devid);
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 0x1022);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, devid);

    memory_region_init_io(&s->mmio, OBJECT(s), &amd_npu_ops, s,
                          "amd-npu-mmio", 4 * KiB);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->mmio);
}

static const Property amd_npu_props[] = {
    DEFINE_PROP_STRING("npu-gen", AmdNPUState, npu_gen),
};
static const VMStateDescription vmstate_amd_npu = {
    .name="amd-npu",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AmdNPUState),VMSTATE_END_OF_LIST()},
};
static void amd_npu_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize   = amd_npu_realize;
    k->vendor_id = 0x1022; /* AMD */
    k->device_id = 0x1502; /* Phoenix NPU default */
    k->class_id  = 0x1200; /* Processing Accelerator */
    k->revision  = 0x00;
    dc->desc         = "AMD XDNA NPU (Neural Processing Unit) stub";
    dc->vmsd         = &vmstate_amd_npu;
    dc->hotpluggable = false;
    device_class_set_props(dc, amd_npu_props);
}
static const TypeInfo amd_npu_info = {
    .name=TYPE_AMD_NPU,.parent=TYPE_PCI_DEVICE,
    .instance_size=sizeof(AmdNPUState),.class_init=amd_npu_class_init,
    .interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}},
};
static void amd_npu_register(void){type_register_static(&amd_npu_info);}
type_init(amd_npu_register)
