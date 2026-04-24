/*
 * QEMU AArch64 TCG CPUs
 *
 * Copyright (c) 2013 Linaro Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html>
 */

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "cpu.h"
#include "qemu/module.h"
#include "qapi/visitor.h"
#include "hw/core/qdev-properties.h"
#include "qemu/units.h"
#include "internals.h"
#include "cpu-features.h"
#include "cpregs.h"

static void aarch64_a35_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    ARMISARegisters *isar = &cpu->isar;

    cpu->dtb_compatible = "arm,cortex-a35";
    set_feature(&cpu->env, ARM_FEATURE_V8);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    set_feature(&cpu->env, ARM_FEATURE_AARCH64);
    set_feature(&cpu->env, ARM_FEATURE_CBAR_RO);
    set_feature(&cpu->env, ARM_FEATURE_EL2);
    set_feature(&cpu->env, ARM_FEATURE_EL3);
    set_feature(&cpu->env, ARM_FEATURE_PMU);

    /* From B2.2 AArch64 identification registers. */
    cpu->midr = 0x411fd040;
    cpu->revidr = 0;
    cpu->ctr = 0x84448004;
    SET_IDREG(isar, ID_PFR0, 0x00000131);
    SET_IDREG(isar, ID_PFR1, 0x00011011);
    SET_IDREG(isar, ID_DFR0, 0x03010066);
    SET_IDREG(isar, ID_AFR0, 0);
    SET_IDREG(isar, ID_MMFR0, 0x10201105);
    SET_IDREG(isar, ID_MMFR1, 0x40000000);
    SET_IDREG(isar, ID_MMFR2, 0x01260000);
    SET_IDREG(isar, ID_MMFR3, 0x02102211);
    SET_IDREG(isar, ID_ISAR0, 0x02101110);
    SET_IDREG(isar, ID_ISAR1, 0x13112111);
    SET_IDREG(isar, ID_ISAR2, 0x21232042);
    SET_IDREG(isar, ID_ISAR3, 0x01112131);
    SET_IDREG(isar, ID_ISAR4, 0x00011142);
    SET_IDREG(isar, ID_ISAR5, 0x00011121);
    SET_IDREG(isar, ID_AA64PFR0, 0x00002222);
    SET_IDREG(isar, ID_AA64PFR1, 0);
    SET_IDREG(isar, ID_AA64DFR0, 0x10305106);
    SET_IDREG(isar, ID_AA64DFR1, 0);
    SET_IDREG(isar, ID_AA64ISAR0, 0x00011120);
    SET_IDREG(isar, ID_AA64ISAR1, 0);
    SET_IDREG(isar, ID_AA64MMFR0, 0x00101122);
    SET_IDREG(isar, ID_AA64MMFR1, 0);
    SET_IDREG(isar, CLIDR, 0x0a200023);
    set_dczid_bs(cpu, 4);

    /* From B2.4 AArch64 Virtual Memory control registers */
    cpu->reset_sctlr = 0x00c50838;

    /* From B2.10 AArch64 performance monitor registers */
    cpu->isar.reset_pmcr_el0 = 0x410a3000;

    /* From B2.29 Cache ID registers */
    /* 32KB L1 dcache */
    cpu->ccsidr[0] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 64, 32 * KiB, 7);
    /* 32KB L1 icache */
    cpu->ccsidr[1] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 64, 32 * KiB, 2);
    /* 512KB L2 cache */
    cpu->ccsidr[2] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 16, 64, 512 * KiB, 7);

    /* From B3.5 VGIC Type register */
    cpu->gic_num_lrs = 4;
    cpu->gic_vpribits = 5;
    cpu->gic_vprebits = 5;
    cpu->gic_pribits = 5;

    /* From C6.4 Debug ID Register */
    cpu->isar.dbgdidr = 0x3516d000;
    /* From C6.5 Debug Device ID Register */
    cpu->isar.dbgdevid = 0x00110f13;
    /* From C6.6 Debug Device ID Register 1 */
    cpu->isar.dbgdevid1 = 0x2;

    /* From Cortex-A35 SIMD and Floating-point Support r1p0 */
    /* From 3.2 AArch32 register summary */
    cpu->reset_fpsid = 0x41034043;

    /* From 2.2 AArch64 register summary */
    cpu->isar.mvfr0 = 0x10110222;
    cpu->isar.mvfr1 = 0x12111111;
    cpu->isar.mvfr2 = 0x00000043;

    /* These values are the same with A53/A57/A72. */
    define_cortex_a72_a57_a53_cp_reginfo(cpu);
}

static void cpu_max_get_sve_max_vq(Object *obj, Visitor *v, const char *name,
                                   void *opaque, Error **errp)
{
    ARMCPU *cpu = ARM_CPU(obj);
    uint32_t value;

    /* All vector lengths are disabled when SVE is off. */
    if (!cpu_isar_feature(aa64_sve, cpu)) {
        value = 0;
    } else {
        value = cpu->sve_max_vq;
    }
    visit_type_uint32(v, name, &value, errp);
}

static void cpu_max_set_sve_max_vq(Object *obj, Visitor *v, const char *name,
                                   void *opaque, Error **errp)
{
    ARMCPU *cpu = ARM_CPU(obj);
    uint32_t max_vq;

    if (!visit_type_uint32(v, name, &max_vq, errp)) {
        return;
    }

    if (max_vq == 0 || max_vq > ARM_MAX_VQ) {
        error_setg(errp, "unsupported SVE vector length");
        error_append_hint(errp, "Valid sve-max-vq in range [1-%d]\n",
                          ARM_MAX_VQ);
        return;
    }

    cpu->sve_max_vq = max_vq;
}

static bool cpu_arm_get_rme(Object *obj, Error **errp)
{
    ARMCPU *cpu = ARM_CPU(obj);
    return cpu_isar_feature(aa64_rme, cpu);
}

static void cpu_arm_set_rme(Object *obj, bool value, Error **errp)
{
    ARMCPU *cpu = ARM_CPU(obj);

    /* Enable FEAT_RME_GPC2 */
    FIELD_DP64_IDREG(&cpu->isar, ID_AA64PFR0, RME, value ? 2 : 0);
}

static void cpu_max_set_l0gptsz(Object *obj, Visitor *v, const char *name,
                                void *opaque, Error **errp)
{
    ARMCPU *cpu = ARM_CPU(obj);
    uint32_t value;

    if (!visit_type_uint32(v, name, &value, errp)) {
        return;
    }

    /* Encode the value for the GPCCR_EL3 field. */
    switch (value) {
    case 30:
    case 34:
    case 36:
    case 39:
        cpu->reset_l0gptsz = value - 30;
        break;
    default:
        error_setg(errp, "invalid value for l0gptsz");
        error_append_hint(errp, "valid values are 30, 34, 36, 39\n");
        break;
    }
}

static void cpu_max_get_l0gptsz(Object *obj, Visitor *v, const char *name,
                                void *opaque, Error **errp)
{
    ARMCPU *cpu = ARM_CPU(obj);
    uint32_t value = cpu->reset_l0gptsz + 30;

    visit_type_uint32(v, name, &value, errp);
}

static const Property arm_cpu_lpa2_property =
    DEFINE_PROP_BOOL("lpa2", ARMCPU, prop_lpa2, true);




static void aarch64_a78ae_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    ARMISARegisters *isar = &cpu->isar;

    cpu->dtb_compatible = "arm,cortex-a78ae";
    set_feature(&cpu->env, ARM_FEATURE_V8);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_AARCH64);
    set_feature(&cpu->env, ARM_FEATURE_EL2);
    set_feature(&cpu->env, ARM_FEATURE_EL3);
    set_feature(&cpu->env, ARM_FEATURE_PMU);

    /* Ordered by 3.2.4 AArch64 registers by functional group */
    SET_IDREG(isar, CLIDR, 0x82000023);
    cpu->ctr = 0x9444c004;
    set_dczid_bs(cpu, 4);
    SET_IDREG(isar, ID_AA64DFR0, 0x0000000110305408ull);
    SET_IDREG(isar, ID_AA64ISAR0, 0x0010100010211120ull);
    SET_IDREG(isar, ID_AA64ISAR1, 0x0000000001200031ull);
    SET_IDREG(isar, ID_AA64MMFR0, 0x0000000000101125ull);
    SET_IDREG(isar, ID_AA64MMFR1, 0x0000000010212122ull);
    SET_IDREG(isar, ID_AA64MMFR2, 0x0000000100001011ull);
    SET_IDREG(isar, ID_AA64PFR0, 0x1100000010111112ull); /* GIC filled in later */
    SET_IDREG(isar, ID_AA64PFR1, 0x0000000000000010ull);
    SET_IDREG(isar, ID_AFR0, 0x00000000);
    SET_IDREG(isar, ID_DFR0, 0x04010088);
    SET_IDREG(isar, ID_ISAR0, 0x02101110);
    SET_IDREG(isar, ID_ISAR1, 0x13112111);
    SET_IDREG(isar, ID_ISAR2, 0x21232042);
    SET_IDREG(isar, ID_ISAR3, 0x01112131);
    SET_IDREG(isar, ID_ISAR4, 0x00010142);
    SET_IDREG(isar, ID_ISAR5, 0x01011121);
    SET_IDREG(isar, ID_ISAR6, 0x00000010);
    SET_IDREG(isar, ID_MMFR0, 0x10201105);
    SET_IDREG(isar, ID_MMFR1, 0x40000000);
    SET_IDREG(isar, ID_MMFR2, 0x01260000);
    SET_IDREG(isar, ID_MMFR3, 0x02122211);
    SET_IDREG(isar, ID_MMFR4, 0x00021110);
    SET_IDREG(isar, ID_PFR0, 0x10010131);
    SET_IDREG(isar, ID_PFR1, 0x00010000); /* GIC filled in later */
    SET_IDREG(isar, ID_PFR2, 0x00000011);
    cpu->midr = 0x410fd423;          /* r0p3 */
    cpu->revidr = 0;

    /* From 3.2.33 CCSIDR_EL1 */
    /* 64KB L1 dcache */
    cpu->ccsidr[0] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 64, 64 * KiB, 7);
    /* 64KB L1 icache */
    cpu->ccsidr[1] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 64, 64 * KiB, 2);
    /* 512KB L2 cache */
    cpu->ccsidr[2] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 8, 64, 512 * KiB, 7);

    /* From 3.2.118 SCTLR_EL3 */
    cpu->reset_sctlr = 0x30c50838;

    /* From 3.4.23 ICH_VTR_EL2 */
    cpu->gic_num_lrs = 4;
    cpu->gic_vpribits = 5;
    cpu->gic_vprebits = 5;
    /* From 3.4.8 ICC_CTLR_EL3 */
    cpu->gic_pribits = 5;

    /* From 3.5.1 AdvSIMD AArch64 register summary */
    cpu->isar.mvfr0 = 0x10110222;
    cpu->isar.mvfr1 = 0x13211111;
    cpu->isar.mvfr2 = 0x00000043;

    /* From 5.5.1 AArch64 PMU register summary */
    cpu->isar.reset_pmcr_el0 = 0x41223000;
}

static void aarch64_a64fx_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    ARMISARegisters *isar = &cpu->isar;

    cpu->dtb_compatible = "arm,a64fx";
    set_feature(&cpu->env, ARM_FEATURE_V8);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    set_feature(&cpu->env, ARM_FEATURE_AARCH64);
    set_feature(&cpu->env, ARM_FEATURE_EL2);
    set_feature(&cpu->env, ARM_FEATURE_EL3);
    set_feature(&cpu->env, ARM_FEATURE_PMU);
    cpu->midr = 0x461f0010;
    cpu->revidr = 0x00000000;
    cpu->ctr = 0x86668006;
    cpu->reset_sctlr = 0x30000180;
    SET_IDREG(isar, ID_AA64PFR0, 0x0000000101111111); /* No RAS Extensions */
    SET_IDREG(isar, ID_AA64PFR1, 0x0000000000000000);
    SET_IDREG(isar, ID_AA64DFR0, 0x0000000010305408);
    SET_IDREG(isar, ID_AA64DFR1, 0x0000000000000000);
    SET_IDREG(isar, ID_AA64AFR0, 0x0000000000000000);
    SET_IDREG(isar, ID_AA64AFR1, 0x0000000000000000);
    SET_IDREG(isar, ID_AA64MMFR0, 0x0000000000001122);
    SET_IDREG(isar, ID_AA64MMFR1, 0x0000000011212100);
    SET_IDREG(isar, ID_AA64MMFR2, 0x0000000000001011);
    SET_IDREG(isar, ID_AA64ISAR0, 0x0000000010211120);
    SET_IDREG(isar, ID_AA64ISAR1, 0x0000000000010001);
    SET_IDREG(isar, ID_AA64ZFR0, 0x0000000000000000);
    SET_IDREG(isar, CLIDR, 0x0000000080000023);
    /* 64KB L1 dcache */
    cpu->ccsidr[0] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 256, 64 * KiB, 7);
    /* 64KB L1 icache */
    cpu->ccsidr[1] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 256, 64 * KiB, 2);
    /* 8MB L2 cache */
    cpu->ccsidr[2] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 16, 256, 8 * MiB, 7);
    set_dczid_bs(cpu, 6); /* 256 bytes */
    cpu->gic_num_lrs = 4;
    cpu->gic_vpribits = 5;
    cpu->gic_vprebits = 5;
    cpu->gic_pribits = 5;

    /* The A64FX supports only 128, 256 and 512 bit vector lengths */
    cpu->sve_vq.supported = (1 << 0)  /* 128bit */
                          | (1 << 1)  /* 256bit */
                          | (1 << 3); /* 512bit */
    aarch64_add_sve_properties(obj);

    cpu->isar.reset_pmcr_el0 = 0x46014040;

    /* TODO:  Add A64FX specific HPC extension registers */
}

