/*
 * QEMU - Apple Mac machine types (Intel era)
 *
 * Defines Q35-based Mac machine types with Apple SMBIOS identity,
 * Apple SMC auto-attach, and optional T2 security chip.
 *
 * All machines use the Q35/ICH9 chipset as the structural backbone
 * (closest to the Intel chipsets used in real Intel Macs) and set
 * SMBIOS Type 1/2 to report Apple Inc. as manufacturer and the
 * corresponding Mac model identifier as product.
 *
 * Usage:
 *   qemu-system-x86_64 -M mac-macbook-pro-15-2019 -m 16384
 *   qemu-system-x86_64 -M mac-mac-pro-2019 -m 32768
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include CONFIG_DEVICES

#include "qemu/units.h"
#include "hw/i386/x86.h"
#include "hw/i386/pc.h"
#include "hw/pci-host/q35.h"
#include "hw/southbridge/ich9.h"
#include "hw/pci/pci.h"
#include "hw/i2c/smbus_eeprom.h"
#include "system/memory.h"
#include "hw/acpi/acpi.h"
#include "qapi/error.h"
#include "qemu/error-report.h"
#include "hw/core/irq.h"
#include "system/kvm.h"
#include "hw/i386/kvm/clock.h"
#include "hw/core/sysbus.h"
#include "hw/mem/nvdimm.h"
#include "hw/i386/acpi-build.h"
#include "target/i386/cpu.h"
#include "migration/global_state.h"
#include "system/numa.h"
#include "hw/firmware/smbios.h"

static void mac_q35_common_options(MachineClass *m)
{
    PCMachineClass *pcmc = PC_MACHINE_CLASS(m);
    pcmc->pci_root_uid = 0;
    pcmc->default_cpu_version = 1;
    pcmc->gigabyte_align = true;
    pcmc->smbios_defaults = true;
    pcmc->default_smbios_ep_type = SMBIOS_ENTRY_POINT_TYPE_64;

    m->family = "mac_q35";
    m->default_machine_opts = "firmware=bios-256k.bin,auto-applesmc=on";
    m->default_display = "std";
    m->default_nic = "e1000e";
    m->no_floppy = 1;
    m->no_parallel = true;
    m->max_cpus = 28;
}

/* ===========================================================================
 * Apple MacBook 13-inch (Core Duo, 2006)
 * =========================================================================== */
static void mac_macbook_2006_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook 13-inch (Core Duo, 2006)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("coreduo-t2500");
    m->max_cpus = 2;
    m->default_ram_size = 512 * MiB;
}

static void mac_macbook_2006_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBook1,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_2006_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_2006_machine_options(mc);
    mc->init = mac_macbook_2006_machine_init;
}

static const TypeInfo mac_macbook_2006_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-2006"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_2006_class_init,
};

static void mac_macbook_2006_register(void)
{
    type_register_static(&mac_macbook_2006_machine_info);
}
type_init(mac_macbook_2006_register);

/* ===========================================================================
 * Apple MacBook 13-inch (Core 2 Duo Santa Rosa, 2007)
 * =========================================================================== */
static void mac_macbook_2007_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook 13-inch (Core 2 Duo Santa Rosa, 2007)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-t7300");
    m->max_cpus = 2;
    m->default_ram_size = 1024 * MiB;
}

static void mac_macbook_2007_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBook3,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_2007_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_2007_machine_options(mc);
    mc->init = mac_macbook_2007_machine_init;
}

static const TypeInfo mac_macbook_2007_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-2007"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_2007_class_init,
};

static void mac_macbook_2007_register(void)
{
    type_register_static(&mac_macbook_2007_machine_info);
}
type_init(mac_macbook_2007_register);

/* ===========================================================================
 * Apple MacBook 13-inch (Core 2 Duo Unibody, 2009)
 * =========================================================================== */
static void mac_macbook_2009_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook 13-inch (Core 2 Duo Unibody, 2009)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-p7550");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_macbook_2009_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBook5,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_2009_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_2009_machine_options(mc);
    mc->init = mac_macbook_2009_machine_init;
}

static const TypeInfo mac_macbook_2009_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-2009"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_2009_class_init,
};

static void mac_macbook_2009_register(void)
{
    type_register_static(&mac_macbook_2009_machine_info);
}
type_init(mac_macbook_2009_register);

/* ===========================================================================
 * Apple MacBook 13-inch (Core 2 Duo, 2010)
 * =========================================================================== */
static void mac_macbook_2010_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook 13-inch (Core 2 Duo, 2010)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-p8600");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_macbook_2010_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBook6,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_2010_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_2010_machine_options(mc);
    mc->init = mac_macbook_2010_machine_init;
}

static const TypeInfo mac_macbook_2010_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-2010"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_2010_class_init,
};

static void mac_macbook_2010_register(void)
{
    type_register_static(&mac_macbook_2010_machine_info);
}
type_init(mac_macbook_2010_register);

/* ===========================================================================
 * Apple MacBook 12-inch (Core M Broadwell, 2015)
 * =========================================================================== */
static void mac_macbook_12_2015_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook 12-inch (Core M Broadwell, 2015)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Broadwell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_12_2015_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBook8,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_12_2015_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_12_2015_machine_options(mc);
    mc->init = mac_macbook_12_2015_machine_init;
}

static const TypeInfo mac_macbook_12_2015_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-12-2015"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_12_2015_class_init,
};

static void mac_macbook_12_2015_register(void)
{
    type_register_static(&mac_macbook_12_2015_machine_info);
}
type_init(mac_macbook_12_2015_register);

/* ===========================================================================
 * Apple MacBook 12-inch (Core m3 Skylake, 2016)
 * =========================================================================== */
static void mac_macbook_12_2016_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook 12-inch (Core m3 Skylake, 2016)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Skylake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_12_2016_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBook9,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_12_2016_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_12_2016_machine_options(mc);
    mc->init = mac_macbook_12_2016_machine_init;
}

static const TypeInfo mac_macbook_12_2016_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-12-2016"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_12_2016_class_init,
};

