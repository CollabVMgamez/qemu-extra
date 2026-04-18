/*
 * Sabrent Rocket 4 Plus-G NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SABRENT_ROCKET_4_PLUS_G "pci-sabrent_rocket_4_plus_g"

OBJECT_DECLARE_SIMPLE_TYPE(SabrentRocket4PlusGState, SABRENT_ROCKET_4_PLUS_G)

typedef struct SabrentRocket4PlusGState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} SabrentRocket4PlusGState;

static uint64_t sabrent_rocket_4_plus_g_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void sabrent_rocket_4_plus_g_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps sabrent_rocket_4_plus_g_mmio_ops = {
    .read = sabrent_rocket_4_plus_g_mmio_read,
    .write = sabrent_rocket_4_plus_g_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_sabrent_rocket_4_plus_g_realize(PCIDevice *pdev, Error **errp)
{
    SabrentRocket4PlusGState *s = SABRENT_ROCKET_4_PLUS_G(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &sabrent_rocket_4_plus_g_mmio_ops, s,
                          "sabrent_rocket_4_plus_g-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_sabrent_rocket_4_plus_g = {
    .name = "sabrent_rocket_4_plus_g",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SabrentRocket4PlusGState),
        VMSTATE_END_OF_LIST()
    }
};

static void sabrent_rocket_4_plus_g_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_sabrent_rocket_4_plus_g_realize;
    k->vendor_id = 0x1b1c;
    k->device_id = 0x5017;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Sabrent Rocket 4 Plus-G";
    dc->vmsd = &vmstate_sabrent_rocket_4_plus_g;
}

static const TypeInfo sabrent_rocket_4_plus_g_info = {
    .name = TYPE_SABRENT_ROCKET_4_PLUS_G,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SabrentRocket4PlusGState),
    .class_init = sabrent_rocket_4_plus_g_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void sabrent_rocket_4_plus_g_register(void)
{
    type_register_static(&sabrent_rocket_4_plus_g_info);
}

type_init(sabrent_rocket_4_plus_g_register)
