/*
 * Marvell QLE2770 32Gb FC PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_MARVELL_QLE2770 "pci-marvell_qle2770"

OBJECT_DECLARE_SIMPLE_TYPE(MarvellQle2770State, MARVELL_QLE2770)

typedef struct MarvellQle2770State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} MarvellQle2770State;

static uint64_t marvell_qle2770_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void marvell_qle2770_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps marvell_qle2770_mmio_ops = {
    .read = marvell_qle2770_mmio_read,
    .write = marvell_qle2770_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_marvell_qle2770_realize(PCIDevice *pdev, Error **errp)
{
    MarvellQle2770State *s = MARVELL_QLE2770(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &marvell_qle2770_mmio_ops, s,
                          "marvell_qle2770-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_marvell_qle2770 = {
    .name = "marvell_qle2770",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, MarvellQle2770State),
        VMSTATE_END_OF_LIST()
    }
};

static void marvell_qle2770_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_marvell_qle2770_realize;
    k->vendor_id = 0x1077;
    k->device_id = 0x2261;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Marvell QLE2770 32Gb FC";
    dc->vmsd = &vmstate_marvell_qle2770;
}

static const TypeInfo marvell_qle2770_info = {
    .name = TYPE_MARVELL_QLE2770,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(MarvellQle2770State),
    .class_init = marvell_qle2770_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void marvell_qle2770_register(void)
{
    type_register_static(&marvell_qle2770_info);
}

type_init(marvell_qle2770_register)
