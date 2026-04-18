/*
 * WD Black SN770M NVMe PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_WD_BLACK_SN770M "pci-wd_black_sn770m"

OBJECT_DECLARE_SIMPLE_TYPE(WdBlackSn770mState, WD_BLACK_SN770M)

typedef struct WdBlackSn770mState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} WdBlackSn770mState;

static uint64_t wd_black_sn770m_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void wd_black_sn770m_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps wd_black_sn770m_mmio_ops = {
    .read = wd_black_sn770m_mmio_read,
    .write = wd_black_sn770m_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_wd_black_sn770m_realize(PCIDevice *pdev, Error **errp)
{
    WdBlackSn770mState *s = WD_BLACK_SN770M(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &wd_black_sn770m_mmio_ops, s,
                          "wd_black_sn770m-mmio", 0x4000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY | PCI_BASE_ADDRESS_MEM_TYPE_64, &s->mmio);
}

static const VMStateDescription vmstate_wd_black_sn770m = {
    .name = "wd_black_sn770m",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, WdBlackSn770mState),
        VMSTATE_END_OF_LIST()
    }
};

static void wd_black_sn770m_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_wd_black_sn770m_realize;
    k->vendor_id = 0x1b1c;
    k->device_id = 0x5015;
    k->class_id = PCI_CLASS_STORAGE_EXPRESS;
    k->revision = 0x01;
    dc->desc = "WD Black SN770M";
    dc->vmsd = &vmstate_wd_black_sn770m;
}

static const TypeInfo wd_black_sn770m_info = {
    .name = TYPE_WD_BLACK_SN770M,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(WdBlackSn770mState),
    .class_init = wd_black_sn770m_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void wd_black_sn770m_register(void)
{
    type_register_static(&wd_black_sn770m_info);
}

type_init(wd_black_sn770m_register)
