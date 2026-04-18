/*
 * Intel I226-IT 2.5GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_I226_IT "pci-intel_i226_it"

OBJECT_DECLARE_SIMPLE_TYPE(IntelI226ItState, INTEL_I226_IT)

typedef struct IntelI226ItState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} IntelI226ItState;

static uint64_t intel_i226_it_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void intel_i226_it_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps intel_i226_it_mmio_ops = {
    .read = intel_i226_it_mmio_read,
    .write = intel_i226_it_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_intel_i226_it_realize(PCIDevice *pdev, Error **errp)
{
    IntelI226ItState *s = INTEL_I226_IT(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_i226_it_mmio_ops, s,
                          "intel_i226_it-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_intel_i226_it = {
    .name = "intel_i226_it",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelI226ItState),
        VMSTATE_END_OF_LIST()
    }
};

static void intel_i226_it_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_intel_i226_it_realize;
    k->vendor_id = 0x8086;
    k->device_id = 0x125e;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Intel I226-IT 2.5GbE";
    dc->vmsd = &vmstate_intel_i226_it;
}

static const TypeInfo intel_i226_it_info = {
    .name = TYPE_INTEL_I226_IT,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelI226ItState),
    .class_init = intel_i226_it_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void intel_i226_it_register(void)
{
    type_register_static(&intel_i226_it_info);
}

type_init(intel_i226_it_register)
