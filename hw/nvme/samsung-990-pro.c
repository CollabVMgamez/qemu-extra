/*
 * Samsung 990 PRO NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_SAMSUNG_990_PRO "pci-samsung_990_pro"

OBJECT_DECLARE_SIMPLE_TYPE(Samsung990ProState, SAMSUNG_990_PRO)

typedef struct Samsung990ProState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} Samsung990ProState;

static uint64_t samsung_990_pro_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void samsung_990_pro_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps samsung_990_pro_mmio_ops = {
    .read = samsung_990_pro_mmio_read,
    .write = samsung_990_pro_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_samsung_990_pro_realize(PCIDevice *pdev, Error **errp)
{
    Samsung990ProState *s = SAMSUNG_990_PRO(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &samsung_990_pro_mmio_ops, s,
                          "samsung_990_pro-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_samsung_990_pro = {
    .name = "samsung_990_pro",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, Samsung990ProState),
        VMSTATE_END_OF_LIST()
    }
};

static void samsung_990_pro_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_samsung_990_pro_realize;
    k->vendor_id = 0x144d;
    k->device_id = 0xa80a;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "Samsung 990 PRO";
    dc->vmsd = &vmstate_samsung_990_pro;
}

static const TypeInfo samsung_990_pro_info = {
    .name = TYPE_SAMSUNG_990_PRO,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(Samsung990ProState),
    .class_init = samsung_990_pro_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void samsung_990_pro_register(void)
{
    type_register_static(&samsung_990_pro_info);
}

type_init(samsung_990_pro_register)
