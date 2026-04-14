/* Intel Arc B770 (BMG-G21, Battlemage, 16GB GDDR6)
 * Vendor: 0x8086, Device: 0xE20C
 * Usage: -device intel-arc-b770
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define INTEL_VID 0x8086
#define GPU_DEV   0xE20C
#define GPU_SS_V  0x8086
#define GPU_SS_D  0x4005
#define GPU_REV   0x05
#define GPU_CLASS 0x0302
#define BAR0_SIZE (16*MiB)
#define BAR2_SIZE (256*MiB)
#define CLK_BASE  2500
#define CLK_BOOST 2800
#define TYPE_INTEL_ARC_B770 "intel-arc-b770"
OBJECT_DECLARE_SIMPLE_TYPE(IntelArcB770State, INTEL_ARC_B770)
struct IntelArcB770State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar2;
    uint32_t clock_mhz; uint64_t clock_last_ns;
    char *gpu_name;
};
static uint32_t clk_f(IntelArcB770State*s){
    uint64_t now=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if(now-s->clock_last_ns>100000000ULL){s->clock_last_ns=now;s->clock_mhz=s->clock_mhz*1664525u+1013904223u;s->clock_mhz=CLK_BASE+(s->clock_mhz%(CLK_BOOST-CLK_BASE+1));}
    return s->clock_mhz;
}
static uint64_t b0r(void*op,hwaddr a,unsigned sz){(void)sz;IntelArcB770State*s=(IntelArcB770State*)op;uint32_t c=clk_f(s);if(a==0xA01C)return(c&0x1FF)<<23;return 0;}
static void b0w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b0={.read=b0r,.write=b0w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t b2r(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;}static void b2w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b2={.read=b2r,.write=b2w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static void realize(PCIDevice*p,Error**e){
    IntelArcB770State*s=(IntelArcB770State*)p;s->clock_mhz=CLK_BASE;s->clock_last_ns=0;
    p->config[PCI_CLASS_PROG]=0;pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SS_V);pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SS_D);
    uint8_t*c=p->config;c[PCI_CAPABILITY_LIST]=0x50;c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;c[0x50]=0x10;c[0x51]=0;pci_set_word(c+0x52,0x0002);pci_set_long(c+0x5C,(0x10<<4)|4);pci_set_word(c+0x62,(0x10<<4)|4|(1<<13));
    memory_region_init_io(&s->bar0,OBJECT(s),&b0,s,"intel-arc-b770-mmio",BAR0_SIZE);pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64,&s->bar0);
    memory_region_init_io(&s->bar2,OBJECT(s),&b2,s,"intel-arc-b770-gtt",BAR2_SIZE);pci_register_bar(p,2,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar2);
}
static const Property props[]={DEFINE_PROP_STRING("gpu-name",IntelArcB770State,gpu_name),};
static const VMStateDescription vms={.name="intel-arc-b770",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,IntelArcB770State),VMSTATE_UINT32(clock_mhz,IntelArcB770State),VMSTATE_UINT64(clock_last_ns,IntelArcB770State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=INTEL_VID;pc->device_id=GPU_DEV;pc->class_id=GPU_CLASS;pc->revision=GPU_REV;dc->desc="Intel Arc B770 (BMG-G21, Battlemage, 16GB GDDR6)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);}
static const TypeInfo ti={.name=TYPE_INTEL_ARC_B770,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(IntelArcB770State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);}type_init(reg)
