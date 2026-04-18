/*
 * Broadcom BCM57504 100GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_BROADCOM_BCM57504 "pci-broadcom_bcm57504"

OBJECT_DECLARE_SIMPLE_TYPE(BroadcomBcm57504State, BROADCOM_BCM57504)

typedef struct BroadcomBcm57504State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} BroadcomBcm57504State;

static uint64_t broadcom_bcm57504_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void broadcom_bcm57504_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps broadcom_bcm57504_mmio_ops = {
    .read = broadcom_bcm57504_mmio_read,
    .write = broadcom_bcm57504_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_broadcom_bcm57504_realize(PCIDevice *pdev, Error **errp)
{
    BroadcomBcm57504State *s = BROADCOM_BCM57504(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &broadcom_bcm57504_mmio_ops, s,
                          "broadcom_bcm57504-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_broadcom_bcm57504 = {
    .name = "broadcom_bcm57504",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, BroadcomBcm57504State),
        VMSTATE_END_OF_LIST()
    }
};

static void broadcom_bcm57504_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_broadcom_bcm57504_realize;
    k->vendor_id = 0x14e4;
    k->device_id = 0x1801;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Broadcom BCM57504 100GbE";
    dc->vmsd = &vmstate_broadcom_bcm57504;
}

static const TypeInfo broadcom_bcm57504_info = {
    .name = TYPE_BROADCOM_BCM57504,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(BroadcomBcm57504State),
    .class_init = broadcom_bcm57504_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void broadcom_bcm57504_register(void)
{
    type_register_static(&broadcom_bcm57504_info);
}

type_init(broadcom_bcm57504_register)
