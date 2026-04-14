/*
 * QEMU - CPU VRM (Voltage Regulator Module) Reporting Stub
 *
 * Emulates a PMBus-compatible VRM visible to hwmon/HWiNFO as:
 *   - CPU VCore voltage (fluctuating around set value)
 *   - CPU current (Imon)
 *   - CPU power draw (Vcore * Imon)
 *
 * Usage:
 *   -device cpu-vrm
 *   -device cpu-vrm,vcore-mv=1250,imon-ma=45000,vrm-model=ASP2205
 *
 * Properties:
 *   vcore-mv   : CPU VCore in millivolts (default 1250 = 1.25V)
 *   imon-ma    : CPU current in milliamps (default 45000 = 45A)
 *   vrm-model  : VRM model string (default "ASP2205")
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

#define TYPE_CPU_VRM "cpu-vrm"
OBJECT_DECLARE_SIMPLE_TYPE(CpuVrmState, CPU_VRM)

struct CpuVrmState {
    SysBusDevice parent_obj;
    MemoryRegion mmio;

    /* Properties */
    uint32_t vcore_mv;    /* millivolts */
    uint32_t imon_ma;     /* milliamps  */
    char    *vrm_model;

    /* Internal */
    uint32_t lcg;
    uint64_t last_tick_ns;
    uint32_t cur_vcore_mv;
    uint32_t cur_imon_ma;
};

static uint32_t vrm_lcg_next(CpuVrmState *s)
{
    s->lcg = s->lcg * 1664525u + 1013904223u;
    return s->lcg;
}

static void vrm_update(CpuVrmState *s)
{
    uint64_t now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    if (now - s->last_tick_ns > 150000000ULL) {
        s->last_tick_ns = now;
        /* Vcore: fluctuate ±20mV */
        uint32_t vj = vrm_lcg_next(s) % 41; /* 0-40 */
        s->cur_vcore_mv = s->vcore_mv - 20 + vj;
        /* Imon: fluctuate ±2A */
        uint32_t ij = vrm_lcg_next(s) % 4001; /* 0-4000 mA */
        s->cur_imon_ma = s->imon_ma - 2000 + ij;
    }
}

/*
 * Register map:
 *   0x00: VCore in millivolts (16-bit little-endian)
 *   0x02: Imon in milliamps (16-bit little-endian)
 *   0x04: Power in milliwatts (32-bit little-endian) = Vcore * Imon / 1000
 *   0x08: VRM model ID (1 byte, 0xAB for ASP2205)
 *   0x09: Phase count (1 byte)
 *   0x0A: Efficiency percent (1 byte)
 *   0x0B: Temperature (1 byte, degrees C)
 */
static uint64_t vrm_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    CpuVrmState *s = opaque;
    vrm_update(s);

    switch (addr) {
    case 0x00: return s->cur_vcore_mv & 0xFF;
    case 0x01: return (s->cur_vcore_mv >> 8) & 0xFF;
    case 0x02: return s->cur_imon_ma & 0xFF;
    case 0x03: return (s->cur_imon_ma >> 8) & 0xFF;
    case 0x04: {
        uint32_t pw = (uint32_t)((uint64_t)s->cur_vcore_mv * s->cur_imon_ma / 1000000ULL);
        return pw & 0xFF;
    }
    case 0x05: {
        uint32_t pw = (uint32_t)((uint64_t)s->cur_vcore_mv * s->cur_imon_ma / 1000000ULL);
        return (pw >> 8) & 0xFF;
    }
    case 0x06: {
        uint32_t pw = (uint32_t)((uint64_t)s->cur_vcore_mv * s->cur_imon_ma / 1000000ULL);
        return (pw >> 16) & 0xFF;
    }
    case 0x07: return 0;
    case 0x08: return 0xAB; /* VRM model ID */
    case 0x09: return 16;   /* 16-phase VRM */
    case 0x0A: return 92;   /* 92% efficiency */
    case 0x0B: {
        /* VRM temperature: 40-65°C based on load */
        uint32_t temp = 40 + (uint32_t)(s->cur_imon_ma / 2000);
        if (temp > 65) temp = 65;
        return temp;
    }
    default:   return 0;
    }
}

static void vrm_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    /* VRM registers are read-only from guest perspective */
    (void)opaque; (void)addr; (void)val; (void)size;
}

static const MemoryRegionOps vrm_mmio_ops = {
    .read  = vrm_mmio_read,
    .write = vrm_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void cpu_vrm_realize(DeviceState *dev, Error **errp)
{
    CpuVrmState *s = CPU_VRM(dev);

    if (!s->vcore_mv) s->vcore_mv = 1250;
    if (!s->imon_ma)  s->imon_ma  = 45000;
    s->cur_vcore_mv = s->vcore_mv;
    s->cur_imon_ma  = s->imon_ma;
    s->lcg          = 0xCAFEBABE;
    s->last_tick_ns = 0;

    memory_region_init_io(&s->mmio, OBJECT(s), &vrm_mmio_ops, s,
                          "cpu-vrm-mmio", 16);
    sysbus_init_mmio(SYS_BUS_DEVICE(dev), &s->mmio);
}

static const Property cpu_vrm_props[] = {
    DEFINE_PROP_UINT32("vcore-mv",  CpuVrmState, vcore_mv,  1250),
    DEFINE_PROP_UINT32("imon-ma",   CpuVrmState, imon_ma,   45000),
    DEFINE_PROP_STRING("vrm-model", CpuVrmState, vrm_model),
};

static const VMStateDescription vmstate_cpu_vrm = {
    .name = "cpu-vrm",
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_UINT32(vcore_mv,       CpuVrmState),
        VMSTATE_UINT32(imon_ma,        CpuVrmState),
        VMSTATE_UINT32(cur_vcore_mv,   CpuVrmState),
        VMSTATE_UINT32(cur_imon_ma,    CpuVrmState),
        VMSTATE_UINT32(lcg,            CpuVrmState),
        VMSTATE_UINT64(last_tick_ns,   CpuVrmState),
        VMSTATE_END_OF_LIST()
    },
};

static void cpu_vrm_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->realize      = cpu_vrm_realize;
    dc->desc         = "CPU VRM (Voltage Regulator Module) reporting stub";
    dc->vmsd         = &vmstate_cpu_vrm;
    dc->hotpluggable = false;
    device_class_set_props(dc, cpu_vrm_props);
}

static const TypeInfo cpu_vrm_info = {
    .name          = TYPE_CPU_VRM,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(CpuVrmState),
    .class_init    = cpu_vrm_class_init,
};

static void cpu_vrm_register(void)
{
    type_register_static(&cpu_vrm_info);
}
type_init(cpu_vrm_register)
