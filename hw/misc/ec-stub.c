/*
 * Embedded Controller (EC) stub for QEMU
 *
 * Emulates a minimal ACPI Embedded Controller (ACPI EC / PNP0C09).
 * The EC manages keyboard, backlight, battery charging, thermal sensors,
 * and fan control on laptops. Without an EC stub, some ACPI drivers
 * may fail to load or report errors.
 *
 * This stub provides:
 *   - EC register space (256 bytes) via ACPI-defined I/O ports
 *   - Plausible thermal sensor readings (CPU temp, fan speed)
 *   - Battery charge controller interface
 *   - Keyboard backlight control registers
 *
 * Best used with -machine pc,laptop-mode=on
 *
 * ACPI EC standard I/O ports:
 *   0x62/0x66 = EC data/command (traditional)
 *   Additional ports via ACPI ECDT table or DSDT _CRS
 *
 * Usage: This is automatically handled via laptop-mode=on.
 *        For manual control: -device acpi-ec
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "hw/isa/isa.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_ACPI_EC "acpi-ec"
OBJECT_DECLARE_SIMPLE_TYPE(AcpiECState, ACPI_EC)

/* EC register offsets */
#define EC_REG_TEMP_CPU      0x07  /* CPU temperature (C) */
#define EC_REG_TEMP_GPU      0x0B  /* GPU temperature (C) */
#define EC_REG_FAN_SPEED_LO  0x0E  /* Fan RPM low byte */
#define EC_REG_FAN_SPEED_HI  0x0F  /* Fan RPM high byte */
#define EC_REG_BAT_CAPACITY  0x2C  /* Battery remaining % */
#define EC_REG_BAT_VOLTAGE   0x2E  /* Battery voltage (in 10mV) */
#define EC_REG_BAT_CURRENT   0x30  /* Battery current (mA, signed) */
#define EC_REG_AC_STATUS     0x42  /* AC adapter: 0=battery, 1=AC */
#define EC_REG_LID_STATUS    0x46  /* Lid: 0=closed, 1=open */
#define EC_REG_KBD_BACKLIGHT 0x5A  /* Keyboard backlight level 0-100 */
#define EC_REG_EC_VERSION    0xFE  /* EC firmware version */
#define EC_REG_STATUS        0xFF  /* EC status register */

/* Standard EC I/O ports */
#define EC_DATA_PORT    0x62
#define EC_CMD_PORT     0x66

#define EC_RAM_SIZE     256

struct AcpiECState {
    ISADevice parent_obj;
    PortioList portio;

    /* EC RAM */
    uint8_t ec_ram[EC_RAM_SIZE];

    /* Internal state */
    uint8_t  last_cmd;
    uint8_t  addr_reg;
    bool     addr_phase;  /* true = next write is address */
    uint64_t last_update_ns;

    /* Configurable properties */
    uint32_t cpu_temp;    /* CPU temperature in Celsius */
    uint32_t gpu_temp;    /* GPU temperature in Celsius */
    uint32_t fan_rpm;     /* Fan speed in RPM */
    uint32_t bat_percent; /* Battery percentage */
};

static void ec_update_sensors(AcpiECState *s)
{
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->last_update_ns < 500000000ULL) return; /* update every 500ms */
    s->last_update_ns = now;

    /* Fluctuate temperature slightly */
    uint32_t noise = (uint32_t)(now >> 27) & 3;
    s->ec_ram[EC_REG_TEMP_CPU] = (uint8_t)(s->cpu_temp + noise);
    s->ec_ram[EC_REG_TEMP_GPU] = (uint8_t)(s->gpu_temp + (noise >> 1));

    /* Fan RPM */
    uint16_t rpm = (uint16_t)(s->fan_rpm + (noise * 50));
    s->ec_ram[EC_REG_FAN_SPEED_LO] = rpm & 0xFF;
    s->ec_ram[EC_REG_FAN_SPEED_HI] = rpm >> 8;

    /* Battery status */
    s->ec_ram[EC_REG_BAT_CAPACITY] = (uint8_t)s->bat_percent;
    s->ec_ram[EC_REG_AC_STATUS]    = 1;   /* AC connected */
    s->ec_ram[EC_REG_LID_STATUS]   = 1;   /* Lid open */
    s->ec_ram[EC_REG_EC_VERSION]   = 0x42; /* EC FW v0x42 */
    s->ec_ram[EC_REG_STATUS]       = 0x00; /* No pending IRQ */

    /* Battery voltage 11.4V = 1140 in 10mV units */
    uint16_t vbat = 1140;
    s->ec_ram[EC_REG_BAT_VOLTAGE]     = vbat & 0xFF;
    s->ec_ram[EC_REG_BAT_VOLTAGE + 1] = vbat >> 8;
}