static CPAccessResult access_actlr_w(CPUARMState *env, const ARMCPRegInfo *r,
                                     bool read)
{
    if (!read) {
        int el = arm_current_el(env);

        /* Because ACTLR_EL2 is constant 0, writes below EL2 trap to EL2. */
        if (el < 2 && arm_is_el2_enabled(env)) {
            return CP_ACCESS_TRAP_EL2;
        }
        /* Because ACTLR_EL3 is constant 0, writes below EL3 trap to EL3. */
        if (el < 3 && arm_feature(env, ARM_FEATURE_EL3)) {
            return CP_ACCESS_TRAP_EL3;
        }
    }
    return CP_ACCESS_OK;
}

static const ARMCPRegInfo neoverse_n1_cp_reginfo[] = {
    { .name = "ATCR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 7, .opc2 = 0,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      /* Traps and enables are the same as for TCR_EL1. */
      .accessfn = access_tvm_trvm, .fgt = FGT_TCR_EL1, },
    { .name = "ATCR_EL2", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 4, .crn = 15, .crm = 7, .opc2 = 0,
      .access = PL2_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "ATCR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 7, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "ATCR_EL12", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 5, .crn = 15, .crm = 7, .opc2 = 0,
      .access = PL2_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "AVTCR_EL2", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 4, .crn = 15, .crm = 7, .opc2 = 1,
      .access = PL2_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUACTLR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 0,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUACTLR2_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 1,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUACTLR3_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 2,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    /*
     * Report CPUCFR_EL1.SCU as 1, as we do not implement the DSU
     * (and in particular its system registers).
     */
    { .name = "CPUCFR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 0, .opc2 = 0,
      .access = PL1_R, .type = ARM_CP_CONST, .resetvalue = 4 },
    { .name = "CPUECTLR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 4,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0x961563010,
      .accessfn = access_actlr_w },
    { .name = "CPUPCR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 1,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPMR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 3,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPOR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 2,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPSELR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPWRCTLR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 2, .opc2 = 7,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "ERXPFGCDN_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 2, .opc2 = 2,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "ERXPFGCTL_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 2, .opc2 = 1,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "ERXPFGF_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 2, .opc2 = 0,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
};

static void __attribute__((unused)) define_neoverse_n1_cp_reginfo(ARMCPU *cpu)
{
    define_arm_cp_regs(cpu, neoverse_n1_cp_reginfo);
}

static const ARMCPRegInfo neoverse_v1_cp_reginfo[] = {
    { .name = "CPUECTLR2_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 5,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUPPMCR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPPMCR2_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 1,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPPMCR3_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 6,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
};

static void __attribute__((unused)) define_neoverse_v1_cp_reginfo(ARMCPU *cpu)
{
    /*
     * The Neoverse V1 has all of the Neoverse N1's IMPDEF
     * registers and a few more of its own.
     */
    define_arm_cp_regs(cpu, neoverse_n1_cp_reginfo);
    define_arm_cp_regs(cpu, neoverse_v1_cp_reginfo);
}



static const ARMCPRegInfo __attribute__((unused)) cortex_a710_cp_reginfo[] = {
    { .name = "CPUACTLR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 0,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUACTLR2_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 1,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUACTLR3_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 2,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUACTLR4_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 3,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUECTLR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 4,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUECTLR2_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 1, .opc2 = 5,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUPPMCR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 2, .opc2 = 4,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPWRCTLR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 2, .opc2 = 7,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "ATCR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 7, .opc2 = 0,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUACTLR5_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 8, .opc2 = 0,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUACTLR6_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 8, .opc2 = 1,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "CPUACTLR7_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 8, .opc2 = 2,
      .access = PL1_RW, .type = ARM_CP_CONST, .resetvalue = 0,
      .accessfn = access_actlr_w },
    { .name = "ATCR_EL2", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 4, .crn = 15, .crm = 7, .opc2 = 0,
      .access = PL2_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "AVTCR_EL2", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 4, .crn = 15, .crm = 7, .opc2 = 1,
      .access = PL2_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPPMCR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPPMCR2_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 1,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPPMCR4_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 4,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPPMCR5_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 5,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPPMCR6_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 2, .opc2 = 6,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUACTLR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 4, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "ATCR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 7, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPSELR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPCR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 1,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPOR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 2,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPMR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 3,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPOR2_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 4,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPMR2_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 5,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPUPFR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 8, .opc2 = 6,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    /*
     * Report CPUCFR_EL1.SCU as 1, as we do not implement the DSU
     * (and in particular its system registers).
     */
    { .name = "CPUCFR_EL1", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 0, .crn = 15, .crm = 0, .opc2 = 0,
      .access = PL1_R, .type = ARM_CP_CONST, .resetvalue = 4 },

    /*
     * Stub RAMINDEX, as we don't actually implement caches, BTB,
     * or anything else with cpu internal memory.
     * "Read" zeros into the IDATA* and DDATA* output registers.
     */
    { .name = "RAMINDEX_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 1, .opc1 = 6, .crn = 15, .crm = 0, .opc2 = 0,
      .access = PL3_W, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "IDATA0_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 0, .opc2 = 0,
      .access = PL3_R, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "IDATA1_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 0, .opc2 = 1,
      .access = PL3_R, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "IDATA2_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 0, .opc2 = 2,
      .access = PL3_R, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "DDATA0_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 1, .opc2 = 0,
      .access = PL3_R, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "DDATA1_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 1, .opc2 = 1,
      .access = PL3_R, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "DDATA2_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 1, .opc2 = 2,
      .access = PL3_R, .type = ARM_CP_CONST, .resetvalue = 0 },
};


/* Extra IMPDEF regs in the N2 beyond those in the A710 */
static const ARMCPRegInfo __attribute__((unused)) neoverse_n2_cp_reginfo[] = {
    { .name = "CPURNDBR_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 3, .opc2 = 0,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
    { .name = "CPURNDPEID_EL3", .state = ARM_CP_STATE_AA64,
      .opc0 = 3, .opc1 = 6, .crn = 15, .crm = 3, .opc2 = 1,
      .access = PL3_RW, .type = ARM_CP_CONST, .resetvalue = 0 },
};


/*
 * -cpu max: a CPU with as many features enabled as our emulation supports.
 * The version of '-cpu max' for qemu-system-arm is defined in cpu32.c;
 * this only needs to handle 64 bits.
 */
void aarch64_max_tcg_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    ARMISARegisters *isar = &cpu->isar;
    uint64_t t;
    uint32_t u;

    /*
     * Unset ARM_FEATURE_BACKCOMPAT_CNTFRQ, which we would otherwise default
     * to because we started with aarch64_a57_initfn(). A 'max' CPU might
     * be a v8.6-or-later one, in which case the cntfrq must be 1GHz; and
     * because it is our "may change" CPU type we are OK with it not being
     * backwards-compatible with how it worked in old QEMU.
     */
    unset_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);

    /*
     * Reset MIDR so the guest doesn't mistake our 'max' CPU type for a real
     * one and try to apply errata workarounds or use impdef features we
     * don't provide.
     * An IMPLEMENTER field of 0 means "reserved for software use";
     * ARCHITECTURE must be 0xf indicating "v7 or later, check ID registers
     * to see which features are present";
     * the VARIANT, PARTNUM and REVISION fields are all implementation
     * defined and we choose to define PARTNUM just in case guest
     * code needs to distinguish this QEMU CPU from other software
     * implementations, though this shouldn't be needed.
     */
    t = FIELD_DP64(0, MIDR_EL1, IMPLEMENTER, 0);
    t = FIELD_DP64(t, MIDR_EL1, ARCHITECTURE, 0xf);
    t = FIELD_DP64(t, MIDR_EL1, PARTNUM, 'Q');
    t = FIELD_DP64(t, MIDR_EL1, VARIANT, 0);
    t = FIELD_DP64(t, MIDR_EL1, REVISION, 0);
    cpu->midr = t;

    /*
     * We're going to set FEAT_S2FWB, which mandates that CLIDR_EL1.{LoUU,LoUIS}
     * are zero.
     */
    u = GET_IDREG(isar, CLIDR);
    u = FIELD_DP32(u, CLIDR_EL1, LOUIS, 0);
    u = FIELD_DP32(u, CLIDR_EL1, LOUU, 0);
    SET_IDREG(isar, CLIDR, u);

    /*
     * Set CTR_EL0.DIC and IDC to tell the guest it doesnt' need to
     * do any cache maintenance for data-to-instruction or
     * instruction-to-guest coherence. (Our cache ops are nops.)
     */
    t = cpu->ctr;
    t = FIELD_DP64(t, CTR_EL0, IDC, 1);
    t = FIELD_DP64(t, CTR_EL0, DIC, 1);
    cpu->ctr = t;

    t = GET_IDREG(isar, ID_AA64ISAR0);
    t = FIELD_DP64(t, ID_AA64ISAR0, AES, 2);      /* FEAT_PMULL */
    t = FIELD_DP64(t, ID_AA64ISAR0, SHA1, 1);     /* FEAT_SHA1 */
    t = FIELD_DP64(t, ID_AA64ISAR0, SHA2, 2);     /* FEAT_SHA512 */
    t = FIELD_DP64(t, ID_AA64ISAR0, CRC32, 1);    /* FEAT_CRC32 */
    t = FIELD_DP64(t, ID_AA64ISAR0, ATOMIC, 3);   /* FEAT_LSE, FEAT_LSE128 */
    t = FIELD_DP64(t, ID_AA64ISAR0, RDM, 1);      /* FEAT_RDM */
    t = FIELD_DP64(t, ID_AA64ISAR0, SHA3, 1);     /* FEAT_SHA3 */
    t = FIELD_DP64(t, ID_AA64ISAR0, SM3, 1);      /* FEAT_SM3 */
    t = FIELD_DP64(t, ID_AA64ISAR0, SM4, 1);      /* FEAT_SM4 */
    t = FIELD_DP64(t, ID_AA64ISAR0, DP, 1);       /* FEAT_DotProd */
    t = FIELD_DP64(t, ID_AA64ISAR0, FHM, 1);      /* FEAT_FHM */
    t = FIELD_DP64(t, ID_AA64ISAR0, TS, 2);       /* FEAT_FlagM2 */
    t = FIELD_DP64(t, ID_AA64ISAR0, TLB, 2);      /* FEAT_TLBIRANGE */
    t = FIELD_DP64(t, ID_AA64ISAR0, RNDR, 1);     /* FEAT_RNG */
    SET_IDREG(isar, ID_AA64ISAR0, t);

    t = GET_IDREG(isar, ID_AA64ISAR1);
    t = FIELD_DP64(t, ID_AA64ISAR1, DPB, 2);      /* FEAT_DPB2 */
    t = FIELD_DP64(t, ID_AA64ISAR1, APA, PauthFeat_FPACCOMBINED);
    t = FIELD_DP64(t, ID_AA64ISAR1, API, 1);
    t = FIELD_DP64(t, ID_AA64ISAR1, JSCVT, 1);    /* FEAT_JSCVT */
    t = FIELD_DP64(t, ID_AA64ISAR1, FCMA, 1);     /* FEAT_FCMA */
    t = FIELD_DP64(t, ID_AA64ISAR1, LRCPC, 2);    /* FEAT_LRCPC2 */
    t = FIELD_DP64(t, ID_AA64ISAR1, FRINTTS, 1);  /* FEAT_FRINTTS */
    t = FIELD_DP64(t, ID_AA64ISAR1, SB, 1);       /* FEAT_SB */
    t = FIELD_DP64(t, ID_AA64ISAR1, SPECRES, 1);  /* FEAT_SPECRES */
    t = FIELD_DP64(t, ID_AA64ISAR1, BF16, 2);     /* FEAT_BF16, FEAT_EBF16 */
    t = FIELD_DP64(t, ID_AA64ISAR1, DGH, 1);      /* FEAT_DGH */
    t = FIELD_DP64(t, ID_AA64ISAR1, I8MM, 1);     /* FEAT_I8MM */
    t = FIELD_DP64(t, ID_AA64ISAR1, XS, 1);       /* FEAT_XS */
    SET_IDREG(isar, ID_AA64ISAR1, t);

    t = GET_IDREG(isar, ID_AA64ISAR2);
    t = FIELD_DP64(t, ID_AA64ISAR2, RPRES, 1);    /* FEAT_RPRES */
    t = FIELD_DP64(t, ID_AA64ISAR2, MOPS, 1);     /* FEAT_MOPS */
    t = FIELD_DP64(t, ID_AA64ISAR2, BC, 1);       /* FEAT_HBC */
    t = FIELD_DP64(t, ID_AA64ISAR2, WFXT, 2);     /* FEAT_WFxT */
    t = FIELD_DP64(t, ID_AA64ISAR2, CSSC, 1);     /* FEAT_CSSC */
    t = FIELD_DP64(t, ID_AA64ISAR2, ATS1A, 1);    /* FEAT_ATS1A */
    SET_IDREG(isar, ID_AA64ISAR2, t);

    t = GET_IDREG(isar, ID_AA64PFR0);
    t = FIELD_DP64(t, ID_AA64PFR0, FP, 1);        /* FEAT_FP16 */
    t = FIELD_DP64(t, ID_AA64PFR0, ADVSIMD, 1);   /* FEAT_FP16 */
    t = FIELD_DP64(t, ID_AA64PFR0, RAS, 2);       /* FEAT_RASv1p1 + FEAT_DoubleFault */
    t = FIELD_DP64(t, ID_AA64PFR0, SVE, 1);
    t = FIELD_DP64(t, ID_AA64PFR0, SEL2, 1);      /* FEAT_SEL2 */
    t = FIELD_DP64(t, ID_AA64PFR0, DIT, 1);       /* FEAT_DIT */
    t = FIELD_DP64(t, ID_AA64PFR0, CSV2, 3);      /* FEAT_CSV2_3 */
    t = FIELD_DP64(t, ID_AA64PFR0, CSV3, 1);      /* FEAT_CSV3 */
    SET_IDREG(isar, ID_AA64PFR0, t);

    t = GET_IDREG(isar, ID_AA64PFR1);
    t = FIELD_DP64(t, ID_AA64PFR1, BT, 1);        /* FEAT_BTI */
    t = FIELD_DP64(t, ID_AA64PFR1, SSBS, 2);      /* FEAT_SSBS2 */
    /*
     * Begin with full support for MTE. This will be downgraded to MTE=0
     * during realize if the board provides no tag memory, much like
     * we do for EL2 with the virtualization=on property.
     */
    t = FIELD_DP64(t, ID_AA64PFR1, MTE, 3);       /* FEAT_MTE3 */
    t = FIELD_DP64(t, ID_AA64PFR1, RAS_FRAC, 0);  /* FEAT_RASv1p1 + FEAT_DoubleFault */
    t = FIELD_DP64(t, ID_AA64PFR1, SME, 2);       /* FEAT_SME2 */
    t = FIELD_DP64(t, ID_AA64PFR1, CSV2_FRAC, 0); /* FEAT_CSV2_3 */
    t = FIELD_DP64(t, ID_AA64PFR1, NMI, 1);       /* FEAT_NMI */
    t = FIELD_DP64(t, ID_AA64PFR1, GCS, 1);       /* FEAT_GCS */
    SET_IDREG(isar, ID_AA64PFR1, t);

    t = GET_IDREG(isar, ID_AA64MMFR0);
    t = FIELD_DP64(t, ID_AA64MMFR0, PARANGE, 6); /* FEAT_LPA: 52 bits */
    t = FIELD_DP64(t, ID_AA64MMFR0, TGRAN16, 1);   /* 16k pages supported */
    t = FIELD_DP64(t, ID_AA64MMFR0, TGRAN16_2, 2); /* 16k stage2 supported */
    t = FIELD_DP64(t, ID_AA64MMFR0, TGRAN64_2, 2); /* 64k stage2 supported */
    t = FIELD_DP64(t, ID_AA64MMFR0, TGRAN4_2, 2);  /*  4k stage2 supported */
    t = FIELD_DP64(t, ID_AA64MMFR0, FGT, 1);       /* FEAT_FGT */
    t = FIELD_DP64(t, ID_AA64MMFR0, ECV, 2);       /* FEAT_ECV */
    SET_IDREG(isar, ID_AA64MMFR0, t);

    t = GET_IDREG(isar, ID_AA64MMFR1);
    t = FIELD_DP64(t, ID_AA64MMFR1, HAFDBS, 2);   /* FEAT_HAFDBS */
    t = FIELD_DP64(t, ID_AA64MMFR1, VMIDBITS, 2); /* FEAT_VMID16 */
    t = FIELD_DP64(t, ID_AA64MMFR1, VH, 1);       /* FEAT_VHE */
    t = FIELD_DP64(t, ID_AA64MMFR1, HPDS, 2);     /* FEAT_HPDS2 */
    t = FIELD_DP64(t, ID_AA64MMFR1, LO, 1);       /* FEAT_LOR */
    t = FIELD_DP64(t, ID_AA64MMFR1, PAN, 3);      /* FEAT_PAN3 */
    t = FIELD_DP64(t, ID_AA64MMFR1, XNX, 1);      /* FEAT_XNX */
    t = FIELD_DP64(t, ID_AA64MMFR1, ETS, 2);      /* FEAT_ETS2 */
    t = FIELD_DP64(t, ID_AA64MMFR1, HCX, 1);      /* FEAT_HCX */
    t = FIELD_DP64(t, ID_AA64MMFR1, AFP, 1);      /* FEAT_AFP */
    t = FIELD_DP64(t, ID_AA64MMFR1, TIDCP1, 1);   /* FEAT_TIDCP1 */
    t = FIELD_DP64(t, ID_AA64MMFR1, CMOW, 1);     /* FEAT_CMOW */
    SET_IDREG(isar, ID_AA64MMFR1, t);

    t = GET_IDREG(isar, ID_AA64MMFR2);
    t = FIELD_DP64(t, ID_AA64MMFR2, CNP, 1);      /* FEAT_TTCNP */
    t = FIELD_DP64(t, ID_AA64MMFR2, UAO, 1);      /* FEAT_UAO */
    t = FIELD_DP64(t, ID_AA64MMFR2, IESB, 1);     /* FEAT_IESB */
    t = FIELD_DP64(t, ID_AA64MMFR2, VARANGE, 1);  /* FEAT_LVA */
    t = FIELD_DP64(t, ID_AA64MMFR2, NV, 2);       /* FEAT_NV2 */
    t = FIELD_DP64(t, ID_AA64MMFR2, ST, 1);       /* FEAT_TTST */
    t = FIELD_DP64(t, ID_AA64MMFR2, AT, 1);       /* FEAT_LSE2 */
    t = FIELD_DP64(t, ID_AA64MMFR2, IDS, 1);      /* FEAT_IDST */
    t = FIELD_DP64(t, ID_AA64MMFR2, FWB, 1);      /* FEAT_S2FWB */
    t = FIELD_DP64(t, ID_AA64MMFR2, TTL, 1);      /* FEAT_TTL */
    t = FIELD_DP64(t, ID_AA64MMFR2, BBM, 2);      /* FEAT_BBM at level 2 */
    t = FIELD_DP64(t, ID_AA64MMFR2, EVT, 2);      /* FEAT_EVT */
    t = FIELD_DP64(t, ID_AA64MMFR2, E0PD, 1);     /* FEAT_E0PD */
    SET_IDREG(isar, ID_AA64MMFR2, t);

    t = GET_IDREG(isar, ID_AA64MMFR3);
    t = FIELD_DP64(t, ID_AA64MMFR3, TCRX, 1);       /* FEAT_TCR2 */
    t = FIELD_DP64(t, ID_AA64MMFR3, SCTLRX, 1);     /* FEAT_SCTLR2 */
    t = FIELD_DP64(t, ID_AA64MMFR3, MEC, 1);        /* FEAT_MEC */
    t = FIELD_DP64(t, ID_AA64MMFR3, SPEC_FPACC, 1); /* FEAT_FPACC_SPEC */
    t = FIELD_DP64(t, ID_AA64MMFR3, S1PIE, 1);    /* FEAT_S1PIE */
    t = FIELD_DP64(t, ID_AA64MMFR3, S2PIE, 1);    /* FEAT_S2PIE */
    t = FIELD_DP64(t, ID_AA64MMFR3, AIE, 1);      /* FEAT_AIE */
    SET_IDREG(isar, ID_AA64MMFR3, t);

    t = GET_IDREG(isar, ID_AA64MMFR4);
    t = FIELD_DP64(t, ID_AA64MMFR4, ASID2, 1);    /* FEAT_ASID2 */
    SET_IDREG(isar, ID_AA64MMFR4, t);

    t = GET_IDREG(isar, ID_AA64ZFR0);
    t = FIELD_DP64(t, ID_AA64ZFR0, SVEVER, 2);    /* FEAT_SVE2p1 */
    t = FIELD_DP64(t, ID_AA64ZFR0, AES, 2);       /* FEAT_SVE_PMULL128 */
    t = FIELD_DP64(t, ID_AA64ZFR0, BITPERM, 1);   /* FEAT_SVE_BitPerm */
    t = FIELD_DP64(t, ID_AA64ZFR0, BFLOAT16, 2);  /* FEAT_BF16, FEAT_EBF16 */
    t = FIELD_DP64(t, ID_AA64ZFR0, B16B16, 1);    /* FEAT_SVE_B16B16 */
    t = FIELD_DP64(t, ID_AA64ZFR0, SHA3, 1);      /* FEAT_SVE_SHA3 */
    t = FIELD_DP64(t, ID_AA64ZFR0, SM4, 1);       /* FEAT_SVE_SM4 */
    t = FIELD_DP64(t, ID_AA64ZFR0, I8MM, 1);      /* FEAT_I8MM */
    t = FIELD_DP64(t, ID_AA64ZFR0, F32MM, 1);     /* FEAT_F32MM */
    t = FIELD_DP64(t, ID_AA64ZFR0, F64MM, 1);     /* FEAT_F64MM */
    SET_IDREG(isar, ID_AA64ZFR0, t);

    t = GET_IDREG(isar, ID_AA64DFR0);
    t = FIELD_DP64(t, ID_AA64DFR0, DEBUGVER, 10); /* FEAT_Debugv8p8 */
    t = FIELD_DP64(t, ID_AA64DFR0, PMUVER, 6);    /* FEAT_PMUv3p5 */
    t = FIELD_DP64(t, ID_AA64DFR0, HPMN0, 1);     /* FEAT_HPMN0 */
    SET_IDREG(isar, ID_AA64DFR0, t);

    t = GET_IDREG(isar, ID_AA64SMFR0);
    t = FIELD_DP64(t, ID_AA64SMFR0, F32F32, 1);   /* FEAT_SME */
    t = FIELD_DP64(t, ID_AA64SMFR0, BI32I32, 1);  /* FEAT_SME2 */
    t = FIELD_DP64(t, ID_AA64SMFR0, B16F32, 1);   /* FEAT_SME */
    t = FIELD_DP64(t, ID_AA64SMFR0, F16F32, 1);   /* FEAT_SME */
    t = FIELD_DP64(t, ID_AA64SMFR0, I8I32, 0xf);  /* FEAT_SME */
    t = FIELD_DP64(t, ID_AA64SMFR0, F16F16, 1);   /* FEAT_SME_F16F16 */
    t = FIELD_DP64(t, ID_AA64SMFR0, B16B16, 1);   /* FEAT_SME_B16B16 */
    t = FIELD_DP64(t, ID_AA64SMFR0, I16I32, 5);   /* FEAT_SME2 */
    t = FIELD_DP64(t, ID_AA64SMFR0, F64F64, 1);   /* FEAT_SME_F64F64 */
    t = FIELD_DP64(t, ID_AA64SMFR0, I16I64, 0xf); /* FEAT_SME_I16I64 */
    t = FIELD_DP64(t, ID_AA64SMFR0, SMEVER, 2);   /* FEAT_SME2p1 */
    t = FIELD_DP64(t, ID_AA64SMFR0, FA64, 1);     /* FEAT_SME_FA64 */
    SET_IDREG(isar, ID_AA64SMFR0, t);

    /* Replicate the same data to the 32-bit id registers.  */
    aa32_max_features(cpu);

#ifdef CONFIG_USER_ONLY
    /*
     * For usermode -cpu max we can use a larger and more efficient DCZ
     * blocksize since we don't have to follow what the hardware does.
     */
    cpu->ctr = 0x80038003; /* 32 byte I and D cacheline size, VIPT icache */
    set_dczid_bs(cpu, 7); /*  512 bytes */
#endif
    cpu->gm_blocksize = 6;  /*  256 bytes */

    cpu->sve_vq.supported = MAKE_64BIT_MASK(0, ARM_MAX_VQ);
    cpu->sme_vq.supported = SVE_VQ_POW2_MAP;

    aarch64_add_pauth_properties(obj);
    aarch64_add_sve_properties(obj);
    aarch64_add_sme_properties(obj);
    object_property_add(obj, "sve-max-vq", "uint32", cpu_max_get_sve_max_vq,
                        cpu_max_set_sve_max_vq, NULL, NULL);
    object_property_add_bool(obj, "x-rme", cpu_arm_get_rme, cpu_arm_set_rme);
    object_property_add(obj, "x-l0gptsz", "uint32", cpu_max_get_l0gptsz,
                        cpu_max_set_l0gptsz, NULL, NULL);
    qdev_property_add_static(DEVICE(obj), &arm_cpu_lpa2_property);
}

static void aarch64_a57_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    ARMISARegisters *isar = &cpu->isar;

    cpu->dtb_compatible = "arm,cortex-a57";
    set_feature(&cpu->env, ARM_FEATURE_V8);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    set_feature(&cpu->env, ARM_FEATURE_AARCH64);
    set_feature(&cpu->env, ARM_FEATURE_CBAR_RO);
    set_feature(&cpu->env, ARM_FEATURE_EL2);
    set_feature(&cpu->env, ARM_FEATURE_EL3);
    set_feature(&cpu->env, ARM_FEATURE_PMU);
    cpu->kvm_target = QEMU_KVM_ARM_TARGET_CORTEX_A57;
    cpu->midr = 0x411fd070;
    cpu->revidr = 0x00000000;
    cpu->reset_fpsid = 0x41034070;
    cpu->isar.mvfr0 = 0x10110222;
    cpu->isar.mvfr1 = 0x12111111;
    cpu->isar.mvfr2 = 0x00000043;
    cpu->ctr = 0x8444c004;
    cpu->reset_sctlr = 0x00c50838;
    SET_IDREG(isar, ID_PFR0, 0x00000131);
    SET_IDREG(isar, ID_PFR1, 0x00011011);
    SET_IDREG(isar, ID_DFR0, 0x03010066);
    SET_IDREG(isar, ID_AFR0, 0x00000000);
    SET_IDREG(isar, ID_MMFR0, 0x10101105);
    SET_IDREG(isar, ID_MMFR1, 0x40000000);
    SET_IDREG(isar, ID_MMFR2, 0x01260000);
    SET_IDREG(isar, ID_MMFR3, 0x02102211);
    SET_IDREG(isar, ID_ISAR0, 0x02101110);
    SET_IDREG(isar, ID_ISAR1, 0x13112111);
    SET_IDREG(isar, ID_ISAR2, 0x21232042);
    SET_IDREG(isar, ID_ISAR3, 0x01112131);
    SET_IDREG(isar, ID_ISAR4, 0x00011142);
    SET_IDREG(isar, ID_ISAR5, 0x00011121);
    SET_IDREG(isar, ID_ISAR6, 0);
    SET_IDREG(isar, ID_AA64PFR0, 0x00002222);
    SET_IDREG(isar, ID_AA64DFR0, 0x10305106);
    SET_IDREG(isar, ID_AA64ISAR0, 0x00011120);
    SET_IDREG(isar, ID_AA64MMFR0, 0x00001124);
    cpu->isar.dbgdidr = 0x3516d000;
    cpu->isar.dbgdevid = 0x01110f13;
    cpu->isar.dbgdevid1 = 0x2;
    cpu->isar.reset_pmcr_el0 = 0x41013000;
    SET_IDREG(isar, CLIDR, 0x0a200023);
    /* 32KB L1 dcache */
    cpu->ccsidr[0] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 64, 32 * KiB, 7);
    /* 48KB L1 icache */
    cpu->ccsidr[1] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 3, 64, 48 * KiB, 2);
    /* 2048KB L2 cache */
    cpu->ccsidr[2] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 16, 64, 2 * MiB, 7);
    set_dczid_bs(cpu, 4); /* 64 bytes */
    cpu->gic_num_lrs = 4;
    cpu->gic_vpribits = 5;
    cpu->gic_vprebits = 5;
    cpu->gic_pribits = 5;
    define_cortex_a72_a57_a53_cp_reginfo(cpu);
}

