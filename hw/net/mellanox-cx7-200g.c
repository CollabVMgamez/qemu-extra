/*
 * Mellanox ConnectX-7 200GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_MELLANOX_CX7_200G "pci-mellanox_cx7_200g"

OBJECT_DECLARE_SIMPLE_TYPE(MellanoxCx7200gState, MELLANOX_CX7_200G)

typedef struct MellanoxCx7200gState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} MellanoxCx7200gState;

static uint64_t mellanox_cx7_200g_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void mellanox_cx7_200g_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps mellanox_cx7_200g_mmio_ops = {
    .read = mellanox_cx7_200g_mmio_read,
    .write = mellanox_cx7_200g_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_mellanox_cx7_200g_realize(PCIDevice *pdev, Error **errp)
{
    MellanoxCx7200gState *s = MELLANOX_CX7_200G(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &mellanox_cx7_200g_mmio_ops, s,
                          "mellanox_cx7_200g-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_mellanox_cx7_200g = {
    .name = "mellanox_cx7_200g",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, MellanoxCx7200gState),
        VMSTATE_END_OF_LIST()
    }
};

static void mellanox_cx7_200g_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_mellanox_cx7_200g_realize;
    k->vendor_id = 0x15b3;
    k->device_id = 0x1021;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Mellanox ConnectX-7 200GbE";
    dc->vmsd = &vmstate_mellanox_cx7_200g;
}

static const TypeInfo mellanox_cx7_200g_info = {
    .name = TYPE_MELLANOX_CX7_200G,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(MellanoxCx7200gState),
    .class_init = mellanox_cx7_200g_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void mellanox_cx7_200g_register(void)
{
    type_register_static(&mellanox_cx7_200g_info);
}

type_init(mellanox_cx7_200g_register)
