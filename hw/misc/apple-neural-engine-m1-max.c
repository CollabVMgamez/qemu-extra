/*
 * Apple Neural Engine (M1 Max, 11 TOPS)
 * Vendor: 0x106B, Device: 0x1804
 * Class: 0x1180 (Processing Accelerator / Neural Processor)
 * Usage: -device apple-neural-engine-m1-max
 * Identity-only stub — shows in lspci, no AI workload processing
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "migration/vmstate.h"
#define NPU_VID    0x106B
#define NPU_DEV    0x1804
#define NPU_SUBSYS_V 0x106B
#define NPU_SUBSYS_D 0x1804
#define NPU_REV    0x00
#define NPU_CLASS  0x1180
#define BAR0_SIZE (4*MiB)
#define TYPE_APPLENEURALENGINEM1MAX "apple-neural-engine-m1-max"
OBJECT_DECLARE_SIMPLE_TYPE(AppleNeuralEngineM1MaxState, APPLENEURALENGINEM1MAX)
struct AppleNeuralEngineM1MaxState {
    PCIDevice parent_obj;
    MemoryRegion bar0;
};
static uint64_t bar0_r(void *o, hwaddr a, unsigned s) { return 0; }
static void bar0_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps bar0_ops = {
    .read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4}};
static void dev_realize(PCIDevice *pdev, Error **errp) {
    AppleNeuralEngineM1MaxState *s = APPLENEURALENGINEM1MAX(pdev);
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, NPU_SUBSYS_V);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, NPU_SUBSYS_D);
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0_ops, s, "apple-neural-engine-m1-max-mmio", BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64, &s->bar0);
}
static const VMStateDescription vms = {
    .name="apple-neural-engine-m1-max",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AppleNeuralEngineM1MaxState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=dev_realize; pc->vendor_id=NPU_VID; pc->device_id=NPU_DEV;
    pc->class_id=NPU_CLASS; pc->revision=NPU_REV;
    dc->desc="Apple Neural Engine (M1 Max, 11 TOPS)"; dc->vmsd=&vms; dc->hotpluggable=false;
    set_bit(DEVICE_CATEGORY_MISC,dc->categories);
}
static const TypeInfo ti={.name=TYPE_APPLENEURALENGINEM1MAX,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AppleNeuralEngineM1MaxState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);}
type_init(reg)
