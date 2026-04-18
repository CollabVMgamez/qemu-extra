/*
 * SK Hynix Gold P31 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SKHYNIX_GOLD_P31 "pci-skhynix_gold_p31"

OBJECT_DECLARE_SIMPLE_TYPE(SkhynixGoldP31State, SKHYNIX_GOLD_P31)

typedef struct SkhynixGoldP31State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} SkhynixGoldP31State;

static uint64_t skhynix_gold_p31_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void skhynix_gold_p31_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps skhynix_gold_p31_mmio_ops = {
    .read = skhynix_gold_p31_mmio_read,
    .write = skhynix_gold_p31_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_skhynix_gold_p31_realize(PCIDevice *pdev, Error **errp)
{
    SkhynixGoldP31State *s = SKHYNIX_GOLD_P31(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &skhynix_gold_p31_mmio_ops, s,
                          "skhynix_gold_p31-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_skhynix_gold_p31 = {
    .name = "skhynix_gold_p31",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SkhynixGoldP31State),
        VMSTATE_END_OF_LIST()
    }
};

static void skhynix_gold_p31_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_skhynix_gold_p31_realize;
    k->vendor_id = 0x1c5c;
    k->device_id = 0x174b;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "SK Hynix Gold P31";
    dc->vmsd = &vmstate_skhynix_gold_p31;
}

static const TypeInfo skhynix_gold_p31_info = {
    .name = TYPE_SKHYNIX_GOLD_P31,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SkhynixGoldP31State),
    .class_init = skhynix_gold_p31_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void skhynix_gold_p31_register(void)
{
    type_register_static(&skhynix_gold_p31_info);
}

type_init(skhynix_gold_p31_register)
