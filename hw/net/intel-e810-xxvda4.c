/*
 * Intel E810-XXVDA4 4x25GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_E810_XXVDA4 "pci-intel_e810_xxvda4"

OBJECT_DECLARE_SIMPLE_TYPE(IntelE810Xxvda4State, INTEL_E810_XXVDA4)

typedef struct IntelE810Xxvda4State {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} IntelE810Xxvda4State;

static uint64_t intel_e810_xxvda4_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void intel_e810_xxvda4_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps intel_e810_xxvda4_mmio_ops = {
    .read = intel_e810_xxvda4_mmio_read,
    .write = intel_e810_xxvda4_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_intel_e810_xxvda4_realize(PCIDevice *pdev, Error **errp)
{
    IntelE810Xxvda4State *s = INTEL_E810_XXVDA4(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_e810_xxvda4_mmio_ops, s,
                          "intel_e810_xxvda4-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_intel_e810_xxvda4 = {
    .name = "intel_e810_xxvda4",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelE810Xxvda4State),
        VMSTATE_END_OF_LIST()
    }
};

static void intel_e810_xxvda4_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_intel_e810_xxvda4_realize;
    k->vendor_id = 0x8086;
    k->device_id = 0x159b;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Intel E810-XXVDA4 4x25GbE";
    dc->vmsd = &vmstate_intel_e810_xxvda4;
}

static const TypeInfo intel_e810_xxvda4_info = {
    .name = TYPE_INTEL_E810_XXVDA4,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelE810Xxvda4State),
    .class_init = intel_e810_xxvda4_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void intel_e810_xxvda4_register(void)
{
    type_register_static(&intel_e810_xxvda4_info);
}

type_init(intel_e810_xxvda4_register)