static void aarch64_a53_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    ARMISARegisters *isar = &cpu->isar;

    cpu->dtb_compatible = "arm,cortex-a53";
    set_feature(&cpu->env, ARM_FEATURE_V8);
    set_feature(&cpu->env, ARM_FEATURE_NEON);
    set_feature(&cpu->env, ARM_FEATURE_GENERIC_TIMER);
    set_feature(&cpu->env, ARM_FEATURE_BACKCOMPAT_CNTFRQ);
    set_feature(&cpu->env, ARM_FEATURE_AARCH64);
    set_feature(&cpu->env, ARM_FEATURE_CBAR_RO);
    set_feature(&cpu->env, ARM_FEATURE_EL2);
    set_feature(&cpu->env, ARM_FEATURE_EL3);
    set_feature(&cpu->env, ARM_FEATURE_PMU);
    cpu->kvm_target = QEMU_KVM_ARM_TARGET_CORTEX_A53;
    cpu->midr = 0x410fd034;
    cpu->revidr = 0x00000100;
    cpu->reset_fpsid = 0x41034070;
    cpu->isar.mvfr0 = 0x10110222;
    cpu->isar.mvfr1 = 0x12111111;
    cpu->isar.mvfr2 = 0x00000043;
    cpu->ctr = 0x84448004; /* L1Ip = VIPT */
    cpu->reset_sctlr = 0x00c50838;
    SET_IDREG(isar, ID_PFR0, 0x00000131);
    SET_IDREG(isar, ID_PFR1, 0x00011011);
    SET_IDREG(isar, ID_DFR0, 0x03010066);
    SET_IDREG(isar, ID_AFR0, 0x00000000);
    SET_IDREG(isar, ID_MMFR0, 0x10101105);
    SET_IDREG(isar, ID_MMFR1, 0x40000000);
    SET_IDREG(isar, ID_MMFR2, 0x01260000);
    SET_IDREG(isar, ID_MMFR3, 0x02102211);
    SET_IDREG(isar, ID_ISAR0, 0x02101110);
    SET_IDREG(isar, ID_ISAR1, 0x13112111);
    SET_IDREG(isar, ID_ISAR2, 0x21232042);
    SET_IDREG(isar, ID_ISAR3, 0x01112131);
    SET_IDREG(isar, ID_ISAR4, 0x00011142);
    SET_IDREG(isar, ID_ISAR5, 0x00011121);
    SET_IDREG(isar, ID_ISAR6, 0);
    SET_IDREG(isar, ID_AA64PFR0, 0x00002222);
    SET_IDREG(isar, ID_AA64DFR0, 0x10305106);
    SET_IDREG(isar, ID_AA64ISAR0, 0x00011120);
    SET_IDREG(isar, ID_AA64MMFR0, 0x00001122); /* 40 bit physical addr */
    cpu->isar.dbgdidr = 0x3516d000;
    cpu->isar.dbgdevid = 0x00110f13;
    cpu->isar.dbgdevid1 = 0x1;
    cpu->isar.reset_pmcr_el0 = 0x41033000;
    SET_IDREG(isar, CLIDR, 0x0a200023);
    /* 32KB L1 dcache */
    cpu->ccsidr[0] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 4, 64, 32 * KiB, 7);
    /* 32KB L1 icache */
    cpu->ccsidr[1] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 1, 64, 32 * KiB, 2);
    /* 1024KB L2 cache */
    cpu->ccsidr[2] = make_ccsidr(CCSIDR_FORMAT_LEGACY, 16, 64, 1 * MiB, 7);
    set_dczid_bs(cpu, 4); /* 64 bytes */
    cpu->gic_num_lrs = 4;
    cpu->gic_vpribits = 5;
    cpu->gic_vprebits = 5;
    cpu->gic_pribits = 5;
    define_cortex_a72_a57_a53_cp_reginfo(cpu);
}




