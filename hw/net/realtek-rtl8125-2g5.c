/*
 * Realtek RTL8125 2.5GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_REALTEK_RTL8125_2G5 "pci-realtek_rtl8125_2g5"

OBJECT_DECLARE_SIMPLE_TYPE(RealtekRtl81252g5State, REALTEK_RTL8125_2G5)

typedef struct RealtekRtl81252g5State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} RealtekRtl81252g5State;

static uint64_t realtek_rtl8125_2g5_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void realtek_rtl8125_2g5_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps realtek_rtl8125_2g5_mmio_ops = {
    .read = realtek_rtl8125_2g5_mmio_read,
    .write = realtek_rtl8125_2g5_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_realtek_rtl8125_2g5_realize(PCIDevice *pdev, Error **errp)
{
    RealtekRtl81252g5State *s = REALTEK_RTL8125_2G5(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &realtek_rtl8125_2g5_mmio_ops, s,
                          "realtek_rtl8125_2g5-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_realtek_rtl8125_2g5 = {
    .name = "realtek_rtl8125_2g5",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, RealtekRtl81252g5State),
        VMSTATE_END_OF_LIST()
    }
};

static void realtek_rtl8125_2g5_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_realtek_rtl8125_2g5_realize;
    k->vendor_id = 0x10ec;
    k->device_id = 0x8125;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Realtek RTL8125 2.5GbE";
    dc->vmsd = &vmstate_realtek_rtl8125_2g5;
}

static const TypeInfo realtek_rtl8125_2g5_info = {
    .name = TYPE_REALTEK_RTL8125_2G5,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RealtekRtl81252g5State),
    .class_init = realtek_rtl8125_2g5_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void realtek_rtl8125_2g5_register(void)
{
    type_register_static(&realtek_rtl8125_2g5_info);
}

type_init(realtek_rtl8125_2g5_register)
