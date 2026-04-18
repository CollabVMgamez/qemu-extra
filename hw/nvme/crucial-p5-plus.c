/*
 * Crucial P5 Plus NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_CRUCIAL_P5_PLUS "pci-crucial_p5_plus"

OBJECT_DECLARE_SIMPLE_TYPE(CrucialP5PlusState, CRUCIAL_P5_PLUS)

typedef struct CrucialP5PlusState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} CrucialP5PlusState;

static uint64_t crucial_p5_plus_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void crucial_p5_plus_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps crucial_p5_plus_mmio_ops = {
    .read = crucial_p5_plus_mmio_read,
    .write = crucial_p5_plus_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_crucial_p5_plus_realize(PCIDevice *pdev, Error **errp)
{
    CrucialP5PlusState *s = CRUCIAL_P5_PLUS(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &crucial_p5_plus_mmio_ops, s,
                          "crucial_p5_plus-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_crucial_p5_plus = {
    .name = "crucial_p5_plus",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, CrucialP5PlusState),
        VMSTATE_END_OF_LIST()
    }
};

static void crucial_p5_plus_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_crucial_p5_plus_realize;
    k->vendor_id = 0x1344;
    k->device_id = 0x5404;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Crucial P5 Plus";
    dc->vmsd = &vmstate_crucial_p5_plus;
}

static const TypeInfo crucial_p5_plus_info = {
    .name = TYPE_CRUCIAL_P5_PLUS,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(CrucialP5PlusState),
    .class_init = crucial_p5_plus_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void crucial_p5_plus_register(void)
{
    type_register_static(&crucial_p5_plus_info);
}

type_init(crucial_p5_plus_register)
