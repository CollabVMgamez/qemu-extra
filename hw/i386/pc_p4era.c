/*
 * QEMU - Pentium 4 era PC platform emulation
 *
 * Covers Intel Pentium 4 chipsets:
 *   i850  (RDRAM, first P4 chipset, 2000)
 *   i845  (SDRAM/DDR, budget P4, 2001)
 *   i850E (RDRAM + HT, 2002)
 *   i845G (integrated graphics, 2002)
 *   i845E (DDR266 + HT, 2002)
 *   i845PE (DDR333, 2002)
 *   i865G  (integrated GFX, 800 MHz FSB, PAT, 2003)
 *   i865PE (discrete GFX, 800 MHz FSB, PAT, 2003)
 *   i875P  (Canterwood, dual channel DDR, PAT, 2003)
 *   i925X  (LGA775, DDR2, PCI-Express, 2004)
 *   i915P  (LGA775, DDR/DDR2, PCIe, 2004)
 *   i915G  (integrated GFX, LGA775, 2004)
 *   i925XE (800/1066 FSB, 2004)
 *   i945P  (DDR2-667, LGA775, 2005)
 *   i945G  (integrated GFX, 2005)
 *   i955X  (dual-channel DDR2, 2005)
 *   i975X  (extreme chipset, XMP memory, 2006)
 *
 * All machines reuse the i440FX PCI host bridge and PIIX4 south bridge
 * (the closest QEMU equivalents) but expose correct chipset descriptions,
 * default CPU models, and sensible platform defaults.
 *
 * Copyright (c) 2024 QEMU P4 Fork Contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include CONFIG_DEVICES

#include "qemu/units.h"
#include "hw/i386/x86.h"
#include "hw/i386/pc.h"
#include "hw/pci-host/i440fx.h"
#include "hw/southbridge/piix.h"
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
/* ---------------------------------------------------------------------------
 * Parse the ram_type property and generate matching SPD EEPROM data.
 * CPU-Z reads SPD byte 2 (the DRAM type code) to show DDR1/DDR2/DDR3/DDR4.
 * ---------------------------------------------------------------------------
 */
static void pc_init_spd(PCMachineState *pcms)
{
    enum sdram_type stype = DDR2; /* default */
    if (pcms->ram_type) {
        if (!g_ascii_strcasecmp(pcms->ram_type, "ddr") ||
            !g_ascii_strcasecmp(pcms->ram_type, "ddr1")) {
            stype = DDR;
        } else if (!g_ascii_strcasecmp(pcms->ram_type, "ddr2")) {
            stype = DDR2;
        } else if (!g_ascii_strcasecmp(pcms->ram_type, "ddr3")) {
            stype = DDR3;
        } else if (!g_ascii_strcasecmp(pcms->ram_type, "ddr4") ||
                   !g_ascii_strcasecmp(pcms->ram_type, "ddr4e")) {
            stype = DDR4;
        } else if (!g_ascii_strcasecmp(pcms->ram_type, "ddr5") ||
                   !g_ascii_strcasecmp(pcms->ram_type, "ddr5e")) {
            stype = DDR5;
        } else if (!g_ascii_strcasecmp(pcms->ram_type, "lpddr4")) {
            stype = DDR4;
        } else if (!g_ascii_strcasecmp(pcms->ram_type, "lpddr5") ||
                   !g_ascii_strcasecmp(pcms->ram_type, "lpddr5x")) {
            stype = DDR5;
        }
    }
    uint8_t *spd = spd_data_generate(stype,
                       MACHINE(pcms)->ram_size > 0 ?
                       MACHINE(pcms)->ram_size : (1 * GiB));
    smbus_eeprom_init(pcms->smbus, 8, spd, 256);
    g_free(spd);
}

/* ---------------------------------------------------------------------------
 * Shared P4-era platform init (identical wiring to pc_piix init path, using
 * the i440FX host bridge + PIIX4 south bridge as the structural backbone).
 * ---------------------------------------------------------------------------
 */