static void aarch64_max_initfn(Object *obj)
{
    /* TCG 'max' CPU: start with A57 feature set then add all TCG extensions */
    aarch64_a57_initfn(obj);
    aarch64_max_tcg_initfn(obj);
}

static void __attribute__((unused)) aarch64_host_initfn(Object *obj)
{
    /* In TCG mode, host == max */
    aarch64_max_initfn(obj);
}

/* ---- Cortex-A55 (small efficient core) ---- */
static void aarch64_a55_initfn(Object *obj)
{
    aarch64_a53_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a55";
    cpu->midr = 0x410FD050; /* ARM Cortex-A55 r0p0 */
}
/* ---- Cortex-A72 ---- */
static void aarch64_a72_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a72";
    cpu->midr = 0x410FD080; /* ARM Cortex-A72 r0p0 */
}
/* ---- Cortex-A75 ---- */
static void aarch64_a75_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a75";
    cpu->midr = 0x410FD0A0; /* ARM Cortex-A75 r0p0 */
}
/* ---- Cortex-A76 ---- */
static void aarch64_a76_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x410FD0B0; /* ARM Cortex-A76 r0p0 */
}
/* ---- Cortex-A77 ---- */
static void aarch64_a77_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x410FD0D0; /* ARM Cortex-A77 r0p0 */
}
/* ---- Cortex-A78 ---- */
static void aarch64_a78_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x410FD410; /* ARM Cortex-A78 r0p0 */
}
/* ---- Cortex-A710 ---- */
static void aarch64_a710_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a710";
    cpu->midr = 0x410FD470; /* ARM Cortex-A710 r0p0 */
}
/* ---- Cortex-A715 ---- */
static void aarch64_a715_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x410FD4D0; /* ARM Cortex-A715 r0p0 */
}
/* ---- Cortex-X1 ---- */
static void aarch64_x1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x1";
    cpu->midr = 0x410FE010; /* ARM Cortex-X1 r0p0 */
}
/* ---- Cortex-X2 ---- */
static void aarch64_x2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x2";
    cpu->midr = 0x410FE170; /* ARM Cortex-X2 r0p0 */
}
/* ---- Cortex-X3 ---- */
static void aarch64_x3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x3";
    cpu->midr = 0x410FE330; /* ARM Cortex-X3 r0p0 */
}
/* ---- Neoverse N1 (server) ---- */
static void aarch64_neoverse_n1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-n1";
    cpu->midr = 0x410FD0C1; /* ARM Neoverse-N1 r3p1 */
}
/* ---- Neoverse N2 (server, SVE2) ---- */
static void aarch64_neoverse_n2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-n2";
    cpu->midr = 0x410FD490; /* ARM Neoverse-N2 r0p0 */
}
/* ---- Neoverse V1 (server, SVE 512-bit) ---- */
static void aarch64_neoverse_v1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-v1";
    cpu->midr = 0x410FD402; /* ARM Neoverse-V1 r1p1 */
}
/* ---- Neoverse V2 (server, SVE2 256-bit) ---- */
static void aarch64_neoverse_v2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-v2";
    cpu->midr = 0x410FE170; /* ARM Neoverse-V2 r0p0 */
}
/* ---- Qualcomm Kryo 485 Gold (Snapdragon 855, SM8150) ---- */
static void aarch64_kryo485_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo485";
    cpu->midr = 0x518F802D; /* Qualcomm Kryo 485 Gold */
}
/* ---- Qualcomm Kryo 585 Gold (Snapdragon 865, SM8250) ---- */
static void aarch64_kryo585_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo585";
    cpu->midr = 0x512F803D; /* Qualcomm Kryo 585 */
}
/* ---- Qualcomm Oryon (Snapdragon X Elite, X1E80100) ---- */
static void aarch64_oryon_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,oryon";
    cpu->midr = 0x518F02B5; /* Qualcomm Oryon (SA8775P / X Elite) */
}
/* ---- MediaTek Cortex-A78 based (Dimensity 9000, MT6983) ---- */
static void aarch64_mt_dimensity9000_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD410; /* Cortex-A78 as used in Dimensity 9000 */
}
/* ---- MediaTek Cortex-A715 based (Dimensity 9200, MT6985) ---- */
static void aarch64_mt_dimensity9200_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D0; /* Cortex-A715 as used in Dimensity 9200 */
}
/* ---- Samsung Exynos M5 (Exynos 990, Mongoose 5) ---- */
static void aarch64_exynos_m5_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,mongoose-m5";
    cpu->midr = 0x530F0014; /* Samsung Mongoose M5 r0p4 */
}
/* ---- Samsung Exynos X1 (Exynos 2100, Cortex-X1 based) ---- */
static void aarch64_exynos_x1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x1";
    cpu->midr = 0x530FE010; /* Cortex-X1 in Samsung Exynos 2100 */
}
/* ---- Samsung Exynos X4 (Exynos 2400, custom X4 core) ---- */
static void aarch64_exynos_x4_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x530FE610; /* Cortex-X4 variant in Exynos 2400 */
}
/* ---- Microsoft Azure Cobalt 100 (Neoverse N2 based) ---- */
static void aarch64_cobalt100_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-n2";
    cpu->midr = 0xB40FD490; /* Microsoft Azure Cobalt 100 (Neoverse N2 variant) */
}
/* ---- Ampere Altra (AmpereOne predecessor, Neoverse N1 based) ---- */
static void aarch64_altra_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,emag";
    cpu->midr = 0xC00FD0C1; /* Ampere Altra / eMAG */
}
/* ---- Ampere AmpereOne (custom ARMv8.6+ core) ---- */
static void aarch64_ampereone_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,ampereone";
    cpu->midr = 0xC00FAC30; /* AmpereOne */
}
static void aarch64_apple_a14_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,icestorm";
    cpu->midr = 0x610f0220; /* Apple Icestorm (A14 efficiency core) */
}
/* ---- Apple A15 Bionic ---- */
static void aarch64_apple_a15_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,everest";
    cpu->midr = 0x610f0250; /* Apple A15 Avalanche core */
}
/* ---- Apple A16 Bionic ---- */
static void aarch64_apple_a16_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,everest";
    cpu->midr = 0x610f0290; /* Apple A16 Everest core */
}
/* ---- Apple A17 Pro ---- */
static void aarch64_apple_a17_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,everest";
    cpu->midr = 0x610f0320; /* Apple A17 Pro core */
}
static void aarch64_apple_m1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,firestorm";
    cpu->midr = 0x610f0230; /* Apple Firestorm (M1 performance core) */
}
static void aarch64_apple_m2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,avalanche";
    cpu->midr = 0x610f0250; /* Apple Avalanche (M2 performance core) */
}
/* ---- Apple M3 (Everest) ---- */
static void aarch64_apple_m3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,everest";
    cpu->midr = 0x610f0290; /* Apple Everest (M3 performance core) */
}
/* ---- Apple M4 ---- */
static void aarch64_apple_m4_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,m4";
    cpu->midr = 0x610f0360; /* Apple M4 performance core */
}

