/*
 * Samsung 990 EVO NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_990_EVO "pci-samsung_990_evo"

OBJECT_DECLARE_SIMPLE_TYPE(Samsung990EvoState, SAMSUNG_990_EVO)

typedef struct Samsung990EvoState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} Samsung990EvoState;

static uint64_t samsung_990_evo_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void samsung_990_evo_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps samsung_990_evo_mmio_ops = {
    .read = samsung_990_evo_mmio_read,
    .write = samsung_990_evo_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_samsung_990_evo_realize(PCIDevice *pdev, Error **errp)
{
    Samsung990EvoState *s = SAMSUNG_990_EVO(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_990_evo_mmio_ops, s,
                          "samsung_990_evo-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_samsung_990_evo = {
    .name = "samsung_990_evo",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Samsung990EvoState),
        VMSTATE_END_OF_LIST()
    }
};

static void samsung_990_evo_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_samsung_990_evo_realize;
    k->vendor_id = 0x144d;
    k->device_id = 0xa80b;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Samsung 990 EVO";
    dc->vmsd = &vmstate_samsung_990_evo;
}

static const TypeInfo samsung_990_evo_info = {
    .name = TYPE_SAMSUNG_990_EVO,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Samsung990EvoState),
    .class_init = samsung_990_evo_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void samsung_990_evo_register(void)
{
    type_register_static(&samsung_990_evo_info);
}

type_init(samsung_990_evo_register)
