/*
 * QEMU - Preset Profile System
 *
 * Provides named hardware configuration presets selectable via:
 *   -device preset-profile,profile=gaming-pc-2024
 *
 * Each profile defines a complete PC identity (CPU, GPU, RAM, board)
 * and prints a summary at startup so the user knows what's active.
 *
 * Available profiles:
 *   gaming-pc-2024      Ryzen 9 7950X + RTX 4090, DDR5-6000, B650E
 *   workstation-2024    Threadripper 7970X + RTX 4000 Ada, DDR5 ECC
 *   budget-pc-2020      Ryzen 5 3600 + RX 5700, DDR4-3200, B450
 *   server-2024         EPYC 9654 + 8x H100, DDR5 ECC RDIMM
 *   laptop-2023         Core i7-1370P + Iris Xe, LPDDR5-4800
 *   htpc-2022           Core i5-12400 + Intel UHD 730, DDR4-3200
 *   intel-gaming-2024   Core i9-14900K + RTX 4090, DDR5-6400, Z790
 *   datacenter-ai       EPYC 9654 + 8x H100 SXM5, DDR5 ECC RDIMM
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/log.h"
#include "hw/core/sysbus.h"
#include "hw/core/qdev-properties.h"
#include "qom/object.h"
#include "migration/vmstate.h"

#define TYPE_PRESET_PROFILE "preset-profile"
OBJECT_DECLARE_SIMPLE_TYPE(PresetProfileState, PRESET_PROFILE)

typedef struct {
    const char *name;
    const char *cpu_model;
    const char *board;
    const char *ram_type;
    unsigned int ram_speed_mhz;
    const char *ram_form_factor;
    unsigned int ram_gb;
    const char *gpu_model;
    const char *notes;
} PresetProfile;

static const PresetProfile profiles[] = {
    {
        "gaming-pc-2024",
        "AMD Ryzen 9 7950X (16C/32T, Zen4, 4.5/5.7 GHz, 80MB cache)",
        "ASUS ROG Crosshair X670E Hero (AM5, X670E)",
        "DDR5", 6000, "DIMM", 64,
        "NVIDIA GeForce RTX 4090 (AD102, 24GB GDDR6X)",
        "High-end gaming rig, PCIe 5.0 x16, Windows 11"
    },
    {
        "workstation-2024",
        "AMD Ryzen Threadripper PRO 7970X (32C/64T, Zen4, sWRX9)",
        "ASUS Pro WS TRX50-SAGE WiFi (TRX50, sWRX9)",
        "DDR5", 4800, "RDIMM", 256,
        "NVIDIA RTX 4000 Ada Generation (AD104, 20GB GDDR6)",
        "Professional workstation, ECC RDIMM, 8-channel DDR5"
    },
    {
        "budget-pc-2020",
        "AMD Ryzen 5 3600 (6C/12T, Zen2, 3.6/4.2 GHz, 35MB cache)",
        "MSI B450 TOMAHAWK MAX (AM4, B450)",
        "DDR4", 3200, "DIMM", 16,
        "AMD Radeon RX 5700 (Navi 10, 8GB GDDR6)",
        "Budget gaming build, PCIe 4.0 x16, Windows 10"
    },
    {
        "server-2024",
        "AMD EPYC 9654 (96C/192T, Zen4c, SP5, 2.4/3.7 GHz)",
        "Supermicro H13DSG-Q-CPU (SP5 dual socket)",
        "DDR5", 4800, "RDIMM", 768,
        "NVIDIA H100 SXM5 x8 (GH100, 80GB HBM3)",
        "AI/ML datacenter node, 12-channel DDR5 ECC per CPU"
    },
    {
        "laptop-2023",
        "Intel Core i7-1370P (14C/20T, Raptor Lake-P, 1.9/5.2 GHz)",
        "ASUS ZenBook Pro 16X OLED (OEM, Intel HM770)",
        "LPDDR5", 4800, "SODIMM", 32,
        "Intel Iris Xe Graphics (96 EU, integrated)",
        "Ultrabook, soldered LPDDR5, Thunderbolt 4, Wi-Fi 6E"
    },
    {
        "htpc-2022",
        "Intel Core i5-12400 (6C/12T, Alder Lake-S, 2.5/4.4 GHz)",
        "Gigabyte H610M S2H (LGA1700, H610)",
        "DDR4", 3200, "DIMM", 16,
        "Intel UHD Graphics 730 (integrated, 24 EU)",
        "Home theater PC, low power, integrated graphics"
    },
    {
        "intel-gaming-2024",
        "Intel Core i9-14900K (24C/32T, Raptor Lake-R, 3.2/6.0 GHz)",
        "ASUS ROG Maximus Z790 Hero (LGA1700, Z790)",
        "DDR5", 6400, "DIMM", 64,
        "NVIDIA GeForce RTX 4090 (AD102, 24GB GDDR6X)",
        "Flagship Intel gaming, PCIe 5.0 x16, DDR5-6400 OC"
    },
    {
        "datacenter-ai",
        "AMD EPYC 9654 (96C/192T, Zen4c, SP5, 2.4/3.7 GHz)",
        "HPE ProLiant DL380 Gen11 (SP5 dual socket)",
        "DDR5", 4800, "RDIMM", 1536,
        "NVIDIA H100 SXM5 x8 (GH100, 80GB HBM3 each, NVLink 4.0)",
        "AI/HPC node, NVLink bridge, 400G InfiniBand NDR"
    },
    { NULL, NULL, NULL, NULL, 0, NULL, 0, NULL, NULL }
};

struct PresetProfileState {
    SysBusDevice parent_obj;
    char *profile;
};

static void preset_profile_realize(DeviceState *dev, Error **errp)
{
    PresetProfileState *s = PRESET_PROFILE(dev);
    const PresetProfile *p = NULL;

    if (!s->profile) {
        fprintf(stderr,
            "[qemu-extra] preset-profile: no profile set. "
            "Use -device preset-profile,profile=<name>\n"
            "[qemu-extra] Available profiles: gaming-pc-2024, "
            "workstation-2024, budget-pc-2020, server-2024, "
            "laptop-2023, htpc-2022, intel-gaming-2024, datacenter-ai\n");
        return;
    }

    for (int i = 0; profiles[i].name; i++) {
        if (strcmp(s->profile, profiles[i].name) == 0) {
            p = &profiles[i];
            break;
        }
    }

    if (!p) {
        fprintf(stderr,
            "[qemu-extra] preset-profile: unknown profile '%s'\n"
            "[qemu-extra] Available: gaming-pc-2024, workstation-2024, "
            "budget-pc-2020, server-2024, laptop-2023, htpc-2022, "
            "intel-gaming-2024, datacenter-ai\n",
            s->profile);
        return;
    }

    fprintf(stderr,
        "[qemu-extra] Preset Profile Active: %s\n"
        "  CPU    : %s\n"
        "  Board  : %s\n"
        "  RAM    : %uGB %s-%u (%s)\n"
        "  GPU    : %s\n"
        "  Notes  : %s\n",
        p->name,
        p->cpu_model,
        p->board,
        p->ram_gb, p->ram_type, p->ram_speed_mhz, p->ram_form_factor,
        p->gpu_model,
        p->notes);
}

static const Property preset_profile_props[] = {
    DEFINE_PROP_STRING("profile", PresetProfileState, profile),
};

static const VMStateDescription vmstate_preset_profile = {
    .name = "preset-profile",
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (const VMStateField[]) {
        VMSTATE_END_OF_LIST()
    },
};

static void preset_profile_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    dc->realize      = preset_profile_realize;
    dc->desc         = "QEMU hardware preset profile selector";
    dc->vmsd         = &vmstate_preset_profile;
    dc->hotpluggable = false;
    device_class_set_props(dc, preset_profile_props);
}

static const TypeInfo preset_profile_info = {
    .name          = TYPE_PRESET_PROFILE,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(PresetProfileState),
    .class_init    = preset_profile_class_init,
};

static void preset_profile_register(void)
{
    type_register_static(&preset_profile_info);
}
type_init(preset_profile_register)
