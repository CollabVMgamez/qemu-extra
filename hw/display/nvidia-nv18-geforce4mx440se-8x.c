/* NVIDIA GeForce4 MX 440SE-8X (NV18)
 * GPU stub. Vendor: 0x10DE, Device: 0x0182
 * Usage: -device nvidia-nv18-geforce4mx440se-8x
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/pci/pci_bus.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define GPU_VID 0x10DE
#define GPU_DEV 0x0182
#define GPU_SS_V 0x10DE
#define GPU_SS_D 0x0010
#define GPU_REV 0xA1
#define GPU_CLASS 0x0300
#define GPU_VRAM_MB 32ULL
#define TYPE_NVIDIANV18GEFORCE4MX440SE8X "nvidia-nv18-geforce4mx440se-8x"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaNv18Geforce4mx440se8xState, NVIDIANV18GEFORCE4MX440SE8X)
struct NvidiaNv18Geforce4mx440se8xState { PCIDevice parent_obj; MemoryRegion mmio,vram; uint32_t gpu_count; char *gpu_name; };
static uint64_t gpu_read(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;}
static void gpu_write(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps gpu_ops={.read=gpu_read,.write=gpu_write,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static const MemoryRegionOps vram_ops={.read=gpu_read,.write=gpu_write,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static void realize(PCIDevice*p,Error**e){ NvidiaNv18Geforce4mx440se8xState*s=(NvidiaNv18Geforce4mx440se8xState*)p; s->gpu_count=1; memory_region_init_io(&s->mmio,OBJECT(s),&gpu_ops,s,"nvidia-nv18-geforce4mx440se-8x-mmio",0x1000000);pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->mmio); memory_region_init_io(&s->vram,OBJECT(s),&vram_ops,s,"nvidia-nv18-geforce4mx440se-8x-vram",GPU_VRAM_MB*1024ULL*1024ULL);pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->vram);if(s->gpu_count>1){PCIBus*bus=pci_get_bus(p);const char*tn=object_get_typename(OBJECT(s));for(uint32_t i=1;i<s->gpu_count&&i<8;i++){PCIDevice*ex=pci_new(-1,tn);if(ex){qdev_prop_set_uint32(DEVICE(ex),"gpu-count",1);pci_realize_and_unref(ex,bus,&error_abort);pci_set_word(ex->config+PCI_CLASS_DEVICE,0x0302);}}}}
static const Property props[]={DEFINE_PROP_UINT32("gpu-count",NvidiaNv18Geforce4mx440se8xState,gpu_count,1),DEFINE_PROP_STRING("gpu-name",NvidiaNv18Geforce4mx440se8xState,gpu_name),};
static const VMStateDescription vms={.name="nvidia-nv18-geforce4mx440se-8x",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,NvidiaNv18Geforce4mx440se8xState),VMSTATE_UINT32(gpu_count,NvidiaNv18Geforce4mx440se8xState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){ DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=GPU_VID;pc->device_id=GPU_DEV;pc->class_id=GPU_CLASS;pc->revision=GPU_REV;dc->desc="NVIDIA GeForce4 MX 440SE-8X (NV18)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories); }
static const TypeInfo ti={.name=TYPE_NVIDIANV18GEFORCE4MX440SE8X,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(NvidiaNv18Geforce4mx440se8xState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);} type_init(reg)