static void mac_macbook_12_2016_register(void)
{
    type_register_static(&mac_macbook_12_2016_machine_info);
}
type_init(mac_macbook_12_2016_register);

/* ===========================================================================
 * Apple MacBook 12-inch (Core m3 Kaby Lake, 2017)
 * =========================================================================== */
static void mac_macbook_12_2017_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook 12-inch (Core m3 Kaby Lake, 2017)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("KabyLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_12_2017_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBook10,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_12_2017_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_12_2017_machine_options(mc);
    mc->init = mac_macbook_12_2017_machine_init;
}

static const TypeInfo mac_macbook_12_2017_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-12-2017"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_12_2017_class_init,
};

static void mac_macbook_12_2017_register(void)
{
    type_register_static(&mac_macbook_12_2017_machine_info);
}
type_init(mac_macbook_12_2017_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core 2 Duo, 2008)
 * =========================================================================== */
static void mac_macbook_air_2008_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core 2 Duo, 2008)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-sl9400");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_macbook_air_2008_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir1,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2008_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2008_machine_options(mc);
    mc->init = mac_macbook_air_2008_machine_init;
}

static const TypeInfo mac_macbook_air_2008_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2008"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2008_class_init,
};

static void mac_macbook_air_2008_register(void)
{
    type_register_static(&mac_macbook_air_2008_machine_info);
}
type_init(mac_macbook_air_2008_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core 2 Duo, 2010)
 * =========================================================================== */
static void mac_macbook_air_2010_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core 2 Duo, 2010)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-sl9600");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_macbook_air_2010_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir3,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2010_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2010_machine_options(mc);
    mc->init = mac_macbook_air_2010_machine_init;
}

static const TypeInfo mac_macbook_air_2010_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2010"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2010_class_init,
};

static void mac_macbook_air_2010_register(void)
{
    type_register_static(&mac_macbook_air_2010_machine_info);
}
type_init(mac_macbook_air_2010_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i5 Sandy Bridge, 2011)
 * =========================================================================== */
static void mac_macbook_air_2011_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i5 Sandy Bridge, 2011)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("SandyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_air_2011_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir4,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2011_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2011_machine_options(mc);
    mc->init = mac_macbook_air_2011_machine_init;
}

static const TypeInfo mac_macbook_air_2011_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2011"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2011_class_init,
};

static void mac_macbook_air_2011_register(void)
{
    type_register_static(&mac_macbook_air_2011_machine_info);
}
type_init(mac_macbook_air_2011_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i5 Ivy Bridge, 2012)
 * =========================================================================== */
static void mac_macbook_air_2012_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i5 Ivy Bridge, 2012)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IvyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_air_2012_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir5,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2012_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2012_machine_options(mc);
    mc->init = mac_macbook_air_2012_machine_init;
}

static const TypeInfo mac_macbook_air_2012_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2012"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2012_class_init,
};

static void mac_macbook_air_2012_register(void)
{
    type_register_static(&mac_macbook_air_2012_machine_info);
}
type_init(mac_macbook_air_2012_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i5 Haswell, 2013)
 * =========================================================================== */
static void mac_macbook_air_2013_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i5 Haswell, 2013)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Haswell");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_air_2013_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir6,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2013_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2013_machine_options(mc);
    mc->init = mac_macbook_air_2013_machine_init;
}

static const TypeInfo mac_macbook_air_2013_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2013"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2013_class_init,
};

static void mac_macbook_air_2013_register(void)
{
    type_register_static(&mac_macbook_air_2013_machine_info);
}
type_init(mac_macbook_air_2013_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i5 Broadwell, 2015)
 * =========================================================================== */
static void mac_macbook_air_2015_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i5 Broadwell, 2015)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Broadwell");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_air_2015_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir7,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2015_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2015_machine_options(mc);
    mc->init = mac_macbook_air_2015_machine_init;
}

static const TypeInfo mac_macbook_air_2015_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2015"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2015_class_init,
};

static void mac_macbook_air_2015_register(void)
{
    type_register_static(&mac_macbook_air_2015_machine_info);
}
type_init(mac_macbook_air_2015_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i5 Broadwell, 2017)
 * =========================================================================== */
static void mac_macbook_air_2017_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i5 Broadwell, 2017)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Broadwell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_air_2017_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir7,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2017_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2017_machine_options(mc);
    mc->init = mac_macbook_air_2017_machine_init;
}

static const TypeInfo mac_macbook_air_2017_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2017"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2017_class_init,
};

static void mac_macbook_air_2017_register(void)
{
    type_register_static(&mac_macbook_air_2017_machine_info);
}
type_init(mac_macbook_air_2017_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i5 Amber Lake, T2, 2018)
 * =========================================================================== */
static void mac_macbook_air_2018_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i5 Amber Lake, T2, 2018)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Skylake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_air_2018_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir8,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2018_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2018_machine_options(mc);
    mc->init = mac_macbook_air_2018_machine_init;
}

static const TypeInfo mac_macbook_air_2018_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2018"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2018_class_init,
};

static void mac_macbook_air_2018_register(void)
{
    type_register_static(&mac_macbook_air_2018_machine_info);
}
type_init(mac_macbook_air_2018_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i5 Amber Lake, T2, 2019)
 * =========================================================================== */
static void mac_macbook_air_2019_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i5 Amber Lake, T2, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Skylake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_air_2019_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir8,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2019_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2019_machine_options(mc);
    mc->init = mac_macbook_air_2019_machine_init;
}

static const TypeInfo mac_macbook_air_2019_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2019"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2019_class_init,
};

static void mac_macbook_air_2019_register(void)
{
    type_register_static(&mac_macbook_air_2019_machine_info);
}
type_init(mac_macbook_air_2019_register);

/* ===========================================================================
 * Apple MacBook Air 13-inch (Core i3 Ice Lake, T2, 2020)
 * =========================================================================== */
