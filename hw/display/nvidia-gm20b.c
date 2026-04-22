#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/pci/pci_ids.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define NVIDIA_VENDOR_ID   0x10DE
#define GM20B_DEVICE_ID    0x13B2
#define GM20B_SUBSYS_VID   0x10DE
#define GM20B_SUBSYS_DID   0x13B2
#define GM20B_REVISION     0xA1
#define GM20B_CLASS        0x0300

#define GPU_VRAM_MB 4096
#define NV_BAR0_SIZE (16 * MiB)
#define NV_BAR1_SIZE (256 * MiB)

#define NV_PMC_BOOT_0 0x000000

#define GM20B_PMC_BOOT_0 0x120000A1

#define CLK_BASE  1000
#define CLK_BOOST 1465

#define TYPE_NVIDIA_GM20B "nvidia-gm20b"
OBJECT_DECLARE_SIMPLE_TYPE(NvidiaGM20BState, NVIDIA_GM20B)

struct NvidiaGM20BState {
    PCIDevice parent_obj;
    MemoryRegion bar0, bar1;
    uint32_t intr_en;
    uint32_t clock_mhz;
    uint64_t clock_last_ns;
    char *gpu_name;
    char *board_partner;
};

static uint32_t gm20b_get_clock(NvidiaGM20BState *s)
{
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->clock_last_ns > 100000000ULL) {
        s->clock_last_ns = now;
        s->clock_mhz = s->clock_mhz * 1664525u + 1013904223u;
        uint32_t r = CLK_BOOST - CLK_BASE;
        s->clock_mhz = CLK_BASE + (s->clock_mhz % (r + 1));
    }
    return s->clock_mhz;
}

static uint64_t gm20b_bar0_read(void *opaque, hwaddr addr, unsigned size)
{
    NvidiaGM20BState *s = opaque;
    switch (addr & ~3ULL) {
    case NV_PMC_BOOT_0:     return GM20B_PMC_BOOT_0;
    case 0x000004:          return 0x00000001;
    case 0x000100:
    case 0x000140:
    case 0x001100:
    case 0x002100:
    case 0x009100:
    case 0x009140:          return 0;
    case 0x009200:          return 31;
    case 0x009210:          return 3;
    case 0x009400: {
        uint64_t ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        uint32_t mhz = gm20b_get_clock(s);
        return (uint32_t)((ns * mhz / 1000ULL) & 0xFFFFFFFFULL);
    }
    case 0x100800:          return 0x0000000C;
    case 0x100200:          return 0x00000002;
    case 0x400700:
    case 0x610020:          return 0;
    default:                return 0;
    }
}

static void gm20b_bar0_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    NvidiaGM20BState *s = opaque;
    switch (addr & ~3ULL) {
    case 0x000140: s->intr_en = (uint32_t)val; break;
    default: break;
    }
}

static const MemoryRegionOps gm20b_bar0_ops = {
    .read = gm20b_bar0_read, .write = gm20b_bar0_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static uint64_t stub_read(void *o, hwaddr a, unsigned s) { return 0; }
static void     stub_write(void *o, hwaddr a, uint64_t v, unsigned s) {}

static const MemoryRegionOps bar1_ops = {
    .read = stub_read, .write = stub_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 8 },
};

static void gm20b_realize(PCIDevice *pdev, Error **errp)
{
    NvidiaGM20BState *s = NVIDIA_GM20B(pdev);
    s->clock_mhz = CLK_BASE;
    s->clock_last_ns = 0;

    pdev->config[PCI_CLASS_PROG] = 0x00;
    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, GM20B_SUBSYS_VID);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, GM20B_SUBSYS_DID);

    if (s->board_partner) {
        static const struct { const char *name; uint16_t vid; } nv_partners[] = {
            {"nvidia",0x10DE},{"asus",0x1043},{"msi",0x1462},{"gigabyte",0x1458},
            {"zotac",0x19DA},{"palit",0x1569},{"pny",0x196E},
            {"lenovo",0x17AA},{"hp",0x103C},{"dell",0x1028},{NULL,0}
        };
        for (int _i = 0; nv_partners[_i].name; _i++) {
            if (g_ascii_strcasecmp(s->board_partner, nv_partners[_i].name) == 0) {
                pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, nv_partners[_i].vid);
                break;
            }
        }
    }

    memory_region_init_io(&s->bar0, OBJECT(s), &gm20b_bar0_ops, s,
                          "nvidia-gm20b-mmio", NV_BAR0_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar0);

    memory_region_init_io(&s->bar1, OBJECT(s), &bar1_ops, s,
                          "nvidia-gm20b-vram", NV_BAR1_SIZE);
    pci_register_bar(pdev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_32 |
                     PCI_BASE_ADDRESS_MEM_PREFETCH, &s->bar1);
}

static const VMStateDescription vmstate_gm20b = {
    .name = "nvidia-gm20b", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, NvidiaGM20BState),
        VMSTATE_UINT32(intr_en, NvidiaGM20BState),
        VMSTATE_UINT32(clock_mhz, NvidiaGM20BState),
        VMSTATE_UINT64(clock_last_ns, NvidiaGM20BState),
        VMSTATE_END_OF_LIST()
    },
};

static const Property gm20b_props[] = {
    DEFINE_PROP_STRING("gpu-name", NvidiaGM20BState, gpu_name),
    DEFINE_PROP_STRING("board-partner", NvidiaGM20BState, board_partner),
};

static void gm20b_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize   = gm20b_realize;
    k->vendor_id = NVIDIA_VENDOR_ID;
    k->device_id = GM20B_DEVICE_ID;
    k->class_id  = GM20B_CLASS;
    k->revision  = GM20B_REVISION;
    dc->desc         = "NVIDIA GM20B (Maxwell, Tegra X1, Nintendo Switch)";
    dc->vmsd         = &vmstate_gm20b;
    dc->hotpluggable = false;
    device_class_set_props(dc, gm20b_props);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo gm20b_info = {
    .name = TYPE_NVIDIA_GM20B,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NvidiaGM20BState),
    .class_init = gm20b_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};

static void gm20b_register(void) { type_register_static(&gm20b_info); }
type_init(gm20b_register)
