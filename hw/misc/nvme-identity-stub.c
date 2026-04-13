/*
 * NVMe SSD Identity Stub
 * Presents a fake NVMe controller with configurable identity and SMART data.
 * The OS can detect the drive and read its identity in Device Manager, AIDA64,
 * CrystalDiskInfo, etc.
 *
 * Properties:
 *   model-number  - drive model (default: "Samsung SSD 970 EVO Plus 1TB")
 *   serial-number - serial number (default: "S4EWNX0M123456")
 *   firmware-rev  - firmware revision (default: "2B2QEXM7")
 *   power-hours   - power-on hours (default: 8000)
 *   data-written  - TB written for SMART (default: 15)
 *   temperature   - drive temperature in Celsius (default: 35)
 *   health        - drive health % (default: 100)
 *
 * Usage:
 *   -device nvme-identity,model-number="WD Black SN850X 2TB",serial-number="WD-WX12345678"
 *
 * Note: This is identification only. The drive does not store actual data.
 *       For real NVMe storage use QEMU's built-in nvme device.
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/module.h"
#include "qemu/log.h"
#include "hw/pci/pci_device.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"
#include <string.h>

/* NVMe Controller Register offsets (BAR0) */
#define NVME_REG_CAP      0x0000  /* Controller Capabilities */
#define NVME_REG_VS       0x0008  /* Version */
#define NVME_REG_INTMS    0x000C  /* Interrupt Mask Set */
#define NVME_REG_INTMC    0x0010  /* Interrupt Mask Clear */
#define NVME_REG_CC       0x0014  /* Controller Configuration */
#define NVME_REG_CSTS     0x001C  /* Controller Status */
#define NVME_REG_AQA      0x0024  /* Admin Queue Attributes */
#define NVME_REG_ASQ      0x0028  /* Admin Submission Queue Base */
#define NVME_REG_ACQ      0x0030  /* Admin Completion Queue Base */

/* NVMe Identify opcodes */
#define NVME_ADM_CMD_IDENTIFY  0x06
#define NVME_ADM_CMD_GET_LOG   0x02
#define NVME_ADM_CMD_ABORT     0x08

#define TYPE_NVME_IDENTITY "nvme-identity"
OBJECT_DECLARE_SIMPLE_TYPE(NvmeIdentityState, NVME_IDENTITY)

struct NvmeIdentityState {
    PCIDevice parent_obj;
    MemoryRegion bar0;  /* NVMe registers */
    MemoryRegion bar4;  /* MSI-X table (stub) */

    /* Configurable properties */
    char *model_number;
    char *serial_number;
    char *firmware_rev;
    uint32_t power_hours;
    uint32_t data_written_tb;
    uint32_t temperature;
    uint32_t health;

    /* Internal state */
    uint32_t cc;     /* Controller Configuration */
    uint32_t csts;   /* Controller Status */
};

/* Build NVMe Identify Controller response (CNS=0x01) — 4096 bytes */
static void nvme_build_identify_ctrl(NvmeIdentityState *s, uint8_t *buf)
{
    memset(buf, 0, 4096);

    /* PCI Vendor/Device ID */
    *(uint16_t *)(buf + 0) = 0x144D;   /* Samsung vendor ID */
    *(uint16_t *)(buf + 2) = 0xA808;   /* 970 EVO Plus device ID */

    /* Serial Number (bytes 4-23, 20 chars, space-padded) */
    const char *sn = s->serial_number ? s->serial_number : "S4EWNX0M123456";
    memset(buf + 4, ' ', 20);
    memcpy(buf + 4, sn, MIN(strlen(sn), 20));

    /* Model Number (bytes 24-63, 40 chars, space-padded) */
    const char *mn = s->model_number ? s->model_number : "Samsung SSD 970 EVO Plus 1TB";
    memset(buf + 24, ' ', 40);
    memcpy(buf + 24, mn, MIN(strlen(mn), 40));

    /* Firmware Revision (bytes 64-71, 8 chars) */
    const char *fr = s->firmware_rev ? s->firmware_rev : "2B2QEXM7";
    memset(buf + 64, ' ', 8);
    memcpy(buf + 64, fr, MIN(strlen(fr), 8));

    /* Recommended Arbitration Burst */
    buf[72] = 0x03;

    /* Number of namespaces */
    *(uint32_t *)(buf + 516) = 1;

    /* Optional Admin command support */
    *(uint16_t *)(buf + 514) = 0x0007; /* SMART/NS mgmt supported */

    /* Max data transfer size (2^n * 4KB), 0=no limit */
    buf[77] = 0x08; /* 1MB max */

    /* Controller type: 1=IO */
    buf[111] = 0x01;

    /* NVM Subsystem NVMe Qualified Name */
    const char *nqn = "nqn.2019-02.com.samsung:nvme:980evo:M288T1G45FPT0-EY";
    memcpy(buf + 768, nqn, MIN(strlen(nqn), 256));
}