static void pc_p4era_init(MachineState *machine)
{
    PCMachineState *pcms = PC_MACHINE(machine);
    PCMachineClass *pcmc = PC_MACHINE_GET_CLASS(pcms);
    X86MachineState *x86ms = X86_MACHINE(machine);
    MemoryRegion *system_memory = get_system_memory();
    MemoryRegion *system_io = get_system_io();
    Object *phb;
    ISABus *isa_bus;
    Object *piix4_pm = NULL;
    qemu_irq smi_irq;
    GSIState *gsi_state;
    MemoryRegion *ram_memory;
    MemoryRegion *pci_memory = NULL;
    ram_addr_t lowmem;
    uint64_t hole64_size = 0;
    PCIDevice *pci_dev;
    DeviceState *dev;
    size_t i;

    assert(pcmc->pci_enabled);

    ram_memory = machine->ram;
    if (!pcms->max_ram_below_4g) {
        pcms->max_ram_below_4g = 0xe0000000; /* 3.5 GB */
    }
    lowmem = pcms->max_ram_below_4g;
    if (machine->ram_size >= pcms->max_ram_below_4g) {
        if (pcmc->gigabyte_align) {
            if (lowmem > 0xc0000000) {
                lowmem = 0xc0000000;
            }
        }
    }
    if (machine->ram_size >= lowmem) {
        x86ms->above_4g_mem_size = machine->ram_size - lowmem;
        x86ms->below_4g_mem_size = lowmem;
    } else {
        x86ms->above_4g_mem_size = 0;
        x86ms->below_4g_mem_size = machine->ram_size;
    }

    pc_machine_init_sgx_epc(pcms);

    if (pcms->stealth_mode) {
        if (!pcms->hide_kvm_features)  pcms->hide_kvm_features = true;
        if (!pcms->hide_hv_signature)  pcms->hide_hv_signature = true;
        if (!pcms->spoof_bios_vendor)  pcms->spoof_bios_vendor = g_strdup("American Megatrends International LLC");
        if (!pcms->spoof_bios_version) pcms->spoof_bios_version = g_strdup("2.1.0");
        if (!pcms->spoof_board_vendor) pcms->spoof_board_vendor = g_strdup("LENOVO");
        if (!pcms->spoof_board_product) pcms->spoof_board_product = g_strdup("ThinkCentre M920q");
        if (!pcms->smbios_manufacturer) pcms->smbios_manufacturer = g_strdup("LENOVO");
        if (!pcms->smbios_product)      pcms->smbios_product = g_strdup("ThinkCentre M920q");
        if (!pcms->smbios_version)      pcms->smbios_version = g_strdup("10RRS0PA00");
    }

    if (pcms->hide_kvm_features) {
        static GlobalProperty hide_kvm_prop = {
            .driver = TYPE_X86_CPU, .property = "kvm", .value = "off",
        };
        qdev_prop_register_global(&hide_kvm_prop);
    }
    if (pcms->hide_hv_signature) {
        static GlobalProperty hide_hv_prop = {
            .driver = TYPE_X86_CPU, .property = "hv-vendor-id", .value = "",
        };
        qdev_prop_register_global(&hide_hv_prop);
    }

    x86_cpus_init(x86ms, pcmc->default_cpu_version);

    if (kvm_enabled()) {
        kvmclock_create(pcmc->kvmclock_create_always);
    }

    pci_memory = g_new(MemoryRegion, 1);
    memory_region_init(pci_memory, NULL, "pci", UINT64_MAX);

    /* Use i440FX as the PCI host bridge scaffold */
    phb = OBJECT(qdev_new(TYPE_I440FX_PCI_HOST_BRIDGE));
    object_property_add_child(OBJECT(machine), "i440fx", phb);
    object_property_set_link(phb, PCI_HOST_PROP_RAM_MEM,
                             OBJECT(ram_memory), &error_fatal);
    object_property_set_link(phb, PCI_HOST_PROP_PCI_MEM,
                             OBJECT(pci_memory), &error_fatal);
    object_property_set_link(phb, PCI_HOST_PROP_SYSTEM_MEM,
                             OBJECT(system_memory), &error_fatal);
    object_property_set_link(phb, PCI_HOST_PROP_IO_MEM,
                             OBJECT(system_io), &error_fatal);
    object_property_set_uint(phb, PCI_HOST_BELOW_4G_MEM_SIZE,
                             x86ms->below_4g_mem_size, &error_fatal);
    object_property_set_uint(phb, PCI_HOST_ABOVE_4G_MEM_SIZE,
                             x86ms->above_4g_mem_size, &error_fatal);
    object_property_set_str(phb, I440FX_HOST_PROP_PCI_TYPE,
                            TYPE_I440FX_PCI_DEVICE, &error_fatal);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(phb), &error_fatal);

    pcms->pcibus = PCI_BUS(qdev_get_child_bus(DEVICE(phb), "pci.0"));

    hole64_size = object_property_get_uint(phb,
                                           PCI_HOST_PROP_PCI_HOLE64_SIZE,
                                            &error_abort);

    /* Propagate RAM details to SMBIOS BEFORE pc_memory_init builds tables */
    if (pcms->ram_type) {
        smbios_set_type17_memory_type(pcms->ram_type);
        if (pcms->ram_speed_mhz > 0)
            smbios_set_type17_speed(pcms->ram_speed_mhz);
        if (pcms->ram_form_factor)
            smbios_set_type17_form_factor(pcms->ram_form_factor);
        if (pcms->ram_part_number)
            smbios_set_type17_part_number(pcms->ram_part_number);
        if (pcms->ram_manufacturer)
            smbios_set_type17_manufacturer(pcms->ram_manufacturer);
    }
    if (pcms->fake_ram_mb)
        smbios_set_fake_ram(pcms->fake_ram_mb, pcms->fake_dimm_mb, pcms->mem_slot_count);

    pc_memory_init(pcms, system_memory, pci_memory, hole64_size);

    gsi_state = pc_gsi_create(&x86ms->gsi, true);

    /* South bridge: PIIX4 (closest to ICH4/ICH5 used in real P4 era) */
    pci_dev = pci_new_multifunction(-1, TYPE_PIIX4_PCI_DEVICE);
    object_property_set_bool(OBJECT(pci_dev), "has-usb",
                             machine_usb(machine), &error_abort);
    object_property_set_bool(OBJECT(pci_dev), "has-acpi",
                             x86_machine_is_acpi_enabled(x86ms),
                             &error_abort);
    object_property_set_bool(OBJECT(pci_dev), "has-pic", false,
                             &error_abort);
    object_property_set_bool(OBJECT(pci_dev), "has-pit", false,
                             &error_abort);
    qdev_prop_set_uint32(DEVICE(pci_dev), "smb_io_base", 0xb100);
    object_property_set_bool(OBJECT(pci_dev), "smm-enabled",
                             x86_machine_is_smm_enabled(x86ms),
                             &error_abort);
    dev = DEVICE(pci_dev);
    for (i = 0; i < ISA_NUM_IRQS; i++) {
        qdev_connect_gpio_out_named(dev, "isa-irqs", i, x86ms->gsi[i]);
    }
    pci_realize_and_unref(pci_dev, pcms->pcibus, &error_fatal);

    isa_bus = ISA_BUS(qdev_get_child_bus(DEVICE(pci_dev), "isa.0"));
    x86ms->rtc = ISA_DEVICE(object_resolve_path_component(OBJECT(pci_dev),
                                                          "rtc"));
    piix4_pm = object_resolve_path_component(OBJECT(pci_dev), "pm");
    dev = DEVICE(object_resolve_path_component(OBJECT(pci_dev), "ide"));
    pci_ide_create_devs(PCI_DEVICE(dev));
    pcms->idebus[0] = qdev_get_child_bus(dev, "ide.0");
    pcms->idebus[1] = qdev_get_child_bus(dev, "ide.1");

    if (x86ms->pic == ON_OFF_AUTO_ON || x86ms->pic == ON_OFF_AUTO_AUTO) {
        pc_i8259_create(isa_bus, gsi_state->i8259_irq);
    }

    ioapic_init_gsi(gsi_state, phb);

    if (tcg_enabled()) {
        x86_register_ferr_irq(x86ms->gsi[13]);
    }

    pc_vga_init(isa_bus, pcms->pcibus);
    pc_basic_device_init(pcms, isa_bus, x86ms->gsi, x86ms->rtc,
                         !MACHINE_CLASS(pcmc)->no_floppy, 0x4);
    pc_nic_init(pcmc, isa_bus, pcms->pcibus);

    if (piix4_pm) {
        smi_irq = qemu_allocate_irq(pc_acpi_smi_interrupt, first_cpu, 0);
        qdev_connect_gpio_out_named(DEVICE(piix4_pm), "smi-irq", 0, smi_irq);
        pcms->smbus = I2C_BUS(qdev_get_child_bus(DEVICE(piix4_pm), "i2c"));
        pc_init_spd(pcms);

        object_property_add_link(OBJECT(machine), PC_MACHINE_ACPI_DEVICE_PROP,
                                 TYPE_HOTPLUG_HANDLER,
                                 (Object **)&x86ms->acpi_dev,
                                 object_property_allow_set_link,
                                 OBJ_PROP_LINK_STRONG);
        object_property_set_link(OBJECT(machine), PC_MACHINE_ACPI_DEVICE_PROP,
                                 piix4_pm, &error_abort);
    }

    if (machine->nvdimms_state->is_enabled) {
        nvdimm_init_acpi_state(machine->nvdimms_state, system_io,
                               x86_nvdimm_acpi_dsmio,
                               x86ms->fw_cfg, OBJECT(pcms));
    }
}