static void mac_macbook_air_2020_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Air 13-inch (Core i3 Ice Lake, T2, 2020)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IceLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_air_2020_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookAir9,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_air_2020_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_air_2020_machine_options(mc);
    mc->init = mac_macbook_air_2020_machine_init;
}

static const TypeInfo mac_macbook_air_2020_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-air-2020"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_air_2020_class_init,
};

static void mac_macbook_air_2020_register(void)
{
    type_register_static(&mac_macbook_air_2020_machine_info);
}
type_init(mac_macbook_air_2020_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch (Core Duo, 2006)
 * =========================================================================== */
static void mac_macbook_pro_15_2006_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch (Core Duo, 2006)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("coreduo-t2600");
    m->max_cpus = 2;
    m->default_ram_size = 1024 * MiB;
}

static void mac_macbook_pro_15_2006_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro1,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2006_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2006_machine_options(mc);
    mc->init = mac_macbook_pro_15_2006_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2006_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2006"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2006_class_init,
};

static void mac_macbook_pro_15_2006_register(void)
{
    type_register_static(&mac_macbook_pro_15_2006_machine_info);
}
type_init(mac_macbook_pro_15_2006_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch (Core 2 Duo, 2007)
 * =========================================================================== */
static void mac_macbook_pro_15_2007_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch (Core 2 Duo, 2007)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-t7700");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_macbook_pro_15_2007_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro3,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2007_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2007_machine_options(mc);
    mc->init = mac_macbook_pro_15_2007_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2007_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2007"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2007_class_init,
};

static void mac_macbook_pro_15_2007_register(void)
{
    type_register_static(&mac_macbook_pro_15_2007_machine_info);
}
type_init(mac_macbook_pro_15_2007_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch (Core 2 Duo Penryn, 2008)
 * =========================================================================== */
static void mac_macbook_pro_15_2008_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch (Core 2 Duo Penryn, 2008)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-p8600");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_macbook_pro_15_2008_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro4,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2008_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2008_machine_options(mc);
    mc->init = mac_macbook_pro_15_2008_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2008_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2008"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2008_class_init,
};

static void mac_macbook_pro_15_2008_register(void)
{
    type_register_static(&mac_macbook_pro_15_2008_machine_info);
}
type_init(mac_macbook_pro_15_2008_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch (Core 2 Duo, 2009)
 * =========================================================================== */
static void mac_macbook_pro_13_2009_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch (Core 2 Duo, 2009)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-p8700");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_macbook_pro_13_2009_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro5,5");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2009_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2009_machine_options(mc);
    mc->init = mac_macbook_pro_13_2009_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2009_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2009"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2009_class_init,
};

static void mac_macbook_pro_13_2009_register(void)
{
    type_register_static(&mac_macbook_pro_13_2009_machine_info);
}
type_init(mac_macbook_pro_13_2009_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch (Core 2 Duo, 2009)
 * =========================================================================== */
static void mac_macbook_pro_15_2009_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch (Core 2 Duo, 2009)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-p8800");
    m->max_cpus = 2;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_pro_15_2009_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro5,3");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2009_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2009_machine_options(mc);
    mc->init = mac_macbook_pro_15_2009_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2009_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2009"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2009_class_init,
};

static void mac_macbook_pro_15_2009_register(void)
{
    type_register_static(&mac_macbook_pro_15_2009_machine_info);
}
type_init(mac_macbook_pro_15_2009_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch (Core i5 Arrandale, 2010)
 * =========================================================================== */
static void mac_macbook_pro_15_2010_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch (Core i5 Arrandale, 2010)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Westmere");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_pro_15_2010_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro6,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2010_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2010_machine_options(mc);
    mc->init = mac_macbook_pro_15_2010_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2010_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2010"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2010_class_init,
};

static void mac_macbook_pro_15_2010_register(void)
{
    type_register_static(&mac_macbook_pro_15_2010_machine_info);
}
type_init(mac_macbook_pro_15_2010_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch (Core i5 Sandy Bridge, 2011)
 * =========================================================================== */
static void mac_macbook_pro_13_2011_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch (Core i5 Sandy Bridge, 2011)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("SandyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_pro_13_2011_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro8,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2011_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2011_machine_options(mc);
    mc->init = mac_macbook_pro_13_2011_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2011_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2011"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2011_class_init,
};

static void mac_macbook_pro_13_2011_register(void)
{
    type_register_static(&mac_macbook_pro_13_2011_machine_info);
}
type_init(mac_macbook_pro_13_2011_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch (Core i7 Sandy Bridge, 2011)
 * =========================================================================== */
static void mac_macbook_pro_15_2011_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch (Core i7 Sandy Bridge, 2011)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("SandyBridge");
    m->max_cpus = 8;
    m->default_ram_size = 4096 * MiB;
}

static void mac_macbook_pro_15_2011_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro8,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2011_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2011_machine_options(mc);
    mc->init = mac_macbook_pro_15_2011_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2011_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2011"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2011_class_init,
};

static void mac_macbook_pro_15_2011_register(void)
{
    type_register_static(&mac_macbook_pro_15_2011_machine_info);
}
type_init(mac_macbook_pro_15_2011_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch Retina (Core i5 Ivy Bridge, 2012)
 * =========================================================================== */
static void mac_macbook_pro_13_2012_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch Retina (Core i5 Ivy Bridge, 2012)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IvyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2012_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro10,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2012_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2012_machine_options(mc);
    mc->init = mac_macbook_pro_13_2012_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2012_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2012"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2012_class_init,
};

static void mac_macbook_pro_13_2012_register(void)
{
    type_register_static(&mac_macbook_pro_13_2012_machine_info);
}
type_init(mac_macbook_pro_13_2012_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch Retina (Core i7 Ivy Bridge, 2012)
 * =========================================================================== */
static void mac_macbook_pro_15_2012_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch Retina (Core i7 Ivy Bridge, 2012)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IvyBridge");
    m->max_cpus = 8;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_15_2012_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro10,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2012_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2012_machine_options(mc);
    mc->init = mac_macbook_pro_15_2012_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2012_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2012"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2012_class_init,
};

