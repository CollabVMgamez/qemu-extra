/*
 * QEMU - NVIDIA nForce2/nForce3 platform emulation (AMD Athlon 64 era)
 *
 * Covers NVIDIA chipsets for AMD K7 (Athlon XP) and K8 (Athlon 64) platforms:
 *
 *   nForce     (MCP, 2001)  - original K7 nForce; single-channel DDR
 *   nForce2    (MCP, 2002)  - K7 Athlon XP; dual-channel DDR400, AGP 8x
 *   nForce2 Ultra (2003)    - nForce2 with 400 MHz FSB
 *   nForce3 150 (2003)      - first K8/Athlon 64 chipset; HyperTransport
 *   nForce3 250 (2004)      - improved K8; single-channel DDR400
 *   nForce3 Ultra (2004)    - nForce3 with DDR500 support
 *   nForce4      (2004)     - K8, PCIe x16, dual-channel DDR
 *   nForce4 Ultra (2005)    - PCIe, dual-ch DDR500
 *   nForce4 SLI  (2004)     - dual PCIe x8/x8 for SLI
 *   nForce4 SLI X16 (2005)  - full PCIe x16+x16 SLI
 *
 * All machines use the i440FX PCI host bridge + PIIX4 south bridge as the
 * structural backbone (the closest QEMU equivalents), but expose accurate
 * descriptions and defaults for each nForce variant.
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
        } else if (!g_ascii_strcasecmp(pcms->ram_type, "ddr4")) {
            stype = DDR4;
        }
    }
    uint8_t *spd = spd_data_generate(stype,
                       MACHINE(pcms)->ram_size > 0 ?
                       MACHINE(pcms)->ram_size : (1 * GiB));
    smbus_eeprom_init(pcms->smbus, 8, spd, 256);
    g_free(spd);
}

/* ---------------------------------------------------------------------------
 * Shared nForce-era init (i440FX host + PIIX4 south, structurally).
 * ---------------------------------------------------------------------------
 */
static void pc_nforce_init(MachineState *machine)
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
        pcms->max_ram_below_4g = 0xe0000000;
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
        if (pcms->ram_speed_mhz)   smbios_set_type17_speed(pcms->ram_speed_mhz);
        if (pcms->ram_form_factor)  smbios_set_type17_form_factor(pcms->ram_form_factor);
        if (pcms->ram_part_number)  smbios_set_type17_part_number(pcms->ram_part_number);
        if (pcms->ram_manufacturer) smbios_set_type17_manufacturer(pcms->ram_manufacturer);
    }
    if (pcms->fake_ram_mb)          smbios_set_fake_ram(pcms->fake_ram_mb, pcms->fake_dimm_mb, pcms->mem_slot_count);

    pc_memory_init(pcms, system_memory, pci_memory, hole64_size);

    gsi_state = pc_gsi_create(&x86ms->gsi, true);

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
 * Common nForce machine options
 * ---------------------------------------------------------------------------
 */
static void pc_nforce_common_options(MachineClass *m)
{
    PCMachineClass *pcmc = PC_MACHINE_CLASS(m);
    pcmc->default_south_bridge = TYPE_PIIX4_PCI_DEVICE;
    pcmc->pci_root_uid = 0;
    pcmc->default_cpu_version = 1;
    pcmc->gigabyte_align = true;

    m->family = "pc_nforce";
    m->default_machine_opts = "firmware=bios-256k.bin";
    m->default_display = "std";
    m->default_nic = "e1000";
    m->no_floppy = false;
    m->no_parallel = false;
}

/* ===========================================================================
 * NVIDIA nForce (MCP, 2001)
 * K7 Athlon XP/Athlon; single-channel DDR, AGP 4x, integrated GFX.
 * Default CPU: legacy athlon (K7 Athlon)
 * =========================================================================== */
static void pc_nforce1_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce MCP (K7 Athlon, DDR single-ch, AGP 4x, 2001)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon");
    m->max_cpus = 1;
}

static void pc_nforce1_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce1_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce1_machine_options(mc);
    mc->init = pc_nforce1_machine_init;
}

static const TypeInfo pc_nforce1_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce1_class_init,
};

