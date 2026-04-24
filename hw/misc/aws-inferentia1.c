/*
 * AWS Inferentia1 (Inf1) ML Inference Accelerator stub
 * Vendor: 0x1D0F (Amazon.com, Inc.), Device: 0xF010
 * Class: 0x1180 (Processing Accelerator / Neural Processor)
 * Usage: -device aws-inferentia1
 * Identity-only stub — shows in lspci, no AI workload processing
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "migration/vmstate.h"
#define NPU_VID    0x1D0F
#define NPU_DEV    0xF010
#define NPU_SUBSYS_V 0x1D0F
#define NPU_SUBSYS_D 0xF010
#define NPU_REV    0x01
#define NPU_CLASS  0x1180
#define BAR0_SIZE (4*MiB)
#define TYPE_AWS_INFERENTIA1 "aws-inferentia1"
OBJECT_DECLARE_SIMPLE_TYPE(AwsInferentia1State, AWS_INFERENTIA1)
struct AwsInferentia1State {
    PCIDevice parent_obj;
    MemoryRegion bar0;
};
static uint64_t bar0_r(void *o, hwaddr a, unsigned s) { return 0; }
static void bar0_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps bar0_ops = {
    .read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4}};
static void dev_realize(PCIDevice *pdev, Error **errp) {
    AwsInferentia1State *s = AWS_INFERENTIA1(pdev);
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, NPU_SUBSYS_V);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, NPU_SUBSYS_D);
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0_ops, s, "aws-inferentia1-mmio", BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64, &s->bar0);
}
static const VMStateDescription vms = {
    .name="aws-inferentia1",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AwsInferentia1State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=dev_realize; pc->vendor_id=NPU_VID; pc->device_id=NPU_DEV;
    pc->class_id=NPU_CLASS; pc->revision=NPU_REV;
    dc->desc="AWS Inferentia1 (Inf1) ML Inference Accelerator"; dc->vmsd=&vms; dc->hotpluggable=false;
    set_bit(DEVICE_CATEGORY_MISC,dc->categories);
}
static const TypeInfo ti={.name=TYPE_AWS_INFERENTIA1,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AwsInferentia1State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}};
static void reg(void){type_register_static(&ti);}
type_init(reg)
