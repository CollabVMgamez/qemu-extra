/*
 * Windows CE / Windows Mobile machine type stubs
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "hw/core/sysbus.h"
#include "hw/core/boards.h"
#include "target/arm/cpu-qom.h"
#include "hw/arm/machines-qom.h"

static void wince_stub_init(MachineState *machine)
{
}

static void wince_vexpress_a9_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Windows CE 6.0/7.0 on Versatile Express (Cortex-A9)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-a9");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 4;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_vexpress_a9_info = {
    .name = MACHINE_TYPE_NAME("wince-vexpress-a9"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_vexpress_a9_class_init,
};

static void wince_vexpress_a15_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Windows Embedded Compact 2013 on Versatile Express (Cortex-A15)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-a15");
    mc->default_ram_size = 512 * MiB;
    mc->max_cpus = 4;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_vexpress_a15_info = {
    .name = MACHINE_TYPE_NAME("wince-vexpress-a15"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_vexpress_a15_class_init,
};

static void wince_vexpress_a7_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Windows Embedded Compact 7/2013 on Versatile Express (Cortex-A7)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-a7");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 4;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_vexpress_a7_info = {
    .name = MACHINE_TYPE_NAME("wince-vexpress-a7"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_vexpress_a7_class_init,
};

static void ipaq_h1910_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HP iPAQ h1910 Pocket PC 2002 (PXA250)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa250");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo ipaq_h1910_info = {
    .name = MACHINE_TYPE_NAME("ipaq-h1910"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = ipaq_h1910_class_init,
};

static void ipaq_h2200_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HP iPAQ h2200 Pocket PC 2003 (PXA255)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa255");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo ipaq_h2200_info = {
    .name = MACHINE_TYPE_NAME("ipaq-h2200"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = ipaq_h2200_class_init,
};

static void ipaq_hx4700_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HP iPAQ hx4700 Windows Mobile 2003 SE (PXA270)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa270");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo ipaq_hx4700_info = {
    .name = MACHINE_TYPE_NAME("ipaq-hx4700"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = ipaq_hx4700_class_init,
};

static void dell_axim_x5_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Dell Axim X5 Pocket PC 2002 (PXA250)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa250");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo dell_axim_x5_info = {
    .name = MACHINE_TYPE_NAME("dell-axim-x5"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = dell_axim_x5_class_init,
};

static void dell_axim_x30_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Dell Axim X30 Windows Mobile 2003 SE (PXA270)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa270");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo dell_axim_x30_info = {
    .name = MACHINE_TYPE_NAME("dell-axim-x30"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = dell_axim_x30_class_init,
};

static void dell_axim_x50v_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Dell Axim X50v Windows Mobile 2003 SE (PXA270)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa270");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo dell_axim_x50v_info = {
    .name = MACHINE_TYPE_NAME("dell-axim-x50v"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = dell_axim_x50v_class_init,
};

static void dell_axim_x51v_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Dell Axim X51v Windows Mobile 5.0 (PXA270)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa270");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo dell_axim_x51v_info = {
    .name = MACHINE_TYPE_NAME("dell-axim-x51v"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = dell_axim_x51v_class_init,
};

static void htc_universal_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Universal Windows Mobile 5.0 (PXA270)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("pxa270");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_universal_info = {
    .name = MACHINE_TYPE_NAME("htc-universal"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_universal_class_init,
};

static void htc_blueangel_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Blue Angel WinMobile 2003 SE (OMAP850)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("omap850");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_blueangel_info = {
    .name = MACHINE_TYPE_NAME("htc-blueangel"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_blueangel_class_init,
};

static void htc_wizard_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Wizard Windows Mobile 5.0 (OMAP850)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("omap850");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_wizard_info = {
    .name = MACHINE_TYPE_NAME("htc-wizard"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_wizard_class_init,
};

static void htc_herald_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Herald WinMobile 6 (OMAP850)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("omap850");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_herald_info = {
    .name = MACHINE_TYPE_NAME("htc-herald"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_herald_class_init,
};

static void htc_touch_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Touch Windows Mobile 6 (MSM7200)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("msm7200");
    mc->default_ram_size = 128 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_touch_info = {
    .name = MACHINE_TYPE_NAME("htc-touch"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_touch_class_init,
};

static void htc_touch_diamond_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Touch Diamond WinMobile 6.1 (MSM7201A)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("msm7201a");
    mc->default_ram_size = 128 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_touch_diamond_info = {
    .name = MACHINE_TYPE_NAME("htc-touch-diamond"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_touch_diamond_class_init,
};

static void htc_touch_pro_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Touch Pro Windows Mobile 6.1 (MSM7201A)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("msm7201a");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_touch_pro_info = {
    .name = MACHINE_TYPE_NAME("htc-touch-pro"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_touch_pro_class_init,
};

static void htc_hd2_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC HD2 Windows Mobile 6.5 (QSD8250)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("qsd8250");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_hd2_info = {
    .name = MACHINE_TYPE_NAME("htc-hd2"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_hd2_class_init,
};

static void htc_titan_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Titan Windows Phone 7 (MSM8255)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("msm8255");
    mc->default_ram_size = 512 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_titan_info = {
    .name = MACHINE_TYPE_NAME("htc-titan"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_titan_class_init,
};

static void htc_touch_hd_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "HTC Touch HD WinMobile 6.1 (MSM7201A)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("msm7201a");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo htc_touch_hd_info = {
    .name = MACHINE_TYPE_NAME("htc-touch-hd"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = htc_touch_hd_class_init,
};

static void samsung_omnia_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Samsung Omnia i900 WinMobile 6.1 (S3C6410)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("s3c6410");
    mc->default_ram_size = 128 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo samsung_omnia_info = {
    .name = MACHINE_TYPE_NAME("samsung-omnia"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = samsung_omnia_class_init,
};

static void samsung_omnia_ii_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Samsung Omnia II i8000 WinMobile 6.5 (S3C6410)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("s3c6410");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo samsung_omnia_ii_info = {
    .name = MACHINE_TYPE_NAME("samsung-omnia-ii"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = samsung_omnia_ii_class_init,
};

static void palm_treo_750_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Palm Treo 750 WinMobile 5.0 (OMAP850)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("omap850");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo palm_treo_750_info = {
    .name = MACHINE_TYPE_NAME("palm-treo-750"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = palm_treo_750_class_init,
};

static void palm_treo_pro_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Palm Treo Pro WinMobile 6.1 (MSM7201A)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("msm7201a");
    mc->default_ram_size = 128 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo palm_treo_pro_info = {
    .name = MACHINE_TYPE_NAME("palm-treo-pro"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = palm_treo_pro_class_init,
};

static void wince_collie_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Sharp Zaurus SL-5500 Pocket PC 2002 (SA-1110)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("sa1110");
    mc->default_ram_size = 32 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_collie_info = {
    .name = MACHINE_TYPE_NAME("wince-collie"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_collie_class_init,
};

static void wondermedia_wm8650_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WonderMedia WM8650 WinCE Tablet";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("wm8650");
    mc->default_ram_size = 128 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wondermedia_wm8650_info = {
    .name = MACHINE_TYPE_NAME("wondermedia-wm8650"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wondermedia_wm8650_class_init,
};

static void wondermedia_wm8850_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WonderMedia WM8850 WinCE Tablet";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("wm8850");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wondermedia_wm8850_info = {
    .name = MACHINE_TYPE_NAME("wondermedia-wm8850"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wondermedia_wm8850_class_init,
};

static void wince_tegra2_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WinEmbedded Compact 7 Tablet (Tegra 2)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("tegra2");
    mc->default_ram_size = 512 * MiB;
    mc->max_cpus = 2;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_tegra2_info = {
    .name = MACHINE_TYPE_NAME("wince-tegra2"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_tegra2_class_init,
};

static void wince_tegra3_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WinEmbedded Compact Tablet (Tegra 3)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("tegra3");
    mc->default_ram_size = 1024 * MiB;
    mc->max_cpus = 4;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_tegra3_info = {
    .name = MACHINE_TYPE_NAME("wince-tegra3"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_tegra3_class_init,
};

static void wince_imx51_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WinEmbedded Compact 7 on i.MX51 (Cortex-A8)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("imx51");
    mc->default_ram_size = 256 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_imx51_info = {
    .name = MACHINE_TYPE_NAME("wince-imx51"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_imx51_class_init,
};

static void wince_imx53_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WinEmbedded Compact 7 on i.MX53 (Cortex-A8)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("imx53");
    mc->default_ram_size = 512 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo wince_imx53_info = {
    .name = MACHINE_TYPE_NAME("wince-imx53"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = wince_imx53_class_init,
};

static void marvell_armada610_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WinEmbedded Compact 7 (Marvell Armada 610)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("armada610");
    mc->default_ram_size = 512 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo marvell_armada610_info = {
    .name = MACHINE_TYPE_NAME("marvell-armada610"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = marvell_armada610_class_init,
};

static void freescale_imx25_pdk_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "WinCE 5.0 on i.MX25 PDK (ARM926EJ-S)";
    mc->init = wince_stub_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("imx21");
    mc->default_ram_size = 64 * MiB;
    mc->max_cpus = 1;
    mc->ignore_memory_transaction_failures = true;
}

static const TypeInfo freescale_imx25_pdk_info = {
    .name = MACHINE_TYPE_NAME("freescale-imx25-pdk"),
    .parent = TYPE_MACHINE,
    .interfaces = arm_machine_interfaces,
    .class_init = freescale_imx25_pdk_class_init,
};

static void wince_machines_register_types(void)
{
    type_register_static(&wince_vexpress_a9_info);
    type_register_static(&wince_vexpress_a15_info);
    type_register_static(&wince_vexpress_a7_info);
    type_register_static(&ipaq_h1910_info);
    
    type_register_static(&ipaq_h2200_info);
    type_register_static(&ipaq_hx4700_info);
    type_register_static(&dell_axim_x5_info);
    type_register_static(&dell_axim_x30_info);
    type_register_static(&dell_axim_x50v_info);
    type_register_static(&dell_axim_x51v_info);
    type_register_static(&htc_universal_info);
    type_register_static(&htc_blueangel_info);
    type_register_static(&htc_wizard_info);
    type_register_static(&htc_herald_info);
    type_register_static(&htc_touch_info);
    type_register_static(&htc_touch_diamond_info);
    type_register_static(&htc_touch_pro_info);
    type_register_static(&htc_hd2_info);
    type_register_static(&htc_titan_info);
    type_register_static(&htc_touch_hd_info);
    type_register_static(&samsung_omnia_info);
    type_register_static(&samsung_omnia_ii_info);
    type_register_static(&palm_treo_750_info);
    type_register_static(&palm_treo_pro_info);
    type_register_static(&wince_collie_info);
    type_register_static(&wondermedia_wm8650_info);
    type_register_static(&wondermedia_wm8850_info);
    type_register_static(&wince_tegra2_info);
    type_register_static(&wince_tegra3_info);
    type_register_static(&wince_imx51_info);
    type_register_static(&wince_imx53_info);
    type_register_static(&marvell_armada610_info);
    type_register_static(&freescale_imx25_pdk_info);
}

type_init(wince_machines_register_types)