static void mac_macbook_pro_15_2012_register(void)
{
    type_register_static(&mac_macbook_pro_15_2012_machine_info);
}
type_init(mac_macbook_pro_15_2012_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch Retina (Core i5 Haswell, 2014)
 * =========================================================================== */
static void mac_macbook_pro_13_2014_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch Retina (Core i5 Haswell, 2014)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Haswell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2014_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro11,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2014_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2014_machine_options(mc);
    mc->init = mac_macbook_pro_13_2014_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2014_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2014"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2014_class_init,
};

static void mac_macbook_pro_13_2014_register(void)
{
    type_register_static(&mac_macbook_pro_13_2014_machine_info);
}
type_init(mac_macbook_pro_13_2014_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch Retina (Core i7 Haswell, 2014)
 * =========================================================================== */
static void mac_macbook_pro_15_2014_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch Retina (Core i7 Haswell, 2014)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Haswell");
    m->max_cpus = 8;
    m->default_ram_size = 16384 * MiB;
}

static void mac_macbook_pro_15_2014_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro11,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2014_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2014_machine_options(mc);
    mc->init = mac_macbook_pro_15_2014_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2014_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2014"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2014_class_init,
};

static void mac_macbook_pro_15_2014_register(void)
{
    type_register_static(&mac_macbook_pro_15_2014_machine_info);
}
type_init(mac_macbook_pro_15_2014_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch Retina (Core i5 Broadwell, 2015)
 * =========================================================================== */
static void mac_macbook_pro_13_2015_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch Retina (Core i5 Broadwell, 2015)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Broadwell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2015_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro12,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2015_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2015_machine_options(mc);
    mc->init = mac_macbook_pro_13_2015_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2015_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2015"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2015_class_init,
};

static void mac_macbook_pro_13_2015_register(void)
{
    type_register_static(&mac_macbook_pro_13_2015_machine_info);
}
type_init(mac_macbook_pro_13_2015_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch Retina (Core i7 CrystalWell, 2015)
 * =========================================================================== */
static void mac_macbook_pro_15_2015_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch Retina (Core i7 CrystalWell, 2015)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Haswell");
    m->max_cpus = 8;
    m->default_ram_size = 16384 * MiB;
}

static void mac_macbook_pro_15_2015_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro11,4");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2015_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2015_machine_options(mc);
    mc->init = mac_macbook_pro_15_2015_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2015_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2015"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2015_class_init,
};

static void mac_macbook_pro_15_2015_register(void)
{
    type_register_static(&mac_macbook_pro_15_2015_machine_info);
}
type_init(mac_macbook_pro_15_2015_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch Touch Bar (Core i5 Skylake, 2016)
 * =========================================================================== */
static void mac_macbook_pro_13_2016_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch Touch Bar (Core i5 Skylake, 2016)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Skylake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2016_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro13,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2016_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2016_machine_options(mc);
    mc->init = mac_macbook_pro_13_2016_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2016_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2016"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2016_class_init,
};

static void mac_macbook_pro_13_2016_register(void)
{
    type_register_static(&mac_macbook_pro_13_2016_machine_info);
}
type_init(mac_macbook_pro_13_2016_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch Touch Bar (Core i7 Skylake, 2016)
 * =========================================================================== */
static void mac_macbook_pro_15_2016_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch Touch Bar (Core i7 Skylake, 2016)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Skylake");
    m->max_cpus = 8;
    m->default_ram_size = 16384 * MiB;
}

static void mac_macbook_pro_15_2016_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro13,3");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2016_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2016_machine_options(mc);
    mc->init = mac_macbook_pro_15_2016_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2016_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2016"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2016_class_init,
};

static void mac_macbook_pro_15_2016_register(void)
{
    type_register_static(&mac_macbook_pro_15_2016_machine_info);
}
type_init(mac_macbook_pro_15_2016_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch Touch Bar (Core i5 Kaby Lake, 2017)
 * =========================================================================== */
static void mac_macbook_pro_13_2017_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch Touch Bar (Core i5 Kaby Lake, 2017)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("KabyLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2017_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro14,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2017_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2017_machine_options(mc);
    mc->init = mac_macbook_pro_13_2017_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2017_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2017"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2017_class_init,
};

static void mac_macbook_pro_13_2017_register(void)
{
    type_register_static(&mac_macbook_pro_13_2017_machine_info);
}
type_init(mac_macbook_pro_13_2017_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch Touch Bar (Core i7 Kaby Lake, 2017)
 * =========================================================================== */
static void mac_macbook_pro_15_2017_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch Touch Bar (Core i7 Kaby Lake, 2017)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("KabyLake");
    m->max_cpus = 8;
    m->default_ram_size = 16384 * MiB;
}

static void mac_macbook_pro_15_2017_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro14,3");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2017_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2017_machine_options(mc);
    mc->init = mac_macbook_pro_15_2017_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2017_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2017"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2017_class_init,
};

static void mac_macbook_pro_15_2017_register(void)
{
    type_register_static(&mac_macbook_pro_15_2017_machine_info);
}
type_init(mac_macbook_pro_15_2017_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch Touch Bar (Core i7 Coffee Lake, T2, 2018)
 * =========================================================================== */
static void mac_macbook_pro_15_2018_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch Touch Bar (Core i7 Coffee Lake, T2, 2018)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 8;
    m->default_ram_size = 16384 * MiB;
}

static void mac_macbook_pro_15_2018_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro15,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2018_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2018_machine_options(mc);
    mc->init = mac_macbook_pro_15_2018_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2018_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2018"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2018_class_init,
};

static void mac_macbook_pro_15_2018_register(void)
{
    type_register_static(&mac_macbook_pro_15_2018_machine_info);
}
type_init(mac_macbook_pro_15_2018_register);

/* ===========================================================================
 * Apple MacBook Pro 15-inch Touch Bar (Core i7 Coffee Lake, T2, 2019)
 * =========================================================================== */