/* ---------------------------------------------------------------------------
 * Common P4-era machine options shared by all Intel chipset variants.
 * ---------------------------------------------------------------------------
 */
static void pc_p4era_common_options(MachineClass *m)
{
    PCMachineClass *pcmc = PC_MACHINE_CLASS(m);
    pcmc->default_south_bridge = TYPE_PIIX4_PCI_DEVICE;
    pcmc->pci_root_uid = 0;
    pcmc->default_cpu_version = 1;
    pcmc->gigabyte_align = true;

    m->family = "pc_p4era";
    m->default_machine_opts = "firmware=bios-256k.bin";
    m->default_display = "std";
    m->default_nic = "e1000";
    m->no_floppy = false;
    m->no_parallel = false;
    m->max_cpus = 2; /* most P4-era boards support at most 2 logical CPUs */
}

/* ===========================================================================
 * Intel i850  (Tehama, 2000)
 * First P4 chipset; RDRAM only, 400 MHz FSB.
 * Default CPU: Willamette (first P4 stepping)
 * =========================================================================== */
static void pc_i850_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i850 Tehama chipset (Pentium 4, RDRAM, 2000)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-willamette");
    m->max_cpus = 1;
}

static void pc_i850_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i850_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i850_machine_options(mc);
    mc->init = pc_i850_machine_init;
}

