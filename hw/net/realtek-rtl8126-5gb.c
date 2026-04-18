/*
 * Realtek RTL8126 5GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_REALTEK_RTL8126_5GB "pci-realtek_rtl8126_5gb"

OBJECT_DECLARE_SIMPLE_TYPE(RealtekRtl81265gbState, REALTEK_RTL8126_5GB)

typedef struct RealtekRtl81265gbState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} RealtekRtl81265gbState;

static uint64_t realtek_rtl8126_5gb_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void realtek_rtl8126_5gb_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps realtek_rtl8126_5gb_mmio_ops = {
    .read = realtek_rtl8126_5gb_mmio_read,
    .write = realtek_rtl8126_5gb_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_realtek_rtl8126_5gb_realize(PCIDevice *pdev, Error **errp)
{
    RealtekRtl81265gbState *s = REALTEK_RTL8126_5GB(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &realtek_rtl8126_5gb_mmio_ops, s,
                          "realtek_rtl8126_5gb-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_realtek_rtl8126_5gb = {
    .name = "realtek_rtl8126_5gb",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, RealtekRtl81265gbState),
        VMSTATE_END_OF_LIST()
    }
};

static void realtek_rtl8126_5gb_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_realtek_rtl8126_5gb_realize;
    k->vendor_id = 0x10ec;
    k->device_id = 0x8126;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Realtek RTL8126 5GbE";
    dc->vmsd = &vmstate_realtek_rtl8126_5gb;
}

static const TypeInfo realtek_rtl8126_5gb_info = {
    .name = TYPE_REALTEK_RTL8126_5GB,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RealtekRtl81265gbState),
    .class_init = realtek_rtl8126_5gb_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void realtek_rtl8126_5gb_register(void)
{
    type_register_static(&realtek_rtl8126_5gb_info);
}

type_init(realtek_rtl8126_5gb_register)