/* ---- Apple M1 Pro (higher-perf Firestorm, more cores) ---- */
static void aarch64_apple_m1_pro_initfn(Object *obj)
{
    aarch64_apple_m1_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,firestorm";
    cpu->midr = 0x610f0231; /* M1 Pro Firestorm r1p0 */
}
/* ---- Apple M1 Max ---- */
static void aarch64_apple_m1_max_initfn(Object *obj)
{
    aarch64_apple_m1_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610f0232; /* M1 Max */
}
/* ---- Apple M2 Pro ---- */
static void aarch64_apple_m2_pro_initfn(Object *obj)
{
    aarch64_apple_m2_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,avalanche";
    cpu->midr = 0x610f0251; /* M2 Pro */
}
/* ---- Apple M2 Max ---- */
static void aarch64_apple_m2_max_initfn(Object *obj)
{
    aarch64_apple_m2_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610f0252; /* M2 Max */
}
/* ---- Apple M2 Ultra ---- */
static void aarch64_apple_m2_ultra_initfn(Object *obj)
{
    aarch64_apple_m2_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610f0253; /* M2 Ultra (2x M2 Max dies) */
}
/* ---- Apple M3 Pro ---- */
static void aarch64_apple_m3_pro_initfn(Object *obj)
{
    aarch64_apple_m3_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,everest";
    cpu->midr = 0x610f0291; /* M3 Pro */
}
/* ---- Apple M3 Max ---- */
static void aarch64_apple_m3_max_initfn(Object *obj)
{
    aarch64_apple_m3_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610f0292; /* M3 Max */
}

/* ---- Qualcomm Kryo 260 (Snapdragon 660, SDM660) ---- */
static void aarch64_snapdragon660_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo260";
    cpu->midr = 0x512F8014; /* Kryo 260 */
}
/* ---- Qualcomm Kryo 360 (Snapdragon 710, SDM710) ---- */
static void aarch64_snapdragon710_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo360";
    cpu->midr = 0x512F8020; /* Kryo 360 */
}
/* ---- Qualcomm Kryo 470 Gold (Snapdragon 730, SM7150) ---- */
static void aarch64_snapdragon730_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo470";
    cpu->midr = 0x512F8024; /* Kryo 470 Gold */
}
/* ---- Qualcomm Kryo 485 Prime (Snapdragon 855+, SM8150-AC) ---- */
static void aarch64_snapdragon855plus_initfn(Object *obj)
{
    aarch64_kryo485_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x518F802E; /* Kryo 485 Prime */
}
/* ---- Qualcomm Kryo 560 (Snapdragon 870, SM8250-AC) ---- */
static void aarch64_snapdragon870_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo560";
    cpu->midr = 0x512F803E; /* Kryo 560 Prime */
}
/* ---- Qualcomm Oryon (Snapdragon 8cx Gen 3, SC8280XP) ---- */
static void aarch64_snapdragon8cxgen3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,sc8280xp";
    cpu->midr = 0x518F02B0; /* Kryo Prime in 8cx Gen3 */
}
/* ---- Qualcomm Oryon (Snapdragon 8 Gen 1, SM8450) ---- */
static void aarch64_snapdragon8gen1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo780";
    cpu->midr = 0x519F0400; /* Cortex-X2 based in SD 8 Gen 1 */
}
/* ---- Qualcomm Oryon (Snapdragon 8 Gen 2, SM8550) ---- */
static void aarch64_snapdragon8gen2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,kryo780";
    cpu->midr = 0x519F0500; /* Cortex-X3 based in SD 8 Gen 2 */
}
/* ---- Qualcomm Oryon (Snapdragon 8 Gen 3, SM8650) ---- */
static void aarch64_snapdragon8gen3_initfn(Object *obj)
{
    aarch64_oryon_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x518F02B6; /* Oryon V1 in SD 8 Gen 3 */
}
/* ---- Qualcomm Oryon (Snapdragon X Plus, X1P64100) ---- */
static void aarch64_snapdragon_xplus_initfn(Object *obj)
{
    aarch64_oryon_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x518F02B3; /* Oryon in SD X Plus */
}


/* ---- Apple A7 Cyclone (iPhone 5s, iPad Air, 2013) ---- */
static void aarch64_apple_a7_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,cyclone";
    cpu->midr = 0x610F0010; /* Apple Cyclone r0p0 PartNum=0x001 */
}
/* ---- Apple A8 Typhoon (iPhone 6, 2014) ---- */
static void aarch64_apple_a8_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,typhoon";
    cpu->midr = 0x610F0020; /* Apple Typhoon PartNum=0x002 */
}
/* ---- Apple A9 Twister (iPhone 6s, 2015) ---- */
static void aarch64_apple_a9_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,twister";
    cpu->midr = 0x610F00B0; /* Apple Twister PartNum=0x00B */
}
/* ---- Apple A10 Fusion Hurricane (iPhone 7, 2016) ---- */
static void aarch64_apple_a10_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,hurricane";
    cpu->midr = 0x610F00F0; /* Apple Hurricane PartNum=0x00F */
}
/* ---- Apple A11 Bionic Monsoon (iPhone 8/X, 2017) ---- */
static void aarch64_apple_a11_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,monsoon";
    cpu->midr = 0x610F0100; /* Apple Monsoon PartNum=0x010 */
}
/* ---- Apple A12 Bionic Vortex (iPhone XS, 2018) ---- */
static void aarch64_apple_a12_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,vortex";
    cpu->midr = 0x610F0110; /* Apple Vortex PartNum=0x011 */
}
/* ---- Apple A13 Bionic Lightning (iPhone 11, 2019) ---- */
static void aarch64_apple_a13_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,lightning";
    cpu->midr = 0x610F0120; /* Apple Lightning PartNum=0x012 */
}
/* ---- Apple M4 Max (Hammerhead, more cores) ---- */
static void aarch64_apple_m4_max_initfn(Object *obj)
{
    aarch64_apple_m4_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610F0370; /* M4 Max PartNum=0x037 */
}
/* ---- Apple M4 Ultra (2x M4 Max dies) ---- */
static void aarch64_apple_m4_ultra_initfn(Object *obj)
{
    aarch64_apple_m4_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610F0380; /* M4 Ultra PartNum=0x038 (estimated) */
}
/* ---- Apple M5 (estimated, 2025+) ---- */
static void aarch64_apple_m5_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,m5";
    cpu->midr = 0x610F0400; /* M5 PartNum=0x040 (estimated) */
}
/* ---- Apple M5 Pro ---- */
static void aarch64_apple_m5_pro_initfn(Object *obj)
{
    aarch64_apple_m5_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610F0410; /* M5 Pro (estimated) */
}
/* ---- Apple M5 Max ---- */
static void aarch64_apple_m5_max_initfn(Object *obj)
{
    aarch64_apple_m5_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->midr = 0x610F0420; /* M5 Max (estimated) */
}
/* ---- Apple M5 Ultra ---- */
static void aarch64_apple_m5_ultra_initfn(Object *obj)
{
    aarch64_apple_m5_initfn(obj);
     ARMCPU *cpu = ARM_CPU(obj);
     cpu->midr = 0x610F0430; /* M5 Ultra (estimated) */
}

/* ============================================================
 * Additional phone/tablet SoC CPU models (v1.36)
 * ============================================================ */

/* Qualcomm Snapdragon 8 Gen 4 (Oryon 2, SM8750, 2024) */
static void aarch64_snapdragon8gen4_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,oryon2";
    cpu->midr = 0x518F02C0; /* Oryon V2 Gold / SM8750 */
}

/* Qualcomm Snapdragon 7s Gen 2 (Cortex-A715 based, SM7435) */
static void aarch64_snapdragon7sgen2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D1; /* Cortex-A715 in SD 7s Gen 2 */
}

/* Qualcomm Snapdragon 4 Gen 2 (Cortex-A78 based, SM4450) */
static void aarch64_snapdragon4gen2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD410; /* Cortex-A78 in SD 4 Gen 2 */
}

/* MediaTek Dimensity 9300 (MT6989, Cortex-X4 based, 2023) */
static void aarch64_mt_dimensity9300_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x412FD640; /* Cortex-X4 in Dimensity 9300 */
}

/* MediaTek Dimensity 9400 (MT6991, Cortex-X925 based, 2024) */
static void aarch64_mt_dimensity9400_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x925";
    cpu->midr = 0x412FD870; /* Cortex-X925 in Dimensity 9400 */
}

/* MediaTek Dimensity 8300 (MT6896, Cortex-A715 based, 2023) */
static void aarch64_mt_dimensity8300_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D2; /* Cortex-A715 in Dimensity 8300 */
}

/* Samsung Exynos 2500 (Cortex-X5 based, estimated) */
static void aarch64_exynos2500_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x5";
    cpu->midr = 0x530FE710; /* Cortex-X5 in Exynos 2500 (estimated) */
}

/* HiSilicon Kirin 9000 (Cortex-A77 based, 2020) */
static void aarch64_kirin9000_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x414FD0B0; /* Cortex-A77 r0p0 in Kirin 9000 */
}

/* HiSilicon Kirin 9010 (Kirin 9000s, Cortex-A77, 2023) */
static void aarch64_kirin9010_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x414FD0B1; /* Cortex-A77 variant in Kirin 9010 */
}

/* Apple A18 Bionic (iPhone 16, 2024) */
static void aarch64_apple_a18_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,coll";
    cpu->midr = 0x610F0290; /* Apple A18 Spindrift/Coll (estimated) */
}

/* Apple A18 Pro (iPhone 16 Pro, 2024) */
static void aarch64_apple_a18pro_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,tahiti";
    cpu->midr = 0x610F02A0; /* Apple A18 Pro Tahiti (estimated) */
}

