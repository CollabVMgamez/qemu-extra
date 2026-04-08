# qemu-extra

A fork of [QEMU](https://www.qemu.org/) adding extensive CPU model libraries, era-accurate PC machine types, and GPU identification stubs for use with hardware detection tools like CPU-Z and GPU-Z.

---

## What's added

### CPU Models (~301 named models)

**Intel Pentium 4 / NetBurst (Family 15)**
- Willamette, Northwood (all steppings), Prescott (all steppings), Gallatin EE, Cedar Mill, Smithfield, Presler, Xeon MP

**Intel Celeron / Celeron D**
- Covington 266MHz through Wolfdale E3400 (1998–2009)

**Intel Core 2 (Family 6)**
- Solo: U2100, U3500
- Duo: E6300, E6600, E8400, E8500, T7600, P8600
- Quad: Q6600, Q9300, Q9550 (correct dual-die cache topology)
- Extreme: QX6700, QX9770 (unlocked multiplier via MSR_IA32_PERF_CTL)

**Intel Xeon (93 models)**
- Nehalem-EP, Westmere-EP, Sandy Bridge-EP, Ivy Bridge-EP, Haswell-EP
- Broadwell-EP/DE, Skylake-SP, Cascade Lake, Ice Lake-SP
- Sapphire Rapids, Emerald Rapids, Xeon W workstation, Xeon D embedded

**AMD Athlon 64 / K8 (Family 15)**
- SledgeHammer, ClawHammer, NewCastle, Winchester, Venice, San Diego, Manchester, Toledo, Orleans, Brisbane

**AMD Phenom II / Athlon II / K10 (Family 16)**
- Phenom II X2/X3/X4/X6 (Callisto, Heka, Deneb, Thuban)
- Athlon II X2/X3/X4 (Regor, Rana, Propus — correct L3=0 for Regor)

**AMD FX / Bulldozer family (Family 21)**
- Bulldozer: FX-4100/6100/8100/8120/8150
- Piledriver: FX-4300/4350/6300/6350/8300/8320/8350
- Steamroller APU: A10-7850K, A10-7800, A8-7600
- Excavator APU: A10-8750, A12-9800
- Opteron: 6380, 6344, 4386

**AMD Ryzen / Zen (Families 23, 25, 26)**
- Ryzen 1000–9000 (Zen through Zen5)
- Threadripper gen 1–5 (TR4, TRX40, sWRX8, sTR5)
- Cezanne APU (Ryzen 5/7 G-series)

---

### GPU Stubs (~206 devices)

PCI devices with correct vendor/device/subsystem IDs, clock fluctuation, temperature, and memory clock readable by GPU-Z.

**NVIDIA GTX (desktop + mobile)**
- Fermi 400/500, Kepler 600/700, Maxwell 900, Pascal 1000, Turing 1600

**NVIDIA Datacenter**
- Tesla K40/K80, Pascal P4/P40/P100, Volta V100/V100S
- Quadro RTX 4000–8000 (Turing), Ampere A10/A16/A30/A40/A100/A800
- Hopper H100/H200/H800, Ada L4/L40/L40S, Blackwell B100/B200, DGX nodes

**NVIDIA RTX**
- RTX 5090 (GB202, Blackwell)

**AMD Radeon (ATI/AMD)**
- AGP era: Rage 128, Radeon DDR/SDR, R100–R480 (Radeon 7000 through X850 XT PE)
- PCIe era: X1000, HD 2000–6000 series
- GCN: HD 7000, R7/R9 200/300 (including R9 Fury/Fury X/Nano HBM)
- RDNA: RX 460–590 (Polaris), Vega 56/64, RX 5000–7000 series

**GPU-Z visible on all devices:**
- GPU clock (fluctuates between base/boost MHz)
- Memory clock (correct per-generation frequency)
- GPU temperature (55–72°C fluctuating)
- Fan speed (30–45%)
- PCIe link width and generation
- VRAM size and type

---

### Machine Types (17 Intel + 10 NVIDIA nForce)

**Intel P4-era chipsets**
```
pc-i850    pc-i845    pc-i845g   pc-i845e   pc-i845pe  pc-i850e
pc-i865g   pc-i865pe  pc-i875p   pc-i925x   pc-i915p   pc-i915g
pc-i925xe  pc-i945p   pc-i945g   pc-i955x   pc-i975x
```

**NVIDIA nForce chipsets**
```
pc-nforce  pc-nforce2  pc-nforce2-ultra  pc-nforce3-150  pc-nforce3-250
pc-nforce3-ultra  pc-nforce4  pc-nforce4-ultra  pc-nforce4-sli  pc-nforce4-sli-x16
```

All machines support the `ram-type` property: `ddr`, `ddr2`, `ddr3`, `ddr4`

---

### Other Fixes

- **AMD brand ID** — Phenom II/Athlon no longer reports as Opteron in CPU-Z
- **Intel brand index** — Pentium 4 Prescott N0 no longer reports as Xeon Potomac
- **Host vendor override** — Named CPU models no longer inherit host CPU vendor under KVM
- **NULL L3 segfault** — Fixed crash on CPUs with no L3 cache under KVM
- **Core 2 Extreme** — Unlocked multiplier via `MSR_IA32_PERF_CTL` (0x199)
- **Cache topology** — Correct Kentsfield/Yorkfield dual-die L2, Phenom II 6MB shared L3, Regor 1MB/core no-L3
- **DDR3/DDR4 SPD** — CPU-Z Memory tab shows correct RAM type via SMBus EEPROM
- **Polaris RX IDs** — RX 470/480/570/580/590 have unique subsystem IDs so Adrenalin installer identifies them correctly

---

## Build

### Dynamic (Ubuntu/Debian — recommended)

```bash
bash configure \
  --target-list=x86_64-softmmu \
  --disable-werror \
  --disable-docs \
  --disable-debug-info \
  --disable-xen \
  --prefix=/usr/local

ninja -C build -j$(nproc)
```

### Static (Arch / any distro — no runtime dependencies)

```bash
bash configure \
  --target-list=x86_64-softmmu \
  --disable-werror \
  --disable-docs \
  --disable-debug-info \
  --disable-xen \
  --disable-opengl \
  --disable-gtk \
  --disable-sdl \
  --disable-slirp \
  --disable-libdaxctl \
  --disable-libudev \
  --disable-spice \
  --disable-smartcard \
  --disable-usb-redir \
  --disable-curl \
  --without-default-features \
  --enable-kvm \
  --enable-vhost-net \
  --audio-drv-list= \
  --static \
  --prefix=/usr/local

ninja -C build -j$(nproc)
```

Static build has no display window — use `-vnc :0` and connect with a VNC client.

---

## Usage

### Basic Windows XP (P4 era)

```bash
./run.sh \
  -machine pc-i875p,accel=kvm,ram-type=ddr \
  -cpu pentium4-northwood-d1-v1 \
  -m 512M \
  -hda winxp.qcow2 \
  -cdrom winxp.iso \
  -vnc :0
```

### Windows 7 with Core 2 Quad and GTX 1080

```bash
./run.sh \
  -machine pc,accel=kvm,ram-type=ddr3 \
  -cpu core2-quad-q9550-v1 \
  -smp cores=4 \
  -m 4G \
  -hda win7.qcow2 \
  -device nvidia-gtx1080 \
  -vnc :0
```

### Windows 10 with Ryzen 9 and RX 6900 XT

```bash
./run.sh \
  -machine pc,accel=kvm,ram-type=ddr4 \
  -cpu ryzen9-5950x-v1 \
  -smp cores=16 \
  -m 16G \
  -hda win10.qcow2 \
  -device amd-rx6900xt \
  -vnc :0
```

### List all available CPUs

```bash
./qemu-system-x86_64 -cpu help
```

### List all available GPU devices

```bash
./qemu-system-x86_64 -device help | grep -E "nvidia-|amd-|ati-"
```

---

## Downloads

Pre-built binaries are on the [Releases](https://github.com/CollabVMgamez/qemu-extra/releases) page:

| File | Description |
|---|---|
| `qemu-p4fork-x86_64.zip` | Dynamic build — Ubuntu/Debian (full features) |
| `qemu-p4fork-x86_64-static.zip` | Static build — any Linux distro, zero dependencies |

---

## Notes

- GPU stubs are **identification only** — drivers will show Code 43 at runtime since there is no real GPU hardware. GPU-Z, Device Manager, and driver installers will recognize the device correctly.
- AGP-era cards (Rage 128, Radeon 9800, etc.) appear on the PCI bus — QEMU has no native AGP bus implementation.
- For actual GPU acceleration, use KVM with VFIO passthrough of a physical GPU.
- The static build drops SDL/GTK/SPICE/audio drivers — use `-vnc :0` for display.
