/*
 * Intel I226-LM 2.5GbE PCI stub
 * Auto-generated for QEMU-Extra
 */

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"
#include "qemu/module.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_INTEL_I226_LM "pci-intel_i226_lm"

OBJECT_DECLARE_SIMPLE_TYPE(IntelI226LmState, INTEL_I226_LM)

typedef struct IntelI226LmState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
} IntelI226LmState;

static uint64_t intel_i226_lm_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void intel_i226_lm_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps intel_i226_lm_mmio_ops = {
    .read = intel_i226_lm_mmio_read,
    .write = intel_i226_lm_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_intel_i226_lm_realize(PCIDevice *pdev, Error **errp)
{
    IntelI226LmState *s = INTEL_I226_LM(pdev);
    memory_region_init_io(&s->mmio, OBJECT(s), &intel_i226_lm_mmio_ops, s,
                          "intel_i226_lm-mmio", 0x10000);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const VMStateDescription vmstate_intel_i226_lm = {
    .name = "intel_i226_lm",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, IntelI226LmState),
        VMSTATE_END_OF_LIST()
    }
};

static void intel_i226_lm_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize = pci_intel_i226_lm_realize;
    k->vendor_id = 0x8086;
    k->device_id = 0x125d;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->desc = "Intel I226-LM 2.5GbE";
    dc->vmsd = &vmstate_intel_i226_lm;
}

static const TypeInfo intel_i226_lm_info = {
    .name = TYPE_INTEL_I226_LM,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(IntelI226LmState),
    .class_init = intel_i226_lm_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { }
    },
};

static void intel_i226_lm_register(void)
{
    type_register_static(&intel_i226_lm_info);
}

type_init(intel_i226_lm_register)
