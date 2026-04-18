/*
 * Crucial T700 PCIe 5.0 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CRUCIAL_T700 "pci-crucial_t700"

OBJECT_DECLARE_SIMPLE_TYPE(CrucialT700State, CRUCIAL_T700)

typedef struct CrucialT700State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} CrucialT700State;

static uint64_t crucial_t700_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void crucial_t700_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps crucial_t700_mmio_ops = {
    .read = crucial_t700_mmio_read,
    .write = crucial_t700_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_crucial_t700_realize(PCIDevice *pdev, Error **errp)
{
    CrucialT700State *s = CRUCIAL_T700(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &crucial_t700_mmio_ops, s,
                          "crucial_t700-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_crucial_t700 = {
    .name = "crucial_t700",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CrucialT700State),
        VMSTATE_END_OF_LIST()
    }
};

static void crucial_t700_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_crucial_t700_realize;
    k->vendor_id = 0x1344;
    k->device_id = 0x5406;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Crucial T700 PCIe 5.0";
    dc->vmsd = &vmstate_crucial_t700;
}

static const TypeInfo crucial_t700_info = {
    .name = TYPE_CRUCIAL_T700,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CrucialT700State),
    .class_init = crucial_t700_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void crucial_t700_register(void)
{
    type_register_static(&crucial_t700_info);
}

type_init(crucial_t700_register)