/* Apple A16 Bionic (iPhone 14 Pro, 2022) */
static void __attribute__((unused)) aarch64_apple_a16_extra_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,everest";
    cpu->midr = 0x610F0260; /* Apple Everest (A16) */
}

/* Google Tensor G1 (Whitechapel, Cortex-X1 based, Pixel 6, 2021) */
static void aarch64_tensor_g1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x1";
    cpu->midr = 0x514F0101; /* Cortex-X1 in Google Tensor G1 */
}

/* Google Tensor G2 (Cloudripper, Cortex-X1 based, Pixel 7, 2022) */
static void aarch64_tensor_g2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x1c";
    cpu->midr = 0x514F0110; /* Cortex-X1C in Tensor G2 */
}

/* Google Tensor G3 (Zuma, Cortex-X3 based, Pixel 8, 2023) */
static void aarch64_tensor_g3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x3";
    cpu->midr = 0x514F0120; /* Cortex-X3 in Tensor G3 */
}

/* Google Tensor G4 (Zuma Pro, Pixel 9, 2024) */
static void aarch64_tensor_g4_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x514F0130; /* Cortex-X4 in Tensor G4 */
}

/* UNISOC Tiger T820 (Cortex-A75, budget SoC) */
static void aarch64_unisoc_t820_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a75";
    cpu->midr = 0x413FD0A1; /* Cortex-A75 r0p1 in UNISOC T820 */
}

/* ============================================================
 * v1.37: Even more phone/tablet SoC CPU models
 * ============================================================ */

/* Qualcomm Snapdragon 8 Elite (Oryon 2 prime, SM8750-AB, 2024) */
static void aarch64_snapdragon8elite_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,oryon2";
    cpu->midr = 0x518F02C5; /* Oryon V2 Prime in SD 8 Elite */
}

/* Qualcomm Snapdragon 8+ Gen 3 (SM8635, Cortex-X4 prime) */
static void aarch64_snapdragon8plusgen3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x519F0600; /* Cortex-X4 in SD 8+ Gen 3 */
}

/* Qualcomm Snapdragon 7 Gen 3 (SM7435-AB, Cortex-A715) */
static void aarch64_snapdragon7gen3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D3; /* Cortex-A715 in SD 7 Gen 3 */
}

/* Qualcomm Snapdragon 7+ Gen 3 (SM7675, Cortex-X2 prime) */
static void aarch64_snapdragon7plusgen3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x2";
    cpu->midr = 0x519F0410; /* Cortex-X2 in SD 7+ Gen 3 */
}

/* Qualcomm Snapdragon 6 Gen 1 (SM6450, Cortex-A78) */
static void aarch64_snapdragon6gen1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD412; /* Cortex-A78 in SD 6 Gen 1 */
}

/* Qualcomm Snapdragon 6 Gen 3 (SM6475, Cortex-A715) */
static void aarch64_snapdragon6gen3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D4; /* Cortex-A715 in SD 6 Gen 3 */
}

/* Qualcomm Snapdragon X Elite 2 (Oryon 3, SC8890, 2025) */
static void aarch64_snapdragon_x_elite2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,oryon3";
    cpu->midr = 0x518F02D0; /* Oryon V3 in SD X Elite 2 */
}

/* MediaTek Dimensity 9200+ (MT6985Z, Cortex-X3 prime) */
static void aarch64_mt_dimensity9200plus_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x3";
    cpu->midr = 0x412FD5E1; /* Cortex-X3 in Dimensity 9200+ */
}

/* MediaTek Dimensity 8200 (MT6895, Cortex-A78) */
static void aarch64_mt_dimensity8200_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD411; /* Cortex-A78 in Dimensity 8200 */
}

/* MediaTek Dimensity 7200 (MT6881, Cortex-A715) */
static void aarch64_mt_dimensity7200_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D5; /* Cortex-A715 in Dimensity 7200 */
}

/* MediaTek Dimensity 7020 (MT6883V/TO, Cortex-A78) */
static void aarch64_mt_dimensity7020_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD413; /* Cortex-A78 in Dimensity 7020 */
}

/* MediaTek Kompanio 1380 (MT8188T, Cortex-A78, Chromebook) */
static void aarch64_mt_kompanio1380_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD415; /* Cortex-A78 in Kompanio 1380 */
}

/* MediaTek Kompanio 1200 (MT8195, Cortex-A78) */
static void aarch64_mt_kompanio1200_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD416; /* Cortex-A78 in Kompanio 1200 */
}

/* Samsung Exynos 1280 (Galaxy A55, Cortex-A78) */
static void aarch64_exynos1280_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x530F5001; /* Samsung custom A78 in Exynos 1280 */
}

/* Samsung Exynos 1380 (Galaxy A54, Cortex-A78) */
static void aarch64_exynos1380_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x530F5002; /* Samsung custom A78 in Exynos 1380 */
}

/* Samsung Exynos 1480 (Galaxy A55 2024, Cortex-A78) */
static void aarch64_exynos1480_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x530F5003; /* Samsung custom A78 in Exynos 1480 */
}

/* Samsung Exynos 1580 (Galaxy A56 2025, Cortex-A720) */
static void aarch64_exynos1580_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a720";
    cpu->midr = 0x530F5100; /* Cortex-A720 in Exynos 1580 */
}

/* HiSilicon Kirin 820 (Cortex-A76, 2020 mid-range) */
static void aarch64_kirin820_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B2; /* Cortex-A76 in Kirin 820 */
}

/* HiSilicon Kirin 830 (Cortex-A710, 2023 mid-range) */
static void aarch64_kirin830_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a710";
    cpu->midr = 0x412FD4A0; /* Cortex-A710 in Kirin 830 */
}

/* HiSilicon Kirin 9100 (Cortex-X3 based, 2024 flagship) */
static void aarch64_kirin9100_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x3";
    cpu->midr = 0x414FD5E2; /* Cortex-X3 in Kirin 9100 */
}

/* UNISOC Tiger T618 (Cortex-A75, budget tablet SoC) */
static void aarch64_unisoc_t618_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a75";
    cpu->midr = 0x413FD0A2; /* Cortex-A75 r0p2 in UNISOC T618 */
}

/* UNISOC Tiger T880 (Cortex-A76, mid-range 2024) */
static void aarch64_unisoc_t880_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B3; /* Cortex-A76 in UNISOC T880 */
}

/* Rockchip RK3588 (Cortex-A710 + A710 + A510, 2022 flagship) */
static void aarch64_rk3588_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a710";
    cpu->midr = 0x412FD4A1; /* Cortex-A710 in RK3588 */
}

/* Rockchip RK3588S (same cores, smaller package) */
static void aarch64_rk3588s_initfn(Object *obj)
{
    aarch64_rk3588_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3588s";
}

/* Rockchip RK3566 (Cortex-A55, SBC/tablet) */
static void aarch64_rk3566_initfn(Object *obj)
{
    aarch64_a55_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3566";
}

/* Rockchip RK3568 (Cortex-A55, SBC/IoT) */
static void aarch64_rk3568_initfn(Object *obj)
{
    aarch64_a55_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3568";
}

/* Google Tensor G5 (Laguna, 2025, Cortex-X5 based estimated) */
static void aarch64_tensor_g5_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x5";
    cpu->midr = 0x514F0140; /* Cortex-X5 in Tensor G5 (estimated) */
}

/* Apple A17 Pro (iPhone 15 Pro, 2023) */
static void aarch64_apple_a17pro_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,collish";
    cpu->midr = 0x610F0280; /* Apple A17 Pro (estimated) */
}

/* Apple A16 Bionic (iPhone 14, 2022) - non-Pro variant */
static void aarch64_apple_a16_bionic_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,napa";
    cpu->midr = 0x610F0261; /* Apple Napa (A16 non-Pro) */
}

/* Apple A19 (estimated, iPhone 17, 2025) */
static void aarch64_apple_a19_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,granita";
    cpu->midr = 0x610F02B0; /* Apple A19 (estimated) */
}

/* Apple A19 Pro (estimated, iPhone 17 Pro, 2025) */
static void aarch64_apple_a19pro_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,vesuvio";
    cpu->midr = 0x610F02B5; /* Apple A19 Pro (estimated) */
}

/* ============================================================
 * v1.38: More ARM64 SoC CPU models + ARM reference cores
 * ============================================================ */

/* ARM reference cores */
static void aarch64_a520_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a520";
    cpu->midr = 0x412FD8D0; /* Cortex-A520 r0p0 */
}
static void aarch64_a725_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a725";
    cpu->midr = 0x412FD8E0; /* Cortex-A725 r0p0 */
}
static void aarch64_x4_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x412FD640; /* Cortex-X4 r0p0 */
}
static void aarch64_x5_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x5";
    cpu->midr = 0x412FD870; /* Cortex-X5 r0p0 */
}
static void aarch64_neoverse_n3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-n3";
    cpu->midr = 0x412FE500; /* Neoverse N3 */
}
static void aarch64_neoverse_v3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-v3";
    cpu->midr = 0x412FE510; /* Neoverse V3 */
}
static void aarch64_neoverse_v3p_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-v3p";
    cpu->midr = 0x412FE511; /* Neoverse V3P */
}

