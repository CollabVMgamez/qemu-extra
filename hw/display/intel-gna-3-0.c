/*
 * Intel GNA 3.0 (Meteor Lake Neural Accelerator)
 * Vendor: 0x8086, Device: 0x7A04
 * Class: 0x1180 (Processing Accelerator)
 * Usage: -device intel-gna-3-0
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "migration/vmstate.h"
#define INTEL_VID    0x8086
#define GPU_DEV      0x7A04
#define GPU_SUBSYS_V 0x8086
#define GPU_SUBSYS_D 0x2204
#define GPU_REV      0x00
#define GPU_CLASS    0x1180
#define BAR0_SIZE (4*MiB)
#define TYPE_INTELGNA30 "intel-gna-3-0"
OBJECT_DECLARE_SIMPLE_TYPE(IntelGna30State, INTELGNA30)
struct IntelGna30State {
    PCIDevice parent_obj;
    MemoryRegion bar0;
};
static uint64_t bar0_r(void *o, hwaddr a, unsigned s) { return 0; }
static void bar0_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps bar0_ops = {
    .read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4}};
static void dev_realize(PCIDevice *pdev, Error **errp) {
    IntelGna30State *s = INTELGNA30(pdev);
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, GPU_SUBSYS_V);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, GPU_SUBSYS_D);
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0_ops, s, "intel-gna-3-0-mmio", BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64, &s->bar0);
}
static const VMStateDescription vms = {
    .name="intel-gna-3-0",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelGna30State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=dev_realize; pc->vendor_id=INTEL_VID; pc->device_id=GPU_DEV;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REV;
    dc->desc="Intel GNA 3.0 (Meteor Lake Neural Accelerator)"; dc->vmsd=&vms; dc->hotpluggable=false;
    set_bit(DEVICE_CATEGORY_MISC,dc->categories);
}
static const TypeInfo ti={.name=TYPE_INTELGNA30,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(IntelGna30State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);}
type_init(reg)