static uint32_t ec_data_read(void *opaque, uint32_t addr)
{
    AcpiECState *s = opaque;
    ec_update_sensors(s);

    if (s->addr_phase) {
        /* Reading before address written — return status */
        return 0x00; /* EC idle */
    }
    /* Return EC RAM at last address */
    return s->ec_ram[s->addr_reg];
}

static void ec_data_write(void *opaque, uint32_t addr, uint32_t val)
{
    AcpiECState *s = opaque;
    if (s->addr_phase) {
        s->addr_reg = val & 0xFF;
        s->addr_phase = false;
    } else {
        s->ec_ram[s->addr_reg] = val & 0xFF;
    }
}

static uint32_t ec_cmd_read(void *opaque, uint32_t addr)
{
    AcpiECState *s = opaque;
    ec_update_sensors(s);
    /* EC status: OBF=0 (output buffer empty), IBF=0 (input buffer empty) */
    return 0x00;
}

static void ec_cmd_write(void *opaque, uint32_t addr, uint32_t val)
{
    AcpiECState *s = opaque;
    s->last_cmd = val & 0xFF;
    switch (val) {
    case 0x80: /* READ */
        s->addr_phase = true;
        break;
    case 0x81: /* WRITE */
        s->addr_phase = true;
        break;
    default:
        break;
    }
}

static const MemoryRegionPortio ec_portio[] = {
    { EC_DATA_PORT, 1, 1, .read = ec_data_read, .write = ec_data_write },
    { EC_CMD_PORT,  1, 1, .read = ec_cmd_read,  .write = ec_cmd_write  },
    PORTIO_END_OF_LIST()
};

static void acpi_ec_realize(DeviceState *dev, Error **errp)
{
    AcpiECState *s = ACPI_EC(dev);
    ISADevice *isa = ISA_DEVICE(dev);

    memset(s->ec_ram, 0, sizeof(s->ec_ram));

    /* Initialize sensor defaults */
    s->ec_ram[EC_REG_TEMP_CPU]    = (uint8_t)s->cpu_temp;
    s->ec_ram[EC_REG_TEMP_GPU]    = (uint8_t)s->gpu_temp;
    s->ec_ram[EC_REG_FAN_SPEED_LO]= (uint8_t)(s->fan_rpm & 0xFF);
    s->ec_ram[EC_REG_FAN_SPEED_HI]= (uint8_t)(s->fan_rpm >> 8);
    s->ec_ram[EC_REG_BAT_CAPACITY]= (uint8_t)s->bat_percent;
    s->ec_ram[EC_REG_AC_STATUS]   = 1;
    s->ec_ram[EC_REG_LID_STATUS]  = 1;
    s->ec_ram[EC_REG_EC_VERSION]  = 0x42;
    s->ec_ram[EC_REG_KBD_BACKLIGHT] = 100; /* full brightness */

    isa_register_portio_list(isa, &s->portio, 0, ec_portio, s, "acpi-ec");
}

static const Property acpi_ec_props[] = {
    DEFINE_PROP_UINT32("cpu-temp",    AcpiECState, cpu_temp,    65),
    DEFINE_PROP_UINT32("gpu-temp",    AcpiECState, gpu_temp,    70),
    DEFINE_PROP_UINT32("fan-rpm",     AcpiECState, fan_rpm,   2200),
    DEFINE_PROP_UINT32("bat-percent", AcpiECState, bat_percent, 87),
};

static const VMStateDescription vmstate_acpi_ec = {
    .name = "acpi-ec", .version_id = 1, .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_UINT8_ARRAY(ec_ram, AcpiECState, EC_RAM_SIZE),
        VMSTATE_UINT8(last_cmd,  AcpiECState),
        VMSTATE_UINT8(addr_reg,  AcpiECState),
        VMSTATE_BOOL(addr_phase, AcpiECState),
        VMSTATE_END_OF_LIST()
    },
};

static void acpi_ec_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->realize  = acpi_ec_realize;
    dc->desc     = "ACPI Embedded Controller (EC) — laptop sensors, battery, fan";
    dc->vmsd     = &vmstate_acpi_ec;
    dc->hotpluggable = false;
    device_class_set_props(dc, acpi_ec_props);
}

static const TypeInfo acpi_ec_info = {
    .name          = TYPE_ACPI_EC,
    .parent        = TYPE_ISA_DEVICE,
    .instance_size = sizeof(AcpiECState),
    .class_init    = acpi_ec_class_init,
};

static void acpi_ec_register_types(void)
{
    type_register_static(&acpi_ec_info);
}
type_init(acpi_ec_register_types)
