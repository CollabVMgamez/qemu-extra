/*
 * WD Blue SN580 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_WD_BLUE_SN580 "pci-wd_blue_sn580"

OBJECT_DECLARE_SIMPLE_TYPE(WdBlueSn580State, WD_BLUE_SN580)

typedef struct WdBlueSn580State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} WdBlueSn580State;

static uint64_t wd_blue_sn580_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void wd_blue_sn580_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps wd_blue_sn580_mmio_ops = {
    .read = wd_blue_sn580_mmio_read,
    .write = wd_blue_sn580_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_wd_blue_sn580_realize(PCIDevice *pdev, Error **errp)
{
    WdBlueSn580State *s = WD_BLUE_SN580(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &wd_blue_sn580_mmio_ops, s,
                          "wd_blue_sn580-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_wd_blue_sn580 = {
    .name = "wd_blue_sn580",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, WdBlueSn580State),
        VMSTATE_END_OF_LIST()
    }
};

static void wd_blue_sn580_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_wd_blue_sn580_realize;
    k->vendor_id = 0x1b1c;
    k->device_id = 0x5016;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "WD Blue SN580";
    dc->vmsd = &vmstate_wd_blue_sn580;
}

static const TypeInfo wd_blue_sn580_info = {
    .name = TYPE_WD_BLUE_SN580,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(WdBlueSn580State),
    .class_init = wd_blue_sn580_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void wd_blue_sn580_register(void)
{
    type_register_static(&wd_blue_sn580_info);
}

type_init(wd_blue_sn580_register)
