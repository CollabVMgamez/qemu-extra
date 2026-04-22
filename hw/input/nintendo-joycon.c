#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/pci/pci_device.h"
#include "hw/pci/pci_ids.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_NINTENDO_JOYCON "nintendo-joycon"
OBJECT_DECLARE_SIMPLE_TYPE(JoyConState, NINTENDO_JOYCON)

struct JoyConState {
    PCIDevice parent_obj;
    MemoryRegion mmio;
    char *side;
};

static uint64_t joycon_read(void *opaque, hwaddr addr, unsigned size)
{
    JoyConState *s = opaque;
    switch (addr & ~3ULL) {
    case 0x00: return 0x20070001;
    case 0x04: return 0x00000001;
    case 0x08: return s->side && !g_ascii_strcasecmp(s->side, "right") ? 0x02 : 0x01;
    case 0x0C: return 0x00000480;
    case 0x10: return 0x00000020;
    case 0x14: return 0x00000008;
    case 0x18: return 0x00001337;
    case 0x30: return 0x0145C718;
    case 0x34: return 0x00000000;
    default:   return 0;
    }
}

static void joycon_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps joycon_ops = {
    .read = joycon_read, .write = joycon_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void joycon_realize(PCIDevice *pdev, Error **errp)
{
    JoyConState *s = NINTENDO_JOYCON(pdev);
    pdev->config[PCI_CLASS_PROG] = 0x00;

    memory_region_init_io(&s->mmio, OBJECT(s), &joycon_ops, s,
                          "joycon-mmio", 4096);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static const Property joycon_props[] = {
    DEFINE_PROP_STRING("side", JoyConState, side),
};

static const VMStateDescription vmstate_joycon = {
    .name = "nintendo-joycon", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, JoyConState),
        VMSTATE_END_OF_LIST()
    },
};

static void joycon_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    k->realize   = joycon_realize;
    k->vendor_id = 0x057E;
    k->device_id = 0x2007;
    k->class_id  = 0xFF00;
    k->revision  = 0x01;
    dc->desc         = "Nintendo Joy-Con Controller (left/right)";
    dc->vmsd         = &vmstate_joycon;
    dc->hotpluggable = false;
    device_class_set_props(dc, joycon_props);
}

static const TypeInfo joycon_info = {
    .name = TYPE_NINTENDO_JOYCON,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(JoyConState),
    .class_init = joycon_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};

static void joycon_register(void) { type_register_static(&joycon_info); }
type_init(joycon_register)