/* Qualcomm Snapdragon 8s Gen 3 (SM8635, Cortex-X4 prime) */
static void aarch64_snapdragon8sgen3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x519F0610; /* Cortex-X4 in SD 8s Gen 3 */
}
/* Qualcomm Snapdragon 8s Gen 2 (SM8735, Cortex-X3) */
static void aarch64_snapdragon8sgen2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x3";
    cpu->midr = 0x519F0520; /* Cortex-X3 in SD 8s Gen 2 */
}
/* Qualcomm Snapdragon 7 Gen 4 (SM7550, Cortex-A720) */
static void aarch64_snapdragon7gen4_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a720";
    cpu->midr = 0x412FD8E1; /* Cortex-A720 in SD 7 Gen 4 */
}
/* Qualcomm Snapdragon 7+ Gen 2 (SM7475, Cortex-A710) */
static void aarch64_snapdragon7plusgen2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a710";
    cpu->midr = 0x412FD4A2; /* Cortex-A710 in SD 7+ Gen 2 */
}
/* Qualcomm Snapdragon 6s Gen 3 (SM6375, Cortex-A78) */
static void aarch64_snapdragon6sgen3_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD414; /* Cortex-A78 in SD 6s Gen 3 */
}
/* Qualcomm Snapdragon 4 Gen 1 (SM4350, Cortex-A78) */
static void aarch64_snapdragon4gen1_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD415; /* Cortex-A78 in SD 4 Gen 1 */
}
/* Qualcomm Snapdragon 480 (SM4350, Cortex-A76) */
static void aarch64_snapdragon480_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B4; /* Cortex-A76 in SD 480 */
}
/* Qualcomm Snapdragon 750G (SM7225, Cortex-A77) */
static void aarch64_snapdragon750g_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x414FD0C0; /* Cortex-A77 in SD 750G */
}
/* Qualcomm Snapdragon 765G (SM7250, Cortex-A76) */
static void aarch64_snapdragon765g_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B5; /* Cortex-A76 in SD 765G */
}
/* Qualcomm Snapdragon 720G (SM7125, Cortex-A76) */
static void aarch64_snapdragon720g_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B6; /* Cortex-A76 in SD 720G */
}
/* Qualcomm Snapdragon 662 (SM6115, Cortex-A73) */
static void aarch64_snapdragon662_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a73";
    cpu->midr = 0x411FD090; /* Cortex-A73 in SD 662 */
}
/* Qualcomm Snapdragon 460 (SM6115, Cortex-A73) */
static void aarch64_snapdragon460_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a73";
    cpu->midr = 0x411FD091; /* Cortex-A73 in SD 460 */
}
/* Qualcomm Snapdragon 439 (MSM8937, Cortex-A53) */
static void aarch64_snapdragon439_initfn(Object *obj)
{
    aarch64_a55_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,msm8937";
    cpu->midr = 0x410FD034; /* Cortex-A53 in SD 439 */
}
/* Qualcomm Snapdragon X2 (Oryon 3, SC8990, 2025 laptop) */
static void aarch64_snapdragon_x2_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,oryon3";
    cpu->midr = 0x518F02D6; /* Oryon V3 in SD X2 */
}
static void aarch64_mt_dimensity9300plus_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x412FD641; /* Cortex-X4 in Dimensity 9300+ */
}
static void aarch64_mt_dimensity8400_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a725";
    cpu->midr = 0x412FD8E2; /* Cortex-A725 in Dimensity 8400 */
}
static void aarch64_mt_dimensity8300ultra_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D6; /* Cortex-A715 in Dimensity 8300-Ultra */
}
static void aarch64_mt_dimensity7300_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a715";
    cpu->midr = 0x412FD4D7; /* Cortex-A715 in Dimensity 7300 */
}
static void aarch64_mt_dimensity7050_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a710";
    cpu->midr = 0x412FD4A3; /* Cortex-A710 in Dimensity 7050 */
}
static void aarch64_mt_dimensity6080_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B7; /* Cortex-A76 in Dimensity 6080 */
}
static void aarch64_mt_dimensity6050_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a75";
    cpu->midr = 0x413FD0A3; /* Cortex-A75 in Dimensity 6050 */
}
static void aarch64_mt_kompanio830_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x412FD417; /* Cortex-A78 in Kompanio 830 */
}
static void aarch64_mt_kompanio520_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B8; /* Cortex-A76 in Kompanio 520 */
}
/* Samsung Exynos 2200 (AMD RDNA2 GPU, Cortex-X2) */
static void aarch64_exynos2200_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x2";
    cpu->midr = 0x530FE020; /* Cortex-X2 in Exynos 2200 */
}
/* Samsung Exynos 2400e (lower binned 2400) */
static void aarch64_exynos2400e_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x4";
    cpu->midr = 0x530FE611; /* Cortex-X4 in Exynos 2400e */
}
/* Samsung Exynos 1480i (budget variant) */
static void aarch64_exynos1480i_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a78";
    cpu->midr = 0x530F5004; /* Samsung custom A78 in Exynos 1480i */
}
/* Samsung Exynos 850 (Cortex-A55, budget Galaxy A12/A22) */
static void aarch64_exynos850_initfn(Object *obj)
{
    aarch64_a55_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "samsung,exynos850";
    cpu->midr = 0x410FD051; /* Cortex-A55 in Exynos 850 */
}
/* HiSilicon Kirin 9100+ */
static void aarch64_kirin9100plus_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-x3";
    cpu->midr = 0x414FD5E3; /* Cortex-X3 in Kirin 9100+ */
}
/* HiSilicon Kirin 990 (Cortex-A76, 2019 flagship) */
static void aarch64_kirin990_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0C2; /* Cortex-A76 in Kirin 990 */
}
/* HiSilicon Kirin 985 (Cortex-A77, 2020) */
static void aarch64_kirin985_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x414FD0C3; /* Cortex-A77 in Kirin 985 */
}
static void aarch64_kirin9000s_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x414FD0B4; /* Cortex-A77 in Kirin 9000s (SMIC) */
}
static void aarch64_kirin9000e_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x414FD0B5; /* Kirin 9000E (lower bin) */
}
static void aarch64_kirin9000l_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a77";
    cpu->midr = 0x414FD0B6; /* Kirin 9000L */
}
/* UNISOC T760 (Cortex-A76, mid-range 2024) */
static void aarch64_unisoc_t760_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0B9; /* Cortex-A76 in UNISOC T760 */
}
static void aarch64_unisoc_t770_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a76";
    cpu->midr = 0x413FD0BA; /* Cortex-A76 in UNISOC T770 */
}
static void aarch64_unisoc_t606_initfn(Object *obj)
{
    aarch64_a55_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a75";
    cpu->midr = 0x413FD0A4; /* Cortex-A55 cluster in UNISOC T606 */
}
static void aarch64_unisoc_t616_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,cortex-a75";
    cpu->midr = 0x413FD0A5; /* Cortex-A75 in UNISOC T616 */
}
/* Rockchip RK3576 (Cortex-A72, mid-range SBC 2024) */
static void aarch64_rk3576_initfn(Object *obj)
{
    aarch64_a72_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3576";
}
/* Rockchip RK3528 (Cortex-A53, budget SBC) */
static void aarch64_rk3528_initfn(Object *obj)
{
    aarch64_a55_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3528";
}
/* Rockchip RK3399 (Cortex-A72, classic SBC 2016) */
static void aarch64_rk3399_initfn(Object *obj)
{
    aarch64_a72_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3399";
}
/* NVIDIA Tegra T234 (Orin, Carmel-8, Jetson Orin) */
static void aarch64_tegra_t234_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nvidia,carmel";
    cpu->midr = 0x4E0F0050; /* NVIDIA Carmel in Tegra T234 */
}
/* NVIDIA Tegra T210 (Denver+A57, Jetson Nano) */
static void aarch64_tegra_t210_initfn(Object *obj)
{
    aarch64_a57_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nvidia,tegra210";
}
/* Qualcomm QCS8550 (robotics/IoT variant of SD 855) */
static void aarch64_qcs8550_initfn(Object *obj)
{
    aarch64_kryo485_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,qcs8550";
}
/* Qualcomm SA8775P (automotive, Cortex-A710) */
static void aarch64_sa8775p_initfn(Object *obj)
{
    aarch64_max_initfn(obj);
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qcom,sa8775p";
    cpu->midr = 0x518F02B7; /* Oryon variant in SA8775P */
}

static void aarch64_nvidia_grace_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nvidia,grace";
    cpu->midr = 0x610F0000;
    cpu->sve_max_vq = 2;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 2, &error_abort);
}

static void aarch64_nvidia_grace_hopper_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "nvidia,grace-hopper";
    cpu->midr = 0x610F0001;
    cpu->sve_max_vq = 2;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 2, &error_abort);
}

static void aarch64_graviton3_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "amazon,graviton3";
    cpu->midr = 0xd0c00000;
    cpu->sve_max_vq = 4;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}

static void aarch64_graviton3e_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "amazon,graviton3e";
    cpu->midr = 0xd0c00002;
    cpu->sve_max_vq = 4;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}

static void aarch64_graviton4_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "amazon,graviton4";
    cpu->midr = 0xd0c00001;
    cpu->sve_max_vq = 4;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}

static void aarch64_kunpeng920_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "hisilicon,kunpeng920";
    cpu->midr = 0x690f0000;
    cpu->sve_max_vq = 4;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}

static void aarch64_kunpeng930_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "hisilicon,kunpeng930";
    cpu->midr = 0x690f0001;
    cpu->sve_max_vq = 4;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}

static void aarch64_thunderx2_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "marvell,thunderx2";
    cpu->midr = 0x0b07f000;
    cpu->sve_max_vq = 2;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 2, &error_abort);
}

static void aarch64_layerscape2088_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls2088a";
    cpu->midr = 0x0b70f000;
    cpu->ctr = 0x8444c004;
}

static void aarch64_layerscape1088_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls1088a";
    cpu->midr = 0x0b70f001;
    cpu->ctr = 0x8444c004;
}

static void aarch64_layerscape2048_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,lx2160a";
    cpu->midr = 0x0b70f002;
    cpu->sve_max_vq = 2;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 2, &error_abort);
}

static void aarch64_am654_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,am654";
    cpu->midr = 0x411fd000;
    cpu->ctr = 0x8444c004;
}

static void aarch64_dmp1k_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,dmp1k";
    cpu->midr = 0x411fd001;
    cpu->ctr = 0x8444c004;
}

static void aarch64_j721e_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,j721e";
    cpu->midr = 0x411fd002;
    cpu->ctr = 0x8444c004;
}

static void aarch64_altra_max_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,emag";
    cpu->midr = 0xc00fac0;
    cpu->sve_max_vq = 2;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 2, &error_abort);
}

static void aarch64_ampereone_192_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,ampereone";
    cpu->midr = 0xc00fac1;
    cpu->sve_max_vq = 4;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}

static void aarch64_ampereone_256_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,ampereone-256";
    cpu->midr = 0xc00fac2;
    cpu->sve_max_vq = 4;
    cpu->ctr = 0x8444c004;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}

#include "v15-arm64-cpus.inc.c"
#include "v152-arm64-cpus.inc.c"

