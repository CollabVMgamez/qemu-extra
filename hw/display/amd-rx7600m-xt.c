/*
 * AMD Radeon RX 7600M XT PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_AMD_RX7600M_XT "pci-amd_rx7600m_xt"

OBJECT_DECLARE_SIMPLE_TYPE(AmdRx7600mXtState, AMD_RX7600M_XT)

typedef struct AmdRx7600mXtState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} AmdRx7600mXtState;

static uint64_t amd_rx7600m_xt_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void amd_rx7600m_xt_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps amd_rx7600m_xt_mmio_ops = {
    .read = amd_rx7600m_xt_mmio_read,
    .write = amd_rx7600m_xt_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_amd_rx7600m_xt_realize(PCIDevice *pdev, Error **errp)
{
    AmdRx7600mXtState *s = AMD_RX7600M_XT(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &amd_rx7600m_xt_mmio_ops, s,
                          "amd_rx7600m_xt-mmio", 0x10000000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}


static const VMStateDescription vmstate_amd_rx7600m_xt = {
    .name = "amd_rx7600m_xt",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, AmdRx7600mXtState),
        VMSTATE_END_OF_LIST()
    }
};

static void amd_rx7600m_xt_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_amd_rx7600m_xt_realize;
    k->vendor_id = 0x1002;
    k->device_id = 0x7480;
    k->class_id = PCI_CLASS_DISPLAY_VGA;
    dc->desc = "AMD Radeon RX 7600M XT";
    dc->vmsd = &vmstate_amd_rx7600m_xt;
}

static const TypeInfo amd_rx7600m_xt_info = {
    .name = TYPE_AMD_RX7600M_XT,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(AmdRx7600mXtState),
    .class_init = amd_rx7600m_xt_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void amd_rx7600m_xt_register(void)
{
    type_register_static(&amd_rx7600m_xt_info);
}

type_init(amd_rx7600m_xt_register)