static void mac_macbook_pro_15_2019_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 15-inch Touch Bar (Core i7 Coffee Lake, T2, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 8;
    m->default_ram_size = 16384 * MiB;
}

static void mac_macbook_pro_15_2019_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro15,3");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_15_2019_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_15_2019_machine_options(mc);
    mc->init = mac_macbook_pro_15_2019_machine_init;
}

static const TypeInfo mac_macbook_pro_15_2019_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-15-2019"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_15_2019_class_init,
};

static void mac_macbook_pro_15_2019_register(void)
{
    type_register_static(&mac_macbook_pro_15_2019_machine_info);
}
type_init(mac_macbook_pro_15_2019_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch Touch Bar (Core i5 Coffee Lake, T2, 2019)
 * =========================================================================== */
static void mac_macbook_pro_13_2019_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch Touch Bar (Core i5 Coffee Lake, T2, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2019_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro15,4");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2019_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2019_machine_options(mc);
    mc->init = mac_macbook_pro_13_2019_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2019_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2019"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2019_class_init,
};

static void mac_macbook_pro_13_2019_register(void)
{
    type_register_static(&mac_macbook_pro_13_2019_machine_info);
}
type_init(mac_macbook_pro_13_2019_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch 2TB (Core i5 Coffee Lake, T2, 2019)
 * =========================================================================== */
static void mac_macbook_pro_13_2019_2tb_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch 2TB (Core i5 Coffee Lake, T2, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2019_2tb_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro15,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2019_2tb_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2019_2tb_machine_options(mc);
    mc->init = mac_macbook_pro_13_2019_2tb_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2019_2tb_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2019-2tb"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2019_2tb_class_init,
};

static void mac_macbook_pro_13_2019_2tb_register(void)
{
    type_register_static(&mac_macbook_pro_13_2019_2tb_machine_info);
}
type_init(mac_macbook_pro_13_2019_2tb_register);

/* ===========================================================================
 * Apple MacBook Pro 16-inch (Core i7 Coffee Lake, T2, 2019)
 * =========================================================================== */
static void mac_macbook_pro_16_2019_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 16-inch (Core i7 Coffee Lake, T2, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 8;
    m->default_ram_size = 16384 * MiB;
}

static void mac_macbook_pro_16_2019_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro16,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_16_2019_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_16_2019_machine_options(mc);
    mc->init = mac_macbook_pro_16_2019_machine_init;
}

static const TypeInfo mac_macbook_pro_16_2019_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-16-2019"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_16_2019_class_init,
};

static void mac_macbook_pro_16_2019_register(void)
{
    type_register_static(&mac_macbook_pro_16_2019_machine_info);
}
type_init(mac_macbook_pro_16_2019_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch Touch Bar (Core i5 Ice Lake, T2, 2020)
 * =========================================================================== */
static void mac_macbook_pro_13_2020_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch Touch Bar (Core i5 Ice Lake, T2, 2020)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IceLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2020_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro16,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2020_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2020_machine_options(mc);
    mc->init = mac_macbook_pro_13_2020_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2020_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2020"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2020_class_init,
};

static void mac_macbook_pro_13_2020_register(void)
{
    type_register_static(&mac_macbook_pro_13_2020_machine_info);
}
type_init(mac_macbook_pro_13_2020_register);

/* ===========================================================================
 * Apple MacBook Pro 13-inch 2TB (Core i5 Coffee Lake, T2, 2020)
 * =========================================================================== */
static void mac_macbook_pro_13_2020_2tb_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple MacBook Pro 13-inch 2TB (Core i5 Coffee Lake, T2, 2020)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_macbook_pro_13_2020_2tb_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacBookPro16,3");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = true;
    pc_q35_init(machine);
}

static void mac_macbook_pro_13_2020_2tb_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_macbook_pro_13_2020_2tb_machine_options(mc);
    mc->init = mac_macbook_pro_13_2020_2tb_machine_init;
}

static const TypeInfo mac_macbook_pro_13_2020_2tb_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-macbook-pro-13-2020-2tb"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_macbook_pro_13_2020_2tb_class_init,
};

static void mac_macbook_pro_13_2020_2tb_register(void)
{
    type_register_static(&mac_macbook_pro_13_2020_2tb_machine_info);
}
type_init(mac_macbook_pro_13_2020_2tb_register);

/* ===========================================================================
 * Apple iMac 20-inch (Core 2 Duo, 2007)
 * =========================================================================== */
static void mac_imac_2007_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 20-inch (Core 2 Duo, 2007)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-t7300");
    m->max_cpus = 2;
    m->default_ram_size = 1024 * MiB;
}

static void mac_imac_2007_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac7,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2007_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2007_machine_options(mc);
    mc->init = mac_imac_2007_machine_init;
}

static const TypeInfo mac_imac_2007_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2007"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2007_class_init,
};

static void mac_imac_2007_register(void)
{
    type_register_static(&mac_imac_2007_machine_info);
}
type_init(mac_imac_2007_register);

/* ===========================================================================
 * Apple iMac 24-inch (Core 2 Duo Penryn, 2008)
 * =========================================================================== */
static void mac_imac_2008_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 24-inch (Core 2 Duo Penryn, 2008)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-e8435");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_imac_2008_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac8,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2008_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2008_machine_options(mc);
    mc->init = mac_imac_2008_machine_init;
}

static const TypeInfo mac_imac_2008_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2008"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2008_class_init,
};

static void mac_imac_2008_register(void)
{
    type_register_static(&mac_imac_2008_machine_info);
}
type_init(mac_imac_2008_register);

/* ===========================================================================
 * Apple iMac 24-inch (Core 2 Duo, 2009)
 * =========================================================================== */
static void mac_imac_2009_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 24-inch (Core 2 Duo, 2009)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-e8135");
    m->max_cpus = 2;
    m->default_ram_size = 4096 * MiB;
}

static void mac_imac_2009_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac9,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2009_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2009_machine_options(mc);
    mc->init = mac_imac_2009_machine_init;
}

