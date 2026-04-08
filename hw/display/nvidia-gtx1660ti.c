/*
 * NVIDIA GeForce GTX 1660 Ti (TU116, Turing)
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

#define GPU_VENDOR_ID  0x10DE
#define GPU_DEVICE_ID  0x2182
#define GPU_SUBSYS_VID 0x10DE
#define GPU_SUBSYS_DID 0x1164
#define GPU_REVISION   0xA1
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
#define GPU_PMC_BOOT_0  0x168000A1
#define GPU_PFB_BOOT_0  0x00000015
#define GPU_PFB_PARTS   0x00000006
#define GPU_PFB_REFCTRL 0x80000073
#define GPU_CLK_BASE    1500
#define GPU_MEM_MHZ     2000
#define GPU_CLK_BOOST   1770
#define GPU_PCIE_GEN    3
#define TYPE_NVIDIA_GTX1660TI "nvidia-gtx1660ti"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaGtx1660tiState, NVIDIA_GTX1660TI)
struct NvidiaGtx1660tiState {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1, bar3, bar5;
    uint32_t intr_en, pfifo_intr_en, clock_mhz;
    uint64_t clock_last_ns;
};
static uint32_t gpu_clk(NvidiaGtx1660tiState *s) {
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        s->clock_mhz = GPU_CLK_BASE + (s->clock_mhz % (GPU_CLK_BOOST - GPU_CLK_BASE + 1));
    }
    return s->clock_mhz;
}
static uint64_t bar0_r(void *opaque, hwaddr addr, unsigned size) {
    NvidiaGtx1660tiState *s = opaque; uint32_t m; uint64_t n;
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

    /* Temperature: GPU-Z reads NV_THERM_I2CS_SENSOR at 0x020050
     * bits[23:16] = raw ADC, formula: celsius = raw - 120
     * Fluctuates 55-72C based on timer + LCG */
    case 0x020050: /* NV_THERM_I2CS_SENSOR */
    case 0x020400: /* NV_THERM_TEMP */
    case 0x020440: { /* NV_THERM_TEMP_1 */
        uint64_t _ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _seed = (uint32_t)(_ns >> 28); /* changes ~270ms */
        _seed = _seed * 1664525u + 1013904223u;
        uint32_t _range = GPU_CLK_BOOST - GPU_CLK_BASE + 1;
        uint32_t _rel = _range ? (_seed % _range) : 0;
        uint32_t _tc = 55 + (_rel * 17) / (_range ? _range : 1);
        _tc += (_seed >> 28) & 3; /* ±3C noise */
        return ((_tc + 120) << 16);
    }
    /* Memory clock PLL: mem_mhz = (27*N)/(M*(1<<P))
     * GPU-Z reads 0x132020, computes freq from N/M/P fields */
    case 0x132020: { /* NV_CLK_MCLK_PLL_COEFF */
        uint32_t _M = 2, _P = 0;
        uint32_t _N = (GPU_MEM_MHZ * _M) / 27;
        uint64_t _ns2 = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        _N += (uint32_t)(_ns2 >> 29) & 1; /* ±1 flicker */
        return (_P << 16) | (_N << 8) | _M;
    }
    case 0x1373f0: /* NV_CLK_MCLK_OUT */
    case 0x00410C: /* NV_PERF_MEM_CLK */
        return GPU_MEM_MHZ;
    case 0x070090: { /* NV_PFAN_PERCENT: 30-45% */
        uint64_t _ns3 = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t _fs = (uint32_t)(_ns3 >> 28) * 1664525u + 1013904223u;
        uint32_t _range2 = GPU_CLK_BOOST - GPU_CLK_BASE + 1;
        return 30 + (_range2 ? ((_fs % _range2) * 15 / _range2) : 0);
    }
    default: return 0;
    }
}
static void bar0_w(void *opaque, hwaddr addr, uint64_t val, unsigned size) {
    NvidiaGtx1660tiState *s = opaque;
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
    NvidiaGtx1660tiState *s=(NvidiaGtx1660tiState*)p;
    s->clock_mhz=GPU_CLK_BASE; s->clock_last_ns=0;
    p->config[PCI_CLASS_PROG]=0;
    pci_set_word(p->config+PCI_SUBSYSTEM_VENDOR_ID,GPU_SUBSYS_VID);
    pci_set_word(p->config+PCI_SUBSYSTEM_ID,GPU_SUBSYS_DID);
    gpu_caps(p);
    memory_region_init_io(&s->bar0,OBJECT(s),&b0ops,s,"nvidia-gtx1660ti-mmio",NV_BAR0_SIZE);
    pci_register_bar(p,0,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar0);
    memory_region_init_io(&s->bar1,OBJECT(s),&b1ops,s,"nvidia-gtx1660ti-vram",NV_BAR1_SIZE);
    pci_register_bar(p,1,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32|PCI_BASE_ADDRESS_MEM_PREFETCH,&s->bar1);
    memory_region_init_io(&s->bar3,OBJECT(s),&b35ops,s,"nvidia-gtx1660ti-ramin",NV_BAR3_SIZE);
    pci_register_bar(p,3,PCI_BASE_ADDRESS_SPACE_MEMORY|PCI_BASE_ADDRESS_MEM_TYPE_32,&s->bar3);
    memory_region_init_io(&s->bar5,OBJECT(s),&b35ops,s,"nvidia-gtx1660ti-vgaio",NV_BAR5_SIZE);
    pci_register_bar(p,5,PCI_BASE_ADDRESS_SPACE_MEMORY,&s->bar5);
}
static const VMStateDescription vms_nvidia_gtx1660ti={.name="nvidia-gtx1660ti",.version_id=1,.minimum_version_id=1,.fields=(const VMStateField[]){VMSTATE_PCI_DEVICE(parent_obj,NvidiaGtx1660tiState),VMSTATE_UINT32(intr_en,NvidiaGtx1660tiState),VMSTATE_UINT32(pfifo_intr_en,NvidiaGtx1660tiState),VMSTATE_UINT32(clock_mhz,NvidiaGtx1660tiState),VMSTATE_UINT64(clock_last_ns,NvidiaGtx1660tiState),VMSTATE_END_OF_LIST()}};
static void ci(ObjectClass *k, const void *d) {
    DeviceClass *dc=DEVICE_CLASS(k); PCIDeviceClass *pc=PCI_DEVICE_CLASS(k);
    pc->realize=gpu_realize; pc->vendor_id=GPU_VENDOR_ID; pc->device_id=0x2182;
    pc->class_id=GPU_CLASS; pc->revision=GPU_REVISION;
    dc->desc="NVIDIA GeForce GTX 1660 Ti (TU116, Turing)"; dc->vmsd=&vms_nvidia_gtx1660ti; dc->hotpluggable=false;
    set_bit(DEVICE_CATEGORY_DISPLAY,dc->categories);
}
static const TypeInfo ti_nvidia_gtx1660ti={.name=TYPE_NVIDIA_GTX1660TI,.parent=TYPE_PCI_DEVICE,.instance_size=sizeof(NvidiaGtx1660tiState),.class_init=ci,.interfaces=(InterfaceInfo[]){{INTERFACE_CONVENTIONAL_PCI_DEVICE},{}}};
static void reg_nvidia_gtx1660ti(void){type_register_static(&ti_nvidia_gtx1660ti);}
type_init(reg_nvidia_gtx1660ti)
