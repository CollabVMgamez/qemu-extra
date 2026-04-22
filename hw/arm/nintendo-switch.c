#include "qemu/osdep.h"
#include "qemu/units.h"
#include "hw/core/sysbus.h"
#include "hw/core/boards.h"
#include "hw/core/qdev-properties.h"
#include "hw/arm/boot.h"
#include "hw/intc/arm_gic.h"
#include "hw/arm/machines-qom.h"
#include "target/arm/cpu-qom.h"
#include "target/arm/cpu.h"
#include "system/system.h"
#include "system/address-spaces.h"
#include "qemu/error-report.h"
#include "qapi/error.h"

#define SWITCH_RAM_SIZE    (4 * GiB)
#define SWITCH_CPU_COUNT  4
#define SWITCH_CPU_TYPE   ARM_CPU_TYPE_NAME("cortex-a57")

#define TEGRA210_GIC_DIST_BASE   0x50041000
#define TEGRA210_GIC_CPU_BASE    0x50042000
#define TEGRA210_GIC_VIFACE_BASE 0x50046000
#define TEGRA210_GIC_VCPU_BASE   0x50048000

#define TEGRA210_UARTA_BASE      0x70006000
#define TEGRA210_UARTB_BASE      0x70006040
#define TEGRA210_UARTC_BASE      0x50000000

#define TEGRA210_TIMER_BASE      0x60005000
#define TEGRA210_RTC_BASE        0x7000E000

#define TEGRA210_HOST1X_BASE     0x50000000
#define TEGRA210_DISPLAY_BASE    0x54200000
#define TEGRA210_DSI_BASE        0x54300000
#define TEGRA210_VIC_BASE        0x54400000
#define TEGRA210_NVENC_BASE      0x54480000
#define TEGRA210_NVDEC_BASE      0x544C0000

#define TEGRA210_TSEC_BASE       0x54500000
#define TEGRA210_TSECB_BASE      0x54580000

#define TEGRA210_GPU_BASE        0x57000000
#define TEGRA210_GPU_SIZE        0x1000000

#define TEGRA210_I2C1_BASE       0x7000C000
#define TEGRA210_I2C2_BASE       0x7000C400
#define TEGRA210_I2C3_BASE       0x7000C500
#define TEGRA210_I2C4_BASE       0x7000C700
#define TEGRA210_I2C5_BASE       0x7000D000
#define TEGRA210_I2C6_BASE       0x7000D100

#define TEGRA210_GPIO_BASE       0x6000D000
#define TEGRA210_PMC_BASE        0x7000E400

#define TEGRA210_SDMMC1_BASE     0x700B0000
#define TEGRA210_SDMMC2_BASE     0x700B0200
#define TEGRA210_SDMMC3_BASE     0x700B0400
#define TEGRA210_SDMMC4_BASE     0x700B0600

#define TEGRA210_XUSB_BASE       0x70090000

#define TEGRA210_PCIE_BASE       0x10003000

typedef struct NintendoSwitchState {
    MachineState parent;
    ARMCPU *cpu[SWITCH_CPU_COUNT];
    DeviceState *gic;
    MemoryRegion sysmem;
    MemoryRegion ram;
} NintendoSwitchState;

static uint64_t tegra_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    switch (addr & ~3ULL) {
    case 0x000: return 0x00000001;
    case 0x004: return 0x21000001;
    default:    return 0;
    }
}