static const TypeInfo pc_i850_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i850"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i850_class_init,
};

static void pc_i850_register(void)
{
    type_register_static(&pc_i850_machine_info);
}
type_init(pc_i850_register);

/* ===========================================================================
 * Intel i845  (Brookdale, 2001)
 * Budget P4 chipset; SDRAM or DDR200, 400 MHz FSB.
 * Default CPU: Northwood B0
 * =========================================================================== */
static void pc_i845_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i845 Brookdale chipset (Pentium 4, DDR/SDRAM, 2001)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-b0");
    m->max_cpus = 1;
}

static void pc_i845_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i845_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i845_machine_options(mc);
    mc->init = pc_i845_machine_init;
}

static const TypeInfo pc_i845_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i845"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i845_class_init,
};

static void pc_i845_register(void)
{
    type_register_static(&pc_i845_machine_info);
}
type_init(pc_i845_register);

/* ===========================================================================
 * Intel i845G  (Brookdale-G, 2002)
 * i845 with integrated Intel Extreme Graphics; AGP 4x.
 * Default CPU: Northwood C1 with HT
 * =========================================================================== */
static void pc_i845g_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i845G Brookdale-G chipset (integrated GFX, DDR, 2002)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-ht");
    m->max_cpus = 2;
}

static void pc_i845g_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i845g_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i845g_machine_options(mc);
    mc->init = pc_i845g_machine_init;
}

