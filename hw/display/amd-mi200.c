/* AMD Instinct MI200 (Aldebaran GFX90A, 128GB HBM2e)
 * AMD GPU stub. Vendor: 0x1002, Device: 0x7408
 * Usage: -device amd-mi200
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
#define AMD_VID 0x1002
#define GPU_DEV 0x7408
#define GPU_SS_V 0x1002
#define GPU_SS_D 0x0C35
#define GPU_REV 0xA1
#define GPU_CLASS 0x0302
#define BAR0_SIZE (16*MiB)
#define BAR1_SIZE (64*MiB)
#define CLK_BASE 906
#define CLK_BOOST 1700
#define TYPE_AMD_MI200 "amd-mi200"
OBJECT_DECLARE_SIMPLE_TYPE(AmdMi200State, AMD_MI200)
struct AmdMi200State { PCIDevice parent_obj; MemoryRegion bar0,bar1; uint32_t clock_mhz; uint64_t clock_last_ns; char *gpu_name; };
static uint32_t clk_f(AmdMi200State*s){ uint64_t now=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); if(now-s->clock_last_ns>100000000ULL){s->clock_last_ns=now;s->clock_mhz=s->clock_mhz*1664525u+1013904223u;s->clock_mhz=CLK_BASE+(s->clock_mhz%(CLK_BOOST-CLK_BASE+1));} return s->clock_mhz; }
static uint64_t b0r(void*op,hwaddr a,unsigned sz){ (void)sz; AmdMi200State*s=(AmdMi200State*)op; uint32_t c=clk_f(s); if(a==0x2A0)return c/10; if(a==0x2540)return 60*8; return 0; }
static void b0w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b0={.read=b0r,.write=b0w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t b1r(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;} static void b1w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b1={.read=b1r,.write=b1w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static void realize(PCIDevice*p,Error**e){ AmdMi200State*s=(AmdMi200State*)p; s->clock_mhz=CLK_BASE;s->clock_last_ns=0; p->config[PCI_CLASS_PROG]=0; pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SS_V); pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SS_D); uint8_t*c=p->config; c[PCI_CAPABILITY_LIST]=0x50;c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;c[0x50]=0x10;c[0x51]=0;pci_set_word(c+0x52,0x0002);pci_set_long(c+0x5C,(0x10<<4)|4);pci_set_word(c+0x62,(0x10<<4)|4|(1<<13)); memory_region_init_io(&s->bar0,OBJECT(s),&b0,s,"amd-mi200-mmio",BAR0_SIZE);pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64,&s->bar0); memory_region_init_io(&s->bar1,OBJECT(s),&b1,s,"amd-mi200-vram",BAR1_SIZE);pci_register_bar(p,2,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1); }
static const Property props[]={DEFINE_PROP_STRING("gpu-name",AmdMi200State,gpu_name),};
static const VMStateDescription vms={.name="amd-mi200",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AmdMi200State),VMSTATE_UINT32(clock_mhz,AmdMi200State),VMSTATE_UINT64(clock_last_ns,AmdMi200State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){ DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=AMD_VID;pc->device_id=GPU_DEV;pc->class_id=GPU_CLASS;pc->revision=GPU_REV;dc->desc="AMD Instinct MI200 (Aldebaran GFX90A, 128GB HBM2e)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories); }
static const TypeInfo ti={.name=TYPE_AMD_MI200,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AmdMi200State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);} type_init(reg)
