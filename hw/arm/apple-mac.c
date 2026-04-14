/*
 * QEMU - Apple Mac machine (AArch64 virt-based)
 *
 * Provides machine types that identify as Apple Mac platforms:
 *   mac-m1        Apple Mac mini / MacBook Air M1 (2020)
 *   mac-m1-pro    Apple MacBook Pro M1 Pro (2021)
 *   mac-m1-max    Apple MacBook Pro M1 Max (2021)
 *   mac-m2        Apple MacBook Air M2 (2022)
 *   mac-m2-pro    Apple MacBook Pro M2 Pro (2023)
 *   mac-m2-max    Apple MacBook Pro M2 Max (2023)
 *   mac-m3        Apple MacBook Air M3 (2024)
 *   mac-m3-pro    Apple MacBook Pro M3 Pro (2024)
 *   mac-m3-max    Apple MacBook Pro M3 Max (2024)
 *
 * These are thin wrappers around the standard 'virt' machine with
 * Apple-appropriate defaults (default CPU, RAM, description).
 * Full Apple Silicon behavior requires real Apple hardware.
 *
 * Usage:
 *   qemu-system-aarch64 -machine mac-m1 -cpu apple-m1 -m 16G \
 *     -bios edk2-aarch64-code.fd -drive ...
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/core/boards.h"
#include "hw/arm/virt.h"
#include "hw/arm/machines-qom.h"
#include "target/arm/cpu-qom.h"
#include "qom/object.h"

/* arm_aarch64_machine_interfaces is provided by target/arm/machine.o */

/* TYPE_VIRT_MACHINE is defined in hw/arm/virt.h as MACHINE_TYPE_NAME("virt") */

/*
 * DEFINE_MAC_MACHINE(cid, mname, cpu_name, ram_gb, desc_str)
 *   cid      - C identifier token (no hyphens)
 *   mname    - machine name string (may have hyphens, e.g. "mac-m1-pro")
 *   cpu_name - CPU model string (e.g. "apple-m1-pro")
 *   ram_gb   - default RAM in GB
 *   desc_str - human-readable description
 */
#define DEFINE_MAC_MACHINE(cid, mname, cpu_name, ram_gb, desc_str)          \
static void mac_##cid##_class_init(ObjectClass *oc, const void *data)       \
{                                                                             \
    MachineClass *mc = MACHINE_CLASS(oc);                                    \
    mc->desc             = desc_str;                                          \
    mc->default_cpu_type = cpu_name "-" TYPE_ARM_CPU;                        \
    mc->default_ram_size = (uint64_t)(ram_gb) * 1024 * 1024 * 1024ULL;     \
    mc->min_cpus         = 1;                                                 \
    mc->default_cpus     = 8;                                                 \
    mc->max_cpus         = 16;                                                \
    mc->no_floppy        = true;                                              \
    mc->no_cdrom         = false;                                             \
    mc->no_parallel      = true;                                              \
}                                                                             \
static const TypeInfo mac_##cid##_machine_type = {                           \
    .name       = MACHINE_TYPE_NAME(mname),                                  \
    .parent     = TYPE_VIRT_MACHINE,                                          \
    .class_init = mac_##cid##_class_init,                                    \
    .interfaces = (const InterfaceInfo []) {                                  \
        { TYPE_TARGET_AARCH64_MACHINE }, { }                                 \
    },                                                                        \
};                                                                            \
static void mac_##cid##_register(void)                                       \
{                                                                             \
    type_register_static(&mac_##cid##_machine_type);                         \
}                                                                             \
type_init(mac_##cid##_register)

DEFINE_MAC_MACHINE(m1,     "mac-m1",     "apple-m1",     16,
    "Apple Mac M1 (2020, 8-core, 16GB unified memory)")
DEFINE_MAC_MACHINE(m1pro,  "mac-m1-pro", "apple-m1-pro", 16,
    "Apple Mac M1 Pro (2021, 10-core, 16GB unified memory)")
DEFINE_MAC_MACHINE(m1max,  "mac-m1-max", "apple-m1-max", 32,
    "Apple Mac M1 Max (2021, 10-core, 32GB unified memory)")
DEFINE_MAC_MACHINE(m2,     "mac-m2",     "apple-m2",      8,
    "Apple Mac M2 (2022, 8-core, 8GB unified memory)")
DEFINE_MAC_MACHINE(m2pro,  "mac-m2-pro", "apple-m2-pro", 16,
    "Apple Mac M2 Pro (2023, 12-core, 16GB unified memory)")
DEFINE_MAC_MACHINE(m2max,  "mac-m2-max", "apple-m2-max", 32,
    "Apple Mac M2 Max (2023, 12-core, 32GB unified memory)")
DEFINE_MAC_MACHINE(m3,     "mac-m3",     "apple-m3",      8,
    "Apple Mac M3 (2023, 8-core, 8GB unified memory)")
DEFINE_MAC_MACHINE(m3pro,  "mac-m3-pro", "apple-m3-pro", 18,
    "Apple Mac M3 Pro (2024, 12-core, 18GB unified memory)")
DEFINE_MAC_MACHINE(m3max,  "mac-m3-max", "apple-m3-max", 36,
    "Apple Mac M3 Max (2024, 14-core, 36GB unified memory)")