static const TypeInfo pc_i845g_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i845g"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i845g_class_init,
};

static void pc_i845g_register(void)
{
    type_register_static(&pc_i845g_machine_info);
}
type_init(pc_i845g_register);

/* ===========================================================================
 * Intel i845E  (Brookdale-E, 2002)
 * DDR266, 533 MHz FSB, HT support.
 * Default CPU: Northwood C1 with HT
 * =========================================================================== */
static void pc_i845e_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i845E Brookdale-E chipset (DDR266, 533 MHz FSB, HT, 2002)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-ht");
    m->max_cpus = 2;
}

static void pc_i845e_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i845e_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i845e_machine_options(mc);
    mc->init = pc_i845e_machine_init;
}

static const TypeInfo pc_i845e_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i845e"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i845e_class_init,
};

static void pc_i845e_register(void)
{
    type_register_static(&pc_i845e_machine_info);
}
type_init(pc_i845e_register);

/* ===========================================================================
 * Intel i845PE  (Brookdale-PE, 2002)
 * DDR333, 533 MHz FSB, no integrated graphics, PAT.
 * Default CPU: Northwood D1
 * =========================================================================== */
static void pc_i845pe_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i845PE Brookdale-PE chipset (DDR333, 533 MHz FSB, 2002)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-d1");
    m->max_cpus = 2;
}

static void pc_i845pe_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i845pe_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i845pe_machine_options(mc);
    mc->init = pc_i845pe_machine_init;
}

static const TypeInfo pc_i845pe_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i845pe"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i845pe_class_init,
};

static void pc_i845pe_register(void)
{
    type_register_static(&pc_i845pe_machine_info);
}
type_init(pc_i845pe_register);

/* ===========================================================================
 * Intel i850E  (Tehama-E, 2002)
 * RDRAM, 533 MHz FSB, HT support.
 * Default CPU: Northwood HT
 * =========================================================================== */
static void pc_i850e_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i850E Tehama-E chipset (RDRAM, 533 MHz FSB, HT, 2002)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-ht");
    m->max_cpus = 2;
}

static void pc_i850e_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i850e_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i850e_machine_options(mc);
    mc->init = pc_i850e_machine_init;
}

static const TypeInfo pc_i850e_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i850e"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i850e_class_init,
};

static void pc_i850e_register(void)
{
    type_register_static(&pc_i850e_machine_info);
}
type_init(pc_i850e_register);

/* ===========================================================================
 * Intel i865G  (Springdale-G, 2003)
 * Dual-channel DDR400, 800 MHz FSB, PAT, integrated GFX, AGP 8x.
 * Default CPU: Northwood D1
 * =========================================================================== */
static void pc_i865g_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i865G Springdale-G chipset (DDR400, 800 MHz FSB, iGFX, 2003)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-d1");
    m->max_cpus = 2;
}

static void pc_i865g_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i865g_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i865g_machine_options(mc);
    mc->init = pc_i865g_machine_init;
}

static const TypeInfo pc_i865g_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i865g"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i865g_class_init,
};

static void pc_i865g_register(void)
{
    type_register_static(&pc_i865g_machine_info);
}
type_init(pc_i865g_register);

/* ===========================================================================
 * Intel i865PE  (Springdale-PE, 2003)
 * Dual-channel DDR400, 800 MHz FSB, PAT, discrete GFX only, AGP 8x.
 * Default CPU: Northwood D1
 * =========================================================================== */
