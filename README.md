# qemu-extra

A heavily enhanced fork of [QEMU](https://www.qemu.org/) with **4,500+ CPU models**, **5,200+ PCI device stubs**, **150+ machine types**, anti-VM detection countermeasures, fake RAM SMBIOS, multi-GPU emulation, and era-accurate hardware configurations for running retro operating systems, embedded platforms, and anti-fingerprint VMs.

---

## Key Features

| Feature | Count |
|---------|-------|
| x86 CPU models (Intel + AMD + VIA + Transmeta) | 4,200+ |
| ARM/ARM64 CPU models | 410+ |
| MIPS CPU models | 47 |
| PCI device stubs (GPUs, NPUs, TPUs, SSDs, NICs, audio, TPM, etc.) | 5,200+ |
| x86 machine types (q35, i440fx, P4-era, nForce, Mac) | 140+ |
| ARM machine types (virt, VExpress, WinCE devices, Mac) | 155+ |
| NVIDIA RTX/GTX/Quadro/Tesla GPU stubs (with multi-GPU) | 224 |
| Intel GPU/GNA/NPU stubs | 136 |
| AMD GPU/NPU stubs | 34 |
| Anti-VM detection features | 6 properties |
| Windows CE/Mobile ARM machines | 33 |

---

## Anti-VM & Hardware Spoofing

qemu-extra can hide virtualization artifacts from guest OS detection tools (CPU-Z, GPU-Z, HWiNFO, etc.):

| Property | Flag | Effect |
|----------|------|--------|
| Hide KVM CPUID | `hide-kvm-features=on` | Removes KVM signature from CPUID leaf 0x40000000 |
| Hide Hyper-V signature | `hide-hv-signature=on` | Removes Hyper-V enlightenments CPUID |
| Spoof BIOS vendor | `spoof-bios-vendor="..."` | Overrides SMBIOS Type 0 BIOS vendor string |
| Spoof BIOS version | `spoof-bios-version="..."` | Overrides SMBIOS Type 0 BIOS version string |
| Spoof board vendor | `spoof-board-vendor="..."` | Overrides SMBIOS Type 2 board manufacturer |
| Spoof board product | `spoof-board-product="..."` | Overrides SMBIOS Type 2 board product name |
| Fake RAM in SMBIOS | `fake-ram-mb=N` | SMBIOS shows N MB while `-m` allocates less |

**Example:** Make a VM look like an ASUS X670E with 32GB RAM while only allocating 512MB:
```
-machine q35,fake-ram-mb=32768,hide-kvm-features=on,hide-hv-signature=on,\
spoof-bios-vendor="American Megatrends International LLC",\
spoof-bios-version="2.17.19",spoof-board-vendor="ASUSTeK COMPUTER INC.",\
spoof-board-product="PRIME X670E-PRO"
```

---

## Multi-GPU Emulation

NVIDIA GPU stubs support the `gpu-count` property to create multiple GPU adapters in the guest:

```
-device nvidia-rtx5090,gpu-count=4
```

This creates 1 primary VGA-class GPU + 3 secondary 3D Controller class GPUs, matching how real multi-GPU systems appear in Device Manager. Works with all RTX/GTX/Quadro/Tesla stubs.

---

## CPU Models

### x86 (4,200+)

**Intel:**
- 486DX/DX2/DX4, Pentium, Pentium MMX, Pentium Pro
- Pentium II (Klamath/Deschutes/Xeon), Pentium III (Katmai/Coppermine/Tualatin)
- Pentium 4 (Willamette/Northwood/Prescott/Cedar Mill/Smithfield/Presler/Dempsey)
- Pentium M, Celeron M, Atom (Diamondville/Pineview/Cedarview/Silvermont)
- Core 2 Solo/Duo/Quad/Extreme (Conroe/Allendale/Wolfdale/Yorkfield/Penryn)
- Core i3/i5/i7/i9 (Nehalem through 14th gen Raptor Lake)
- Xeon (Prestonia/Nocona/Core/Westmere/Sandy Bridge/Ivy Bridge/Haswell/Broadwell/Skylake/Cascade Lake/Ice Lake/Sapphire Rapids/Emerald Rapids)
- Itanium note: QEMU has no ia64 target — Itanium is not emulatable

**AMD:**
- K5/K6/K6-2/K6-III, Athlon/Duron, Athlon 64/X2/FX/Opteron
- Phenom/Phenom II, Athlon II, Sempron, Turion
- Ryzen 1000–9000, Ryzen AI 300, Ryzen Threadripper (all generations)
- EPYC (Napoli/Rome/Milan/Genoa/Turin/Siena)
- Geode GX/LX/NX, Elan SC400/SC520

**VIA:**
- C3 (Samuel2/Ezra/Nehemiah), C7/C7-M, Eden/Eden-Nano/Nano-3000/X2

**Transmeta:**
- Crusoe TM3200/TM5400/TM5600/TM5800, Efficeon TM8000/TM8600/TM8800

### ARM/ARM64 (410+)

**Cortex series:** A5, A7, A8, A9, A15, A17, A32, A35, A53, A55, A57, A72, A76, A77, A78, A710, A715, A520, X1–X4, R5, R52

**Application CPUs:**
- Apple A7–A19, M1–M4 (Pro/Max/Ultra)
- Qualcomm Snapdragon (Scorpion/Krait/Kryo — MSM7xxx/QSD8xxx/835/845/888/8 Gen 1–3)
- Samsung Exynos (3110–2400)
- MediaTek Dimensity (700–9400)
- HiSilicon Kunpeng (920–930)
- Marvell Armada/PJ1/PJ4
- NVIDIA Tegra 2/3
- Freescale/NXP i.MX (21–93)
- XScale PXA (250–935), StrongARM SA-1110

**WinCE/Mobile SoCs:**
- Samsung S3C2410/2440/6410
- TI OMAP730/850/2420/3430/3630
- Cirrus EP93xx, Atmel AT91SAM9, NXP LPC3xxx
- VIA WonderMedia WM8505/8650/8850

### MIPS (47)

- SGI R4000/R4400/R4600
- NEC VR5432/VR7701
- MIPS 4Kc/4Km/4KEc/24Kc/24Kf/34Kf/74Kf
- M14K/M14Kc, P5600, I6400/I6500
- Ingenic XBurst (JZ4725B/JZ4770/JZ4780)
- Loongson 2E/2F/2K1000/3A1000/3A4000
- Cavium Octeon 68XX/73XX
- Broadcom BCM3302, Atheros AR7161/AR9344/QCA9561
- MediaTek MT7621, IDT RC32364

**Itanium:** Not supported — QEMU has no ia64 emulation target. Itanium cannot be emulated by any current version of QEMU.

---

## Machine Types

### x86

| Machine | Description |
|---------|-------------|
| `pc` | Standard i440fx PIIX3 (i386/x86_64) |
| `q35` | Q35 ICH9 chipset (modern x86) |
| `pc_p4era` | Pentium 4 era (i845/i865/i875 + ICH4/ICH5) |
| `pc_nforce` | NVIDIA nForce 2/3/4 chipset |
| `mac-macbook-*` | Apple MacBook (2006–2017, i386/x86_64) |
| `mac-macmini-*` | Apple Mac mini (2006–2014) |
| `mac-imac-*` | Apple iMac (2006–2017) |
| `mac-macpro-*` | Apple Mac Pro (2006–2013) |

### ARM

| Machine | Description |
|---------|-------------|
| `virt` | QEMU ARM Virtual Machine |
| `vexpress-a9/a15` | Versatile Express |
| `raspi0/1ap/2b/3ap/3b/4b` | Raspberry Pi |
| `collie` | Sharp Zaurus SL-5500 |
| `wince-vexpress-a9/a15/a7` | WinCE on VExpress |
| `ipaq-*` | HP iPAQ Pocket PC |
| `dell-axim-*` | Dell Axim Pocket PC |
| `htc-*` | HTC Windows Mobile phones |
| `samsung-omnia*` | Samsung Omnia |
| `palm-treo-*` | Palm Treo |
| `wondermedia-*` | WonderMedia WinCE tablets |
| `wince-tegra*` | Tegra WinCE/WinRT tablets |
| `mac-m1/m2/m3` | Apple Silicon Macs |

---

## Device Stubs

All device stubs appear as PCI devices in the guest with correct vendor/device IDs, class codes, and subsystem IDs. They show up in Device Manager, GPU-Z, CPU-Z, and other hardware detection tools.

### NVIDIA GPUs (224)
- GeForce RTX 20/30/40/50 series
- GeForce GTX 600–1000 series
- Quadro P/RTX series
- Tesla V100/A100/H100/H200/B200/GB200

### Intel GPUs (80)
- HD Graphics, UHD Graphics, Iris/Iris Pro/Xe/Xe HP/Xe HPC/Arc

### Intel NPUs (56)
- GNA 1.0–3.5, NPU 3720/3900/3950

### AMD GPUs/NPUs (34)
- Radeon 660M/680M/760M/780M/880M/890M/90M
- XDNA NPU (Ryzen AI Phoenix/Strix/Hawk Point)

### Google TPUs (10)
- Cloud TPU v2–v5p, Edge TPU Coral (Dev/M.2/Mini/PCIe)

### Storage (18+4)
- 18 NVMe SSD stubs (Samsung/Omega/Intel/WD/Kioxia/SkHynix)
- 4 SmartNIC stubs

### Other
- 5 Audio codec stubs, 4 TPM 2.0 stubs, 4 Thunderbolt stubs
- Fan controller, CPU VRM, DDR5 SPD, PWM controller

---

## Troubleshooting

### GPU-Z doesn't detect GPU stubs properly
- GPU stubs are PCI device stubs only — they don't have real 3D rendering capability
- GPU-Z may show "NVIDIA" vendor and correct device ID but will report 0MB VRAM or fail VBIOS detection
- This is expected — the stubs exist for hardware identification, not for running GPU workloads
- Use `-vga std` or `-vga virtio` for the actual display, then `-device nvidia-rtx5090` as a secondary device

### CPU-Z misidentifying CPUs ("16/61" instead of brand string)
- This was a bug in v1.5 and earlier — caused by missing `.xlevel = 0x80000008` in CPU definitions
- Fixed in v1.51 — all 64-bit CPUs now have correct xlevel for extended CPUID brand strings
- If still seeing "16/61", ensure you're using v1.51 or later

### Which features need KVM enabled
| Feature | KVM Required | Notes |
|---------|-------------|-------|
| `-cpu host` | Yes | Passes host CPU features directly |
| `hide-kvm-features` | Yes | Hides KVM CPUID leaf (meaningless without KVM) |
| `hide-hv-signature` | Yes | Hides Hyper-V enlightenments (needs KVM) |
| Fake RAM (SMBIOS) | No | Works with TCG |
| Multi-GPU (gpu-count) | No | Works with TCG |
| Spoof BIOS/board | No | Works with TCG |
| Custom CPU models | No | Works with TCG |

### Performance tips for older OS (Win95/98/ME)
- Use `-machine pc` or `-machine pc_p4era` (not q35)
- Use `-vga cirrus` (most compatible with Win9x drivers)
- Use `-accel tcg` (KVM is incompatible with these machine types)
- Use `-soundhw sb16` for Sound Blaster 16
- Keep RAM at 64–256MB (more can cause Win9x issues)
- Use `-cpu` matching the era (486/Pentium/Pentium II) for best compatibility

### Windows XP performance
- Use `-machine q35,accel=kvm -cpu host` for best speed
- Or era-accurate: `-machine pc_p4era,accel=kvm -cpu Intel-Pentium-4-2800`
- Install VirtIO drivers for disk/network for major speed boost
- Use `-device qemu-xhci -device usb-tablet` for mouse integration

---

## Build Instructions

### Linux (x86_64)

**Prerequisites:** gcc, meson, ninja, glib2.0-dev, libslirp-dev, pixman-devel, flex, bison

```bash
# Dynamic build
./configure --target-list=x86_64-softmmu,aarch64-softmmu,i386-softmmu,mips-softmmu,mips64-softmmu,mipsel-softmmu,mips64el-softmmu \
    --enable-slirp --enable-kvm --enable-vhost-net
ninja -C build -j$(nproc)

# Static build (build libslirp from source first)
git clone https://gitlab.freedesktop.org/slirp/libslirp.git
cd libslirp && meson setup build --default-library=static --prefix=/tmp/libslirp-prefix
ninja -C build install && cd ..

PKG_CONFIG_PATH=/tmp/libslirp-prefix/lib/x86_64-linux-gnu/pkgconfig \
./configure --target-list=x86_64-softmmu,aarch64-softmmu,i386-softmmu,mips-softmmu,mips64-softmmu,mipsel-softmmu,mips64el-softmmu \
    --enable-slirp --static --enable-kvm --enable-vhost-net \
    --disable-gtk --disable-sdl --disable-opengl --disable-virglrenderer \
    --disable-curl --disable-alsa --disable-pa --disable-sndio \
    --disable-libudev --disable-rdma --disable-spice --disable-usb-redir \
    --disable-vde --disable-netmap --disable-capstone --disable-gio \
    --disable-vte --disable-keyring --disable-libusb --disable-xkbcommon

mkdir build-static && cd build-static
ninja -j$(nproc)
```

### macOS
Not officially supported. QEMU-extra tracks upstream QEMU which has limited macOS build support. You may be able to build from source using Homebrew dependencies, but many features (KVM, vhost-net) are Linux-only. Use the Linux build in a VM or Docker container.

### Windows
Not officially supported for building. Use the pre-built Linux binaries in WSL2, or build in a Linux Docker container. The resulting binaries are Linux ELF executables and will not run natively on Windows.

---

## Changelog

### v1.51 (Current)
- **MIPS CPU support**: 14 new MIPS CPUs (SGI R4400/R4600, NEC VR7701, Ingenic JZ47xx, Cavium Octeon 73XX, Loongson 2K1000, Broadcom/Atheros/MediaTek WiSoCs)
- **MIPS targets**: Added mips/mips64/mipsel/mips64el-softmmu to build
- **Windows CE/Mobile**: 70 ARM32 CPU models (XScale PXA, S3C, OMAP, Qualcomm MSM/QSD, NVIDIA Tegra, Freescale i.MX, Cirrus EP93xx, Atmel AT91SAM9)
- **WinCE machines**: 33 ARM machine types (iPAQ, Dell Axim, HTC phones, Samsung Omnia, Palm Treo, Zaurus, WonderMedia, Tegra tablets)
- **WinCE x86 CPUs**: 44 x86 CPUs for Windows CE x86 builds (AMD Geode/LX/NX, VIA Eden/C3/C7/Nano, Transmeta Crusoe/Efficeon, Intel Pentium M/Celeron M/Atom/486)
- **AMD brand string fix**: All 218 AMD entries had `brand_id=255` (corrupted CPUID EBX), now `brand_id=0`
- **iGPU name removal**: AMD APU model_ids no longer show "with Radeon Graphics" — use separate GPU device stubs instead
- **xlevel fix for all CPUs**: All 1,612 previously-bare entries now have `.xlevel = 0x80000008`, fixing "16/61" brand string display in guest OS
- **README**: Comprehensive documentation with troubleshooting, examples, hardware compatibility

### v1.5
- **2,007 unique x86 CPUs** (4,136 with aliases) via 7 include files
- **272 ARM64 CPUs** (67 new: Rockchip, Exynos, Dimensity, Kunpeng, Layerscape)
- 5 AMD XDNA NPU stubs, 9 AMD APU iGPU stubs, 20 retro GPU stubs
- Feature flags added to all 1,144 bare CPU entries (no more boot failures)
- CPUID_ACPI + CPUID_SS + CPUID_EXT3_LAHF_LM added to all entries
- phys-bits warning removed (silently auto-adjusts)

### v1.42
- 92 more CPU models (Intel 12th-14th gen, AMD Ryzen 7000/9000 X3D, APUs, Ryzen AI 300)
- 239 old Xeon CPU models (Prestonia through Broadwell-EP)
- Anti-VM property wiring (hide-kvm, hide-hv, spoof-bios, spoof-board)
- Duplicate CPU entry resolution across all files

### v1.41b
- Preset Profile System, Hypervisor Detection Countermeasures
- Fan Controller/PWM, CPU VRM, DDR5/LPDDR SPD
- Intel NPUs, AMD NPUs, Intel GPUs (80 stubs), AMD Datacenter GPUs, NVIDIA GB300+DGX
- ARM64 phone SoCs, 30+ more ARM64 SoCs, 50+ more ARM64 SoCs
- 377+ x86 CPU models (Celerons, Pentiums, Core U/Y/T, Atom server/embedded)
- 75 NVIDIA RTX stubs (20/30/40/50/PRO series)
- 69 Intel Mac machine types, Apple T2, SMBIOS overrides

---

## Contributing

### Adding new CPU models (x86)

1. Create or edit an include file in `target/i386/` (e.g., `my-cpus.inc.c`)
2. Add entries to `builtin_x86_defs[]` format with all required fields:
   ```c
   {
       .name = "My-CPU-Model",
       .level = 0x0F,
       .xlevel = 0x80000008,  // Required for 64-bit CPUs!
       .vendor = CPUID_VENDOR_INTEL,
       .family = 6,
       .model = 142,
       .stepping = 12,
       .brand_id = 0,
       .features[FEAT_1_EDX] = CPUID_FP87 | ... | CPUID_ACPI | CPUID_SS,
       .features[FEAT_1_ECX] = CPUID_EXT_SSE3 | ... | CPUID_EXT_HYPERVISOR,
       .features[FEAT_8000_0001_EDX] = CPUID_EXT2_LM | CPUID_EXT2_NX | ...,
       .features[FEAT_8000_0001_ECX] = CPUID_EXT3_LAHF_LM,
       .model_id = "My CPU Brand String (max 48 chars)",
   },
   ```
3. Add `#include "my-cpus.inc.c"` in `target/i386/cpu.c` before the closing `};` of `builtin_x86_defs[]`
4. **Must include**: `.xlevel = 0x80000008` for any 64-bit CPU, `CPUID_ACPI | CPUID_SS` in FEAT_1_EDX, `CPUID_EXT3_LAHF_LM` in FEAT_8000_0001_ECX for LM-capable CPUs

### Adding GPU/NPU/TPU stubs

1. Create a .c file in the appropriate directory (`hw/display/`, `hw/misc/`)
2. Follow the PCI stub pattern: `OBJECT_DECLARE_SIMPLE_TYPE`, struct with `PCIDevice parent + MemoryRegion mmio`, MMIO ops, realize, Property array, VMStateDescription, class_init, TypeInfo, `type_init()`
3. Add the file to `hw/display/meson.build` or `hw/misc/meson.build`

### Adding ARM machine types

1. Add entries to `hw/arm/wince-machines.c` following the pattern
2. Must include `.interfaces = arm_machine_interfaces` in TypeInfo
3. Must include `hw/arm/machines-qom.h` and `target/arm/cpu-qom.h`

---

## Hardware Compatibility Matrix

### Windows Versions vs CPU Families

| Windows | Recommended CPUs | Min Features |
|---------|------------------|--------------|
| Win95/98/ME | 486 – Pentium III | SSE, APIC |
| Win2000 | Pentium Pro – Pentium 4 | PAE, ACPI |
| WinXP | Pentium III – Core 2 Duo | SSE2, ACPI, APIC |
| WinVista/7 | Core 2 Duo – Core i7 | SSE3, NX, ACPI |
| Win8/8.1 | Core i3 – Core i7 | SSE4.2, NX, ACPI |
| Win10 | Core i5 – Ryzen 7 | SSE4.2, NX, LM, ACPI |
| Win11 | Core i7 8th+ gen, Ryzen 3000+ | SSE4.2, NX, LM, ACPI, AVX2 |

### Windows CE vs CPU Architectures

| WinCE Version | ARM CPUs | x86 CPUs | MIPS CPUs |
|--------------|----------|----------|-----------|
| CE 2.x/3.0 | SA-1110 | 486DX, Geode GX | VR5432, R4000 |
| CE 4.x | PXA25x | Geode LX, VIA C3 | VR7701, 4Kc |
| CE 5.0 | PXA27x, OMAP850 | Geode NX, VIA Eden | 24Kc, BCM3302 |
| CE 6.0 | Cortex-A8/A9, MSM7200 | Atom N270 | 24Kf, MT7621 |
| Compact 7 | Cortex-A9/A15, QSD8250 | Atom D510 | 34Kf, 74Kf |
| Compact 2013 | Cortex-A15/A7 | Atom E6xx | P5600, I6400 |

---

## Quick Start

Download the latest release from [Releases](https://github.com/CollabVMgamez/qemu-extra/releases) and extract:

```bash
unzip qemu-p4fork-v1.51.zip
cd qemu-p4fork-v1.51

# Run Windows XP on a Pentium 4
bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm \
    -cpu Intel-Pentium-4-2800 \
    -m 512 \
    -hda disk.qcow2 \
    -cdrom winxp.iso \
    -boot d \
    -nic user

# Run with anti-VM features
bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm,fake-ram-mb=32768,hide-kvm-features=on \
    -cpu AMD-Ryzen-7-7700X \
    -m 4096 \
    -hda disk.qcow2 \
    -nic user

# Static binary (no shared libs needed)
bin-static/qemu-system-x86_64 -L share/qemu ...

# List available CPUs
bin/qemu-system-x86_64 -cpu help

# List available machines
bin/qemu-system-x86_64 -machine help

# List available devices
bin/qemu-system-x86_64 -device ?
```

---

## License

GPL-2.0-or-later (same as upstream QEMU). This fork incorporates all upstream QEMU code under the same license.
