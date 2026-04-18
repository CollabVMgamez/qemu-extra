/*
 * Samsung 870 EVO Plus NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_870_EVO_PLUS "pci-samsung_870_evo_plus"

OBJECT_DECLARE_SIMPLE_TYPE(Samsung870EvoPlusState, SAMSUNG_870_EVO_PLUS)

typedef struct Samsung870EvoPlusState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} Samsung870EvoPlusState;

static uint64_t samsung_870_evo_plus_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void samsung_870_evo_plus_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps samsung_870_evo_plus_mmio_ops = {
    .read = samsung_870_evo_plus_mmio_read,
    .write = samsung_870_evo_plus_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_samsung_870_evo_plus_realize(PCIDevice *pdev, Error **errp)
{
    Samsung870EvoPlusState *s = SAMSUNG_870_EVO_PLUS(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_870_evo_plus_mmio_ops, s,
                          "samsung_870_evo_plus-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_samsung_870_evo_plus = {
    .name = "samsung_870_evo_plus",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Samsung870EvoPlusState),
        VMSTATE_END_OF_LIST()
    }
};

static void samsung_870_evo_plus_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_samsung_870_evo_plus_realize;
    k->vendor_id = 0x144d;
    k->device_id = 0xa804;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Samsung 870 EVO Plus";
    dc->vmsd = &vmstate_samsung_870_evo_plus;
}

static const TypeInfo samsung_870_evo_plus_info = {
    .name = TYPE_SAMSUNG_870_EVO_PLUS,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Samsung870EvoPlusState),
    .class_init = samsung_870_evo_plus_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void samsung_870_evo_plus_register(void)
{
    type_register_static(&samsung_870_evo_plus_info);
}

type_init(samsung_870_evo_plus_register)