static const ARMCPUInfo aarch64_cpus[] = {
    { .name = "cortex-a35",         .initfn = aarch64_a35_initfn },
    { .name = "cortex-a55",         .initfn = aarch64_a55_initfn },
    { .name = "cortex-a72",         .initfn = aarch64_a72_initfn },
    { .name = "cortex-a76",         .initfn = aarch64_a76_initfn },
    /*
     * The Cortex-A78AE differs slightly from the plain Cortex-A78. We don't
     * currently model the latter.
     */
    { .name = "cortex-a78ae",       .initfn = aarch64_a78ae_initfn },
    { .name = "cortex-a710",        .initfn = aarch64_a710_initfn },
    { .name = "a64fx",              .initfn = aarch64_a64fx_initfn },
    { .name = "neoverse-n1",        .initfn = aarch64_neoverse_n1_initfn },
    { .name = "neoverse-v1",        .initfn = aarch64_neoverse_v1_initfn },
    { .name = "neoverse-n2",        .initfn = aarch64_neoverse_n2_initfn },
    { .name = "cortex-a75",                     .initfn = aarch64_a75_initfn },
    { .name = "cortex-a77",                     .initfn = aarch64_a77_initfn },
    { .name = "cortex-a78",                     .initfn = aarch64_a78_initfn },
    { .name = "cortex-a715",                     .initfn = aarch64_a715_initfn },
    { .name = "cortex-x1",                     .initfn = aarch64_x1_initfn },
    { .name = "cortex-x2",                     .initfn = aarch64_x2_initfn },
    { .name = "cortex-x3",                     .initfn = aarch64_x3_initfn },
    { .name = "neoverse-v2",                     .initfn = aarch64_neoverse_v2_initfn },
    { .name = "snapdragon-855",                     .initfn = aarch64_kryo485_initfn },
    { .name = "snapdragon-865",                     .initfn = aarch64_kryo585_initfn },
    { .name = "snapdragon-x-elite",                     .initfn = aarch64_oryon_initfn },
    { .name = "dimensity-9000",                     .initfn = aarch64_mt_dimensity9000_initfn },
    { .name = "dimensity-9200",                     .initfn = aarch64_mt_dimensity9200_initfn },
    { .name = "exynos-990",                     .initfn = aarch64_exynos_m5_initfn },
    { .name = "exynos-2100",                     .initfn = aarch64_exynos_x1_initfn },
    { .name = "exynos-2400",                     .initfn = aarch64_exynos_x4_initfn },
    { .name = "azure-cobalt-100",                     .initfn = aarch64_cobalt100_initfn },
    { .name = "ampere-altra",                     .initfn = aarch64_altra_initfn },
    { .name = "ampere-one",                     .initfn = aarch64_ampereone_initfn },
    { .name = "apple-a14",                     .initfn = aarch64_apple_a14_initfn },
    { .name = "apple-a15",                     .initfn = aarch64_apple_a15_initfn },
    { .name = "apple-a16",                     .initfn = aarch64_apple_a16_initfn },
    { .name = "apple-a17",                     .initfn = aarch64_apple_a17_initfn },
    { .name = "apple-m1",                     .initfn = aarch64_apple_m1_initfn },
    { .name = "apple-m2",                     .initfn = aarch64_apple_m2_initfn },
    { .name = "apple-m3",                     .initfn = aarch64_apple_m3_initfn },
    /* Apple M variants */
    { .name = "apple-m1-pro",       .initfn = aarch64_apple_m1_pro_initfn },
    { .name = "apple-m1-max",       .initfn = aarch64_apple_m1_max_initfn },
    { .name = "apple-m2-pro",       .initfn = aarch64_apple_m2_pro_initfn },
    { .name = "apple-m2-max",       .initfn = aarch64_apple_m2_max_initfn },
    { .name = "apple-m2-ultra",     .initfn = aarch64_apple_m2_ultra_initfn },
    { .name = "apple-m3-pro",       .initfn = aarch64_apple_m3_pro_initfn },
    { .name = "apple-m3-max",       .initfn = aarch64_apple_m3_max_initfn },
    /* Snapdragon additional */
    { .name = "snapdragon-660",     .initfn = aarch64_snapdragon660_initfn },
    { .name = "snapdragon-710",     .initfn = aarch64_snapdragon710_initfn },
    { .name = "snapdragon-730",     .initfn = aarch64_snapdragon730_initfn },
    { .name = "snapdragon-855-plus",.initfn = aarch64_snapdragon855plus_initfn },
    { .name = "snapdragon-870",     .initfn = aarch64_snapdragon870_initfn },
    { .name = "snapdragon-8cx-gen3",.initfn = aarch64_snapdragon8cxgen3_initfn },
    { .name = "snapdragon-8-gen1",  .initfn = aarch64_snapdragon8gen1_initfn },
    { .name = "snapdragon-8-gen2",  .initfn = aarch64_snapdragon8gen2_initfn },
    { .name = "snapdragon-8-gen3",  .initfn = aarch64_snapdragon8gen3_initfn },
    { .name = "snapdragon-x-plus",  .initfn = aarch64_snapdragon_xplus_initfn },
    /* Apple early A-series */
    { .name = "apple-a7",           .initfn = aarch64_apple_a7_initfn },
    { .name = "apple-a8",           .initfn = aarch64_apple_a8_initfn },
    { .name = "apple-a9",           .initfn = aarch64_apple_a9_initfn },
    { .name = "apple-a10",          .initfn = aarch64_apple_a10_initfn },
    { .name = "apple-a11",          .initfn = aarch64_apple_a11_initfn },
    { .name = "apple-a12",          .initfn = aarch64_apple_a12_initfn },
    { .name = "apple-a13",          .initfn = aarch64_apple_a13_initfn },
    /* Apple M4 variants */
    { .name = "apple-m4-max",       .initfn = aarch64_apple_m4_max_initfn },
    { .name = "apple-m4-ultra",     .initfn = aarch64_apple_m4_ultra_initfn },
    /* Apple M5 (estimated) */
    { .name = "apple-m5",           .initfn = aarch64_apple_m5_initfn },
    { .name = "apple-m5-pro",       .initfn = aarch64_apple_m5_pro_initfn },
    { .name = "apple-m5-max",       .initfn = aarch64_apple_m5_max_initfn },
    { .name = "apple-m5-ultra",     .initfn = aarch64_apple_m5_ultra_initfn },
    { .name = "apple-m4",                     .initfn = aarch64_apple_m4_initfn },
    /* v1.36: additional phone/tablet SoC models */
    { .name = "snapdragon-8-gen4",  .initfn = aarch64_snapdragon8gen4_initfn },
    { .name = "snapdragon-7s-gen2", .initfn = aarch64_snapdragon7sgen2_initfn },
    { .name = "snapdragon-4-gen2",  .initfn = aarch64_snapdragon4gen2_initfn },
    { .name = "dimensity-9300",     .initfn = aarch64_mt_dimensity9300_initfn },
    { .name = "dimensity-9400",     .initfn = aarch64_mt_dimensity9400_initfn },
    { .name = "dimensity-8300",     .initfn = aarch64_mt_dimensity8300_initfn },
    { .name = "exynos-2500",        .initfn = aarch64_exynos2500_initfn },
    { .name = "kirin-9000",         .initfn = aarch64_kirin9000_initfn },
    { .name = "kirin-9010",         .initfn = aarch64_kirin9010_initfn },
    { .name = "apple-a18",          .initfn = aarch64_apple_a18_initfn },
    { .name = "apple-a18-pro",      .initfn = aarch64_apple_a18pro_initfn },
    { .name = "tensor-g1",          .initfn = aarch64_tensor_g1_initfn },
    { .name = "tensor-g2",          .initfn = aarch64_tensor_g2_initfn },
    { .name = "tensor-g3",          .initfn = aarch64_tensor_g3_initfn },
    { .name = "tensor-g4",          .initfn = aarch64_tensor_g4_initfn },
    { .name = "unisoc-t820",        .initfn = aarch64_unisoc_t820_initfn },
    /* v1.37: additional phone/tablet/SoC models */
    { .name = "snapdragon-8-elite",     .initfn = aarch64_snapdragon8elite_initfn },
    { .name = "snapdragon-8-plus-gen3", .initfn = aarch64_snapdragon8plusgen3_initfn },
    { .name = "snapdragon-7-gen3",      .initfn = aarch64_snapdragon7gen3_initfn },
    { .name = "snapdragon-7-plus-gen3", .initfn = aarch64_snapdragon7plusgen3_initfn },
    { .name = "snapdragon-6-gen1",      .initfn = aarch64_snapdragon6gen1_initfn },
    { .name = "snapdragon-6-gen3",      .initfn = aarch64_snapdragon6gen3_initfn },
    { .name = "snapdragon-x-elite-2",   .initfn = aarch64_snapdragon_x_elite2_initfn },
    { .name = "dimensity-9200-plus",    .initfn = aarch64_mt_dimensity9200plus_initfn },
    { .name = "dimensity-8200",         .initfn = aarch64_mt_dimensity8200_initfn },
    { .name = "dimensity-7200",         .initfn = aarch64_mt_dimensity7200_initfn },
    { .name = "dimensity-7020",         .initfn = aarch64_mt_dimensity7020_initfn },
    { .name = "kompanio-1380",          .initfn = aarch64_mt_kompanio1380_initfn },
    { .name = "kompanio-1200",          .initfn = aarch64_mt_kompanio1200_initfn },
    { .name = "exynos-1280",            .initfn = aarch64_exynos1280_initfn },
    { .name = "exynos-1380",            .initfn = aarch64_exynos1380_initfn },
    { .name = "exynos-1480",            .initfn = aarch64_exynos1480_initfn },
    { .name = "exynos-1580",            .initfn = aarch64_exynos1580_initfn },
    { .name = "kirin-820",              .initfn = aarch64_kirin820_initfn },
    { .name = "kirin-830",              .initfn = aarch64_kirin830_initfn },
    { .name = "kirin-9100",             .initfn = aarch64_kirin9100_initfn },
    { .name = "unisoc-t618",            .initfn = aarch64_unisoc_t618_initfn },
    { .name = "unisoc-t880",            .initfn = aarch64_unisoc_t880_initfn },
    { .name = "rk3588",                 .initfn = aarch64_rk3588_initfn },
    { .name = "rk3588s",                .initfn = aarch64_rk3588s_initfn },
    { .name = "rk3566",                 .initfn = aarch64_rk3566_initfn },
    { .name = "rk3568",                 .initfn = aarch64_rk3568_initfn },
    { .name = "tensor-g5",              .initfn = aarch64_tensor_g5_initfn },
    { .name = "apple-a17-pro",          .initfn = aarch64_apple_a17pro_initfn },
    { .name = "apple-a16-bionic",       .initfn = aarch64_apple_a16_bionic_initfn },
    { .name = "apple-a19",              .initfn = aarch64_apple_a19_initfn },
    { .name = "apple-a19-pro",          .initfn = aarch64_apple_a19pro_initfn },
    /* v1.38: even more ARM64 phone/tablet/SoC models */
    { .name = "snapdragon-8s-gen3",     .initfn = aarch64_snapdragon8sgen3_initfn },
    { .name = "snapdragon-8s-gen2",     .initfn = aarch64_snapdragon8sgen2_initfn },
    { .name = "snapdragon-7-gen4",      .initfn = aarch64_snapdragon7gen4_initfn },
    { .name = "snapdragon-7-plus-gen2", .initfn = aarch64_snapdragon7plusgen2_initfn },
    { .name = "snapdragon-6s-gen3",     .initfn = aarch64_snapdragon6sgen3_initfn },
    { .name = "snapdragon-4-gen1",      .initfn = aarch64_snapdragon4gen1_initfn },
    { .name = "snapdragon-480",         .initfn = aarch64_snapdragon480_initfn },
    { .name = "snapdragon-750g",        .initfn = aarch64_snapdragon750g_initfn },
    { .name = "snapdragon-765g",        .initfn = aarch64_snapdragon765g_initfn },
    { .name = "snapdragon-720g",        .initfn = aarch64_snapdragon720g_initfn },
    { .name = "snapdragon-662",         .initfn = aarch64_snapdragon662_initfn },
    { .name = "snapdragon-460",         .initfn = aarch64_snapdragon460_initfn },
    { .name = "snapdragon-439",         .initfn = aarch64_snapdragon439_initfn },
    { .name = "snapdragon-x2",          .initfn = aarch64_snapdragon_x2_initfn },
    { .name = "dimensity-9300-plus",    .initfn = aarch64_mt_dimensity9300plus_initfn },
    { .name = "dimensity-8400",         .initfn = aarch64_mt_dimensity8400_initfn },
    { .name = "dimensity-8300-ultra",   .initfn = aarch64_mt_dimensity8300ultra_initfn },
    { .name = "dimensity-7300",         .initfn = aarch64_mt_dimensity7300_initfn },
    { .name = "dimensity-7050",         .initfn = aarch64_mt_dimensity7050_initfn },
    { .name = "dimensity-6080",         .initfn = aarch64_mt_dimensity6080_initfn },
    { .name = "dimensity-6050",         .initfn = aarch64_mt_dimensity6050_initfn },
    { .name = "kompanio-830",           .initfn = aarch64_mt_kompanio830_initfn },
    { .name = "kompanio-520",           .initfn = aarch64_mt_kompanio520_initfn },
    { .name = "exynos-2200",            .initfn = aarch64_exynos2200_initfn },
    { .name = "exynos-2400e",           .initfn = aarch64_exynos2400e_initfn },
    { .name = "exynos-1480i",           .initfn = aarch64_exynos1480i_initfn },
    { .name = "exynos-850",             .initfn = aarch64_exynos850_initfn },
    { .name = "kirin-9100-plus",        .initfn = aarch64_kirin9100plus_initfn },
    { .name = "kirin-990",              .initfn = aarch64_kirin990_initfn },
    { .name = "kirin-985",              .initfn = aarch64_kirin985_initfn },
    { .name = "kirin-9000s",            .initfn = aarch64_kirin9000s_initfn },
    { .name = "kirin-9000e",            .initfn = aarch64_kirin9000e_initfn },
    { .name = "kirin-9000l",            .initfn = aarch64_kirin9000l_initfn },
    { .name = "unisoc-t760",            .initfn = aarch64_unisoc_t760_initfn },
    { .name = "unisoc-t770",            .initfn = aarch64_unisoc_t770_initfn },
    { .name = "unisoc-t606",            .initfn = aarch64_unisoc_t606_initfn },
    { .name = "unisoc-t616",            .initfn = aarch64_unisoc_t616_initfn },
    { .name = "rk3576",                 .initfn = aarch64_rk3576_initfn },
    { .name = "rk3528",                 .initfn = aarch64_rk3528_initfn },
    { .name = "rk3399",                 .initfn = aarch64_rk3399_initfn },
    { .name = "nvidia-tegra-t234",      .initfn = aarch64_tegra_t234_initfn },
    { .name = "nvidia-tegra-t210",      .initfn = aarch64_tegra_t210_initfn },
    { .name = "qualcomm-qcs8550",       .initfn = aarch64_qcs8550_initfn },
    { .name = "qualcomm-sa8775p",       .initfn = aarch64_sa8775p_initfn },
    { .name = "cortex-a520",            .initfn = aarch64_a520_initfn },
    { .name = "cortex-a725",            .initfn = aarch64_a725_initfn },
    { .name = "cortex-x4",              .initfn = aarch64_x4_initfn },
    { .name = "cortex-x5",              .initfn = aarch64_x5_initfn },
    { .name = "neoverse-n3",            .initfn = aarch64_neoverse_n3_initfn },
    { .name = "neoverse-v3",            .initfn = aarch64_neoverse_v3_initfn },
    { .name = "neoverse-v3p",           .initfn = aarch64_neoverse_v3p_initfn },
    /* v1.41: Server/embedded ARM64 CPUs */
    { .name = "nvidia-grace",           .initfn = aarch64_nvidia_grace_initfn },
    { .name = "nvidia-grace-hopper",    .initfn = aarch64_nvidia_grace_hopper_initfn },
    { .name = "graviton3",              .initfn = aarch64_graviton3_initfn },
    { .name = "graviton3e",            .initfn = aarch64_graviton3e_initfn },
    { .name = "graviton4",              .initfn = aarch64_graviton4_initfn },
    { .name = "kunpeng920",             .initfn = aarch64_kunpeng920_initfn },
    { .name = "kunpeng930",             .initfn = aarch64_kunpeng930_initfn },
    { .name = "thunderx2",              .initfn = aarch64_thunderx2_initfn },
    { .name = "layerscape2088",         .initfn = aarch64_layerscape2088_initfn },
    { .name = "layerscape1088",         .initfn = aarch64_layerscape1088_initfn },
    { .name = "layerscape2048",         .initfn = aarch64_layerscape2048_initfn },
    { .name = "am654",                  .initfn = aarch64_am654_initfn },
    { .name = "dmp1k",                  .initfn = aarch64_dmp1k_initfn },
    { .name = "j721e",                  .initfn = aarch64_j721e_initfn },
    { .name = "altra-max",              .initfn = aarch64_altra_max_initfn },
    { .name = "ampereone-192",          .initfn = aarch64_ampereone_192_initfn },
    { .name = "ampereone-256",          .initfn = aarch64_ampereone_256_initfn },
#include "v15-arm64-table.inc.c"
#include "v152-arm64-table.inc.c"
};

static void aarch64_cpu_register_types(void)
{
    size_t i;

    for (i = 0; i < ARRAY_SIZE(aarch64_cpus); ++i) {
        arm_cpu_register(&aarch64_cpus[i]);
    }
}

type_init(aarch64_cpu_register_types)
