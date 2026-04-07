/*
 * ATI Radeon X1900 XT (R580, PCIe x16, 512MB GDDR3, 2006)
 * Auto-generated AMD/ATI GPU stub for QEMU P4 fork.
 * AGP cards are exposed on the PCI bus (QEMU has no native AGP bus).
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define GPU_VENDOR_ID  0x1002
#define GPU_DEVICE_ID  0x7240
#define GPU_SUBSYS_VID 0x1002
#define GPU_SUBSYS_DID 0x7240
#define GPU_REVISION   0x00
#define GPU_CLASS      0x0300
#define NV_BAR0_SIZE (16*MiB)
#define NV_BAR1_SIZE (256*MiB)
#define NV_BAR3_SIZE (32*MiB)
#define NV_BAR5_SIZE (128*KiB)
#define ATI_CHIP_ID     0x000000
#define ATI_CHIP_REV    0x000004
#define ATI_MEM_SIZE    0x0F0028
#define ATI_CLK_CNTL    0x000008
#define NV_PTIMER_TIME_0 0x009400
#define NV_PTIMER_TIME_1 0x009410
#define NV_PFB_BOOT_0    0x100800
#define NV_PFB_PARTS     0x100200
#define NV_PFB_REFCTRL   0x100210
#define GPU_CHIP_ID     0x00000009
#define GPU_PFB_BOOT_0  0x00000012
#define GPU_PFB_PARTS   0x00000004
#define GPU_PFB_REFCTRL 0x00000043
#define GPU_CLK_BASE    625
#define GPU_CLK_BOOST   625
#define GPU_PCIE_GEN    2
#define GPU_VRAM_MB     512
#define TYPE_ATI_RADEON_X1900XT "ati-radeon-x1900xt"
OBJECT_DECLARE_SIMPLE_TYPE(AtiRadeonX1900xtState, ATI_RADEON_X1900XT)
struct AtiRadeonX1900xtState {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en, clock_mhz;
    uint64_t clock_last_ns;
};
static uint32_t gpu_clk(AtiRadeonX1900xtState *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (GPU_CLK_BOOST - GPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    AtiRadeonX1900xtState *s = opaque; uint32_t m; uint64_t n;
    switch (addr & ~3ULL) {
    case ATI_CHIP_ID:   return GPU_CHIP_ID;
    case ATI_CHIP_REV:  return 0x00000001;
    case ATI_MEM_SIZE:  return (uint32_t)(GPU_VRAM_MB * 1024 * 1024);
    case NV_PFB_BOOT_0: return GPU_PFB_BOOT_0;
    case NV_PFB_PARTS: return GPU_PFB_PARTS;
    case NV_PFB_REFCTRL: return GPU_PFB_REFCTRL;
    case NV_PTIMER_TIME_0: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)((n*m/1000ULL)&0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFFULL);
    default: return 0;
    }
}
static void bar0_w(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps b0ops={.read=bar0_r,.write=bar0_w,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
static uint64_t sr(void*o,hwaddr a,unsigned s){return 0;}
static void sw(void*o,hwaddr a,uint64_t v,unsigned s){}
static const MemoryRegionOps b1ops={.read=sr,.write=sw,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=8}};
static const MemoryRegionOps b35ops={.read=sr,.write=sw,.endianness=DEVICE_LITTLE_ENDIAN,.valid={.min_access_size=1,.max_access_size=4}};
#define MC 0x50
#define PC 0x60
static void gpu_caps(PCIDevice *p) {
    uint8_t *c=p->config;
    c[PCI_CAPABILITY_LIST]=MC; c[PCI_STATUS]|=PCI_STATUS_CAP_LIST;
    c[MC]=0x05;c[MC+1]=PC;c[MC+2]=0x01;c[MC+3]=0;
    c[PC]=0x10;c[PC+1]=0;
    pci_set_word(c+PC+2,0x0002); pci_set_long(c+PC+4,0x00008000|GPU_PCIE_GEN);
    pci_set_word(c+PC+8,0); pci_set_word(c+PC+10,0);
    pci_set_long(c+PC+12,(0x10<<4)|GPU_PCIE_GEN);
    pci_set_word(c+PC+16,0);
    pci_set_word(c+PC+18,(0x10<<4)|GPU_PCIE_GEN|(1<<13));
}
static void gpu_realize(PCIDevice *p, Error **e) {
    AtiRadeonX1900xtState *s=(AtiRadeonX1900xtState*)p;
    s->clock_mhz=GPU_CLK_BASE; s->clock_last_ns=0;
    p->config[PCI_CLASS_PROG]=0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SUBSYS_DID);
    gpu_caps(p);
    memory_region_init_io(&s->bar0,OBJECT(s),&b0ops,s,"ati-radeon-x1900xt-mmio",NV_BAR0_SIZE);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar0);
    memory_region_init_io(&s->bar1,OBJECT(s),&b1ops,s,"ati-radeon-x1900xt-vram",NV_BAR1_SIZE);
    pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1);
    memory_region_init_io(&s->bar3,OBJECT(s),&b35ops,s,"ati-radeon-x1900xt-ramin",NV_BAR3_SIZE);
    pci_register_bar(p,3,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar3);
    memory_region_init_io(&s->bar5,OBJECT(s),&b35ops,s,"ati-radeon-x1900xt-vgaio",NV_BAR5_SIZE);
    pci_register_bar(p,5,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->bar5);
}
static const VMStateDescription vms_ati_radeon_x1900xt={.name="ati-radeon-x1900xt",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AtiRadeonX1900xtState),VMSTATE_UINT32(intr_en,AtiRadeonX1900xtState),VMSTATE_UINT32(clock_mhz,AtiRadeonX1900xtState),VMSTATE_UINT64(clock_last_ns,AtiRadeonX1900xtState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=gpu_realize; pc->vendor_id=GPU_VENDOR_ID; pc->device_id=0x7240;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REVISION;
    dc->desc="ATI Radeon X1900 XT (R580, PCIe x16, 512MB GDDR3, 2006)"; dc->vmsd=&vms_ati_radeon_x1900xt; dc->hotpluggable=false;
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti_ati_radeon_x1900xt={.name=TYPE_ATI_RADEON_X1900XT,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AtiRadeonX1900xtState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg_ati_radeon_x1900xt(void){type_register_static(&ti_ati_radeon_x1900xt);}
type_init(reg_ati_radeon_x1900xt)
