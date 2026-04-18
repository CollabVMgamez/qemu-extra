/*
 * Corsair MP700 PCIe 5.0 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CORSAIR_MP700 "pci-corsair_mp700"

OBJECT_DECLARE_SIMPLE_TYPE(CorsairMp700State, CORSAIR_MP700)

typedef struct CorsairMp700State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} CorsairMp700State;

static uint64_t corsair_mp700_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void corsair_mp700_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps corsair_mp700_mmio_ops = {
    .read = corsair_mp700_mmio_read,
    .write = corsair_mp700_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_corsair_mp700_realize(PCIDevice *pdev, Error **errp)
{
    CorsairMp700State *s = CORSAIR_MP700(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &corsair_mp700_mmio_ops, s,
                          "corsair_mp700-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_corsair_mp700 = {
    .name = "corsair_mp700",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CorsairMp700State),
        VMSTATE_END_OF_LIST()
    }
};

static void corsair_mp700_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_corsair_mp700_realize;
    k->vendor_id = 0x1b1c;
    k->device_id = 0x5019;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Corsair MP700 PCIe 5.0";
    dc->vmsd = &vmstate_corsair_mp700;
}

static const TypeInfo corsair_mp700_info = {
    .name = TYPE_CORSAIR_MP700,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CorsairMp700State),
    .class_init = corsair_mp700_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void corsair_mp700_register(void)
{
    type_register_static(&corsair_mp700_info);
}

type_init(corsair_mp700_register)
