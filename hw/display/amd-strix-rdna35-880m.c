/* AMD Strix Point Radeon 880M (Ryzen AI 7 365 iGPU)
 * AMD APU iGPU stub. Vendor: 0x1002, Device: 0x1901
 * Usage: -device amd-strix-rdna35-880m
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
#define GPU_VID 0x1002
#define GPU_DEV 0x1901
#define GPU_SS_V 0x1022
#define GPU_SS_D 0x1901
#define GPU_REV 0xC9
#define GPU_CLASS 0x0300
#define GPU_VRAM_MB 2048ULL
#define TYPE_AMDSTRIXRDNA35880M "amd-strix-rdna35-880m"
OBJECT_DECLARE_SIMPLE_TYPE(AmdStrixRdna35880mState, AMDSTRIXRDNA35880M)
struct AmdStrixRdna35880mState { PCIDevice parent_obj; MemoryRegion mmio,vram; uint32_t gpu_count; char *gpu_name; };
static uint64_t gpu_read(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;}
static void gpu_write(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps gpu_ops={.read=gpu_read,.write=gpu_write,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static const MemoryRegionOps vram_ops={.read=gpu_read,.write=gpu_write,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static void realize(PCIDevice*p,Error**e){ AmdStrixRdna35880mState*s=(AmdStrixRdna35880mState*)p; s->gpu_count=1; memory_region_init_io(&s->mmio,OBJECT(s),&gpu_ops,s,"amd-strix-rdna35-880m-mmio",0x1000000);pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->mmio); memory_region_init_io(&s->vram,OBJECT(s),&vram_ops,s,"amd-strix-rdna35-880m-vram",GPU_VRAM_MB*1024ULL*1024ULL);pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->vram);if(s->gpu_count>1){PCIBus*bus=pci_get_bus(p);const char*tn=object_get_typename(OBJECT(s));for(uint32_t i=1;i<s->gpu_count&&i<8;i++){PCIDevice*ex=pci_create_simple(bus,-1,tn);if(ex){qdev_prop_set_uint32(DEVICE(ex),"gpu-count",1);Error*le=NULL;qdev_realize(DEVICE(ex),&bus->qbus,&le);if(le){error_free(le);break;}}}}}
static const Property props[]={DEFINE_PROP_UINT32("gpu-count",AmdStrixRdna35880mState,gpu_count,1),DEFINE_PROP_STRING("gpu-name",AmdStrixRdna35880mState,gpu_name),};
static const VMStateDescription vms={.name="amd-strix-rdna35-880m",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AmdStrixRdna35880mState),VMSTATE_UINT32(gpu_count,AmdStrixRdna35880mState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){ DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=GPU_VID;pc->device_id=GPU_DEV;pc->class_id=GPU_CLASS;pc->revision=GPU_REV;dc->desc="AMD Strix Point Radeon 880M (Ryzen AI 7 365 iGPU)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories); }
static const TypeInfo ti={.name=TYPE_AMDSTRIXRDNA35880M,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AmdStrixRdna35880mState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);} type_init(reg)
