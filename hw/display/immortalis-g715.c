/*
 * MediaTek Immortalis-G715 (Dimensity 9200, integrated)
 * Mobile/SoC integrated GPU stub for QEMU P4 fork.
 * Note: these have no standard PCI IDs; vendor IDs are representative only.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define GPU_VENDOR_ID  0x14C3
#define GPU_DEVICE_ID  0xD715
#define GPU_SUBSYS_VID 0x14C3
#define GPU_SUBSYS_DID 0xD715
#define GPU_REVISION   0xA1
#define GPU_CLASS      0x0300
#define GPU_MEM_MHZ    848
#define NV_BAR0_SIZE (16*MiB)
#define NV_BAR1_SIZE (64*MiB)
#define NV_BAR3_SIZE (8*MiB)
#define NV_BAR5_SIZE (64*KiB)
#define GPU_PMC_BOOT_0  0x14C30001
#define GPU_PFB_BOOT_0  0x00000015
#define GPU_PFB_PARTS   0x00000001
#define GPU_PFB_REFCTRL 0x80000083
#define VPD_CAP_OFFSET 0x70
#define GPU_CLK_BASE    848
#define GPU_CLK_BOOST   848
#define NV_PMC_BOOT_0   0x000000
#define ATI_MEM_SIZE    0x0F0028
#define NV_PTIMER_TIME_0 0x009400
#define NV_PTIMER_TIME_1 0x009410
#define NV_PFB_BOOT_0    0x100800
#define NV_PFB_PARTS_R   0x100200
#define NV_PFB_REFCTRL_R 0x100210
#define GPU_VRAM_MB     8

#define TYPE_IMMORTALIS_G715 "immortalis-g715"
OBJECT_DECLARE_SIMPLE_TYPE(ImmortalisG715State, IMMORTALIS_G715)
struct ImmortalisG715State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    uint32_t gpu_count;
    char *gpu_name;
};
static uint32_t gpu_clk(ImmortalisG715State *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (GPU_CLK_BOOST - GPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    ImmortalisG715State *s = opaque; uint32_t m; uint64_t n;
    switch (addr & ~3ULL) {
    case NV_PMC_BOOT_0:     return GPU_PMC_BOOT_0;
    case NV_PFB_BOOT_0:     return GPU_PFB_BOOT_0;
    case NV_PFB_PARTS_R:    return GPU_PFB_PARTS;
    case NV_PFB_REFCTRL_R:  return GPU_PFB_REFCTRL;
    case ATI_MEM_SIZE:      return (uint32_t)(GPU_VRAM_MB * 1024 * 1024);
    case 0x000E0300: { /* AMD GCN temperature */
        uint64_t _ns=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _t=45+(uint32_t)((_ns>>28)%25); return _t * 8;
    }
    case 0x0000C0FC:        return gpu_clk(s);
    case 0x0000C100:        return GPU_MEM_MHZ;
    case 0x020050: case 0x020400: case 0x020440: {
        uint64_t _ns2=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _t2=45+(uint32_t)((_ns2>>28)%25); return ((_t2+120)<<16);
    }
    case 0x132020: return (0<<16)|(((GPU_MEM_MHZ*2)/27)<<8)|2;
    case 0x1373f0: case 0x00410C: return GPU_MEM_MHZ;
    case NV_PTIMER_TIME_0: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)((n*m/1000ULL)&0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFFULL);
    default: return 0;
    }
}
static void bar0_w(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps b0ops={.read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t sr(void*o,hwaddr a,unsigned s){return 0;}
static void sw(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps bops={.read=sr,.write=sw,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static void gpu_realize(PCIDevice *p, Error **e) {
    ImmortalisG715State *s=(ImmortalisG715State*)p;
    s->clock_mhz=GPU_CLK_BASE; s->clock_last_ns=0; s->gpu_count=1;
    p->config[PCI_CLASS_PROG]=0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SUBSYS_DID);

    /* PCI VPD capability — allows lspci -vv and GPU-Z to read custom product name */
    if (s->gpu_name && s->gpu_name[0]) {
        uint8_t *c = p->config;
        size_t name_len = strlen(s->gpu_name);
        if (name_len > 40) name_len = 40;  /* cap at 40 chars */

        /* Chain VPD after existing caps: find end of cap chain and add 0x70 */
        /* MSI at 0x50 points to PCIe at 0x60, PCIe next=0x00 -> add VPD */
        if (c[0x61] == 0x00) c[0x61] = VPD_CAP_OFFSET;  /* PCIe next -> VPD */
        else if (c[0x51] == 0x60) c[0x51] = VPD_CAP_OFFSET; /* MSI next -> VPD */

        c[VPD_CAP_OFFSET + 0] = 0x03;   /* Cap ID: VPD */
        c[VPD_CAP_OFFSET + 1] = 0x00;   /* next cap: end */
        /* VPD address register: address=0, F=1 (data ready) */
        c[VPD_CAP_OFFSET + 2] = 0x00;
        c[VPD_CAP_OFFSET + 3] = 0x80;   /* F=1: data ready */
        /* VPD data at offset 0: first 4 bytes of VPD ROM */
        /* VPD ROM: 0x82 [len_lo][len_hi] [name...] 0x79 [end] */
        /* We put the name directly in config bytes 0x74..0x7F */
        /* lspci reads VPD_DATA (cap+4) = config[VPD_CAP+4..+7] */
        /* For a 40-char name, lspci does multiple reads at address 0,4,8... */
        /* Simplest: store VPD ROM in bytes 0x74 onward */
        uint8_t *vpd = c + VPD_CAP_OFFSET + 4;
        vpd[0] = 0x82;                    /* Large Resource: Product Name */
        vpd[1] = (uint8_t)name_len;       /* length low */
        vpd[2] = 0x00;                    /* length high */
        memcpy(vpd + 3, s->gpu_name, name_len);
        vpd[3 + name_len] = 0x79;         /* VPD-R end tag */
    }
    p->config[PCI_CAPABILITY_LIST]=0x50; p->config[PCI_STATUS]|=PCI_STATUS_CAP_LIST;
    p->config[0x50]=0x05;p->config[0x51]=0x00;p->config[0x52]=0x01;p->config[0x53]=0x00;
    memory_region_init_io(&s->bar0,OBJECT(s),&b0ops,s,"immortalis-g715-mmio",NV_BAR0_SIZE);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar0);
    memory_region_init_io(&s->bar1,OBJECT(s),&bops,s,"immortalis-g715-vram",NV_BAR1_SIZE);
    pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1);
    memory_region_init_io(&s->bar3,OBJECT(s),&bops,s,"immortalis-g715-ramin",NV_BAR3_SIZE);
    pci_register_bar(p,3,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar3);
    memory_region_init_io(&s->bar5,OBJECT(s),&bops,s,"immortalis-g715-vgaio",NV_BAR5_SIZE);
    pci_register_bar(p,5,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->bar5);
}
static const Property gpu_props_immortalis_g715[]={DEFINE_PROP_UINT32("gpu-count",ImmortalisG715State,gpu_count,1),
    DEFINE_PROP_STRING("gpu-name",ImmortalisG715State,gpu_name)};
static const VMStateDescription vms_immortalis_g715={.name="immortalis-g715",.version_id=1,.minimum_version_id=1,
    .fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,ImmortalisG715State),VMSTATE_UINT32(clock_mhz,ImmortalisG715State),VMSTATE_UINT64(clock_last_ns,ImmortalisG715State),VMSTATE_UINT32(gpu_count,ImmortalisG715State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=gpu_realize; pc->vendor_id=GPU_VENDOR_ID; pc->device_id=0xD715;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REVISION;
    dc->desc="MediaTek Immortalis-G715 (Dimensity 9200, integrated)"; dc->vmsd=&vms_immortalis_g715; dc->hotpluggable=false;
    device_class_set_props(dc,gpu_props_immortalis_g715);
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti_immortalis_g715={.name=TYPE_IMMORTALIS_G715,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(ImmortalisG715State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg_immortalis_g715(void){type_register_static(&ti_immortalis_g715);}
type_init(reg_immortalis_g715)
