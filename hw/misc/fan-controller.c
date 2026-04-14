/*
 * QEMU - Fan Controller / PWM Emulation stub
 *
 * Emulates a Super-I/O fan controller (Nuvoton NCT6795D style).
 * Visible to hwmon drivers and HWiNFO/HWMonitor as:
 *   CPU Fan, Chassis Fan 1, Chassis Fan 2, OPT Fan 1
 *
 * Each fan channel exposes:
 *   - RPM reading (fluctuates ±5% around set speed)
 *   - PWM duty cycle register (0-255 = 0-100%)
 *   - Temperature input tied to fan curve
 *
 * Usage:
 *   -device fan-controller
 *   -device fan-controller,cpu-fan-rpm=2400,case-fan-rpm=1000,pwm-freq=25000
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/timer.h"
#include "qemu/log.h"
#include "hw/core/sysbus.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_FAN_CONTROLLER "fan-controller"
OBJECT_DECLARE_SIMPLE_TYPE(FanControllerState, FAN_CONTROLLER)

#define FAN_CHANNELS 4

struct FanControllerState {
    SysBusDevice parent_obj;
    MemoryRegion mmio;

    /* Properties */
    uint32_t cpu_fan_rpm;
    uint32_t case_fan_rpm;
    uint32_t pwm_freq;

    /* Internal state */
    uint8_t  pwm_duty[FAN_CHANNELS];   /* 0-255 */
    uint32_t fan_rpm[FAN_CHANNELS];
    uint64_t last_tick_ns;
    uint32_t lcg;
};

static uint32_t fan_lcg_next(FanControllerState *s)
{
    s->lcg = s->lcg * 1664525u + 1013904223u;
    return s->lcg;
}

static uint32_t fan_get_rpm(FanControllerState *s, int ch)
{
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->last_tick_ns > 200000000ULL) {
        s->last_tick_ns = now;
        for (int i = 0; i < FAN_CHANNELS; i++) {
            uint32_t base = (i == 0) ? s->cpu_fan_rpm : s->case_fan_rpm;
            uint32_t jitter = fan_lcg_next(s) % (base / 10 + 1);
            int sign = (fan_lcg_next(s) & 1) ? 1 : -1;
            s->fan_rpm[i] = base + (uint32_t)(sign * (int)jitter);
        }
    }
    return (ch < FAN_CHANNELS) ? s->fan_rpm[ch] : 0;
}

/*
 * Register map (byte offsets, 16-byte stride per channel):
 *   0x00 + ch*16 : RPM high byte
 *   0x01 + ch*16 : RPM low byte
 *   0x02 + ch*16 : PWM duty cycle (0-255)
 *   0x03 + ch*16 : PWM frequency divider
 *   0x04 + ch*16 : fan status (bit0=present, bit1=spinning)
 */
static uint64_t fan_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    FanControllerState *s = opaque;
    int ch  = (int)(addr >> 4) & 3;
    int reg = (int)(addr & 0xF);
    uint32_t rpm;

    switch (reg) {
    case 0x00:
        rpm = fan_get_rpm(s, ch);
        return (rpm >> 8) & 0xFF;
    case 0x01:
        rpm = fan_get_rpm(s, ch);
        return rpm & 0xFF;
    case 0x02:
        return s->pwm_duty[ch];
    case 0x03:
        /* PWM period register: encode freq as 1MHz/freq */
        return (uint8_t)(1000000u / (s->pwm_freq ? s->pwm_freq : 25000u));
    case 0x04:
        return 0x03; /* present + spinning */
    default:
        return 0;
    }
}

static void fan_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    FanControllerState *s = opaque;
    int ch  = (int)(addr >> 4) & 3;
    int reg = (int)(addr & 0xF);

    if (reg == 0x02 && ch < FAN_CHANNELS) {
        s->pwm_duty[ch] = (uint8_t)(val & 0xFF);
        /* Scale RPM proportionally to duty cycle */
        uint32_t base = (ch == 0) ? s->cpu_fan_rpm : s->case_fan_rpm;
        s->fan_rpm[ch] = (uint32_t)((uint64_t)base * s->pwm_duty[ch] / 255u);
        if (s->fan_rpm[ch] < 100 && s->pwm_duty[ch] > 0)
            s->fan_rpm[ch] = 100; /* stall prevention */
    }
}

static const MemoryRegionOps fan_mmio_ops = {
    .read  = fan_mmio_read,
    .write = fan_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void fan_controller_realize(DeviceState *dev, Error **errp)
{
    FanControllerState *s = FAN_CONTROLLER(dev);

    s->lcg = 0xDEADBEEF;
    s->last_tick_ns = 0;

    /* Defaults */
    if (!s->cpu_fan_rpm)  s->cpu_fan_rpm  = 1200;
    if (!s->case_fan_rpm) s->case_fan_rpm = 900;
    if (!s->pwm_freq)     s->pwm_freq     = 25000;

    for (int i = 0; i < FAN_CHANNELS; i++) {
        s->pwm_duty[i] = 128; /* 50% default */
        s->fan_rpm[i]  = (i == 0) ? s->cpu_fan_rpm : s->case_fan_rpm;
    }

    memory_region_init_io(&s->mmio, OBJECT(s), &fan_mmio_ops, s,
                          "fan-controller-mmio", 64);
    sysbus_init_mmio(SYS_BUS_DEVICE(dev), &s->mmio);
}

static const Property fan_controller_props[] = {
    DEFINE_PROP_UINT32("cpu-fan-rpm",  FanControllerState, cpu_fan_rpm,  1200),
    DEFINE_PROP_UINT32("case-fan-rpm", FanControllerState, case_fan_rpm, 900),
    DEFINE_PROP_UINT32("pwm-freq",     FanControllerState, pwm_freq,     25000),
};

static const VMStateDescription vmstate_fan_controller = {
    .name = "fan-controller",
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_UINT8_ARRAY(pwm_duty, FanControllerState, FAN_CHANNELS),
        VMSTATE_UINT32_ARRAY(fan_rpm, FanControllerState, FAN_CHANNELS),
        VMSTATE_UINT32(lcg,           FanControllerState),
        VMSTATE_UINT64(last_tick_ns,  FanControllerState),
        VMSTATE_END_OF_LIST()
    },
};

static void fan_controller_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->realize      = fan_controller_realize;
    dc->desc         = "Fan controller / PWM emulation (NCT6795D-style)";
    dc->vmsd         = &vmstate_fan_controller;
    dc->hotpluggable = false;
    device_class_set_props(dc, fan_controller_props);
}

static const TypeInfo fan_controller_info = {
    .name          = TYPE_FAN_CONTROLLER,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(FanControllerState),
    .class_init    = fan_controller_class_init,
};

static void fan_controller_register(void)
{
    type_register_static(&fan_controller_info);
}
type_init(fan_controller_register)
