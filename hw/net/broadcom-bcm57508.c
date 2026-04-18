/*
 * Broadcom BCM57508 200GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_BROADCOM_BCM57508 "pci-broadcom_bcm57508"

OBJECT_DECLARE_SIMPLE_TYPE(BroadcomBcm57508State, BROADCOM_BCM57508)

typedef struct BroadcomBcm57508State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} BroadcomBcm57508State;

static uint64_t broadcom_bcm57508_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void broadcom_bcm57508_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps broadcom_bcm57508_mmio_ops = {
    .read = broadcom_bcm57508_mmio_read,
    .write = broadcom_bcm57508_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_broadcom_bcm57508_realize(PCIDevice *pdev, Error **errp)
{
    BroadcomBcm57508State *s = BROADCOM_BCM57508(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &broadcom_bcm57508_mmio_ops, s,
                          "broadcom_bcm57508-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_broadcom_bcm57508 = {
    .name = "broadcom_bcm57508",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, BroadcomBcm57508State),
        VMSTATE_END_OF_LIST()
    }
};

static void broadcom_bcm57508_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_broadcom_bcm57508_realize;
    k->vendor_id = 0x14e4;
    k->device_id = 0x1800;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Broadcom BCM57508 200GbE";
    dc->vmsd = &vmstate_broadcom_bcm57508;
}

static const TypeInfo broadcom_bcm57508_info = {
    .name = TYPE_BROADCOM_BCM57508,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(BroadcomBcm57508State),
    .class_init = broadcom_bcm57508_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void broadcom_bcm57508_register(void)
{
    type_register_static(&broadcom_bcm57508_info);
}

type_init(broadcom_bcm57508_register)
