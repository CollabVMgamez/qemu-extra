/*
 * WD Black SN770 NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_WD_BLACK_SN770 "pci-wd_black_sn770"

OBJECT_DECLARE_SIMPLE_TYPE(WdBlackSn770State, WD_BLACK_SN770)

typedef struct WdBlackSn770State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} WdBlackSn770State;

static uint64_t wd_black_sn770_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void wd_black_sn770_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps wd_black_sn770_mmio_ops = {
    .read = wd_black_sn770_mmio_read,
    .write = wd_black_sn770_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_wd_black_sn770_realize(PCIDevice *pdev, Error **errp)
{
    WdBlackSn770State *s = WD_BLACK_SN770(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &wd_black_sn770_mmio_ops, s,
                          "wd_black_sn770-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_wd_black_sn770 = {
    .name = "wd_black_sn770",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, WdBlackSn770State),
        VMSTATE_END_OF_LIST()
    }
};

static void wd_black_sn770_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_wd_black_sn770_realize;
    k->vendor_id = 0x1b1c;
    k->device_id = 0x5014;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "WD Black SN770";
    dc->vmsd = &vmstate_wd_black_sn770;
}

static const TypeInfo wd_black_sn770_info = {
    .name = TYPE_WD_BLACK_SN770,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(WdBlackSn770State),
    .class_init = wd_black_sn770_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void wd_black_sn770_register(void)
{
    type_register_static(&wd_black_sn770_info);
}

type_init(wd_black_sn770_register)