static void pc_i865pe_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i865PE Springdale-PE chipset (DDR400, 800 MHz FSB, AGP 8x, 2003)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-d1");
    m->max_cpus = 2;
}

static void pc_i865pe_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i865pe_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i865pe_machine_options(mc);
    mc->init = pc_i865pe_machine_init;
}

static const TypeInfo pc_i865pe_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i865pe"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i865pe_class_init,
};

static void pc_i865pe_register(void)
{
    type_register_static(&pc_i865pe_machine_info);
}
type_init(pc_i865pe_register);

/* ===========================================================================
 * Intel i875P  (Canterwood, 2003)
 * Dual-channel DDR400, 800 MHz FSB, PAT (Performance Acceleration Technology),
 * AGP 8x. The flagship enthusiast chipset of the Northwood/Prescott era.
 * Default CPU: Northwood D1 (highest bin)
 * =========================================================================== */
static void pc_i875p_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i875P Canterwood chipset (DDR400 dual-ch, 800 MHz FSB, PAT, 2003)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-northwood-d1");
    m->max_cpus = 2;
}

static void pc_i875p_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i875p_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i875p_machine_options(mc);
    mc->init = pc_i875p_machine_init;
}

static const TypeInfo pc_i875p_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i875p"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i875p_class_init,
};

static void pc_i875p_register(void)
{
    type_register_static(&pc_i875p_machine_info);
}
type_init(pc_i875p_register);

/* ===========================================================================
 * Intel i925X  (Alderwood, 2004)
 * First LGA775 chipset; DDR2-533, PCIe x16, 800 MHz FSB.
 * Default CPU: Prescott D0 with EM64T
 * =========================================================================== */
static void pc_i925x_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i925X Alderwood chipset (DDR2, PCIe, LGA775, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-prescott");
    m->max_cpus = 2;
}

static void pc_i925x_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i925x_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i925x_machine_options(mc);
    mc->init = pc_i925x_machine_init;
}

static const TypeInfo pc_i925x_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i925x"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i925x_class_init,
};

static void pc_i925x_register(void)
{
    type_register_static(&pc_i925x_machine_info);
}
type_init(pc_i925x_register);

/* ===========================================================================
 * Intel i915P  (Grantsdale, 2004)
 * LGA775, DDR/DDR2, PCIe x16, 800 MHz FSB; no integrated GFX.
 * Default CPU: Prescott E0 with EM64T + NX
 * =========================================================================== */
static void pc_i915p_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i915P Grantsdale chipset (DDR/DDR2, PCIe, LGA775, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-prescott-e0");
    m->max_cpus = 2;
}

static void pc_i915p_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i915p_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i915p_machine_options(mc);
    mc->init = pc_i915p_machine_init;
}

static const TypeInfo pc_i915p_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i915p"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i915p_class_init,
};

static void pc_i915p_register(void)
{
    type_register_static(&pc_i915p_machine_info);
}
type_init(pc_i915p_register);

/* ===========================================================================
 * Intel i915G  (Grantsdale-G, 2004)
 * LGA775, DDR/DDR2, PCIe x16, integrated Intel GMA 900.
 * Default CPU: Prescott E0
 * =========================================================================== */
static void pc_i915g_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i915G Grantsdale-G chipset (DDR/DDR2, PCIe, GMA 900, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-prescott-e0");
    m->max_cpus = 2;
}

static void pc_i915g_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i915g_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i915g_machine_options(mc);
    mc->init = pc_i915g_machine_init;
}

static const TypeInfo pc_i915g_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i915g"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i915g_class_init,
};

static void pc_i915g_register(void)
{
    type_register_static(&pc_i915g_machine_info);
}
type_init(pc_i915g_register);

/* ===========================================================================
 * Intel i925XE  (Alderwood-E, 2004)
 * i925X with 1066 MHz FSB support; used with P4 EE.
 * Default CPU: Pentium 4 EE (Prescott-based)
 * =========================================================================== */
