/*
 * NVIDIA GeForce4 MX 440 (NV17, AGP 8x, 64MB DDR, 2002)
 * NVIDIA AGP-era GPU stub with AGP capability for QEMU P4 fork.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define GPU_VENDOR_ID  0x10DE
#define GPU_DEVICE_ID  0x0171
#define GPU_SUBSYS_VID 0x10DE
#define GPU_SUBSYS_DID 0x0171
#define GPU_REVISION   0x00
#define GPU_CLASS      0x0300
#define GPU_MEM_MHZ    270
#define NV_BAR0_SIZE (16*MiB)
#define NV_BAR1_SIZE (64*MiB)
#define NV_BAR3_SIZE (16*MiB)
#define NV_BAR5_SIZE (128*KiB)
#define ATI_CHIP_ID     0x000000
#define ATI_MEM_SIZE    0x0F0028
#define NV_PMC_BOOT_0   0x000000
#define NV_PMC_BOOT_1   0x000004
#define NV_PTIMER_TIME_0 0x009400
#define NV_PTIMER_TIME_1 0x009410
#define NV_PFB_BOOT_0   0x100800
#define NV_PFB_PARTS    0x100200
#define NV_PFB_REFCTRL  0x100210
#define GPU_PMC_BOOT_0  0x00000007
#define GPU_PFB_BOOT_0  0x00000011
#define GPU_PFB_PARTS   0x00000002
#define GPU_PFB_REFCTRL 0x00000029
#define GPU_CLK_BASE    270
#define GPU_CLK_BOOST   270
#define AGP_CAP_OFFSET  0x40

#define TYPE_NVIDIA_NV17_GEFORCE4MX440 "nvidia-nv17-geforce4mx440"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaNv17Geforce4mx440State, NVIDIA_NV17_GEFORCE4MX440)
struct NvidiaNv17Geforce4mx440State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en, clock_mhz;
    uint64_t clock_last_ns;
    uint32_t gpu_count;
};
static uint32_t gpu_clk(NvidiaNv17Geforce4mx440State *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (GPU_CLK_BOOST - GPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    NvidiaNv17Geforce4mx440State *s = opaque; uint32_t m; uint64_t n;
    switch (addr & ~3ULL) {
    case NV_PMC_BOOT_0:    return GPU_PMC_BOOT_0;
    case NV_PMC_BOOT_1:    return 0x00000001;
    case NV_PFB_BOOT_0:    return GPU_PFB_BOOT_0;
    case NV_PFB_PARTS:     return GPU_PFB_PARTS;
    case NV_PFB_REFCTRL:   return GPU_PFB_REFCTRL;
    case ATI_MEM_SIZE:     return (uint32_t)(64 * 1024 * 1024);
    case NV_PTIMER_TIME_0: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)((n*m/1000ULL)&0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFFULL);
    /* Temperature */
    case 0x020050: case 0x020400: case 0x020440: {
        uint64_t _ns=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _t=50+(uint32_t)((_ns>>28)%20); return ((_t+120)<<16);
    }
    case 0x132020: return (0<<16)|(((GPU_MEM_MHZ*2)/27)<<8)|2;
    case 0x1373f0: case 0x00410C: return GPU_MEM_MHZ;
    default: return 0;
    }
}
static void bar0_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps b0ops={.read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t sr(void*o,hwaddr a,unsigned s){return 0;}
static void sw(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps b135ops={.read=sr,.write=sw,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};

static void gpu_realize(PCIDevice *p, Error **e) {
    NvidiaNv17Geforce4mx440State *s=(NvidiaNv17Geforce4mx440State*)p;
    s->clock_mhz=GPU_CLK_BASE; s->clock_last_ns=0; s->gpu_count=1;
    p->config[PCI_CLASS_PROG]=0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SUBSYS_DID);
    /* AGP 2.0 capability at offset 0x40 */
    {
        uint8_t *c=p->config;
        c[PCI_CAPABILITY_LIST]=AGP_CAP_OFFSET;
        c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;
        c[AGP_CAP_OFFSET+0]=0x02; /* AGP cap ID */
        c[AGP_CAP_OFFSET+1]=0x00; /* no next cap */
        c[AGP_CAP_OFFSET+2]=0x02; /* AGP 2.0 */
        c[AGP_CAP_OFFSET+3]=0x00;
        pci_set_long(c+AGP_CAP_OFFSET+4, 0x1F000217); /* AGP_STATUS */
        pci_set_long(c+AGP_CAP_OFFSET+8, 0x00000000); /* AGP_COMMAND */
    }
    memory_region_init_io(&s->bar0,OBJECT(s),&b0ops,s,"nvidia-nv17-geforce4mx440-mmio",NV_BAR0_SIZE);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar0);
    memory_region_init_io(&s->bar1,OBJECT(s),&b135ops,s,"nvidia-nv17-geforce4mx440-vram",NV_BAR1_SIZE);
    pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1);
    memory_region_init_io(&s->bar3,OBJECT(s),&b135ops,s,"nvidia-nv17-geforce4mx440-ramin",NV_BAR3_SIZE);
    pci_register_bar(p,3,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar3);
    memory_region_init_io(&s->bar5,OBJECT(s),&b135ops,s,"nvidia-nv17-geforce4mx440-vgaio",NV_BAR5_SIZE);
    pci_register_bar(p,5,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->bar5);
}
static const Property gpu_props_nvidia_nv17_geforce4mx440[]={DEFINE_PROP_UINT32("gpu-count",NvidiaNv17Geforce4mx440State,gpu_count,1)};
static const VMStateDescription vms_nvidia_nv17_geforce4mx440={.name="nvidia-nv17-geforce4mx440",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,NvidiaNv17Geforce4mx440State),VMSTATE_UINT32(intr_en,NvidiaNv17Geforce4mx440State),VMSTATE_UINT32(clock_mhz,NvidiaNv17Geforce4mx440State),VMSTATE_UINT64(clock_last_ns,NvidiaNv17Geforce4mx440State),VMSTATE_UINT32(gpu_count,NvidiaNv17Geforce4mx440State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=gpu_realize; pc->vendor_id=GPU_VENDOR_ID; pc->device_id=0x0171;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REVISION;
    dc->desc="NVIDIA GeForce4 MX 440 (NV17, AGP 8x, 64MB DDR, 2002)"; dc->vmsd=&vms_nvidia_nv17_geforce4mx440; dc->hotpluggable=false;
    device_class_set_props(dc,gpu_props_nvidia_nv17_geforce4mx440);
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti_nvidia_nv17_geforce4mx440={.name=TYPE_NVIDIA_NV17_GEFORCE4MX440,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(NvidiaNv17Geforce4mx440State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg_nvidia_nv17_geforce4mx440(void){type_register_static(&ti_nvidia_nv17_geforce4mx440);}
type_init(reg_nvidia_nv17_geforce4mx440)
