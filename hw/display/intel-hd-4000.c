/*
 * Intel HD Graphics 4000 (Ivy Bridge GT2, 16EU)
 * Vendor: 0x8086, Device: 0x0162
 * Class: 0x0300, PCIe
 * Usage: -device intel-hd-4000
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
#define GPU_DEV      0x0162
#define GPU_SUBSYS_V 0x8086
#define GPU_SUBSYS_D 0x2111
#define GPU_REV      0x09
#define GPU_CLASS    0x0300
#define BAR0_SIZE (16*MiB)
#define BAR2_SIZE (256*MiB)
#define GPU_CLK_BASE  650
#define GPU_CLK_BOOST 1050
#define TYPE_INTELHD4000 "intel-hd-4000"
OBJECT_DECLARE_SIMPLE_TYPE(IntelHd4000State, INTELHD4000)
struct IntelHd4000State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar2;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    char *gpu_name;
};
static uint32_t igpu_clk(IntelHd4000State *s) {
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
    IntelHd4000State *s = opaque;
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
    IntelHd4000State *s = INTELHD4000(pdev);
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
    memory_region_init_io(&s->bar0, OBJECT(s), &bar0_ops, s, "intel-hd-4000-mmio", BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64, &s->bar0);
    memory_region_init_io(&s->bar2, OBJECT(s), &bar2_ops, s, "intel-hd-4000-gtt", BAR2_SIZE);
    pci_register_bar(pdev, 2, PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64|PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar2);
}
static const Property igpu_props[] = {
    DEFINE_PROP_STRING("gpu-name", IntelHd4000State, gpu_name),
};
static const VMStateDescription vms = {
    .name="intel-hd-4000",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelHd4000State),VMSTATE_UINT32(clock_mhz,IntelHd4000State),VMSTATE_UINT64(clock_last_ns,IntelHd4000State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=igpu_realize; pc->vendor_id=INTEL_VID; pc->device_id=GPU_DEV;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REV;
    dc->desc="Intel HD Graphics 4000 (Ivy Bridge GT2, 16EU)"; dc->vmsd=&vms; dc->hotpluggable=false;
    device_class_set_props(dc, igpu_props);
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti={.name=TYPE_INTELHD4000,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(IntelHd4000State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);}
type_init(reg)
