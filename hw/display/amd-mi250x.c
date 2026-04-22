/* AMD Instinct MI250X (Aldebaran GFX90A, 128GB HBM2e)
 * AMD GPU stub. Vendor: 0x1002, Device: 0x7408
 * Usage: -device amd-mi250x
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
#define GPU_SS_D 0x0C38
#define GPU_REV 0xA1
#define GPU_CLASS 0x0302
#define BAR0_SIZE (16*MiB)
#define BAR1_SIZE (64*MiB)
#define CLK_BASE 906
#define CLK_BOOST 1700
#define TYPE_AMD_MI250X "amd-mi250x"
OBJECT_DECLARE_SIMPLE_TYPE(AmdMi250xState, AMD_MI250X)
struct AmdMi250xState { PCIDevice parent_obj; MemoryRegion bar0,bar1; uint32_t clock_mhz; uint64_t clock_last_ns; char *gpu_name;     char *board_partner;
};
static uint32_t clk_f(AmdMi250xState*s){ uint64_t now=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); if(now-s->clock_last_ns>100000000ULL){s->clock_last_ns=now;s->clock_mhz=s->clock_mhz*1664525u+1013904223u;s->clock_mhz=CLK_BASE+(s->clock_mhz%(CLK_BOOST-CLK_BASE+1));} return s->clock_mhz; }
static uint64_t b0r(void*op,hwaddr a,unsigned sz){ (void)sz; AmdMi250xState*s=(AmdMi250xState*)op; uint32_t c=clk_f(s); if(a==0x2A0)return c/10; if(a==0x2540)return 60*8; return 0; }
static void b0w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b0={.read=b0r,.write=b0w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t b1r(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;} static void b1w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b1={.read=b1r,.write=b1w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static void realize(PCIDevice*p,Error**e){ AmdMi250xState*s=(AmdMi250xState*)p; s->clock_mhz=CLK_BASE;s->clock_last_ns=0; p->config[PCI_CLASS_PROG]=0; pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SS_V);
    if (s->board_partner) {
        static const struct { const char *name; uint16_t vid; } amd_partners[] = {
            {"amd",0x1002},{"asus",0x1043},{"msi",0x1462},{"gigabyte",0x1458},
            {"sapphire",0x1B0A},{"xfx",0x1682},{"powercolor",0x148C},
            {"his",0x17AF},{"xfx",0x1682},{"visiontek",0x1545},
            {"lenovo",0x17AA},{"hp",0x103C},{"dell",0x1028},{NULL,0}
        };
        for (int _i = 0; amd_partners[_i].name; _i++) {
            if (g_ascii_strcasecmp(s->board_partner, amd_partners[_i].name) == 0) {
                pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID, amd_partners[_i].vid);
                break;
            }
        }
    }
 pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SS_D); uint8_t*c=p->config; c[PCI_CAPABILITY_LIST]=0x50;c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;c[0x50]=0x10;c[0x51]=0;pci_set_word(c+0x52,0x0002);pci_set_long(c+0x5C,(0x10<<4)|4);pci_set_word(c+0x62,(0x10<<4)|4|(1<<13)); memory_region_init_io(&s->bar0,OBJECT(s),&b0,s,"amd-mi250x-mmio",BAR0_SIZE);pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64,&s->bar0); memory_region_init_io(&s->bar1,OBJECT(s),&b1,s,"amd-mi250x-vram",BAR1_SIZE);pci_register_bar(p,2,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1); }
static const Property props[]={DEFINE_PROP_STRING("gpu-name",AmdMi250xState,gpu_name),    DEFINE_PROP_STRING("board-partner", AmdMi250xState, board_partner),
};
static const VMStateDescription vms={.name="amd-mi250x",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AmdMi250xState),VMSTATE_UINT32(clock_mhz,AmdMi250xState),VMSTATE_UINT64(clock_last_ns,AmdMi250xState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){ DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=AMD_VID;pc->device_id=GPU_DEV;pc->class_id=GPU_CLASS;pc->revision=GPU_REV;dc->desc="AMD Instinct MI250X (Aldebaran GFX90A, 128GB HBM2e)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories); }
static const TypeInfo ti={.name=TYPE_AMD_MI250X,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AmdMi250xState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);} type_init(reg)
