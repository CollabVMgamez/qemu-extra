/* AMD XDNA 2 PRO NPU (Ryzen AI PRO, Strix Point)
 * AMD NPU stub. Vendor: 0x1022, Device: 0x17F1
 * Usage: -device amd-xdna-2-pro
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later */
#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/pci/pci_bus.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#define NPU_VID 0x1022
#define NPU_DEV 0x17F1
#define NPU_SS_V 0x1022
#define NPU_SS_D 0x17F1
#define NPU_REV 0x01
#define NPU_CLASS 0x0880
#define TYPE_AMDXDNA2PRO "amd-xdna-2-pro"
OBJECT_DECLARE_SIMPLE_TYPE(AmdXDNA2PROState, AMDXDNA2PRO)
struct AmdXDNA2PROState { PCIDevice parent_obj; MemoryRegion mmio; uint32_t npu_count; char *npu_name; };
static uint64_t npu_read(void*o,hwaddr a,unsigned s){(void)o;(void)a;(void)s;return 0;}
static void npu_write(void*o,hwaddr a,uint64_t v,unsigned s){(void)o;(void)a;(void)v;(void)s;}
static const MemoryRegionOps npu_ops={.read=npu_read,.write=npu_write,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static void realize(PCIDevice*p,Error**e){ AmdXDNA2PROState*s=(AmdXDNA2PROState*)p; s->npu_count=1; memory_region_init_io(&s->mmio,OBJECT(s),&npu_ops,s,"amd-xdna-2-pro-mmio",0x10000); pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->mmio); }
static const Property props[]={DEFINE_PROP_UINT32("npu-count",AmdXDNA2PROState,npu_count,1),DEFINE_PROP_STRING("npu-name",AmdXDNA2PROState,npu_name),};
static const VMStateDescription vms={.name="amd-xdna-2-pro",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AmdXDNA2PROState),VMSTATE_UINT32(npu_count,AmdXDNA2PROState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass*k,const void*d){ DeviceClass*dc=DEVICE_CLASS(k);PCIDeviceClass*pc=PCI_DEVICE_CLASS(k);pc->realize=realize;pc->vendor_id=NPU_VID;pc->device_id=NPU_DEV;pc->class_id=NPU_CLASS;pc->revision=NPU_REV;dc->desc="AMD XDNA 2 PRO NPU (Ryzen AI PRO, Strix Point)";dc->vmsd=&vms;dc->hotpluggable=false;device_class_set_props(dc,props);set_bit(DEVICE_CATEGORY_MISC,dc->categories); }
static const TypeInfo ti={.name=TYPE_AMDXDNA2PRO,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AmdXDNA2PROState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg(void){type_register_static(&ti);} type_init(reg)
