/*
 * ARM32 CPU models for Windows CE/Mobile era SoCs
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

static void pxa255_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa255";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69052D05;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa250_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa250";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69052D01;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa260_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa260";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69052D06;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa261_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa261";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69052D07;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa262_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa262";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69052D08;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa270_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa270";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69054110;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa271_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa271";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69054111;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa272_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa272";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69054112;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa300_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa300";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69056810;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa310_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa310";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69056820;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa320_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa320";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69056830;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa930_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa930";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69056850;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa935_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa935";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x69056860;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void s3c2410_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s3c2410";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41129200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void s3c2412_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s3c2412";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000078;
}

static void s3c2440_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s3c2440";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41129200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void s3c2442_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s3c2442";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41129200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void s3c2443_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s3c2443";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069261;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000078;
}

static void s3c6410_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s3c6410";
    set_feature(&cpu->env, ARM_FEATURE_V6K);
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x410FB760;
    cpu->ctr = 0x0D3322D2;
    cpu->reset_sctlr = 0x00C50078;
}

static void omap730_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,omap730";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_OMAPCP);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void omap850_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,omap850";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_OMAPCP);
    cpu->midr = 0x41069265;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void omap2420_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,omap2420";
    set_feature(&cpu->env, ARM_FEATURE_V6K);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x410FB724;
    cpu->ctr = 0x0D1A2192;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm7200_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm7200";
    set_feature(&cpu->env, ARM_FEATURE_V6K);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x410FB764;
    cpu->ctr = 0x0D1A2192;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm7201a_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm7201a";
    set_feature(&cpu->env, ARM_FEATURE_V6K);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x410FB765;
    cpu->ctr = 0x0D1A2192;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm7500_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm7500";
    set_feature(&cpu->env, ARM_FEATURE_V6K);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x410FB763;
    cpu->ctr = 0x0D1A2192;
    cpu->reset_sctlr = 0x00C50078;
}

static void qsd8250_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,qsd8250";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x511F0D50;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void qsd8650_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,qsd8650";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x511F0D60;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm7227_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm7227";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x510FB767;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm7227a_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm7227a";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x510FB768;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm7627_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm7627";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x510FB769;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm8255_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm8255";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x510F00D0;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void msm8655_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm8655";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x510F00D1;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void imx21_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx21";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void imx31_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx31";
    set_feature(&cpu->env, ARM_FEATURE_V6K);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x410FB360;
    cpu->ctr = 0x0D1A2192;
    cpu->reset_sctlr = 0x00C50078;
}

static void imx35_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx35";
    set_feature(&cpu->env, ARM_FEATURE_V6K);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    cpu->midr = 0x410FB361;
    cpu->ctr = 0x0D1A2192;
    cpu->reset_sctlr = 0x00C50078;
}

static void imx51_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx51";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC085;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void imx53_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx53";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC085;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void s5pv210_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s5pv210";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC090;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void s5pc110_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,s5pc110";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC090;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void omap3430_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,omap3430";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC090;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void omap3630_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,omap3630";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC090;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void tegra2_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nvidia,tegra2";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC093;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void tegra3_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nvidia,tegra3";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x412FC094;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void apq8060_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,apq8060";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x510F00D0;
    cpu->ctr = 0x8244C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void pxa168_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa168";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x56055810;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void pxa910_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,pxa910";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    set_feature(&cpu->env, ARM_FEATURE_STRONGARM);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    cpu->midr = 0x56056820;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void armada610_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,armada-610";
    set_feature(&cpu->env, ARM_FEATURE_V7);
    set_feature(&cpu->env, ARM_FEATURE_THUMB2);
    set_feature(&cpu->env, ARM_FEATURE_V7MP);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_VAPA);
    set_feature(&cpu->env, ARM_FEATURE_MPIDR);
    set_feature(&cpu->env, ARM_FEATURE_VBAR);
    set_feature(&cpu->env, ARM_FEATURE_DUMMY_C15_REGS);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    cpu->midr = 0x562F5810;
    cpu->ctr = 0x8444C004;
    cpu->reset_sctlr = 0x00C50078;
}

static void ep9301_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "cirrus,ep9301";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41029200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void ep9302_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "cirrus,ep9302";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41029200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void ep9307_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "cirrus,ep9307";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41029200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void ep9312_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "cirrus,ep9312";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41029200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void ep9315_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "cirrus,ep9315";
    set_feature(&cpu->env, ARM_FEATURE_V4T);
    cpu->midr = 0x41029200;
    cpu->ctr = 0x0D1D1192;
    cpu->reset_sctlr = 0x00000070;
}

static void at91sam9260_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9260";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9261_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9261";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069261;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9263_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9263";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069263;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9g10_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9g10";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x410A9260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9g20_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9g20";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x410A9260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9g45_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9g45";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x410B9268;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9m10_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9m10";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x410B9268;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9n12_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9n12";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x410C9268;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void at91sam9x5_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "atmel,at91sam9x5";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x410C9268;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void lpc3180_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nxp,lpc3180";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void lpc3250_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nxp,lpc3250";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void spear300_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "st,spear300";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void spear310_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "st,spear310";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void spear320_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "st,spear320";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void spear600_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "st,spear600";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069260;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void wm8505_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "via,wm8505";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069266;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void wm8650_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "via,wm8650";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069267;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

static void wm8850_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "via,wm8850";
    set_feature(&cpu->env, ARM_FEATURE_V5);
    cpu->midr = 0x41069268;
    cpu->ctr = 0x0D172172;
    cpu->reset_sctlr = 0x00000078;
}