static void pc_i925xe_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i925XE Alderwood-E chipset (DDR2, 1066 MHz FSB, PCIe, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-ee");
    m->max_cpus = 2;
}

static void pc_i925xe_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i925xe_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i925xe_machine_options(mc);
    mc->init = pc_i925xe_machine_init;
}

static const TypeInfo pc_i925xe_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i925xe"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i925xe_class_init,
};

static void pc_i925xe_register(void)
{
    type_register_static(&pc_i925xe_machine_info);
}
type_init(pc_i925xe_register);

/* ===========================================================================
 * Intel i945P  (Lakeport, 2005)
 * LGA775, DDR2-667, PCIe x16, 800/1066 MHz FSB; no integrated GFX.
 * Default CPU: Prescott G1 (highest clocked P4)
 * =========================================================================== */
static void pc_i945p_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i945P Lakeport chipset (DDR2-667, PCIe, LGA775, 2005)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-prescott-g1");
    m->max_cpus = 2;
}

static void pc_i945p_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i945p_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i945p_machine_options(mc);
    mc->init = pc_i945p_machine_init;
}

static const TypeInfo pc_i945p_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i945p"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i945p_class_init,
};

static void pc_i945p_register(void)
{
    type_register_static(&pc_i945p_machine_info);
}
type_init(pc_i945p_register);

/* ===========================================================================
 * Intel i945G  (Lakeport-G, 2005)
 * LGA775, DDR2-667, PCIe x16, integrated Intel GMA 950.
 * Default CPU: Cedar Mill (65nm P4 with VT)
 * =========================================================================== */
static void pc_i945g_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i945G Lakeport-G chipset (DDR2-667, PCIe, GMA 950, 2005)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-cedarmill");
    m->max_cpus = 2;
}

static void pc_i945g_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i945g_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i945g_machine_options(mc);
    mc->init = pc_i945g_machine_init;
}

static const TypeInfo pc_i945g_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i945g"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i945g_class_init,
};

static void pc_i945g_register(void)
{
    type_register_static(&pc_i945g_machine_info);
}
type_init(pc_i945g_register);

/* ===========================================================================
 * Intel i955X  (Lossless, 2005)
 * Dual-channel DDR2-667, PCIe x16, 1066 MHz FSB, ECC support.
 * Default CPU: Pentium 4 Prescott G1
 * =========================================================================== */
static void pc_i955x_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i955X Lossless chipset (DDR2 dual-ch, PCIe, 1066 MHz FSB, 2005)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentium4-prescott-g1");
    m->max_cpus = 2;
}

static void pc_i955x_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i955x_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i955x_machine_options(mc);
    mc->init = pc_i955x_machine_init;
}

static const TypeInfo pc_i955x_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i955x"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i955x_class_init,
};

static void pc_i955x_register(void)
{
    type_register_static(&pc_i955x_machine_info);
}
type_init(pc_i955x_register);

/* ===========================================================================
 * Intel i975X  (Broadwater-X, 2006)
 * Dual-channel DDR2-800, PCIe x16, 1066 MHz FSB; XMP memory profiles.
 * Flagship final P4-era enthusiast chipset.
 * Default CPU: Pentium D Presler (dual-core 65nm)
 * =========================================================================== */
static void pc_i975x_machine_options(MachineClass *m)
{
    pc_p4era_common_options(m);
    m->desc = "Intel i975X Broadwater-X chipset (DDR2-800 dual-ch, PCIe, XMP, 2006)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("pentiumd-presler");
    m->max_cpus = 4; /* Presler is dual-core with HT = 4 logical */
}

static void pc_i975x_machine_init(MachineState *machine)
{
    pc_p4era_init(machine);
}

static void pc_i975x_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_i975x_machine_options(mc);
    mc->init = pc_i975x_machine_init;
}

static const TypeInfo pc_i975x_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-i975x"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_i975x_class_init,
};

static void pc_i975x_register(void)
{
    type_register_static(&pc_i975x_machine_info);
}
type_init(pc_i975x_register);
