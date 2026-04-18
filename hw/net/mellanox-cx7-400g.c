/*
 * Mellanox ConnectX-7 400GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_MELLANOX_CX7_400G "pci-mellanox_cx7_400g"

OBJECT_DECLARE_SIMPLE_TYPE(MellanoxCx7400gState, MELLANOX_CX7_400G)

typedef struct MellanoxCx7400gState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} MellanoxCx7400gState;

static uint64_t mellanox_cx7_400g_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void mellanox_cx7_400g_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps mellanox_cx7_400g_mmio_ops = {
    .read = mellanox_cx7_400g_mmio_read,
    .write = mellanox_cx7_400g_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_mellanox_cx7_400g_realize(PCIDevice *pdev, Error **errp)
{
    MellanoxCx7400gState *s = MELLANOX_CX7_400G(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &mellanox_cx7_400g_mmio_ops, s,
                          "mellanox_cx7_400g-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_mellanox_cx7_400g = {
    .name = "mellanox_cx7_400g",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, MellanoxCx7400gState),
        VMSTATE_END_OF_LIST()
    }
};

static void mellanox_cx7_400g_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_mellanox_cx7_400g_realize;
    k->vendor_id = 0x15b3;
    k->device_id = 0x1022;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Mellanox ConnectX-7 400GbE";
    dc->vmsd = &vmstate_mellanox_cx7_400g;
}

static const TypeInfo mellanox_cx7_400g_info = {
    .name = TYPE_MELLANOX_CX7_400G,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(MellanoxCx7400gState),
    .class_init = mellanox_cx7_400g_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void mellanox_cx7_400g_register(void)
{
    type_register_static(&mellanox_cx7_400g_info);
}

type_init(mellanox_cx7_400g_register)
