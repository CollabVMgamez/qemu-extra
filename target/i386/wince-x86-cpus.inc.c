/*
 * x86 CPU models for Windows CE/Embedded x86 builds
 * AMD Geode, VIA Eden/C3/C7/Nano, Transmeta Crusoe/Efficeon,
 * Intel Pentium M, Celeron M, Atom, 486
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

    {
        .name = "AMD-Geode-GX-300",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 5,
        .model = 10,
        .stepping = 1,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "AMD Geode(TM) GX Processor",
    },
    {
        .name = "AMD-Geode-GX-366",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 5,
        .model = 10,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "AMD Geode(TM) GX 366MHz Processor",
    },
    {
        .name = "AMD-Geode-LX-500",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 5,
        .model = 10,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "AMD Geode(TM) LX 500MHz Processor",
    },
    {
        .name = "AMD-Geode-LX-600",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 5,
        .model = 10,
        .stepping = 4,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "AMD Geode(TM) LX 600MHz Processor",
    },
    {
        .name = "AMD-Geode-LX-800",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 5,
        .model = 10,
        .stepping = 5,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "AMD Geode(TM) LX 800MHz Processor",
    },
    {
        .name = "AMD-Geode-NX-1250",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 10,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "AMD Geode NX 1250@1500 Processor",
    },
    {
        .name = "AMD-Geode-NX-1500",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 10,
        .stepping = 1,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "AMD Geode NX 1500@1800 Processor",
    },
    {
        .name = "AMD-Elan-SC520",
        .level = 1,
        .xlevel = 0,
        .vendor = CPUID_VENDOR_AMD,
        .family = 4,
        .model = 9,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            0,
        .features[FEAT_8000_0001_ECX] =
            0,
        .model_id = "AMD Elan SC520",
    },
    {
        .name = "VIA-Eden-ESP-4000",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 7,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA Eden Processor 4000MHz",
    },
    {
        .name = "VIA-Eden-ESP-5000",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 7,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA Eden Processor 5000MHz",
    },
    {
        .name = "VIA-Eden-Nano-U2250",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 15,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_CX16 | CPUID_EXT_SSE41 | CPUID_EXT_POPCNT | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA Eden Nano U2250 1.0GHz",
    },
    {
        .name = "VIA-Eden-Nano-U2500",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 15,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_CX16 | CPUID_EXT_SSE41 | CPUID_EXT_POPCNT | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA Eden Nano U2500 1.2GHz",
    },
    {
        .name = "VIA-Eden-X2-U4200",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 15,
        .stepping = 4,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_CX16 | CPUID_EXT_SSE41 | CPUID_EXT_POPCNT | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA Eden X2 U4200 1.0GHz Dual",
    },
    {
        .name = "VIA-C3-Samuel2-733",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 7,
        .stepping = 1,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C3 733MHz Processor",
    },
    {
        .name = "VIA-C3-Ezra-800",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 7,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C3 800MHz Processor",
    },
    {
        .name = "VIA-C3-Nehemiah-1000",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 9,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C3 1GHz Processor",
    },
    {
        .name = "VIA-C7-M-770",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 10,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C7-M Processor 770MHz",
    },
    {
        .name = "VIA-C7-M-1200",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 10,
        .stepping = 1,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C7-M Processor 1200MHz",
    },
    {
        .name = "VIA-C7-D-1600",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 10,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C7-D Processor 1600MHz",
    },
    {
        .name = "VIA-C7-1500",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 10,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C7 Processor 1500MHz",
    },
    {
        .name = "VIA-C7-2000",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 10,
        .stepping = 4,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA C7 Processor 2000MHz",
    },
    {
        .name = "VIA-Nano-3000-U1700",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 15,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_CX16 | CPUID_EXT_SSE41 | CPUID_EXT_POPCNT | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA Nano 3000 U1700 1.0GHz",
    },
    {
        .name = "VIA-Nano-3000-U2700",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 6,
        .model = 15,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_CX16 | CPUID_EXT_SSE41 | CPUID_EXT_POPCNT | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "VIA Nano 3000 U2700 1.2GHz",
    },
    {
        .name = "Transmeta-Crusoe-TM3200",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 5,
        .model = 4,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Transmeta Crusoe TM3200",
    },
    {
        .name = "Transmeta-Crusoe-TM5400",
        .level = 5,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 5,
        .model = 4,
        .stepping = 1,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Transmeta Crusoe TM5400",
    },
    {
        .name = "Transmeta-Efficeon-TM8000",
        .level = 15,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_AMD,
        .family = 15,
        .model = 2,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL | CPUID_EXT2_MMXEXT | CPUID_EXT2_FFXSR | CPUID_EXT2_3DNOWEXT | CPUID_EXT2_3DNOW,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Transmeta Efficeon TM8000",
    },
    {
        .name = "Intel-Pentium-M-733",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 9,
        .stepping = 5,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Pentium(R) M processor 733",
    },
    {
        .name = "Intel-Atom-Z500",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU Z500",
    },
    {
        .name = "Intel-Atom-Z510",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU Z510",
    },
    {
        .name = "Intel-Atom-Z520",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU Z520",
    },
    {
        .name = "Intel-Atom-Z530",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU Z530",
    },
    {
        .name = "Intel-Atom-Z540",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 3,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU Z540",
    },
    {
        .name = "Intel-Atom-N270",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 2,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU N270",
    },
    {
        .name = "Intel-Atom-N450",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 10,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU N450",
    },
    {
        .name = "Intel-Atom-N455",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 10,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU N455",
    },
    {
        .name = "Intel-Atom-D510",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 10,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU D510",
    },
    {
        .name = "Intel-Atom-E640",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 10,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU E640",
    },
    {
        .name = "Intel-Atom-E680",
        .level = 10,
        .xlevel = 0x80000008,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 6,
        .model = 28,
        .stepping = 10,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC | CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT | CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            CPUID_EXT_SSE3 | CPUID_EXT_SSSE3 | CPUID_EXT_MONITOR | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] =
            CPUID_EXT2_LM | CPUID_EXT2_NX | CPUID_EXT2_SYSCALL,
        .features[FEAT_8000_0001_ECX] =
            CPUID_EXT3_LAHF_LM,
        .model_id = "Intel(R) Atom(TM) CPU E680",
    },
    {
        .name = "Intel-486DX-33",
        .level = 1,
        .xlevel = 0,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 4,
        .model = 3,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            0,
        .features[FEAT_8000_0001_ECX] =
            0,
        .model_id = "Intel486 DX 33MHz",
    },
    {
        .name = "Intel-486DX-50",
        .level = 1,
        .xlevel = 0,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 4,
        .model = 3,
        .stepping = 1,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            0,
        .features[FEAT_8000_0001_ECX] =
            0,
        .model_id = "Intel486 DX 50MHz",
    },
    {
        .name = "Intel-486DX2-66",
        .level = 1,
        .xlevel = 0,
        .vendor = CPUID_VENDOR_INTEL,
        .family = 4,
        .model = 4,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            0,
        .features[FEAT_8000_0001_ECX] =
            0,
        .model_id = "Intel486 DX2 66MHz",
    },
    {
        .name = "AMD-486DX2-66",
        .level = 1,
        .xlevel = 0,
        .vendor = CPUID_VENDOR_AMD,
        .family = 4,
        .model = 8,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            0,
        .features[FEAT_8000_0001_ECX] =
            0,
        .model_id = "AMD486DX2-66",
    },
    {
        .name = "AMD-486DX4-100",
        .level = 1,
        .xlevel = 0,
        .vendor = CPUID_VENDOR_AMD,
        .family = 4,
        .model = 8,
        .stepping = 1,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            0,
        .features[FEAT_8000_0001_ECX] =
            0,
        .model_id = "AMD486DX4-100",
    },
    {
        .name = "AMD-Elan-SC400",
        .level = 1,
        .xlevel = 0,
        .vendor = CPUID_VENDOR_AMD,
        .family = 4,
        .model = 5,
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] =
            CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE | CPUID_TSC | CPUID_MSR | CPUID_ACPI | CPUID_SS,
        .features[FEAT_1_ECX] =
            0,
        .features[FEAT_8000_0001_EDX] =
            0,
        .features[FEAT_8000_0001_ECX] =
            0,
        .model_id = "AMD Elan SC400",
    },