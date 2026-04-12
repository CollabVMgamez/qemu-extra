/*
 * ATI Radeon X800 XT (R420, AGP 8x, 256MB GDDR3, 2004)
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
#define GPU_DEVICE_ID  0x4A4A
#define GPU_SUBSYS_VID 0x1002
#define GPU_SUBSYS_DID 0x4A4A
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
#define GPU_CHIP_ID     0x00000006
#define GPU_PFB_BOOT_0  0x00000012
#define GPU_PFB_PARTS   0x00000004
#define GPU_PFB_REFCTRL 0x00000039
#define GPU_CLK_BASE    520
#define GPU_MEM_MHZ     500
#define GPU_CLK_BOOST   520
#define GPU_PCIE_GEN    1
#define GPU_VRAM_MB     256
#define TYPE_ATI_RADEON_X800XT "ati-radeon-x800xt"
OBJECT_DECLARE_SIMPLE_TYPE(AtiRadeonX800xtState, ATI_RADEON_X800XT)
struct AtiRadeonX800xtState {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en, clock_mhz;
    uint32_t gpu_count;
    uint64_t clock_last_ns;
};
static uint32_t gpu_clk(AtiRadeonX800xtState *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (GPU_CLK_BOOST - GPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    AtiRadeonX800xtState *s = opaque; uint32_t m; uint64_t n;
    switch (addr & ~3ULL) {
    case ATI_CHIP_ID:   return GPU_CHIP_ID;
    case ATI_CHIP_REV:  return 0x00000001;
    case ATI_MEM_SIZE:  return (uint32_t)(GPU_VRAM_MB * 1024 * 1024);
    case NV_PFB_BOOT_0: return GPU_PFB_BOOT_0;
    case NV_PFB_PARTS: return GPU_PFB_PARTS;
    case NV_PFB_REFCTRL: return GPU_PFB_REFCTRL;
    case NV_PTIMER_TIME_0: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)((n*m/1000ULL)&0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFFULL);


    /* AMD GCN thermal/clock registers that GPU-Z reads:
     * CG_MULT_THERMAL_STATUS: bits[12:0] = temp * 8
     * CURCLK_GFXIP_FREQ: GPU clock in MHz
     * CURCLK_MEMIP_FREQ: memory clock in MHz */
    case 0x000E0300: { /* CG_MULT_THERMAL_STATUS */
        uint64_t _ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _seed = (uint32_t)(_ns >> 28);
        _seed = _seed * 1664525u + 1013904223u;
        uint32_t _range = GPU_CLK_BOOST - GPU_CLK_BASE + 1;
        uint32_t _tc = 55 + (_range ? (_seed % _range) * 17 / _range : 0);
        _tc += (_seed >> 28) & 3;
        return _tc * 8; /* bits[12:0] = temp * 8 */
    }
    case 0x0000C0FC: /* CURCLK_GFXIP_FREQ - GPU clock in MHz */ {
        uint64_t _ns2 = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _s2 = (uint32_t)(_ns2 >> 26) * 1664525u + 1013904223u;
        uint32_t _r2 = GPU_CLK_BOOST - GPU_CLK_BASE + 1;
        return GPU_CLK_BASE + (_r2 ? _s2 % _r2 : 0);
    }
    case 0x0000C100: /* CURCLK_MEMIP_FREQ - memory clock in MHz */
        return GPU_MEM_MHZ;
    case 0x00009498: /* MC_ARB_RAMCFG - memory config */
        return 0x00000200; /* 256-bit bus */
    /* SMC mailbox: GPU-Z sends messages to read sensors */
    case 0x0001A008: /* SMC_RESP_0 - always return 1 (success) */
        return 0x00000001;
    case 0x0001A020: { /* SMC_MSG_ARG_0 - return temp in mC for GetTemperature */
        uint64_t _ns3 = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _s3 = (uint32_t)(_ns3 >> 28) * 1664525u + 1013904223u;
        uint32_t _r3 = GPU_CLK_BOOST - GPU_CLK_BASE + 1;
        uint32_t _tc3 = 55 + (_r3 ? (_s3 % _r3) * 17 / _r3 : 0);
        return _tc3 * 1000; /* millidegrees Celsius */
    }
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
    AtiRadeonX800xtState *s=(AtiRadeonX800xtState*)p;
    s->clock_mhz=GPU_CLK_BASE; s->clock_last_ns=0;
    p->config[PCI_CLASS_PROG]=0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SUBSYS_DID);

    /* AGP Capability (PCI_CAP_ID_AGP = 0x02) at config offset 0x40.
     * Required by Linux agpgart and Windows AGP miniport to recognize
     * this as an AGP device. */
    {
        uint8_t *c = p->config;
        uint8_t agp_off = 0x40;
        /* Point cap list to AGP cap, AGP points to MSI at 0x50 */
        c[PCI_CAPABILITY_LIST] = agp_off;
        c[PCI_STATUS] |= PCI_STATUS_CAP_LIST;
        c[agp_off + 0] = 0x02;          /* Cap ID: AGP */
        c[agp_off + 1] = 0x50;          /* Next cap: MSI at 0x50 */
        c[agp_off + 2] = 0x02;          /* AGP major version: 2 */
        c[agp_off + 3] = 0x00;          /* AGP minor version: .0 */
        /* AGP_STATUS: RQ=31, SBA, FW, RATE=0x7 (1x/2x/4x) */
        pci_set_long(c + agp_off + 4, 0x1F000217);
        /* AGP_COMMAND: 0 = AGP disabled until driver enables it */
        pci_set_long(c + agp_off + 8, 0x00000000);
    }
    gpu_caps(p);
    memory_region_init_io(&s->bar0,OBJECT(s),&b0ops,s,"ati-radeon-x800xt-mmio",NV_BAR0_SIZE);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar0);
    memory_region_init_io(&s->bar1,OBJECT(s),&b1ops,s,"ati-radeon-x800xt-vram",NV_BAR1_SIZE);
    pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1);
    memory_region_init_io(&s->bar3,OBJECT(s),&b35ops,s,"ati-radeon-x800xt-ramin",NV_BAR3_SIZE);
    pci_register_bar(p,3,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar3);
    memory_region_init_io(&s->bar5,OBJECT(s),&b35ops,s,"ati-radeon-x800xt-vgaio",NV_BAR5_SIZE);
    pci_register_bar(p,5,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->bar5);
}
static const VMStateDescription vms_ati_radeon_x800xt={.name="ati-radeon-x800xt",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AtiRadeonX800xtState),VMSTATE_UINT32(intr_en,AtiRadeonX800xtState),VMSTATE_UINT32(clock_mhz,AtiRadeonX800xtState),VMSTATE_UINT64(clock_last_ns,AtiRadeonX800xtState),VMSTATE_END_OF_LIST()}};
static const Property gpu_multi_props_AtiRadeonX800xtState[] = {
    DEFINE_PROP_UINT32("gpu-count", AtiRadeonX800xtState, gpu_count, 1),
};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=gpu_realize; pc->vendor_id=GPU_VENDOR_ID; pc->device_id=0x4A4A;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REVISION;
    dc->desc="ATI Radeon X800 XT (R420, AGP 8x, 256MB GDDR3, 2004)"; dc->vmsd=&vms_ati_radeon_x800xt; dc->hotpluggable=false;
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti_ati_radeon_x800xt={.name=TYPE_ATI_RADEON_X800XT,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AtiRadeonX800xtState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg_ati_radeon_x800xt(void){type_register_static(&ti_ati_radeon_x800xt);}
type_init(reg_ati_radeon_x800xt)
