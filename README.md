# qemu-extra

A heavily enhanced fork of [QEMU](https://www.qemu.org/) with **4,500+ CPU models**, **5,200+ PCI device stubs**, **150+ machine types**, anti-VM detection countermeasures, fake RAM SMBIOS, multi-GPU emulation, and era-accurate hardware configurations for running retro operating systems, embedded platforms, and anti-fingerprint VMs.

**Ideal for:** Retro OS emulation • Hardware spoofing • Embedded platform development • Educational emulation • VM anti-detection testing

---

## Table of Contents

1. [Quick Start](#quick-start)
2. [Key Features At a Glance](#key-features-at-a-glance)
3. [CPU Models](#cpu-models)
4. [Machine Types](#machine-types)
5. [Device Stubs](#device-stubs)
6. [Anti-VM & Hardware Spoofing](#anti-vm--hardware-spoofing)
7. [Multi-GPU Emulation](#multi-gpu-emulation)
8. [Installation & Build](#installation--build)
9. [Common Scenarios](#common-scenarios)
10. [Hardware Compatibility Matrix](#hardware-compatibility-matrix)
11. [Troubleshooting Guide](#troubleshooting-guide)
12. [Contributing](#contributing)
13. [Changelog](#changelog)
14. [License](#license)

---

## Quick Start

**Get running in 30 seconds:**

```bash
# 1. Download and extract
unzip qemu-p4fork-v1.51.zip && cd qemu-p4fork-v1.51

# 2. Run Windows XP on a Pentium 4
./bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm \
    -cpu Intel-Pentium-4-2800 \
    -m 512 \
    -hda disk.qcow2 \
    -cdrom winxp.iso \
    -boot d \
    -nic user
```

**First time?** Start with the [Common Scenarios](#common-scenarios) section below — it has real-world examples for Windows, Linux, and retro systems.

---

## Key Features At a Glance

| Feature | Count/Details |
|---------|---------------|
| **x86 CPU models** | 4,200+ (Intel, AMD, VIA, Transmeta) |
| **ARM/ARM64 CPUs** | 410+ (Cortex, Snapdragon, Exynos, Tegra, Apple Silicon, etc.) |
| **MIPS CPUs** | 47 (SGI, Loongson, Ingenic, Cavium, Broadcom, MediaTek) |
| **PCI device stubs** | 5,200+ (GPUs, NPUs, TPUs, storage, audio, security) |
| **x86 machine types** | 140+ (q35, i440fx, P4-era, nForce, Mac) |
| **ARM machine types** | 155+ (virt, VExpress, WinCE, Mac, Tegra, Raspberry Pi) |
| **Anti-VM features** | Hide KVM, hide Hyper-V, spoof BIOS/board, fake RAM |
| **Multi-GPU** | GPU stacking (1 primary VGA + N secondary 3D controllers) |

---

## CPU Models

### x86 (4,200+ models)

**Intel:**
- Vintage: 486DX/DX2/DX4, Pentium, Pentium MMX, Pentium Pro
- P2/P3 era: Pentium II (Klamath/Deschutes/Xeon), Pentium III (Katmai/Coppermine/Tualatin)
- P4 era: Willamette, Northwood, Prescott, Cedar Mill, Smithfield, Presler, Dempsey
- Mobile: Pentium M, Celeron M, Atom (Diamondville/Pineview/Cedarview/Silvermont)
- Core 2: Solo, Duo, Quad, Extreme (all steppings of Conroe/Allendale/Wolfdale/Yorkfield/Penryn)
- Core i-series: i3/i5/i7/i9 (Nehalem through 14th gen Raptor Lake)
- Server: Xeon (Prestonia through Emerald Rapids, all architectures)

**AMD:**
- Vintage: K5, K6, K6-2, K6-III, Athlon, Duron
- Athlon 64 era: Athlon 64, X2, FX, Opteron (all steppings)
- Phenom era: Phenom, Phenom II, Athlon II, Sempron, Turion
- Ryzen: 1000–9000 series, Ryzen AI 300, all Threadripper generations
- Server: EPYC (Napoli, Rome, Milan, Genoa, Turin, Siena)
- Embedded: Geode GX/LX/NX, Elan SC400/SC520

**VIA:**
- C3 (Samuel2, Ezra, Nehemiah), C7/C7-M, Eden/Eden-Nano/Nano-3000/X2

**Transmeta:**
- Crusoe TM3200/TM5400/TM5600/TM5800
- Efficeon TM8000/TM8600/TM8800

### ARM/ARM64 (410+ models)

**ARM Cortex cores:**
- Classic: Cortex-A5, A7, A8, A9, A15, A17
- Modern: A32, A35, A53, A55, A57, A72, A76, A77, A78, A710, A715, A720
- Real-time: R5, R52
- High-performance: X1–X4

**Mobile SoCs:**
- Apple: A7–A19, M1–M4 (Pro/Max/Ultra)
- Qualcomm: Snapdragon (Scorpion/Krait/Kryo — MSM7xxx/QSD8xxx/835/845/888/8 Gen 1–3)
- Samsung: Exynos 3110–2400
- MediaTek: Dimensity 700–9400
- HiSilicon: Kunpeng 920–930
- Nvidia: Tegra 2, 3, X1, X2
- Marvell: Armada, PJ1, PJ4

**Embedded/Industrial:**
- Freescale/NXP: i.MX 21–93
- ARM XScale: PXA 250–935
- StrongARM: SA-1110
- Other: Samsung S3C, TI OMAP, Cirrus EP93xx, Atmel AT91SAM9, NXP LPC3xxx

**Windows CE/Mobile SoCs:**
- Samsung S3C2410/2440/6410
- TI OMAP (730/850/2420/3430/3630)
- Cirrus EP93xx, Atmel AT91SAM9, NXP LPC3xxx
- VIA WonderMedia WM8505/8650/8850

### MIPS (47 models)

- **SGI:** R4000, R4400, R4600
- **NEC:** VR5432, VR7701
- **Standard:** MIPS 4Kc/4Km/4KEc, 24Kc/24Kf, 34Kf, 74Kf, M14K/M14Kc, P5600, I6400/I6500
- **Ingenic:** XBurst (JZ4725B, JZ4770, JZ4780)
- **Loongson:** 2E/2F, 2K1000, 3A1000, 3A4000
- **Cavium:** Octeon 68XX, 73XX
- **Broadcom/Atheros/MediaTek:** BCM3302, AR7161, AR9344, QCA9561, MT7621
- **IDT:** RC32364

**Note:** Itanium (ia64) is not supported — QEMU has no ia64 emulation target.

---

## Machine Types

### x86 Machines

| Machine | Chipset | Era | Use Case |
|---------|---------|-----|----------|
| `pc` | PIIX3 (i440fx) | 1997–2010 | Generic i386/x86_64, Win95–WinXP |
| `q35` | ICH9 (Q35) | 2007+ | Modern x86_64, Win7–Win11, Linux |
| `pc_p4era` | ICH4/ICH5 (i845/i875) | 2000–2004 | Pentium 4 systems, WinXP era |
| `pc_nforce2` | NVIDIA nForce 2 | 2002–2003 | AMD Athlon XP systems |
| `pc_nforce3` | NVIDIA nForce 3 | 2003–2004 | AMD Athlon 64 systems |
| `pc_nforce4` | NVIDIA nForce 4 | 2004–2006 | AMD Athlon 64 X2 systems |
| `mac-macbook-*` | Intel + Apple chipset | 2006–2017 | MacBook (i386/x86_64) |
| `mac-macmini-*` | Intel + Apple chipset | 2006–2014 | Mac mini |
| `mac-imac-*` | Intel + Apple chipset | 2006–2017 | iMac |
| `mac-macpro-*` | Intel + Apple chipset | 2006–2013 | Mac Pro (Xeon) |

### ARM Machines

| Machine | Architecture | Use Case |
|---------|-------------|----------|
| `virt` | ARMv7/ARMv8 | Generic QEMU ARM virtual machine |
| `vexpress-a9` | ARMv7 | Versatile Express (Cortex-A9) |
| `vexpress-a15` | ARMv8 | Versatile Express (Cortex-A15) |
| `raspi0`, `raspi1ap`, `raspi2b`, `raspi3ap`, `raspi3b`, `raspi4b` | ARMv6/ARMv7/ARMv8 | Raspberry Pi emulation |
| `collie` | ARMv4 | Sharp Zaurus SL-5500 (PDA) |
| `wince-vexpress-a*` | ARMv7/ARMv8 | Windows CE on VExpress |
| `ipaq-*` | ARMv4/ARMv5 | HP iPAQ Pocket PC |
| `dell-axim-*` | ARMv5 | Dell Axim handheld |
| `htc-*` | ARMv6/ARMv7 | HTC Windows Mobile phones |
| `samsung-omnia*` | ARMv7 | Samsung Omnia smartphone |
| `palm-treo-*` | ARMv5/ARMv6 | Palm Treo smartphone |
| `wondermedia-*` | ARMv5 | WonderMedia WinCE tablets |
| `wince-tegra*` | ARMv7 | Tegra-based tablets (WinCE/WinRT) |
| `mac-m1`, `mac-m2`, `mac-m3` | ARMv8 | Apple Silicon Macs |

---

## Device Stubs

All device stubs appear as **real PCI devices** in the guest with correct vendor/device IDs, class codes, and subsystem IDs. They show up in Device Manager, GPU-Z, CPU-Z, HWiNFO, and other hardware detection tools.

### NVIDIA GPUs (224 stubs)

**Desktop & Mobile:**
- GeForce RTX 20/30/40/50 series
- GeForce GTX 600–1000 series (Kepler through Ampere)
- GeForce MX 100–550 (mobile)

**Workstation:**
- Quadro P-series, RTX-series (Turing, Ampere)

**Datacenter:**
- Tesla V100, A100, H100, H200
- DGX A100/H100/H800

### Intel GPUs & NPUs (136 stubs)

**Integrated Graphics:**
- HD Graphics (Ivy Bridge–10th gen)
- UHD Graphics (11th+ gen)
- Iris/Iris Pro/Iris Plus
- Xe-HPG (Arc discrete GPUs A380–A770)

**NPUs (Neural Processing Units):**
- GNA 1.0–3.5 (10th–12th gen)
- NPU 3720, 3900, 3950 (13th+ gen Meteor Lake+)

### AMD GPUs & NPUs (34 stubs)

**Integrated (RDNA APUs):**
- Radeon 660M/680M/760M/780M/880M/890M/900M

**Discrete (not in qemu-extra but placeholders for expansion):**
- Radeon RX series placeholders for future

**NPUs (AI accelerators):**
- XDNA (Ryzen AI Phoenix, Strix Point, Hawk Point)

### Google TPUs (10 stubs)

- Cloud TPU v2–v5p (for datacenter emulation)
- Edge TPU Coral (Dev, M.2, Mini, PCIe variants)

### Storage (22 stubs)

**NVMe SSDs (18):**
- Samsung 970 EVO/Pro, 980 Pro/Evo
- Omega (simulated high-end)
- Intel 760P/665P
- WD Black SN850X
- Kioxia Exceria
- SK Hynix Platinum

**SmartNICs (4):**
- Mellanox ConnectX, NVIDIA BlueField variants

### Audio & Miscellaneous

| Device | Count | Examples |
|--------|-------|----------|
| Audio codecs | 5 | Realtek ALC, Conexant, VIA |
| TPM 2.0 | 4 | Discrete TPM modules |
| Thunderbolt | 4 | Alpine Ridge variants |
| Fan controller | 1 | IT8792 fanless system monitor |
| CPU VRM | 1 | Voltage regulator emulation |
| DDR5 SPD | 1 | Memory module identification |
| PWM controller | 1 | Fan/LED control |

---

## Anti-VM & Hardware Spoofing

Hide the fact that you're running in a virtual machine. These features let guest OS and detection tools (CPU-Z, GPU-Z, HWiNFO) think they're on real hardware.

### Available Properties

| Property | Flag | Effect | Requires KVM? |
|----------|------|--------|---------------|
| Hide KVM CPUID | `hide-kvm-features=on` | Removes KVM signature from CPUID leaf 0x40000000 | Yes |
| Hide Hyper-V signature | `hide-hv-signature=on` | Removes Hyper-V enlightenments from CPUID | Yes |
| Spoof BIOS vendor | `spoof-bios-vendor="..."` | Overrides SMBIOS Type 0 (BIOS) vendor string | No |
| Spoof BIOS version | `spoof-bios-version="..."` | Overrides SMBIOS Type 0 BIOS version | No |
| Spoof board vendor | `spoof-board-vendor="..."` | Overrides SMBIOS Type 2 (board) manufacturer | No |
| Spoof board product | `spoof-board-product="..."` | Overrides SMBIOS Type 2 board name | No |
| Fake RAM (SMBIOS) | `fake-ram-mb=N` | SMBIOS reports N MB while actual allocation is less | No |

### Example: Convincing ASUS X670E Build

Make a VM with 512MB real RAM appear as a 32GB ASUS motherboard:

```bash
./bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,\
accel=kvm,\
fake-ram-mb=32768,\
hide-kvm-features=on,\
hide-hv-signature=on,\
spoof-bios-vendor="American Megatrends International LLC",\
spoof-bios-version="2.17.19",\
spoof-board-vendor="ASUSTeK COMPUTER INC.",\
spoof-board-product="PRIME X670E-PRO" \
    -cpu AMD-Ryzen-7-7700X \
    -m 512 \
    -hda disk.qcow2
```

**Result in guest:** CPU-Z/GPU-Z sees a real ASUS board with 32GB RAM, but QEMU only consumes 512MB.

---

## Multi-GPU Emulation

Stack multiple GPUs in a single VM. Create one primary VGA-class GPU + N secondary 3D controller GPUs.

### How It Works

```bash
# Single GPU (default)
-device nvidia-rtx5090

# 4 GPUs (1 VGA primary + 3 secondary)
-device nvidia-rtx5090,gpu-count=4
```

This matches how real multi-GPU systems appear in Device Manager (e.g., 4 separate NVIDIA adapters). Works with all NVIDIA RTX/GTX/Quadro/Tesla stubs.

### Real-World Example: GPU Cluster Sim

```bash
./bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm \
    -cpu AMD-Ryzen-9-7950X \
    -m 16G \
    -hda disk.qcow2 \
    -device nvidia-rtx5090,gpu-count=4 \
    -device nvidia-a100,gpu-count=2 \
    -nic user
```

Guest sees 6 total NVIDIA adapters (4 RTX 5090s + 2 A100s).

---

## Installation & Build

### Option 1: Download Pre-Built Binaries (Easiest)

Visit [Releases](https://github.com/CollabVMgamez/qemu-extra/releases) and download:

- **`qemu-p4fork-v1.51.zip`** — Dynamic build (Ubuntu/Debian, full features)
- **`qemu-p4fork-v1.51-static.zip`** — Static build (any Linux distro, no dependencies)

Extract and run directly — no compilation needed.

### Option 2: Build from Source

#### Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential meson ninja-build libglib2.0-dev \
    libslirp-dev libpixman-1-dev flex bison pkg-config
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc meson ninja-build glib2-devel libslirp-devel \
    pixman-devel flex bison
```

**macOS:**
Not officially supported. QEMU upstream has limited macOS support. Recommend using Linux VM or Docker.

**Windows:**
Not officially supported for building. Use WSL2 (Windows Subsystem for Linux) or Docker.

#### Dynamic Build (Recommended for development)

```bash
git clone https://github.com/CollabVMgamez/qemu-extra.git
cd qemu-extra

./configure \
    --target-list=x86_64-softmmu,i386-softmmu,aarch64-softmmu,arm-softmmu,mips-softmmu,mips64-softmmu,mipsel-softmmu,mips64el-softmmu \
    --enable-slirp \
    --enable-kvm \
    --enable-vhost-net \
    --prefix=/usr/local

ninja -C build -j$(nproc)
sudo ninja -C build install
```

**Test it:**
```bash
qemu-system-x86_64 -version
qemu-system-aarch64 -cpu help | head -20
```

#### Static Build (No runtime dependencies)

For building a binary that works on any Linux distro:

```bash
# Build libslirp from source first
git clone https://gitlab.freedesktop.org/slirp/libslirp.git
cd libslirp
meson setup build --default-library=static --prefix=/tmp/libslirp-prefix
ninja -C build install
cd ..

# Build QEMU with static libslirp
PKG_CONFIG_PATH=/tmp/libslirp-prefix/lib/x86_64-linux-gnu/pkgconfig \
./configure \
    --target-list=x86_64-softmmu,i386-softmmu,aarch64-softmmu,arm-softmmu,mips-softmmu,mips64-softmmu,mipsel-softmmu,mips64el-softmmu \
    --enable-slirp \
    --static \
    --enable-kvm \
    --enable-vhost-net \
    --disable-gtk \
    --disable-sdl \
    --disable-opengl \
    --disable-virglrenderer \
    --disable-curl \
    --disable-alsa \
    --disable-pa \
    --disable-sndio \
    --disable-libudev \
    --disable-rdma \
    --disable-spice \
    --disable-usb-redir \
    --disable-vde \
    --disable-netmap \
    --disable-capstone \
    --disable-gio \
    --disable-vte \
    --disable-keyring \
    --disable-libusb \
    --disable-xkbcommon \
    --prefix=/usr/local

mkdir build-static && cd build-static
ninja -j$(nproc)
sudo ninja install
```

**Note:** Static build has no GUI (SDL/GTK) — use `-vnc :0` and a VNC client for display.

---

## Common Scenarios

### Scenario 1: Windows XP on Pentium 4 (Classic)

**What you need:**
- Windows XP ISO
- 512MB–1GB disk image (create with `qemu-img create -f qcow2 winxp.qcow2 10G`)

```bash
./bin/qemu-system-x86_64 -L share/qemu \
    -machine pc,accel=kvm \
    -cpu Intel-Pentium-4-2800 \
    -m 512 \
    -smp cores=1 \
    -hda winxp.qcow2 \
    -cdrom winxp.iso \
    -boot d \
    -soundhw sb16 \
    -nic user,model=rtl8139 \
    -vga std
```

**Why these settings:**
- `-machine pc` — Old PIIX3 chipset (WinXP expected this)
- `-cpu Intel-Pentium-4-2800` — Matches era (P4 was common for XP)
- `-soundhw sb16` — Sound Blaster 16 (drivers widely available for XP)
- `-nic user,model=rtl8139` — Realtek NIC (good driver support in XP)
- `-vga std` — VGA-compatible graphics (Win XP VGA driver works)

### Scenario 2: Windows 7 with Modern Hardware

**What you need:**
- Windows 7 ISO
- 20GB disk image
- Modern CPU/GPU support

```bash
./bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm \
    -cpu Intel-Core-i7-3770 \
    -m 4G \
    -smp cores=4 \
    -hda win7.qcow2 \
    -cdrom win7.iso \
    -boot d \
    -device intel-hd3000 \
    -device nvidia-gtx970 \
    -device intel-82579lm,netdev=net0 \
    -netdev user,id=net0 \
    -soundhw hda \
    -vga qxl
```

**Why these settings:**
- `-machine q35` — Modern ICH9 chipset (Win7 native support)
- `-cpu Intel-Core-i7-3770` — IvyBridge (good balance for Win7)
- `-smp cores=4` — Multi-core (Win7 loves this)
- GPU stubs — GPU-Z/Device Manager will show real devices
- `-soundhw hda` — High Definition Audio (modern standard)
- `-vga qxl` — QXL provides acceleration

### Scenario 3: Linux ARM (Raspberry Pi 4 Emulation)

**What you need:**
- Raspberry Pi OS kernel/rootfs
- ARM64 Linux knowledge

```bash
./bin/qemu-system-aarch64 -L share/qemu \
    -machine raspi4b \
    -cpu cortex-a72 \
    -m 4G \
    -drive if=sd,file=raspi-os.img \
    -nic user,model=usb \
    -device usb-net,netdev=net0 \
    -netdev user,id=net0 \
    -serial stdio \
    -append "root=/dev/mmcblk0p2 rw"
```

### Scenario 4: Anti-VM Detection Test (Looks Real to Tools)

```bash
./bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm,\
fake-ram-mb=32768,\
hide-kvm-features=on,\
hide-hv-signature=on,\
spoof-bios-vendor="Dell Inc.",\
spoof-bios-version="1.21.0",\
spoof-board-vendor="Dell Inc.",\
spoof-board-product="OptiPlex 7090" \
    -cpu AMD-Ryzen-9-5950X \
    -m 2G \
    -hda disk.qcow2 \
    -nic user \
    -device nvidia-rtx2080,gpu-count=2
```

**In guest:** CPU-Z, GPU-Z, HWiNFO all report real hardware (Dell OptiPlex, Ryzen 9, RTX 2080) despite being a VM with only 2GB RAM.

### Scenario 5: Windows CE on ARM (Handheld Emulation)

```bash
./bin/qemu-system-arm -L share/qemu \
    -machine ipaq-h3100 \
    -cpu xscale-pxa255 \
    -m 64 \
    -drive if=sd,file=wince.img \
    -serial stdio
```

---

## Hardware Compatibility Matrix

### Windows Versions vs Recommended CPUs

| Windows | Minimum Features | Recommended CPU | Notes |
|---------|------------------|-----------------|-------|
| Win95/98/ME | APIC, FPU, MMX | Pentium or Pentium Pro | Use `-machine pc`, avoid modern features |
| Win2000 | PAE, ACPI, APIC | Pentium Pro – Pentium 4 | Needs ACPI |
| WinXP | SSE2, ACPI, NX | Pentium 4 or Core 2 Duo | Core 2 = better stability |
| WinVista/7 | SSE3, NX, ACPI | Core 2 Duo – Core i7 | i7 recommended for smooth experience |
| Win8/8.1 | SSE4.2, SSSE3, NX | Core i5 or newer | AVX helpful |
| Win10 | SSE4.2, AVX2, NX, LM | Core i5 6th+ gen or Ryzen 1000+ | Prefers newer CPUs |
| Win11 | SSE4.2, AVX2, NX, LM, TPM2 | Core i7 8th+ gen or Ryzen 3000+ | Requires specific CPUID bits |

### Windows CE / Mobile Support

| OS | ARM CPUs | x86 CPUs | Best Machine Type |
|--------|----------|----------|------------------|
| CE 2.x / 3.0 | StrongARM SA-1110 | Pentium, Geode GX | `collie` or `virt` |
| CE 4.x | XScale PXA250 | Geode LX, VIA C3 | `ipaq-h3100` |
| CE 5.0 | XScale PXA27x, OMAP850 | Geode NX, VIA Eden | `ipaq-h5500` |
| CE 6.0 | Cortex-A8/A9, Snapdragon | Atom N270 | `wince-vexpress-a9` |
| Compact 7 | Cortex-A9/A15, QSD8250 | Atom D510 | `wince-vexpress-a15` |
| Compact 2013 | Cortex-A15/A7, Snapdragon 800 | Atom E6xx series | `wince-vexpress-a15` |

### KVM Support Requirements

| Feature | KVM Required? | Notes |
|---------|---------------|-------|
| `-cpu host` | **Yes** | Passes through host CPU directly |
| `hide-kvm-features=on` | **Yes** | Hides CPUID leaf 0x40000000 (KVM signature) |
| `hide-hv-signature=on` | **Yes** | Hides Hyper-V enlightenments |
| Fake RAM (SMBIOS) | No | Works fine with TCG |
| Multi-GPU stacking | No | TCG compatible |
| BIOS/board spoofing | No | TCG compatible |
| Custom CPU models | No | TCG works for any CPU definition |

---

## Troubleshooting Guide

### Issue: CPU-Z shows "16/61" instead of CPU name

**Cause:** Missing `.xlevel = 0x80000008` in CPU definition (pre-v1.51 bug).

**Solution:** Update to **v1.51 or later**. All x86_64 CPUs now have proper xlevel.

**Workaround (if stuck on older version):**
```bash
grep -n "your-cpu-name" target/i386/cpu.c
# Then add .xlevel = 0x80000008 to that entry and recompile
```

---

### Issue: GPU-Z shows 0MB VRAM or "BIOS not found"

**Cause:** GPU stubs are identification-only — no real video memory.

**Solution:** This is **expected behavior**. GPU stubs exist so GPU-Z detects the device correctly. They don't have actual VRAM or rendering.

**What works:**
- GPU-Z sees correct vendor/device ID 
- Device Manager shows the GPU 
- HWiNFO lists specifications 

**What doesn't work:**
- 3D acceleration 
- GPU-Z VBIOS detection 
- Actual compute/graphics workloads 

For real GPU acceleration, use VFIO passthrough of physical hardware.

---

### Issue: Windows XP boots very slowly or freezes

**Cause:** Either machine type/CPU mismatch or missing drivers.

**Solutions:**

**1. Use era-correct machine & CPU:**
```bash
# WRONG (modern hardware confuses WinXP):
-machine q35 -cpu Intel-Core-i7-12700K

# RIGHT (period-appropriate):
-machine pc -cpu Intel-Pentium-4-3000
```

**2. Disable unnecessary devices:**
```bash
# Remove USB 3.0, AHCI, etc. that WinXP doesn't understand:
-device qemu-xhci  #  Remove this
-device ahci        #  Remove this

# Use IDE/PIIX instead:
-device ide-hd,drive=disk0
-drive id=disk0,if=ide,file=winxp.qcow2
```

**3. Reduce CPU count:**
```bash
# XP can struggle with >2 cores:
-smp cores=1,threads=1  # Start with 1 core
```

---

### Issue: KVM not available ("KVM not found" error)

**Cause:** KVM not enabled in BIOS, or not running on Linux/supported CPU.

**Check KVM support:**
```bash
grep -E 'vmx|svm' /proc/cpuinfo    # Should show vmx (Intel) or svm (AMD)
sudo kvm-ok                         # If available, tells you if KVM works
ls -l /dev/kvm                      # Should exist and be readable
```

**Enable KVM (Ubuntu/Debian):**
```bash
sudo apt-get install qemu-kvm
sudo addgroup $(id -u -n) kvm      # Add your user to kvm group
newgrp kvm                          # Activate group membership
```

**If KVM unavailable, use TCG:**
```bash
-accel tcg  # Software CPU emulation (slower but works everywhere)
```

---

### Issue: "No such file or directory" for QEMU binaries

**Cause:** Missing `-L share/qemu` parameter or wrong path.

**Solution:**
```bash
# If you extracted the zip to ~/qemu-p4fork-v1.51:
cd ~/qemu-p4fork-v1.51
./bin/qemu-system-x86_64 -L share/qemu ...
   ↑ Full relative path    ↑ Required parameter
```

**Or, set PATH:**
```bash
export PATH="$HOME/qemu-p4fork-v1.51/bin:$PATH"
export QEMU_DATADIR="$HOME/qemu-p4fork-v1.51/share/qemu"
qemu-system-x86_64 ...  # Now works without ./bin/
```

---

### Issue: Network not working in guest

**Cause:** Missing NIC device or incorrect model.

**Solution:**

For **most guests** (use user networking):
```bash
-nic user                    # Default, works everywhere
-nic user,model=rtl8139      # Realtek (good for old OS like WinXP)
-nic user,model=e1000        # Intel (good for modern OS)
```

For **complex networking** (bridging, etc.):
```bash
# First, create a bridge (one-time setup):
sudo brctl addbr qemubridge0
sudo ip link set qemubridge0 up
sudo ip addr add 192.168.100.1/24 dev qemubridge0

# Then use it:
-nic bridge,br=qemubridge0
```

---

### Issue: QEMU uses too much CPU even when idle

**Cause:** TCG (software CPU emulation) without sleep optimization.

**Solution:**

Use **KVM** if available:
```bash
-accel kvm  # Dramatically faster, uses less CPU
```

Or optimize TCG:
```bash
-accel tcg,thread=multi  # Multi-threaded TCG
-icount shift=1          # Limit CPU cycles
```

---

### Issue: Disk image corrupted or won't boot

**Cause:** Wrong disk interface type (AHCI vs IDE vs NVMe).

**Solution:**

Check what the guest expects. For **old OS (Win95–WinXP)**:
```bash
# IDE (safest for retro OS):
-drive file=disk.qcow2,format=qcow2,if=ide

# For modern OS (Win7+):
-drive file=disk.qcow2,format=qcow2,if=virtio  # Needs VirtIO drivers
```

Recover a corrupted disk:
```bash
# Back up first:
cp disk.qcow2 disk.qcow2.backup

# Repair:
qemu-img check -r all disk.qcow2
```

---

## Contributing

### Adding New CPU Models (x86)

**Step 1: Create or edit an include file**

Create `target/i386/my-cpus.inc.c` with CPU definitions:

```c
/* AMD Ryzen 9000 series (Zen5) additions */
static const X86CPUDefinition builtin_x86_defs[] = {
    {
        .name = "AMD-Ryzen-9-9950X",
        .level = 0x0D,
        .xlevel = 0x80000008,      /* ← REQUIRED for 64-bit CPUs */
        .vendor = CPUID_VENDOR_AMD,
        .family = 0x19,
        .model = 0xA4,             /* ← Get this from real CPUID */
        .stepping = 0,
        .brand_id = 0,
        .features[FEAT_1_EDX] = CPUID_FP87 | CPUID_VME | CPUID_DE | CPUID_PSE |
            CPUID_TSC | CPUID_MSR | CPUID_PAE | CPUID_MCE | CPUID_CX8 | CPUID_APIC |
            CPUID_SEP | CPUID_MTRR | CPUID_PGE | CPUID_MCA | CPUID_CMOV | CPUID_PAT |
            CPUID_PSE36 | CPUID_CLFLUSH | CPUID_MMX | CPUID_FXSR | CPUID_SSE | CPUID_SSE2 |
            CPUID_ACPI | CPUID_SS,   /* ← ACPI and SS required */
        .features[FEAT_1_ECX] = CPUID_EXT_SSE3 | CPUID_EXT_MONITOR | CPUID_EXT_DS_CPL |
            CPUID_EXT_VMX | CPUID_EXT_EST | CPUID_EXT_TM2 | CPUID_EXT_CID | CPUID_EXT_CX16 |
            CPUID_EXT_XTPR | CPUID_EXT_PDCM | CPUID_EXT_X2APIC | CPUID_EXT_HYPERVISOR,
        .features[FEAT_8000_0001_EDX] = CPUID_EXT2_LM | CPUID_EXT2_SYSCALL |
            CPUID_EXT2_NX | CPUID_EXT2_FFXSR | CPUID_EXT2_RDTSCP | CPUID_EXT2_3DNOW |
            CPUID_EXT2_3DNOWEXT,
        .features[FEAT_8000_0001_ECX] = CPUID_EXT3_LAHF_LM | CPUID_EXT3_CMP_LEG,
        .features[FEAT_8000_0008_EBX] = CPUID_EXT3_PREFCORE,
        .features[FEAT_7_0_EBX] = CPUID_7_0_EBX_SMEP | CPUID_7_0_EBX_SMAP |
            CPUID_7_0_EBX_BMI1 | CPUID_7_0_EBX_BMI2,
        .features[FEAT_7_0_ECX] = CPUID_7_0_ECX_AVX512BW,
        .model_id = "AMD Ryzen 9 9950X 16-Core Processor",
    },
};
```

**Step 2: Include in cpu.c**

Edit `target/i386/cpu.c` and add before the closing `};`:

```c
#include "my-cpus.inc.c"
```

**Step 3: Required fields checklist**

-  `.name` — Unique, lowercase with hyphens (e.g., `AMD-Ryzen-9-9950X`)
-  `.xlevel = 0x80000008` — **For 64-bit CPUs only**
-  `.level` and `.xlevel` — From real CPUID dump
-  `.family`, `.model`, `.stepping` — From real CPUID
-  `CPUID_ACPI | CPUID_SS` in FEAT_1_EDX — Modern OS requires these
- `CPUID_EXT3_LAHF_LM` in FEAT_8000_0001_ECX — For LM-capable CPUs
-  `.model_id` — Human-readable brand string (max 48 chars)

**Step 4: Test**

```bash
./configure --target-list=x86_64-softmmu && ninja -C build
./build/qemu-system-x86_64 -cpu help | grep AMD-Ryzen-9-9950X
```

**Getting CPUID data from real hardware:**

```bash
# On the real CPU, run:
cpuid -l 0x00000000 -l 0x80000000
cpuid -l 0x00000001
cpuid -l 0x80000001
# Copy the EAX/EBX/ECX/EDX values into your CPU definition
```

---

### Adding GPU/NPU/TPU Device Stubs

**Step 1: Create device file** (`hw/display/nvidia-stub.c`):

```c
#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "qapi/error.h"

typedef struct NVIDIAGPUState {
    PCIDevice parent;
    MemoryRegion mmio;
} NVIDIAGPUState;

OBJECT_DECLARE_SIMPLE_TYPE(NVIDIAGPUState, NVIDIA_GPU)

static void nvidia_gpu_realize(PCIDevice *pci_dev, Error **errp)
{
    NVIDIAGPUState *s = NVIDIA_GPU(pci_dev);

    /* Set up memory regions for MMIO */
    memory_region_init(&s->mmio, OBJECT(s), "nvidia-gpu-mmio", 0x1000000);
    pci_register_bar(pci_dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);

    pci_set_word(pci_dev, PCI_VENDOR_ID, 0x10DE);  /* NVIDIA */
    pci_set_word(pci_dev, PCI_DEVICE_ID, 0x2706);  /* RTX 4090 */
    pci_set_byte(pci_dev, PCI_CLASS_PROG, 0x00);
    pci_set_word(pci_dev, PCI_CLASS_DEVICE, 0x0302);  /* 3D Controller */
}

static void nvidia_gpu_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    PCIDeviceClass *pc = PCI_DEVICE_CLASS(oc);

    pc->realize = nvidia_gpu_realize;
    dc->desc = "NVIDIA RTX 4090";
}

static const TypeInfo nvidia_gpu_info = {
    .name          = TYPE_NVIDIA_GPU,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NVIDIAGPUState),
    .class_init    = nvidia_gpu_class_init,
    .interfaces    = (InterfaceInfo[]) {
        { INTERFACE_PCIE_DEVICE },
        { }
    }
};

static void nvidia_gpu_register(void)
{
    type_register_static(&nvidia_gpu_info);
}

type_init(nvidia_gpu_register)
```

**Step 2: Add to build file** (`hw/display/meson.build`):

```python
softmmu_ss.add(when: 'CONFIG_PCI', if_true: files(
    # ... existing files ...
    'nvidia-stub.c',
))
```

**Step 3: Test**

```bash
./build/qemu-system-x86_64 -device nvidia-gpu -device help | grep nvidia
```

---

### Adding ARM Machine Types

**Step 1: Edit `hw/arm/wince-machines.c`:**

```c
static void my_new_machine_init(MachineState *machine)
{
    // Initialize your machine
}

static const TypeInfo my_new_machine_info = {
    .name          = MACHINE_TYPE_NAME("my-new-machine"),
    .parent        = TYPE_MACHINE,
    .instance_size = sizeof(MyMachineState),
    .instance_init = my_new_machine_init,
    .interfaces    = arm_machine_interfaces,
};

static void my_register_types(void)
{
    type_register_static(&my_new_machine_info);
}

type_init(my_register_types)
```

**Step 2: Build and test**

```bash
./build/qemu-system-arm -machine help | grep my-new-machine
```

---

### Code Style & Requirements

- **C99 standard** — Use modern C
- **Include guards** — `#ifndef FEATURE_H` / `#define FEATURE_H` / `#endif`
- **Documentation** — Inline comments for complex logic
- **Testing** — Verify with `qemu-system-*-cpu help` and `-device help`
- **Naming** — Hyphens for QEMU names, underscores for C symbols

---

### Submitting Changes

1. Fork the repository
2. Create a feature branch: `git checkout -b add-ryzen-9000-cpus`
3. Make changes and test
4. Commit with clear message: `"Add Ryzen 9000 series (Zen5) CPU models"`
5. Push and open a pull request
6. Include:
   - Description of what was added
   - Real CPUID values (where applicable)
   - Test results (e.g., `CPU-Z screenshots`, `qemu-system-* -cpu help` output)

---

## Changelog

### v1.51 (Current)

**Major additions:**
- MIPS CPU support: 14 new models (SGI R4400/R4600, NEC VR7701, Ingenic JZ47xx, Cavium Octeon, Loongson 2K1000, Broadcom/Atheros/MediaTek WiSoCs)
- MIPS targets: Added mips/mips64/mipsel/mips64el-softmmu to build
- Windows CE/Mobile: 70 ARM32 CPU models (XScale PXA, S3C, OMAP, Snapdragon, Tegra, Freescale i.MX, Cirrus, Atmel)
- WinCE machines: 33 ARM machine types (iPAQ, Dell Axim, HTC phones, Samsung Omnia, Palm Treo, Zaurus, WonderMedia, Tegra tablets)
- WinCE x86 CPUs: 44 models (AMD Geode/LX/NX, VIA Eden/C3/C7/Nano, Transmeta Crusoe/Efficeon, Intel Pentium M/Atom)

**Bug fixes:**
- AMD brand string fix: 218 AMD entries had corrupted `brand_id=255` → now `brand_id=0`
- iGPU name removal: AMD APUs no longer show "with Radeon Graphics" in model_id
- xlevel fix: All 1,612 previously-bare entries now have `.xlevel = 0x80000008`, fixing "16/61" CPU-Z display

**Docs:**
- Comprehensive README with troubleshooting, examples, and compatibility matrix

### v1.5

- 2,007 unique x86 CPUs (4,136 with aliases)
- 272 ARM64 CPUs (67 new: Rockchip, Exynos, Dimensity, Kunpeng, Layerscape)
- 5 AMD XDNA NPU stubs, 9 AMD iGPU stubs, 20 retro GPU stubs
- Feature flags added to all 1,144 bare CPU entries
- CPUID_ACPI + CPUID_SS + CPUID_EXT3_LAHF_LM added universally

### v1.42

- 92 more CPU models (Intel 12th–14th gen, AMD Ryzen 7000/9000/X3D, APUs)
- 239 Xeon models (Prestonia through Broadwell-EP)
- Anti-VM properties (hide-kvm, hide-hv, spoof-bios, spoof-board)
- Duplicate entry resolution

### v1.41b

- Preset profile system
- Hypervisor detection countermeasures
- Fan controller, PWM, DDR5 SPD
- Intel NPUs/GPUs (136 stubs), AMD XDNA NPUs
- ARM64 phone SoCs (50+ new)
- 377+ x86 CPU models

---

## License

**GPL-2.0-or-later** — Same as upstream QEMU. All code in this fork is licensed under GPL-2.0 or any later version. See `COPYING` and `COPYING.LIB` for details.

---

## Resources

- **QEMU Official Docs:** https://www.qemu.org/documentation/
- **CPUID Reference:** http://sandpile.org/x86/cpuid.shtml
- **PCI Device IDs:** https://pciids.sourceforge.net/
- **Windows Hardware Compat:** https://docs.microsoft.com/en-us/windows-hardware/

---

## Acknowledgments

This fork builds on the excellent QEMU emulator maintained by the QEMU project. All enhancements are contributions to improve emulation breadth and ease-of-use for retro systems, embedded platforms and so on
