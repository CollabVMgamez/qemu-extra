/* NVIDIA RTX 4000 Ada Generation (AD104, Ada, 20GB GDDR6)
 * NVIDIA GPU stub. Vendor: 0x10DE, Device: 0x2687
 * Usage: -device nvidia-rtx4000-ada
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/pci/pci_bus.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define NV_VID 0x10DE
#define GPU_DEV 0x2687
#define GPU_SS_V 0x10DE
#define GPU_SS_D 0x13B2
#define GPU_REV 0xA1
#define GPU_CLASS 0x0300
#define NV_BAR0_SIZE (16*MiB)
#define NV_BAR1_SIZE (128*MiB)
#define NV_BAR3_SIZE (32*MiB)
#define NV_BAR5_SIZE (128*KiB)
#define CLK_BASE 1680
#define CLK_BOOST 2175
#define PMC_BOOT_0 0x192000A1
#define PFB_PARTS 0x0000000A
#define PFB_REFCTRL 0x800006C9
#define TYPE_NVIDIARTX4000ADA "nvidia-rtx4000-ada"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaRtx4000AdaState, NVIDIARTX4000ADA)
struct NvidiaRtx4000AdaState { PCIDevice parent_obj; MemoryRegion bar0,bar1,bar3,bar5; uint32_t intr_en,pfifo_intr_en,clock_mhz; uint64_t clock_last_ns; uint32_t gpu_count; char *gpu_name,*board_partner; };
static uint32_t clk_f(NvidiaRtx4000AdaState*s){ uint64_t now=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); if(now-s->clock_last_ns>100000000ULL){s->clock_last_ns=now;s->clock_mhz=s->clock_mhz*1664525u+1013904223u;s->clock_mhz=CLK_BASE+(s->clock_mhz%(CLK_BOOST-CLK_BASE+1));} return s->clock_mhz; }
static uint64_t b0r(void*op,hwaddr a,unsigned sz){ (void)sz; NvidiaRtx4000AdaState*s=(NvidiaRtx4000AdaState*)op; uint32_t m; uint64_t n;
switch(a&~3ULL){
case 0x000000: return PMC_BOOT_0;
case 0x000004: return 1;
case 0x000100: case 0x000140: case 0x001100: case 0x002100: return 0;
case 0x009200: return 31; case 0x009210: return 3;
case 0x009400: m=clk_f(s);n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);return(uint32_t)((n*m/1000ULL)&0xFFFFFFFF);
case 0x009410: m=clk_f(s);n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);return(uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFF);
case 0x100800: return 0x00000015;
case 0x100200: case 0x11020C: return PFB_PARTS;
case 0x100210: return PFB_REFCTRL;
case 0x400700: case 0x610020: return 0;
case 0x020050: case 0x020400: case 0x020440: { uint64_t ns=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); uint32_t seed=(uint32_t)(ns>>28)*1664525u+1013904223u; uint32_t tc=55+(seed%(CLK_BOOST-CLK_BASE+1))*17/(CLK_BOOST-CLK_BASE+1); return((tc+120)<<16); }
case 0x132020: { uint32_t M=2,P=0,N=(1750*M)/27; return(P<<16)|(N<<8)|M; }
case 0x1373f0: case 0x00410C: return 1750;
case 0x070090: return 35;
default: return 0;
} }
static void b0w(void*o,hwaddr a,uint64_t v,unsigned s){ NvidiaRtx4000AdaState*ss=(NvidiaRtx4000AdaState*)o; if((a&~3ULL)==0x000140)ss->intr_en=(uint32_t)v; else if((a&~3ULL)==0x002100)ss->pfifo_intr_en=(uint32_t)v; }
static const MemoryRegionOps b0ops={.read=b0r,.write=b0w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t sr(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;} static void sw(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b1ops={.read=sr,.write=sw,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static const MemoryRegionOps b35ops={.read=sr,.write=sw,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void realize(PCIDevice*p,Error**e){ NvidiaRtx4000AdaState*s=(NvidiaRtx4000AdaState*)p; s->clock_mhz=CLK_BASE;s->clock_last_ns=0;s->gpu_count=1; p->config[PCI_CLASS_PROG]=0;pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SS_V);
    if (s->board_partner) {
        static const struct { const char *name; uint16_t vid; } nv_partners[] = {
            {"nvidia",0x10DE},{"asus",0x1043},{"msi",0x1462},{"gigabyte",0x1458},{"evga",0x3842},
            {"zotac",0x19DA},{"palit",0x1569},{"pny",0x196E},{"inno3d",0x1ACC},
            {"colorful",0x7377},{"gainward",0x1569},{"galax",0x1B4C},
            {"lenovo",0x17AA},{"hp",0x103C},{"dell",0x1028},{NULL,0}
        };
        for (int _i = 0; nv_partners[_i].name; _i++) {
            if (g_ascii_strcasecmp(s->board_partner, nv_partners[_i].name) == 0) {
                pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, nv_partners[_i].vid);
                break;
            }
        }
    }
pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SS_D); uint8_t*c=p->config;c[PCI_CAPABILITY_LIST]=0x50;c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;c[0x50]=0x05;c[0x51]=0x60;c[0x52]=0x01;c[0x53]=0;c[0x60]=0x10;c[0x61]=0;pci_set_word(c+0x62,0x0002);pci_set_long(c+0x64,0x00008000|4);pci_set_long(c+0x6C,(0x10<<4)|4);pci_set_word(c+0x72,(0x10<<4)|4|(1<<13)); memory_region_init_io(&s->bar0,OBJECT(s),&b0ops,s,"nvidia-rtx4000-ada-mmio",NV_BAR0_SIZE);pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar0); memory_region_init_io(&s->bar1,OBJECT(s),&b1ops,s,"nvidia-rtx4000-ada-vram",NV_BAR1_SIZE);pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1); memory_region_init_io(&s->bar3,OBJECT(s),&b35ops,s,"nvidia-rtx4000-ada-ramin",NV_BAR3_SIZE);pci_register_bar(p,3,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar3); memory_region_init_io(&s->bar5,OBJECT(s),&b35ops,s,"nvidia-rtx4000-ada-vgaio",NV_BAR5_SIZE);pci_register_bar(p,5,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->bar5);if(s->gpu_count>1){PCIBus*bus=pci_get_bus(p);const char*tn=object_get_typename(OBJECT(s));for(uint32_t i=1;i<s->gpu_count&&i<8;i++){PCIDevice*ex=pci_new(-1,tn);if(ex){qdev_prop_set_uint32(DEVICE(ex),"gpu-count",1);pci_realize_and_unref(ex,bus,&error_abort);pci_set_word(ex->config+PCI_CLASS_DEVICE,0x0302);}}} }
static const Property props[]={DEFINE_PROP_UINT32("gpu-count",NvidiaRtx4000AdaState,gpu_count,1),DEFINE_PROP_STRING("gpu-name",NvidiaRtx4000AdaState,gpu_name),DEFINE_PROP_STRING("board-partner",NvidiaRtx4000AdaState,board_partner),};
static const VMStateDescription vms={.name="nvidia-rtx4000-ada",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,NvidiaRtx4000AdaState),VMSTATE_UINT32(intr_en,NvidiaRtx4000AdaState),VMSTATE_UINT32(pfifo_intr_en,NvidiaRtx4000AdaState),VMSTATE_UINT32(clock_mhz,NvidiaRtx4000AdaState),VMSTATE_UINT64(clock_last_ns,NvidiaRtx4000AdaState),VMSTATE_UINT32(gpu_count,NvidiaRtx4000AdaState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){ DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=NV_VID;pc->device_id=GPU_DEV;pc->class_id=GPU_CLASS;pc->revision=GPU_REV;dc->desc="NVIDIA RTX 4000 Ada Generation (AD104, Ada, 20GB GDDR6)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories); }
static const TypeInfo ti={.name=TYPE_NVIDIARTX4000ADA,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(NvidiaRtx4000AdaState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);} type_init(reg)