static void tegra_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps tegra_mmio_ops = {
    .read = tegra_mmio_read,
    .write = tegra_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static uint64_t tegra_gpu_read(void *opaque, hwaddr addr, unsigned size)
{
    switch (addr & ~3ULL) {
    case 0x000000: return 0x12D000A1;
    case 0x000004: return 0x00000001;
    case 0x100800: return 0x00000010;
    case 0x100200: return 0x00000002;
    case 0x009100: return 0;
    case 0x009400: {
        uint64_t ns = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        return (uint32_t)((ns / 1000) & 0xFFFFFFFFULL);
    }
    default:       return 0;
    }
}

static void tegra_gpu_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps tegra_gpu_ops = {
    .read = tegra_gpu_read,
    .write = tegra_gpu_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static uint64_t tegra_display_read(void *opaque, hwaddr addr, unsigned size)
{
    if (addr < 8) return 0x00000001;
    return 0;
}

static void tegra_display_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
}

static const MemoryRegionOps tegra_display_ops = {
    .read = tegra_display_read,
    .write = tegra_display_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = { .min_access_size = 1, .max_access_size = 4 },
};

static void nintendo_switch_init(MachineState *machine)
{
    NintendoSwitchState *nss = g_new0(NintendoSwitchState, 1);
    MemoryRegion *sysmem = get_system_memory();
    int i;

    memory_region_init_ram(&nss->ram, NULL, "switch.ram", SWITCH_RAM_SIZE,
                           &error_fatal);
    memory_region_add_subregion(sysmem, 0x80000000, &nss->ram);

    for (i = 0; i < SWITCH_CPU_COUNT; i++) {
        Object *cpuobj = object_new(ARM_CPU_TYPE_NAME("cortex-a57"));
        ARMCPU *cpu = ARM_CPU(cpuobj);

        qdev_prop_set_uint32(DEVICE(cpuobj), "mp-affinity", i);
        qdev_prop_set_bit(DEVICE(cpuobj), "has-el3", false);
        qdev_prop_set_bit(DEVICE(cpuobj), "has-el2", false);

        object_property_set_bool(cpuobj, "realized", true, &error_fatal);
        nss->cpu[i] = cpu;
    }

    nss->gic = qdev_new(TYPE_ARM_GIC);
    qdev_prop_set_uint32(nss->gic, "num-irq", 288);
    qdev_prop_set_uint32(nss->gic, "num-cpu", SWITCH_CPU_COUNT);
    qdev_prop_set_bit(nss->gic, "has-virtext", true);
    qdev_prop_set_bit(nss->gic, "has-securityext", false);
    qdev_realize(nss->gic, NULL, &error_fatal);

    SysBusDevice *gic_sbd = SYS_BUS_DEVICE(nss->gic);
    memory_region_add_subregion(sysmem, TEGRA210_GIC_DIST_BASE,
                                sysbus_mmio_get_region(gic_sbd, 0));
    memory_region_add_subregion(sysmem, TEGRA210_GIC_CPU_BASE,
                                sysbus_mmio_get_region(gic_sbd, 1));

    for (i = 0; i < SWITCH_CPU_COUNT; i++) {
        sysbus_connect_irq(gic_sbd, i, qdev_get_gpio_in(DEVICE(nss->cpu[i]), ARM_CPU_IRQ));
        sysbus_connect_irq(gic_sbd, i + SWITCH_CPU_COUNT,
                           qdev_get_gpio_in(DEVICE(nss->cpu[i]), ARM_CPU_FIQ));
    }

    MemoryRegion *mr;

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_gpu_ops, NULL,
                          "tegra210-gpu", TEGRA210_GPU_SIZE);
    memory_region_add_subregion(sysmem, TEGRA210_GPU_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_display_ops, NULL,
                          "tegra210-display", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_DISPLAY_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-host1x", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_HOST1X_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-vic", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_VIC_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-nvenc", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_NVENC_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-nvdec", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_NVDEC_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-tsec", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_TSEC_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-gpio", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_GPIO_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-pmc", 0x1000);
    memory_region_add_subregion(sysmem, TEGRA210_PMC_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-timer", 0x1000);
    memory_region_add_subregion(sysmem, TEGRA210_TIMER_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-rtc", 0x100);
    memory_region_add_subregion(sysmem, TEGRA210_RTC_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-sdmmc1", 0x200);
    memory_region_add_subregion(sysmem, TEGRA210_SDMMC1_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-sdmmc4-emmc", 0x200);
    memory_region_add_subregion(sysmem, TEGRA210_SDMMC4_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-xusb", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_XUSB_BASE, mr);

    mr = g_new0(MemoryRegion, 1);
    memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL,
                          "tegra210-pcie", 0x10000);
    memory_region_add_subregion(sysmem, TEGRA210_PCIE_BASE, mr);

    for (int idx = 0; idx < 6; idx++) {
        hwaddr base[] = {
            TEGRA210_I2C1_BASE, TEGRA210_I2C2_BASE, TEGRA210_I2C3_BASE,
            TEGRA210_I2C4_BASE, TEGRA210_I2C5_BASE, TEGRA210_I2C6_BASE
        };
        char name[32];
        snprintf(name, sizeof(name), "tegra210-i2c%d", idx + 1);
        mr = g_new0(MemoryRegion, 1);
        memory_region_init_io(mr, NULL, &tegra_mmio_ops, NULL, name, 0x100);
        memory_region_add_subregion(sysmem, base[idx], mr);
    }
}

static void nintendo_switch_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Nintendo Switch HAC-001 (NVIDIA Tegra X1 T210, Cortex-A57, 4GB LPDDR4)";
    mc->init = nintendo_switch_init;
    mc->default_cpu_type = SWITCH_CPU_TYPE;
    mc->default_ram_size = SWITCH_RAM_SIZE;
    mc->max_cpus = SWITCH_CPU_COUNT;
    mc->ignore_memory_transaction_failures = true;
    mc->default_ram_id = "switch.ram";
}

static const TypeInfo nintendo_switch_info = {
    .name = MACHINE_TYPE_NAME("nintendo-switch"),
    .parent = TYPE_MACHINE,
    .instance_size = sizeof(NintendoSwitchState),
    .class_init = nintendo_switch_class_init,
    .interfaces = aarch64_machine_interfaces,
};

static void nintendo_switch_v2_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Nintendo Switch HAC-001(-01) Mariko v2 (Tegra X1 T214, Cortex-A57, 4GB LPDDR4)";
    mc->init = nintendo_switch_init;
    mc->default_cpu_type = SWITCH_CPU_TYPE;
    mc->default_ram_size = SWITCH_RAM_SIZE;
    mc->max_cpus = SWITCH_CPU_COUNT;
    mc->ignore_memory_transaction_failures = true;
    mc->default_ram_id = "switch.ram";
}

static const TypeInfo nintendo_switch_v2_info = {
    .name = MACHINE_TYPE_NAME("nintendo-switch-v2"),
    .parent = TYPE_MACHINE,
    .instance_size = sizeof(NintendoSwitchState),
    .class_init = nintendo_switch_v2_class_init,
    .interfaces = aarch64_machine_interfaces,
};

static void nintendo_switch_lite_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Nintendo Switch Lite HDH-001 (Tegra X1 T214, Cortex-A57, 4GB LPDDR4)";
    mc->init = nintendo_switch_init;
    mc->default_cpu_type = SWITCH_CPU_TYPE;
    mc->default_ram_size = SWITCH_RAM_SIZE;
    mc->max_cpus = SWITCH_CPU_COUNT;
    mc->ignore_memory_transaction_failures = true;
    mc->default_ram_id = "switch.ram";
}

static const TypeInfo nintendo_switch_lite_info = {
    .name = MACHINE_TYPE_NAME("nintendo-switch-lite"),
    .parent = TYPE_MACHINE,
    .instance_size = sizeof(NintendoSwitchState),
    .class_init = nintendo_switch_lite_class_init,
    .interfaces = aarch64_machine_interfaces,
};

static void nintendo_switch_oled_class_init(ObjectClass *oc, const void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "Nintendo Switch OLED MOD.-001 (Tegra X1 T214, Cortex-A57, 4GB LPDDR4, 64GB eMMC)";
    mc->init = nintendo_switch_init;
    mc->default_cpu_type = SWITCH_CPU_TYPE;
    mc->default_ram_size = SWITCH_RAM_SIZE;
    mc->max_cpus = SWITCH_CPU_COUNT;
    mc->ignore_memory_transaction_failures = true;
    mc->default_ram_id = "switch.ram";
}

static const TypeInfo nintendo_switch_oled_info = {
    .name = MACHINE_TYPE_NAME("nintendo-switch-oled"),
    .parent = TYPE_MACHINE,
    .instance_size = sizeof(NintendoSwitchState),
    .class_init = nintendo_switch_oled_class_init,
    .interfaces = aarch64_machine_interfaces,
};

static void nintendo_switch_register(void)
{
    type_register_static(&nintendo_switch_info);
    type_register_static(&nintendo_switch_v2_info);
    type_register_static(&nintendo_switch_lite_info);
    type_register_static(&nintendo_switch_oled_info);
}
type_init(nintendo_switch_register)
