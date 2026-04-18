/* v1.52 ARM64 CPU initfn functions - 17 CPUs */

static void aarch64_oryon_x1e_78_100_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,oryon-x1e-78-100";
    cpu->midr = 1360003088;
    cpu->ctr = 0x84448000;
}

static void aarch64_oryon_x1e_80_100_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,oryon-x1e-80-100";
    cpu->midr = 1360003089;
    cpu->ctr = 0x84448000;
}

static void aarch64_oryon_x1p_64_100_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,oryon-x1p-64-100";
    cpu->midr = 1360003090;
    cpu->ctr = 0x84448000;
}

static void aarch64_oryon_x1e_84_100_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,oryon-x1e-84-100";
    cpu->midr = 1360003091;
    cpu->ctr = 0x84448000;
}

static void aarch64_cortex_x5_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x5";
    cpu->midr = 1091518464;
    cpu->ctr = 0x84448000;
}

static void aarch64_cortex_a730_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a730";
    cpu->midr = 1091531520;
    cpu->ctr = 0x8444c000;
}

static void aarch64_dimensity_9400_x5_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "mediatek,mt6990";
    cpu->midr = 1091518465;
    cpu->ctr = 0x84448000;
}

static void aarch64_dimensity_9300_x4_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "mediatek,mt6989";
    cpu->midr = 1091516416;
    cpu->ctr = 0x84448000;
}

static void aarch64_dimensity_9300_a720_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "mediatek,mt6989-a720";
    cpu->midr = 1091531264;
    cpu->ctr = 0x8444c000;
}

static void aarch64_kirin_9100_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "hisilicon,kirin9100";
    cpu->midr = 1208958976;
    cpu->ctr = 0x84448000;
}

static void aarch64_kirin_9010_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "hisilicon,kirin9010";
    cpu->midr = 1208954880;
    cpu->ctr = 0x84448000;
}

static void aarch64_ampereone_a1_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,ampereone";
    cpu->midr = 2151612417;
    cpu->ctr = 0x84448000;
}

static void aarch64_ampereone_a128_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,ampereone-128";
    cpu->midr = 2151612418;
    cpu->ctr = 0x84448000;
}

static void aarch64_lx2160a_a72_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,lx2160a";
    cpu->midr = 1091555456;
    cpu->ctr = 0x84448000;
}

static void aarch64_lx2160a_a72_2_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,lx2160a-r2";
    cpu->midr = 1091555457;
    cpu->ctr = 0x84448000;
}


static void aarch64_fujitsu_2a64fx_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fujitsu,2a64fx";
    cpu->midr = 1175388162;
    cpu->ctr = 0x84448000;
}

