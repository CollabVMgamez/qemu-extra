/*
 * Seagate FireCuda 540 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SEAGATE_FIRECUDA_540 "pci-seagate_firecuda_540"

OBJECT_DECLARE_SIMPLE_TYPE(SeagateFirecuda540State, SEAGATE_FIRECUDA_540)

typedef struct SeagateFirecuda540State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} SeagateFirecuda540State;

static uint64_t seagate_firecuda_540_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void seagate_firecuda_540_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps seagate_firecuda_540_mmio_ops = {
    .read = seagate_firecuda_540_mmio_read,
    .write = seagate_firecuda_540_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_seagate_firecuda_540_realize(PCIDevice *pdev, Error **errp)
{
    SeagateFirecuda540State *s = SEAGATE_FIRECUDA_540(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &seagate_firecuda_540_mmio_ops, s,
                          "seagate_firecuda_540-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_seagate_firecuda_540 = {
    .name = "seagate_firecuda_540",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SeagateFirecuda540State),
        VMSTATE_END_OF_LIST()
    }
};

static void seagate_firecuda_540_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_seagate_firecuda_540_realize;
    k->vendor_id = 0x1b1c;
    k->device_id = 0x5018;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Seagate FireCuda 540";
    dc->vmsd = &vmstate_seagate_firecuda_540;
}

static const TypeInfo seagate_firecuda_540_info = {
    .name = TYPE_SEAGATE_FIRECUDA_540,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SeagateFirecuda540State),
    .class_init = seagate_firecuda_540_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void seagate_firecuda_540_register(void)
{
    type_register_static(&seagate_firecuda_540_info);
}

type_init(seagate_firecuda_540_register)
