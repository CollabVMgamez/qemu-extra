/*
 * AMD Radeon RX 460 (Polaris11, GCN 4.0)
 * Auto-generated GPU stub for QEMU P4 fork.
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
#define GPU_DEVICE_ID  0x67EF
#define GPU_SUBSYS_VID 0x1002
#define GPU_SUBSYS_DID 0x0460
#define GPU_REVISION   0xC7
#define GPU_CLASS      0x0300
#define NV_BAR0_SIZE (16*MiB)
#define NV_BAR1_SIZE (256*MiB)
#define NV_BAR3_SIZE (32*MiB)
#define NV_BAR5_SIZE (128*KiB)
#define NV_PMC_BOOT_0       0x000000
#define NV_PMC_BOOT_1       0x000004
#define NV_PMC_INTR_0       0x000100
#define NV_PMC_INTR_EN_0    0x000140
#define NV_PBUS_INTR_0      0x001100
#define NV_PFIFO_INTR_0     0x002100
#define NV_PTIMER_NUMERATOR  0x009200
#define NV_PTIMER_DENOMINATOR 0x009210
#define NV_PTIMER_TIME_0    0x009400
#define NV_PTIMER_TIME_1    0x009410
#define NV_PFB_BOOT_0       0x100800
#define NV_PFB_PARTS        0x100200
#define NV_PFB_REFCTRL      0x100210
#define NV_PFB_MEM_PARTS    0x11020C
#define NV_PGRAPH_STATUS    0x400700
#define NV_PDISP_INTR_0     0x610020
#define GPU_PMC_BOOT_0  0x128000C7
#define GPU_PFB_BOOT_0  0x00000013
#define GPU_PFB_PARTS   0x00000004
#define GPU_PFB_REFCTRL 0x80000053
#define GPU_CLK_BASE    1090
#define GPU_MEM_MHZ     1750
#define GPU_CLK_BOOST   1200
#define GPU_PCIE_GEN    3
#define TYPE_AMD_RX460 "amd-rx460"
OBJECT_DECLARE_SIMPLE_TYPE(AmdRx460State, AMD_RX460)
struct AmdRx460State {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en, pfifo_intr_en, clock_mhz;
    uint64_t clock_last_ns;
};
static uint32_t gpu_clk(AmdRx460State *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (GPU_CLK_BOOST - GPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    AmdRx460State *s = opaque; uint32_t m; uint64_t n;
    switch (addr & ~3ULL) {
    case NV_PMC_BOOT_0:         return GPU_PMC_BOOT_0;
    case NV_PMC_BOOT_1:         return 1;
    case NV_PMC_INTR_0: case NV_PMC_INTR_EN_0: case NV_PBUS_INTR_0: case NV_PFIFO_INTR_0: return 0;
    case NV_PTIMER_NUMERATOR:   return 31;
    case NV_PTIMER_DENOMINATOR: return 3;
    case NV_PTIMER_TIME_0: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)((n*m/1000ULL)&0xFFFFFFFFULL);
    case NV_PTIMER_TIME_1: m=gpu_clk(s); n=qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL); return (uint32_t)(((n*m/1000ULL)>>32)&0xFFFFFFFFULL);
    case NV_PFB_BOOT_0:         return GPU_PFB_BOOT_0;
    case NV_PFB_PARTS: case NV_PFB_MEM_PARTS: return GPU_PFB_PARTS;
    case NV_PFB_REFCTRL:        return GPU_PFB_REFCTRL;


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
static void bar0_w(void *opaque, hwaddr addr, uint64_t val, unsigned size) {
    AmdRx460State *s = opaque;
    if ((addr&~3ULL)==NV_PMC_INTR_EN_0) s->intr_en=(uint32_t)val;
    else if ((addr&~3ULL)==NV_PFIFO_INTR_0) s->pfifo_intr_en=(uint32_t)val;
}
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
    AmdRx460State *s=(AmdRx460State*)p;
    s->clock_mhz=GPU_CLK_BASE; s->clock_last_ns=0;
    p->config[PCI_CLASS_PROG]=0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SUBSYS_DID);
    gpu_caps(p);
    memory_region_init_io(&s->bar0,OBJECT(s),&b0ops,s,"amd-rx460-mmio",NV_BAR0_SIZE);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar0);
    memory_region_init_io(&s->bar1,OBJECT(s),&b1ops,s,"amd-rx460-vram",NV_BAR1_SIZE);
    pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1);
    memory_region_init_io(&s->bar3,OBJECT(s),&b35ops,s,"amd-rx460-ramin",NV_BAR3_SIZE);
    pci_register_bar(p,3,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar3);
    memory_region_init_io(&s->bar5,OBJECT(s),&b35ops,s,"amd-rx460-vgaio",NV_BAR5_SIZE);
    pci_register_bar(p,5,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->bar5);
}
static const VMStateDescription vms_amd_rx460={.name="amd-rx460",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,AmdRx460State),VMSTATE_UINT32(intr_en,AmdRx460State),VMSTATE_UINT32(pfifo_intr_en,AmdRx460State),VMSTATE_UINT32(clock_mhz,AmdRx460State),VMSTATE_UINT64(clock_last_ns,AmdRx460State),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=gpu_realize; pc->vendor_id=GPU_VENDOR_ID; pc->device_id=0x67EF;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REVISION;
    dc->desc="AMD Radeon RX 460 (Polaris11, GCN 4.0)"; dc->vmsd=&vms_amd_rx460; dc->hotpluggable=false;
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti_amd_rx460={.name=TYPE_AMD_RX460,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(AmdRx460State),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg_amd_rx460(void){type_register_static(&ti_amd_rx460);}
type_init(reg_amd_rx460)
