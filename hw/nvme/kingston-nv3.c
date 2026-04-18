/*
 * Kingston NV3 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_KINGSTON_NV3 "pci-kingston_nv3"

OBJECT_DECLARE_SIMPLE_TYPE(KingstonNv3State, KINGSTON_NV3)

typedef struct KingstonNv3State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} KingstonNv3State;

static uint64_t kingston_nv3_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void kingston_nv3_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps kingston_nv3_mmio_ops = {
    .read = kingston_nv3_mmio_read,
    .write = kingston_nv3_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_kingston_nv3_realize(PCIDevice *pdev, Error **errp)
{
    KingstonNv3State *s = KINGSTON_NV3(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &kingston_nv3_mmio_ops, s,
                          "kingston_nv3-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_kingston_nv3 = {
    .name = "kingston_nv3",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, KingstonNv3State),
        VMSTATE_END_OF_LIST()
    }
};

static void kingston_nv3_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_kingston_nv3_realize;
    k->vendor_id = 0x2646;
    k->device_id = 0x501b;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Kingston NV3";
    dc->vmsd = &vmstate_kingston_nv3;
}

static const TypeInfo kingston_nv3_info = {
    .name = TYPE_KINGSTON_NV3,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(KingstonNv3State),
    .class_init = kingston_nv3_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void kingston_nv3_register(void)
{
    type_register_static(&kingston_nv3_info);
}

type_init(kingston_nv3_register)