static const TypeInfo mac_imac_2009_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2009"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2009_class_init,
};

static void mac_imac_2009_register(void)
{
    type_register_static(&mac_imac_2009_machine_info);
}
type_init(mac_imac_2009_register);

/* ===========================================================================
 * Apple iMac 21.5-inch (Core i3 Clarkdale, 2010)
 * =========================================================================== */
static void mac_imac_2010_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 21.5-inch (Core i3 Clarkdale, 2010)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Westmere");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_imac_2010_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac11,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2010_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2010_machine_options(mc);
    mc->init = mac_imac_2010_machine_init;
}

static const TypeInfo mac_imac_2010_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2010"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2010_class_init,
};

static void mac_imac_2010_register(void)
{
    type_register_static(&mac_imac_2010_machine_info);
}
type_init(mac_imac_2010_register);

/* ===========================================================================
 * Apple iMac 27-inch (Core i5 Sandy Bridge, 2011)
 * =========================================================================== */
static void mac_imac_2011_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 27-inch (Core i5 Sandy Bridge, 2011)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("SandyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_imac_2011_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac12,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2011_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2011_machine_options(mc);
    mc->init = mac_imac_2011_machine_init;
}

static const TypeInfo mac_imac_2011_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2011"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2011_class_init,
};

static void mac_imac_2011_register(void)
{
    type_register_static(&mac_imac_2011_machine_info);
}
type_init(mac_imac_2011_register);

/* ===========================================================================
 * Apple iMac 21.5-inch (Core i5 Ivy Bridge, 2012)
 * =========================================================================== */
static void mac_imac_2012_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 21.5-inch (Core i5 Ivy Bridge, 2012)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IvyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_imac_2012_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac13,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2012_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2012_machine_options(mc);
    mc->init = mac_imac_2012_machine_init;
}

static const TypeInfo mac_imac_2012_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2012"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2012_class_init,
};

static void mac_imac_2012_register(void)
{
    type_register_static(&mac_imac_2012_machine_info);
}
type_init(mac_imac_2012_register);

/* ===========================================================================
 * Apple iMac 27-inch (Core i5 Haswell, 2013)
 * =========================================================================== */
static void mac_imac_2013_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 27-inch (Core i5 Haswell, 2013)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Haswell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_imac_2013_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac14,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2013_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2013_machine_options(mc);
    mc->init = mac_imac_2013_machine_init;
}

static const TypeInfo mac_imac_2013_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2013"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2013_class_init,
};

static void mac_imac_2013_register(void)
{
    type_register_static(&mac_imac_2013_machine_info);
}
type_init(mac_imac_2013_register);

/* ===========================================================================
 * Apple iMac 27-inch 5K (Core i5 Haswell, 2014)
 * =========================================================================== */
static void mac_imac_2014_5k_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 27-inch 5K (Core i5 Haswell, 2014)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Haswell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_imac_2014_5k_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac15,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2014_5k_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2014_5k_machine_options(mc);
    mc->init = mac_imac_2014_5k_machine_init;
}

static const TypeInfo mac_imac_2014_5k_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2014-5k"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2014_5k_class_init,
};

static void mac_imac_2014_5k_register(void)
{
    type_register_static(&mac_imac_2014_5k_machine_info);
}
type_init(mac_imac_2014_5k_register);

/* ===========================================================================
 * Apple iMac 27-inch 5K (Core i5 Broadwell, 2015)
 * =========================================================================== */
static void mac_imac_2015_5k_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 27-inch 5K (Core i5 Broadwell, 2015)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Broadwell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_imac_2015_5k_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac17,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2015_5k_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2015_5k_machine_options(mc);
    mc->init = mac_imac_2015_5k_machine_init;
}

static const TypeInfo mac_imac_2015_5k_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2015-5k"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2015_5k_class_init,
};

static void mac_imac_2015_5k_register(void)
{
    type_register_static(&mac_imac_2015_5k_machine_info);
}
type_init(mac_imac_2015_5k_register);

/* ===========================================================================
 * Apple iMac 21.5-inch (Core i5 Kaby Lake, 2017)
 * =========================================================================== */
static void mac_imac_2017_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 21.5-inch (Core i5 Kaby Lake, 2017)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("KabyLake");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_imac_2017_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac18,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2017_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2017_machine_options(mc);
    mc->init = mac_imac_2017_machine_init;
}

static const TypeInfo mac_imac_2017_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2017"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2017_class_init,
};

static void mac_imac_2017_register(void)
{
    type_register_static(&mac_imac_2017_machine_info);
}
type_init(mac_imac_2017_register);

/* ===========================================================================
 * Apple iMac 27-inch 5K (Core i5 Kaby Lake, 2017)
 * =========================================================================== */
static void mac_imac_2017_5k_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 27-inch 5K (Core i5 Kaby Lake, 2017)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("KabyLake");
    m->max_cpus = 4;
    m->default_ram_size = 16384 * MiB;
}

static void mac_imac_2017_5k_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac18,3");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2017_5k_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2017_5k_machine_options(mc);
    mc->init = mac_imac_2017_5k_machine_init;
}

static const TypeInfo mac_imac_2017_5k_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2017-5k"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2017_5k_class_init,
};

static void mac_imac_2017_5k_register(void)
{
    type_register_static(&mac_imac_2017_5k_machine_info);
}
type_init(mac_imac_2017_5k_register);

/* ===========================================================================
 * Apple iMac 21.5-inch (Core i5 Coffee Lake, 2019)
 * =========================================================================== */
static void mac_imac_2019_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 21.5-inch (Core i5 Coffee Lake, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 6;
    m->default_ram_size = 8192 * MiB;
}

static void mac_imac_2019_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac19,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2019_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2019_machine_options(mc);
    mc->init = mac_imac_2019_machine_init;
}

static const TypeInfo mac_imac_2019_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2019"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2019_class_init,
};

static void mac_imac_2019_register(void)
{
    type_register_static(&mac_imac_2019_machine_info);
}
type_init(mac_imac_2019_register);

