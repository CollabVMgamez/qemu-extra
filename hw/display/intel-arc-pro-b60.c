/*
 * Intel Arc Pro B60 (Battlemage BMG-G21, 120W, 16GB)
 * Vendor: 0x8086, Device: 0xE20D
 * Class: 0x0302, PCIe
 * Usage: -device intel-arc-pro-b60
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define INTEL_VID    0x8086
#define GPU_DEV      0xE20D
#define GPU_SUBSYS_V 0x8086
#define GPU_SUBSYS_D 0x220D
#define GPU_REV      0x01
#define GPU_CLASS    0x0302
#define BAR0_SIZE (64*MiB)
#define BAR2_SIZE (16384*MiB)
#define GPU_CLK_BASE  300
#define GPU_CLK_BOOST 2400
#define TYPE_INTELARCPROB60 "intel-arc-pro-b60"
OBJECT_DECLARE_SIMPLE_TYPE(IntelArcProB60State, INTELARCPROB60)
struct IntelArcProB60State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar2;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    char *gpu_name;
};
static uint32_t igpu_clk(IntelArcProB60State *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (GPU_CLK_BOOST - GPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
#define MBOX_ADDR 0x138124
#define RPSTAT1   0x0A01C
#define CDCLK_CTL 0x46000
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    IntelArcProB60State *s = opaque;
    uint32_t clk = igpu_clk(s);
    switch (addr) {
    case RPSTAT1:    return (clk & 0x1FF) << 23;
    case CDCLK_CTL:  return 0x00000002;
    case MBOX_ADDR:  return 0;
    default:         return 0;
    }
}
static void bar0_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps bar0_ops = {
    .read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4}};
static uint64_t bar2_r(void *o, hwaddr a, unsigned s) { return 0; }
static void bar2_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps bar2_ops = {
    .read=bar2_r,.write=bar2_w,.endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=8}};
static void igpu_realize(PCIDevice *pdev, Error **errp) {
    IntelArcProB60State *s = INTELARCPROB60(pdev);
    s->clock_mhz = GPU_CLK_BASE; s->clock_last_ns = 0;
    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, GPU_SUBSYS_V);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, GPU_SUBSYS_D);
    uint8_t *c = pdev->config;
    c[PCI_CAPABILITY_LIST] = 0x50; c[PCI_STATUS] |= PCI_STATUS_CAP_LIST;
    c[0x50]=0x10; c[0x51]=0x00;
    pci_set_word(c+0x52, 0x0002);
    pci_set_long(c+0x60, (0x10<<4)|0x3);
    pci_set_word(c+0x62, (0x10<<4)|0x3|(1<<13));
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0_ops, s, "intel-arc-pro-b60-mmio", BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64, &s->bar0);
    memory_region_init_io(&s->bar2, OBJECT(s), &bar2_ops, s, "intel-arc-pro-b60-gtt", BAR2_SIZE);
    pci_register_bar(pdev, 2, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64|PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar2);
}
static const Property igpu_props[] = {
    DEFINE_PROP_STRING("gpu-name", IntelArcProB60State, gpu_name),
};
static const VMStateDescription vms = {
    .name="intel-arc-pro-b60",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelArcProB60State),VMSTATE_UINT32(clock_mhz,IntelArcProB60State),VMSTATE_UINT64(clock_last_ns,IntelArcProB60State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=igpu_realize; pc->vendor_id=INTEL_VID; pc->device_id=GPU_DEV;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REV;
    dc->desc="Intel Arc Pro B60 (Battlemage BMG-G21, 120W, 16GB)"; dc->vmsd=&vms; dc->hotpluggable=false;
    device_class_set_props(dc, igpu_props);
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti={.name=TYPE_INTELARCPROB60,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(IntelArcProB60State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);}
type_init(reg)
