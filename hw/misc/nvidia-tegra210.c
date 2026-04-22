#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/pci/pci_ids.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_NVIDIA_TEGRA210 "nvidia-tegra210"
OBJECT_DECLARE_SIMPLE_TYPE(Tegra210State, NVIDIA_TEGRA210)

struct Tegra210State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *model;
};

static uint64_t tegra210_read(void *opaque, hwaddr addr, unsigned size)
{
    switch (addr & ~3ULL) {
    case 0x000: return 0x21000001;
    case 0x004: return 0x00000001;
    case 0x008: return 0x00000120;
    case 0x010: return 0x80000000;
    case 0x014: return 0x00000400;
    default:    return 0;
    }
}

static void tegra210_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps tegra210_ops = {
    .read = tegra210_read, .write = tegra210_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void tegra210_realize(PCIDevice *pdev, Error **errp)
{
    Tegra210State *s = NVIDIA_TEGRA210(pdev);
    pdev->config[PCI_CLASS_PROG] = 0x00;

    memory_region_init_io(&s->mmio, OBJECT(s), &tegra210_ops, s,
                          "tegra210-mmio", 16384);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property tegra210_props[] = {
    DEFINE_PROP_STRING("model", Tegra210State, model),
};

static const VMStateDescription vmstate_tegra210 = {
    .name = "nvidia-tegra210", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Tegra210State),
        VMSTATE_END_OF_LIST()
    },
};

static void tegra210_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize   = tegra210_realize;
    k->vendor_id = 0x10DE;
    k->device_id = 0x0EAA;
    k->class_id  = 0x1200;
    k->revision  = 0x01;
    dc->desc         = "NVIDIA Tegra X1 (T210) SoC Processor";
    dc->vmsd         = &vmstate_tegra210;
    dc->hotpluggable = false;
    device_class_set_props(dc, tegra210_props);
}

static const TypeInfo tegra210_info = {
    .name = TYPE_NVIDIA_TEGRA210,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Tegra210State),
    .class_init = tegra210_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};

static void tegra210_register(void) { type_register_static(&tegra210_info); }
type_init(tegra210_register)