/* ===========================================================================
 * Apple iMac 27-inch 5K (Core i5 Coffee Lake, 2019)
 * =========================================================================== */
static void mac_imac_2019_5k_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 27-inch 5K (Core i5 Coffee Lake, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 6;
    m->default_ram_size = 16384 * MiB;
}

static void mac_imac_2019_5k_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac19,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2019_5k_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2019_5k_machine_options(mc);
    mc->init = mac_imac_2019_5k_machine_init;
}

static const TypeInfo mac_imac_2019_5k_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2019-5k"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2019_5k_class_init,
};

static void mac_imac_2019_5k_register(void)
{
    type_register_static(&mac_imac_2019_5k_machine_info);
}
type_init(mac_imac_2019_5k_register);

/* ===========================================================================
 * Apple iMac 27-inch 5K (Core i5 Comet Lake, 2020)
 * =========================================================================== */
static void mac_imac_2020_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac 27-inch 5K (Core i5 Comet Lake, 2020)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CometLake");
    m->max_cpus = 6;
    m->default_ram_size = 16384 * MiB;
}

static void mac_imac_2020_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMac20,2");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_2020_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_2020_machine_options(mc);
    mc->init = mac_imac_2020_machine_init;
}

static const TypeInfo mac_imac_2020_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-2020"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_2020_class_init,
};

static void mac_imac_2020_register(void)
{
    type_register_static(&mac_imac_2020_machine_info);
}
type_init(mac_imac_2020_register);

/* ===========================================================================
 * Apple iMac Pro 27-inch 5K (Xeon W Skylake-W, T2, 2017)
 * =========================================================================== */
static void mac_imac_pro_2017_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple iMac Pro 27-inch 5K (Xeon W Skylake-W, T2, 2017)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Skylake");
    m->max_cpus = 16;
    m->default_ram_size = 32768 * MiB;
}

static void mac_imac_pro_2017_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("iMacPro1,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_imac_pro_2017_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_imac_pro_2017_machine_options(mc);
    mc->init = mac_imac_pro_2017_machine_init;
}

static const TypeInfo mac_imac_pro_2017_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-imac-pro-2017"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_imac_pro_2017_class_init,
};

static void mac_imac_pro_2017_register(void)
{
    type_register_static(&mac_imac_pro_2017_machine_info);
}
type_init(mac_imac_pro_2017_register);

/* ===========================================================================
 * Apple Mac Pro (Xeon Harpertown, 2008)
 * =========================================================================== */
static void mac_mac_pro_2008_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac Pro (Xeon Harpertown, 2008)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Penryn");
    m->max_cpus = 8;
    m->default_ram_size = 8192 * MiB;
}

static void mac_mac_pro_2008_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacPro3,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_pro_2008_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_pro_2008_machine_options(mc);
    mc->init = mac_mac_pro_2008_machine_init;
}

static const TypeInfo mac_mac_pro_2008_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-pro-2008"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_pro_2008_class_init,
};

static void mac_mac_pro_2008_register(void)
{
    type_register_static(&mac_mac_pro_2008_machine_info);
}
type_init(mac_mac_pro_2008_register);

/* ===========================================================================
 * Apple Mac Pro (Xeon Nehalem, 2009)
 * =========================================================================== */
static void mac_mac_pro_2009_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac Pro (Xeon Nehalem, 2009)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Nehalem");
    m->max_cpus = 8;
    m->default_ram_size = 8192 * MiB;
}

static void mac_mac_pro_2009_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacPro4,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_pro_2009_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_pro_2009_machine_options(mc);
    mc->init = mac_mac_pro_2009_machine_init;
}

static const TypeInfo mac_mac_pro_2009_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-pro-2009"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_pro_2009_class_init,
};

static void mac_mac_pro_2009_register(void)
{
    type_register_static(&mac_mac_pro_2009_machine_info);
}
type_init(mac_mac_pro_2009_register);

/* ===========================================================================
 * Apple Mac Pro (Xeon Westmere, 2010)
 * =========================================================================== */
static void mac_mac_pro_2010_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac Pro (Xeon Westmere, 2010)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Westmere");
    m->max_cpus = 12;
    m->default_ram_size = 8192 * MiB;
}

static void mac_mac_pro_2010_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacPro5,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_pro_2010_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_pro_2010_machine_options(mc);
    mc->init = mac_mac_pro_2010_machine_init;
}

static const TypeInfo mac_mac_pro_2010_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-pro-2010"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_pro_2010_class_init,
};

static void mac_mac_pro_2010_register(void)
{
    type_register_static(&mac_mac_pro_2010_machine_info);
}
type_init(mac_mac_pro_2010_register);

/* ===========================================================================
 * Apple Mac Pro Cylinder (Xeon E5 Ivy Bridge-EP, 2013)
 * =========================================================================== */
static void mac_mac_pro_2013_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac Pro Cylinder (Xeon E5 Ivy Bridge-EP, 2013)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IvyBridge");
    m->max_cpus = 12;
    m->default_ram_size = 16384 * MiB;
}

static void mac_mac_pro_2013_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacPro6,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_pro_2013_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_pro_2013_machine_options(mc);
    mc->init = mac_mac_pro_2013_machine_init;
}

static const TypeInfo mac_mac_pro_2013_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-pro-2013"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_pro_2013_class_init,
};

static void mac_mac_pro_2013_register(void)
{
    type_register_static(&mac_mac_pro_2013_machine_info);
}
type_init(mac_mac_pro_2013_register);

/* ===========================================================================
 * Apple Mac Pro Tower (Xeon W Cascade Lake-W, T2, 2019)
 * =========================================================================== */
static void mac_mac_pro_2019_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac Pro Tower (Xeon W Cascade Lake-W, T2, 2019)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Cascadelake");
    m->max_cpus = 28;
    m->default_ram_size = 32768 * MiB;
}

static void mac_mac_pro_2019_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacPro7,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_pro_2019_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_pro_2019_machine_options(mc);
    mc->init = mac_mac_pro_2019_machine_init;
}