static void pc_nforce1_register(void)
{
    type_register_static(&pc_nforce1_machine_info);
}
type_init(pc_nforce1_register);

/* ===========================================================================
 * NVIDIA nForce2 (MCP2 / nForce2-ST, 2002)
 * K7 Athlon XP; dual-channel DDR400, AGP 8x, integrated GFX (GeForce 4 MX).
 * Default CPU: athlon (highest K7 stepping modeled)
 * =========================================================================== */
static void pc_nforce2_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce2 (K7 Athlon XP, DDR400 dual-ch, AGP 8x, 2002)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon");
    m->max_cpus = 1;
}

static void pc_nforce2_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce2_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce2_machine_options(mc);
    mc->init = pc_nforce2_machine_init;
}

static const TypeInfo pc_nforce2_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce2"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce2_class_init,
};

static void pc_nforce2_register(void)
{
    type_register_static(&pc_nforce2_machine_info);
}
type_init(pc_nforce2_register);

/* ===========================================================================
 * NVIDIA nForce2 Ultra (nForce2-U, 2003)
 * K7 Athlon XP 3200+; dual-channel DDR400, 400 MHz FSB, AGP 8x.
 * Default CPU: athlon
 * =========================================================================== */
static void pc_nforce2u_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce2 Ultra (K7 Athlon XP, 400 MHz FSB, DDR400 dual-ch, 2003)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon");
    m->max_cpus = 1;
}

static void pc_nforce2u_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce2u_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce2u_machine_options(mc);
    mc->init = pc_nforce2u_machine_init;
}

static const TypeInfo pc_nforce2u_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce2-ultra"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce2u_class_init,
};

static void pc_nforce2u_register(void)
{
    type_register_static(&pc_nforce2u_machine_info);
}
type_init(pc_nforce2u_register);

/* ===========================================================================
 * NVIDIA nForce3 150 (2003)
 * First K8/Athlon 64 chipset; HyperTransport 800 MHz, single-ch DDR400.
 * Default CPU: athlon64-clawhammer (socket 754)
 * =========================================================================== */
static void pc_nforce3_150_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce3 150 (Athlon 64, HT 800 MHz, DDR400, 2003)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon64-clawhammer");
    m->max_cpus = 1;
}

static void pc_nforce3_150_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce3_150_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce3_150_machine_options(mc);
    mc->init = pc_nforce3_150_machine_init;
}

static const TypeInfo pc_nforce3_150_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce3-150"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce3_150_class_init,
};

static void pc_nforce3_150_register(void)
{
    type_register_static(&pc_nforce3_150_machine_info);
}
type_init(pc_nforce3_150_register);

/* ===========================================================================
 * NVIDIA nForce3 250 (2004)
 * K8; improved I/O, single-channel DDR400, HT 800/1000 MHz, AGP 8x.
 * Default CPU: athlon64-newcastle (socket 754/939)
 * =========================================================================== */
static void pc_nforce3_250_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce3 250 (Athlon 64, HT 1000 MHz, DDR400, AGP 8x, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon64-newcastle");
    m->max_cpus = 1;
}

static void pc_nforce3_250_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce3_250_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce3_250_machine_options(mc);
    mc->init = pc_nforce3_250_machine_init;
}

static const TypeInfo pc_nforce3_250_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce3-250"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce3_250_class_init,
};

static void pc_nforce3_250_register(void)
{
    type_register_static(&pc_nforce3_250_machine_info);
}
type_init(pc_nforce3_250_register);

/* ===========================================================================
 * NVIDIA nForce3 Ultra (2004)
 * K8; DDR500 support, HT 1000 MHz, AGP 8x; highest single-ch nForce3.
 * Default CPU: athlon64-winchester (90nm socket 939)
 * =========================================================================== */
static void pc_nforce3u_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce3 Ultra (Athlon 64, DDR500, HT 1000 MHz, AGP 8x, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon64-winchester");
    m->max_cpus = 1;
}

static void pc_nforce3u_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce3u_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce3u_machine_options(mc);
    mc->init = pc_nforce3u_machine_init;
}

static const TypeInfo pc_nforce3u_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce3-ultra"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce3u_class_init,
};