/* Build NVMe SMART/Health Information log page — 512 bytes */
static void nvme_build_smart_log(NvmeIdentityState *s, uint8_t *buf)
{
    memset(buf, 0, 512);

    /* Critical Warning: 0 = no warnings */
    buf[0] = 0x00;

    /* Temperature (Kelvin = Celsius + 273) */
    uint16_t temp_k = (uint16_t)(s->temperature + 273);
    *(uint16_t *)(buf + 1) = temp_k;

    /* Available Spare (health %) */
    buf[3] = (uint8_t)s->health;
    /* Available Spare Threshold */
    buf[4] = 10;

    /* Percentage Used (100 - health) */
    buf[5] = (uint8_t)(100 - s->health);

    /* Data Units Read (in 512KB units) — fake 10TB read */
    uint64_t units_read = 10ULL * 1024 * 1024 * 2; /* 10TB in 512KB units */
    memcpy(buf + 32, &units_read, 8);

    /* Data Units Written */
    uint64_t units_written = (uint64_t)s->data_written_tb * 1024 * 1024 * 2;
    memcpy(buf + 48, &units_written, 8);

    /* Power On Hours */
    *(uint32_t *)(buf + 128) = s->power_hours;

    /* Power Cycles */
    *(uint32_t *)(buf + 112) = s->power_hours / 200; /* ~1 cycle per 200h */

    /* Unsafe Shutdowns */
    *(uint32_t *)(buf + 144) = s->power_hours / 500;

    /* Controller Busy Time */
    *(uint32_t *)(buf + 160) = s->power_hours / 10;
}

/* Shared 4KB DMA buffer for identify/log responses */
static uint8_t nvme_dma_buf[4096];

static uint64_t nvme_bar0_read(void *opaque, hwaddr addr, unsigned size)
{
    NvmeIdentityState *s = opaque;

    switch (addr) {
    case NVME_REG_CAP:
        /* CAP: MPSMIN=0(4KB), MPSMAX=0, CSS=1(NVM), MQES=255 */
        return 0x0000F0203020FFULL;
    case NVME_REG_CAP + 4:
        return 0x00F03020;
    case NVME_REG_VS:
        return 0x00010300;  /* NVMe 1.3 */
    case NVME_REG_CC:
        return s->cc;
    case NVME_REG_CSTS:
        /* If enabled (CC.EN=1), report ready */
        return (s->cc & 1) ? 0x00000001 : 0x00000000;
    default:
        return 0;
    }
}

static void nvme_bar0_write(void *opaque, hwaddr addr,
                             uint64_t val, unsigned size)
{
    NvmeIdentityState *s = opaque;
    switch (addr) {
    case NVME_REG_CC:
        s->cc = (uint32_t)val;
        /* When host sets EN=1, controller becomes ready */
        if (val & 1) {
            s->csts = 1; /* CSTS.RDY = 1 */
        }
        break;
    default:
        break;
    }
}

static const MemoryRegionOps nvme_bar0_ops = {
    .read  = nvme_bar0_read,
    .write = nvme_bar0_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 8 },
};