static const TypeInfo mac_mac_pro_2019_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-pro-2019"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_pro_2019_class_init,
};

static void mac_mac_pro_2019_register(void)
{
    type_register_static(&mac_mac_pro_2019_machine_info);
}
type_init(mac_mac_pro_2019_register);

/* ===========================================================================
 * Apple Mac mini (Core 2 Duo, 2007)
 * =========================================================================== */
static void mac_mac_mini_2007_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac mini (Core 2 Duo, 2007)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-t7200");
    m->max_cpus = 2;
    m->default_ram_size = 1024 * MiB;
}

static void mac_mac_mini_2007_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacMini2,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_mini_2007_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_mini_2007_machine_options(mc);
    mc->init = mac_mac_mini_2007_machine_init;
}

static const TypeInfo mac_mac_mini_2007_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-mini-2007"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_mini_2007_class_init,
};

static void mac_mac_mini_2007_register(void)
{
    type_register_static(&mac_mac_mini_2007_machine_info);
}
type_init(mac_mac_mini_2007_register);

/* ===========================================================================
 * Apple Mac mini (Core 2 Duo, 2009)
 * =========================================================================== */
static void mac_mac_mini_2009_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac mini (Core 2 Duo, 2009)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-p7550");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_mac_mini_2009_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacMini3,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_mini_2009_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_mini_2009_machine_options(mc);
    mc->init = mac_mac_mini_2009_machine_init;
}

static const TypeInfo mac_mac_mini_2009_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-mini-2009"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_mini_2009_class_init,
};

static void mac_mac_mini_2009_register(void)
{
    type_register_static(&mac_mac_mini_2009_machine_info);
}
type_init(mac_mac_mini_2009_register);

/* ===========================================================================
 * Apple Mac mini (Core 2 Duo, 2010)
 * =========================================================================== */
static void mac_mac_mini_2010_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac mini (Core 2 Duo, 2010)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("core2duo-p8600");
    m->max_cpus = 2;
    m->default_ram_size = 2048 * MiB;
}

static void mac_mac_mini_2010_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacMini4,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_mini_2010_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_mini_2010_machine_options(mc);
    mc->init = mac_mac_mini_2010_machine_init;
}

static const TypeInfo mac_mac_mini_2010_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-mini-2010"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_mini_2010_class_init,
};

static void mac_mac_mini_2010_register(void)
{
    type_register_static(&mac_mac_mini_2010_machine_info);
}
type_init(mac_mac_mini_2010_register);

/* ===========================================================================
 * Apple Mac mini (Core i5 Sandy Bridge, 2011)
 * =========================================================================== */
static void mac_mac_mini_2011_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac mini (Core i5 Sandy Bridge, 2011)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("SandyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_mac_mini_2011_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacMini5,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_mini_2011_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_mini_2011_machine_options(mc);
    mc->init = mac_mac_mini_2011_machine_init;
}

static const TypeInfo mac_mac_mini_2011_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-mini-2011"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_mini_2011_class_init,
};

static void mac_mac_mini_2011_register(void)
{
    type_register_static(&mac_mac_mini_2011_machine_info);
}
type_init(mac_mac_mini_2011_register);

/* ===========================================================================
 * Apple Mac mini (Core i5 Ivy Bridge, 2012)
 * =========================================================================== */
static void mac_mac_mini_2012_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac mini (Core i5 Ivy Bridge, 2012)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("IvyBridge");
    m->max_cpus = 4;
    m->default_ram_size = 4096 * MiB;
}

static void mac_mac_mini_2012_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacMini6,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_mini_2012_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_mini_2012_machine_options(mc);
    mc->init = mac_mac_mini_2012_machine_init;
}

static const TypeInfo mac_mac_mini_2012_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-mini-2012"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_mini_2012_class_init,
};

static void mac_mac_mini_2012_register(void)
{
    type_register_static(&mac_mac_mini_2012_machine_info);
}
type_init(mac_mac_mini_2012_register);

/* ===========================================================================
 * Apple Mac mini (Core i5 Haswell, 2014)
 * =========================================================================== */
static void mac_mac_mini_2014_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac mini (Core i5 Haswell, 2014)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("Haswell");
    m->max_cpus = 4;
    m->default_ram_size = 8192 * MiB;
}

static void mac_mac_mini_2014_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacMini7,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_mini_2014_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_mini_2014_machine_options(mc);
    mc->init = mac_mac_mini_2014_machine_init;
}

static const TypeInfo mac_mac_mini_2014_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-mini-2014"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_mini_2014_class_init,
};

static void mac_mac_mini_2014_register(void)
{
    type_register_static(&mac_mac_mini_2014_machine_info);
}
type_init(mac_mac_mini_2014_register);

/* ===========================================================================
 * Apple Mac mini (Core i3 Coffee Lake, T2, 2018)
 * =========================================================================== */
static void mac_mac_mini_2018_machine_options(MachineClass *m)
{
    mac_q35_common_options(m);
    m->desc = "Apple Mac mini (Core i3 Coffee Lake, T2, 2018)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("CoffeeLake");
    m->max_cpus = 6;
    m->default_ram_size = 8192 * MiB;
}

static void mac_mac_mini_2018_machine_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    pcms->smbios_manufacturer = g_strdup("Apple Inc.");
    pcms->smbios_product = g_strdup("MacMini8,1");
    pcms->smbios_version = g_strdup("1.0");
    pcms->auto_applesmc = true;
    pcms->laptop_mode = false;
    pc_q35_init(machine);
}

static void mac_mac_mini_2018_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mac_mac_mini_2018_machine_options(mc);
    mc->init = mac_mac_mini_2018_machine_init;
}

static const TypeInfo mac_mac_mini_2018_machine_info = {
    .name       = MACHINE_TYPE_NAME("mac-mac-mini-2018"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = mac_mac_mini_2018_class_init,
};

static void mac_mac_mini_2018_register(void)
{
    type_register_static(&mac_mac_mini_2018_machine_info);
}
type_init(mac_mac_mini_2018_register);