static void pc_nforce3u_register(void)
{
    type_register_static(&pc_nforce3u_machine_info);
}
type_init(pc_nforce3u_register);

/* ===========================================================================
 * NVIDIA nForce4 (CK804, 2004)
 * K8 socket 939; dual-channel DDR400, PCIe x16, HT 1000 MHz.
 * Default CPU: athlon64-venice (90nm, SSE3, socket 939)
 * =========================================================================== */
static void pc_nforce4_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce4 CK804 (Athlon 64, DDR400 dual-ch, PCIe x16, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon64-venice");
    m->max_cpus = 2;
}

static void pc_nforce4_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce4_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce4_machine_options(mc);
    mc->init = pc_nforce4_machine_init;
}

static const TypeInfo pc_nforce4_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce4"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce4_class_init,
};

static void pc_nforce4_register(void)
{
    type_register_static(&pc_nforce4_machine_info);
}
type_init(pc_nforce4_register);

/* ===========================================================================
 * NVIDIA nForce4 Ultra (2005)
 * K8 socket 939; dual-channel DDR500, PCIe x16, HT 1000 MHz.
 * Default CPU: athlon64-sandiego (90nm, 4000+, socket 939)
 * =========================================================================== */
static void pc_nforce4u_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce4 Ultra (Athlon 64, DDR500 dual-ch, PCIe x16, 2005)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon64-sandiego");
    m->max_cpus = 2;
}

static void pc_nforce4u_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce4u_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce4u_machine_options(mc);
    mc->init = pc_nforce4u_machine_init;
}

static const TypeInfo pc_nforce4u_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce4-ultra"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce4u_class_init,
};

static void pc_nforce4u_register(void)
{
    type_register_static(&pc_nforce4u_machine_info);
}
type_init(pc_nforce4u_register);

/* ===========================================================================
 * NVIDIA nForce4 SLI (2004)
 * K8 socket 939; dual-channel DDR400, dual PCIe x8 for SLI.
 * Default CPU: athlon64x2-manchester (dual-core Athlon 64 X2)
 * =========================================================================== */
static void pc_nforce4_sli_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce4 SLI (Athlon 64 X2, DDR400 dual-ch, dual PCIe x8 SLI, 2004)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon64x2-manchester");
    m->max_cpus = 4; /* dual-core with 2 logical per core via HT */
}

static void pc_nforce4_sli_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce4_sli_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce4_sli_machine_options(mc);
    mc->init = pc_nforce4_sli_machine_init;
}

static const TypeInfo pc_nforce4_sli_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce4-sli"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce4_sli_class_init,
};

static void pc_nforce4_sli_register(void)
{
    type_register_static(&pc_nforce4_sli_machine_info);
}
type_init(pc_nforce4_sli_register);

/* ===========================================================================
 * NVIDIA nForce4 SLI X16 (2005)
 * K8 socket 939; dual-channel DDR500, full PCIe x16+x16 SLI.
 * Default CPU: athlon64x2-toledo (highest dual-core Athlon 64 X2)
 * =========================================================================== */
static void pc_nforce4_slix16_machine_options(MachineClass *m)
{
    pc_nforce_common_options(m);
    m->desc = "NVIDIA nForce4 SLI X16 (Athlon 64 X2, DDR500 dual-ch, PCIe x16+x16, 2005)";
    m->default_cpu_type = X86_CPU_TYPE_NAME("athlon64x2-toledo");
    m->max_cpus = 4;
}

static void pc_nforce4_slix16_machine_init(MachineState *machine)
{
    pc_nforce_init(machine);
}

static void pc_nforce4_slix16_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    pc_nforce4_slix16_machine_options(mc);
    mc->init = pc_nforce4_slix16_machine_init;
}

static const TypeInfo pc_nforce4_slix16_machine_info = {
    .name       = MACHINE_TYPE_NAME("pc-nforce4-sli-x16"),
    .parent     = TYPE_PC_MACHINE,
    .class_init = pc_nforce4_slix16_class_init,
};

static void pc_nforce4_slix16_register(void)
{
    type_register_static(&pc_nforce4_slix16_machine_info);
}
type_init(pc_nforce4_slix16_register);
