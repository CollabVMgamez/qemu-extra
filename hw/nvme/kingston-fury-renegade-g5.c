/*
 * Kingston Fury Renegade G5 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_KINGSTON_FURY_RENEGADE_G5 "pci-kingston_fury_renegade_g5"

OBJECT_DECLARE_SIMPLE_TYPE(KingstonFuryRenegadeG5State, KINGSTON_FURY_RENEGADE_G5)

typedef struct KingstonFuryRenegadeG5State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} KingstonFuryRenegadeG5State;

static uint64_t kingston_fury_renegade_g5_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void kingston_fury_renegade_g5_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps kingston_fury_renegade_g5_mmio_ops = {
    .read = kingston_fury_renegade_g5_mmio_read,
    .write = kingston_fury_renegade_g5_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_kingston_fury_renegade_g5_realize(PCIDevice *pdev, Error **errp)
{
    KingstonFuryRenegadeG5State *s = KINGSTON_FURY_RENEGADE_G5(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &kingston_fury_renegade_g5_mmio_ops, s,
                          "kingston_fury_renegade_g5-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_kingston_fury_renegade_g5 = {
    .name = "kingston_fury_renegade_g5",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, KingstonFuryRenegadeG5State),
        VMSTATE_END_OF_LIST()
    }
};

static void kingston_fury_renegade_g5_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_kingston_fury_renegade_g5_realize;
    k->vendor_id = 0x2646;
    k->device_id = 0x501e;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Kingston Fury Renegade G5";
    dc->vmsd = &vmstate_kingston_fury_renegade_g5;
}

static const TypeInfo kingston_fury_renegade_g5_info = {
    .name = TYPE_KINGSTON_FURY_RENEGADE_G5,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(KingstonFuryRenegadeG5State),
    .class_init = kingston_fury_renegade_g5_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void kingston_fury_renegade_g5_register(void)
{
    type_register_static(&kingston_fury_renegade_g5_info);
}

type_init(kingston_fury_renegade_g5_register)