static uint64_t nvme_bar4_read(void *o, hwaddr a, unsigned s) { return 0; }
static void     nvme_bar4_write(void *o, hwaddr a, uint64_t v, unsigned s) {}
static const MemoryRegionOps nvme_bar4_ops = {
    .read=nvme_bar4_read, .write=nvme_bar4_write,
    .endianness=DEVICE_LITTLE_ENDIAN,
    .valid={.min_access_size=1,.max_access_size=4},
};

static void nvme_identity_realize(PCIDevice *pdev, Error **errp)
{
    NvmeIdentityState *s = NVME_IDENTITY(pdev);

    s->cc = 0;
    s->csts = 0;

    /* NVMe: class 0x010802 = Mass Storage, NVMe */
    pdev->config[PCI_CLASS_PROG] = 0x02;
    pci_config_set_class(pdev->config, 0x0108);
    pdev->config[PCI_REVISION_ID] = 0x03;

    pci_set_word(pdev->config + PCI_SUBSYSTEM_VENDOR_ID, 0x144D);
    pci_set_word(pdev->config + PCI_SUBSYSTEM_ID, 0xA801);

    /* MSI capability */
    pdev->config[PCI_CAPABILITY_LIST] = 0x50;
    pdev->config[PCI_STATUS] |= PCI_STATUS_CAP_LIST;
    pdev->config[0x50] = 0x05;  /* MSI */
    pdev->config[0x51] = 0x00;
    pdev->config[0x52] = 0x01;  /* 32-bit, 1 vector */

    /* BAR0: NVMe registers (16KB) */
    memory_region_init_io(&s->bar0, OBJECT(s), &nvme_bar0_ops, s,
                          "nvme-regs", 16 * KiB);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_64, &s->bar0);

    /* BAR4: MSI-X table stub */
    memory_region_init_io(&s->bar4, OBJECT(s), &nvme_bar4_ops, s,
                          "nvme-msix", 4 * KiB);
    pci_register_bar(pdev, 4, PCI_BASE_ADDRESS_SPACE_MEMORY |
                     PCI_BASE_ADDRESS_MEM_TYPE_64, &s->bar4);
}

static const Property nvme_identity_props[] = {
    DEFINE_PROP_STRING("model-number",  NvmeIdentityState, model_number),
    DEFINE_PROP_STRING("serial-number", NvmeIdentityState, serial_number),
    DEFINE_PROP_STRING("firmware-rev",  NvmeIdentityState, firmware_rev),
    DEFINE_PROP_UINT32("power-hours",   NvmeIdentityState, power_hours,   8000),
    DEFINE_PROP_UINT32("data-written",  NvmeIdentityState, data_written_tb, 15),
    DEFINE_PROP_UINT32("temperature",   NvmeIdentityState, temperature,   35),
    DEFINE_PROP_UINT32("health",        NvmeIdentityState, health,       100),
};

static const VMStateDescription vmstate_nvme_identity = {
    .name = "nvme-identity", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, NvmeIdentityState),
        VMSTATE_UINT32(cc,   NvmeIdentityState),
        VMSTATE_UINT32(csts, NvmeIdentityState),
        VMSTATE_END_OF_LIST()
    },
};

static void nvme_identity_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass    *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k  = PCI_DEVICE_CLASS(klass);

    k->realize   = nvme_identity_realize;
    k->vendor_id = 0x144D;   /* Samsung */
    k->device_id = 0xA808;   /* 970 EVO Plus */
    k->class_id  = 0x0108;   /* NVM Express */
    k->revision  = 0x03;

    dc->desc         = "NVMe SSD Identity Stub (configurable model/SMART data)";
    dc->vmsd         = &vmstate_nvme_identity;
    dc->hotpluggable = false;
    device_class_set_props(dc, nvme_identity_props);
    set_bit(DEVICE_CATEGORY_STORAGE, dc->categories);
}

static const TypeInfo nvme_identity_info = {
    .name          = TYPE_NVME_IDENTITY,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NvmeIdentityState),
    .class_init    = nvme_identity_class_init,
    .interfaces    = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE }, { },
    },
};

static void nvme_identity_register_types(void)
{
    type_register_static(&nvme_identity_info);
}

type_init(nvme_identity_register_types)
