/*
 * SK Hynix Platinum P41 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SKHYNIX_PLATINUM_P41 "pci-skhynix_platinum_p41"

OBJECT_DECLARE_SIMPLE_TYPE(SkhynixPlatinumP41State, SKHYNIX_PLATINUM_P41)

typedef struct SkhynixPlatinumP41State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} SkhynixPlatinumP41State;

static uint64_t skhynix_platinum_p41_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void skhynix_platinum_p41_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps skhynix_platinum_p41_mmio_ops = {
    .read = skhynix_platinum_p41_mmio_read,
    .write = skhynix_platinum_p41_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_skhynix_platinum_p41_realize(PCIDevice *pdev, Error **errp)
{
    SkhynixPlatinumP41State *s = SKHYNIX_PLATINUM_P41(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &skhynix_platinum_p41_mmio_ops, s,
                          "skhynix_platinum_p41-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_skhynix_platinum_p41 = {
    .name = "skhynix_platinum_p41",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, SkhynixPlatinumP41State),
        VMSTATE_END_OF_LIST()
    }
};

static void skhynix_platinum_p41_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_skhynix_platinum_p41_realize;
    k->vendor_id = 0x1c5c;
    k->device_id = 0x174a;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "SK Hynix Platinum P41";
    dc->vmsd = &vmstate_skhynix_platinum_p41;
}

static const TypeInfo skhynix_platinum_p41_info = {
    .name = TYPE_SKHYNIX_PLATINUM_P41,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(SkhynixPlatinumP41State),
    .class_init = skhynix_platinum_p41_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void skhynix_platinum_p41_register(void)
{
    type_register_static(&skhynix_platinum_p41_info);
}

type_init(skhynix_platinum_p41_register)
