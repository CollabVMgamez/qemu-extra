/* NVIDIA GeForce RTX 4090 Mobile (AD103, 16GB GDDR6)
 * Vendor: 0x10DE, Device: 0x2780
 * Usage: -device nvidia-rtx4090m
 * Copyright (c) 2024-2025 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define GPU_VID 0x10DE
#define GPU_DEV 0x2780
#define GPU_SS_V 0x10DE
#define GPU_SS_D 0x17C4
#define GPU_CLASS 0x0302
#define BAR0_SIZE (4*MiB)
#define BAR1_SIZE (64*MiB)
#define TYPE_NVIDIA_RTX4090M "nvidia-rtx4090m"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaRtx4090mState, NVIDIA_RTX4090M)
struct NvidiaRtx4090mState { PCIDevice parent_obj; MemoryRegion bar0,bar1; uint32_t clock_mhz; uint64_t clock_last_ns; char *gpu_name;     char *board_partner;
};
static uint64_t b0r(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;}
static void b0w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b0={.read=b0r,.write=b0w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t b1r(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;}
static void b1w(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps b1={.read=b1r,.write=b1w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static void realize(PCIDevice*p,Error**e){{
    NvidiaRtx4090mState*s=(NvidiaRtx4090mState*)p; s->clock_mhz=0;s->clock_last_ns=0;
    p->config[PCI_CLASS_PROG]=0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SS_V);
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

    pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SS_D);
    uint8_t*c=p->config;c[PCI_CAPABILITY_LIST]=0x50;c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;
    c[0x50]=0x10;c[0x51]=0;pci_set_word(c+0x52,0x0002);pci_set_long(c+0x5C,(0x10<<4)|3);
    memory_region_init_io(&s->bar0,OBJECT(s),&b0,s,"nvidia-rtx4090m-mmio",BAR0_SIZE);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64,&s->bar0);
    memory_region_init_io(&s->bar1,OBJECT(s),&b1,s,"nvidia-rtx4090m-vram",BAR1_SIZE);
    pci_register_bar(p,2,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_64|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1);
}}
static const Property props[]={DEFINE_PROP_STRING("gpu-name",NvidiaRtx4090mState,gpu_name),    DEFINE_PROP_STRING("board-partner", NvidiaRtx4090mState, board_partner),
};
static const VMStateDescription vms={.name="nvidia-rtx4090m",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,NvidiaRtx4090mState),VMSTATE_UINT32(clock_mhz,NvidiaRtx4090mState),VMSTATE_UINT64(clock_last_ns,NvidiaRtx4090mState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=GPU_VID;pc->device_id=GPU_DEV;pc->class_id=GPU_CLASS;pc->revision=0;dc->desc="NVIDIA GeForce RTX 4090 Mobile (AD103, 16GB GDDR6)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);}
static const TypeInfo ti={.name=TYPE_NVIDIA_RTX4090M,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(NvidiaRtx4090mState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);} type_init(reg)
