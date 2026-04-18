/*
 * Broadcom BCM57502 50GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_BROADCOM_BCM57502 "pci-broadcom_bcm57502"

OBJECT_DECLARE_SIMPLE_TYPE(BroadcomBcm57502State, BROADCOM_BCM57502)

typedef struct BroadcomBcm57502State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} BroadcomBcm57502State;

static uint64_t broadcom_bcm57502_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void broadcom_bcm57502_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps broadcom_bcm57502_mmio_ops = {
    .read = broadcom_bcm57502_mmio_read,
    .write = broadcom_bcm57502_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_broadcom_bcm57502_realize(PCIDevice *pdev, Error **errp)
{
    BroadcomBcm57502State *s = BROADCOM_BCM57502(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &broadcom_bcm57502_mmio_ops, s,
                          "broadcom_bcm57502-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_broadcom_bcm57502 = {
    .name = "broadcom_bcm57502",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, BroadcomBcm57502State),
        VMSTATE_END_OF_LIST()
    }
};

static void broadcom_bcm57502_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_broadcom_bcm57502_realize;
    k->vendor_id = 0x14e4;
    k->device_id = 0x1802;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Broadcom BCM57502 50GbE";
    dc->vmsd = &vmstate_broadcom_bcm57502;
}

static const TypeInfo broadcom_bcm57502_info = {
    .name = TYPE_BROADCOM_BCM57502,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(BroadcomBcm57502State),
    .class_init = broadcom_bcm57502_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void broadcom_bcm57502_register(void)
{
    type_register_static(&broadcom_bcm57502_info);
}

type_init(broadcom_bcm57502_register)
