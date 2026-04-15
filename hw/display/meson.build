hw_display_modules = {}

system_ss.add(when: 'CONFIG_DDC', if_true: files('i2c-ddc.c'))
system_ss.add(when: 'CONFIG_EDID', if_true: files('edid-generate.c', 'edid-region.c'))

system_ss.add(when: 'CONFIG_FW_CFG_DMA', if_true: files('ramfb.c'))
stub_ss.add(files('ramfb-stubs.c'))
system_ss.add(when: 'CONFIG_FW_CFG_DMA', if_true: files('ramfb-standalone.c'))

system_ss.add(when: 'CONFIG_VGA_CIRRUS', if_true: files('cirrus_vga.c'))
system_ss.add(when: ['CONFIG_VGA_CIRRUS', 'CONFIG_VGA_ISA'], if_true: files('cirrus_vga_isa.c'))
system_ss.add(when: 'CONFIG_G364FB', if_true: files('g364fb.c'))
system_ss.add(when: 'CONFIG_JAZZ_LED', if_true: files('jazz_led.c'))
system_ss.add(when: 'CONFIG_PL110', if_true: files('pl110.c'))
system_ss.add(when: 'CONFIG_SII9022', if_true: files('sii9022.c'))
system_ss.add(when: 'CONFIG_SSD0303', if_true: files('ssd0303.c'))
system_ss.add(when: 'CONFIG_SSD0323', if_true: files('ssd0323.c'))
system_ss.add(when: 'CONFIG_XEN_BUS', if_true: files('xenfb.c'))

system_ss.add(when: 'CONFIG_VGA_ISA', if_true: files('vga-isa.c'))
system_ss.add(when: 'CONFIG_VGA_MMIO', if_true: files('vga-mmio.c'))
system_ss.add(when: 'CONFIG_VMWARE_VGA', if_true: files('vmware_vga.c'))
system_ss.add(when: 'CONFIG_BOCHS_DISPLAY', if_true: files('bochs-display.c'))
system_ss.add(when: 'CONFIG_VGA_PCI', if_true: files('vga-pci.c'))

system_ss.add(when: 'CONFIG_EXYNOS4', if_true: files('exynos4210_fimd.c'))
system_ss.add(when: 'CONFIG_FRAMEBUFFER', if_true: files('framebuffer.c'))

system_ss.add(when: 'CONFIG_RASPI', if_true: files('bcm2835_fb.c'))
system_ss.add(when: 'CONFIG_SM501', if_true: files('sm501.c'))
system_ss.add(when: 'CONFIG_TCX', if_true: files('tcx.c'))
system_ss.add(when: 'CONFIG_CG3', if_true: files('cg3.c'))
system_ss.add(when: 'CONFIG_MACFB', if_true: files('macfb.c'))
system_ss.add(when: 'CONFIG_NEXTCUBE', if_true: files('next-fb.c'))

system_ss.add(when: 'CONFIG_VGA', if_true: files('vga.c'))
system_ss.add(when: 'CONFIG_VIRTIO', if_true: files('virtio-dmabuf.c'))
system_ss.add(when: 'CONFIG_DM163', if_true: files('dm163.c'))

stub_ss.add([files('acpi-vga-stub.c'), pixman])
if (config_all_devices.has_key('CONFIG_VGA_CIRRUS') or
    config_all_devices.has_key('CONFIG_VGA_PCI') or
    config_all_devices.has_key('CONFIG_VMWARE_VGA') or
    config_all_devices.has_key('CONFIG_ATI_VGA')
   )
  system_ss.add(when: 'CONFIG_ACPI', if_true: files('acpi-vga.c'))
endif

if config_all_devices.has_key('CONFIG_QXL')
  qxl_ss = ss.source_set()
  qxl_ss.add(when: 'CONFIG_QXL', if_true: [files('qxl.c', 'qxl-logger.c', 'qxl-render.c'),
                                           pixman, spice])
  qxl_ss.add(when: 'CONFIG_ACPI', if_true: files('acpi-vga.c'))
  hw_display_modules += {'qxl': qxl_ss}
endif

system_ss.add(when: 'CONFIG_DPCD', if_true: files('dpcd.c'))
system_ss.add(when: 'CONFIG_XLNX_DISPLAYPORT', if_true: files('xlnx_dp.c'))

system_ss.add(when: 'CONFIG_ARTIST', if_true: files('artist.c'))

system_ss.add(when: 'CONFIG_ATI_VGA', if_true: [files('ati.c', 'ati_2d.c', 'ati_dbg.c'), pixman])

system_ss.add(when: [pvg, 'CONFIG_MAC_PVG_PCI'],     if_true: [files('apple-gfx.m', 'apple-gfx-pci.m')])
system_ss.add(when: [pvg, 'CONFIG_MAC_PVG_MMIO'],    if_true: [files('apple-gfx.m', 'apple-gfx-mmio.m')])

if config_all_devices.has_key('CONFIG_VIRTIO_GPU')
  virtio_gpu_ss = ss.source_set()
  virtio_gpu_ss.add(when: 'CONFIG_VIRTIO_GPU',
                    if_true: [files('virtio-gpu-base.c', 'virtio-gpu.c'), pixman])
  if host_os == 'linux'
    virtio_gpu_ss.add(files('virtio-gpu-udmabuf.c'))
  else
    virtio_gpu_ss.add(files('virtio-gpu-udmabuf-stubs.c'))
  endif
  virtio_gpu_ss.add(when: 'CONFIG_VHOST_USER_GPU', if_true: files('vhost-user-gpu.c'))
  hw_display_modules += {'virtio-gpu': virtio_gpu_ss}

  if virgl.found() and opengl.found()
    virtio_gpu_gl_ss = ss.source_set()
    virtio_gpu_gl_ss.add(when: ['CONFIG_VIRTIO_GPU', virgl, opengl],
                         if_true: [files('virtio-gpu-gl.c', 'virtio-gpu-virgl.c'), pixman, virgl])
    hw_display_modules += {'virtio-gpu-gl': virtio_gpu_gl_ss}
  endif

  if rutabaga.found()
    virtio_gpu_rutabaga_ss = ss.source_set()
    virtio_gpu_rutabaga_ss.add(when: ['CONFIG_VIRTIO_GPU', rutabaga],
                               if_true: [files('virtio-gpu-rutabaga.c'), pixman])
    hw_display_modules += {'virtio-gpu-rutabaga': virtio_gpu_rutabaga_ss}
  endif
endif

if config_all_devices.has_key('CONFIG_VIRTIO_PCI')
  virtio_gpu_pci_ss = ss.source_set()
  virtio_gpu_pci_ss.add(when: ['CONFIG_VIRTIO_GPU', 'CONFIG_VIRTIO_PCI'],
                        if_true: [files('virtio-gpu-pci.c'), pixman])
  virtio_gpu_pci_ss.add(when: ['CONFIG_VHOST_USER_GPU', 'CONFIG_VIRTIO_PCI'],
                        if_true: files('vhost-user-gpu-pci.c'))
  hw_display_modules += {'virtio-gpu-pci': virtio_gpu_pci_ss}

  if virgl.found() and opengl.found()
    virtio_gpu_pci_gl_ss = ss.source_set()
    virtio_gpu_pci_gl_ss.add(when: ['CONFIG_VIRTIO_GPU', 'CONFIG_VIRTIO_PCI', virgl, opengl],
                             if_true: [files('virtio-gpu-pci-gl.c'), pixman])
    hw_display_modules += {'virtio-gpu-pci-gl': virtio_gpu_pci_gl_ss}
  endif
  if rutabaga.found()
    virtio_gpu_pci_rutabaga_ss = ss.source_set()
    virtio_gpu_pci_rutabaga_ss.add(when: ['CONFIG_VIRTIO_GPU', 'CONFIG_VIRTIO_PCI', rutabaga],
                                   if_true: [files('virtio-gpu-pci-rutabaga.c'), pixman])
    hw_display_modules += {'virtio-gpu-pci-rutabaga': virtio_gpu_pci_rutabaga_ss}
  endif
endif

if config_all_devices.has_key('CONFIG_VIRTIO_VGA')
  virtio_vga_ss = ss.source_set()
  virtio_vga_ss.add(when: 'CONFIG_VIRTIO_VGA',
                    if_true: [files('virtio-vga.c'), pixman])
  virtio_vga_ss.add(when: 'CONFIG_VHOST_USER_VGA',
                    if_true: files('vhost-user-vga.c'))
  virtio_vga_ss.add(when: 'CONFIG_ACPI', if_true: files('acpi-vga.c'))
  hw_display_modules += {'virtio-vga': virtio_vga_ss}

  if virgl.found() and opengl.found()
    virtio_vga_gl_ss = ss.source_set()
    virtio_vga_gl_ss.add(when: ['CONFIG_VIRTIO_VGA', virgl, opengl],
                         if_true: [files('virtio-vga-gl.c'), pixman])
    virtio_vga_gl_ss.add(when: 'CONFIG_ACPI', if_true: files('acpi-vga.c'))
    hw_display_modules += {'virtio-vga-gl': virtio_vga_gl_ss}
  endif

  if rutabaga.found()
    virtio_vga_rutabaga_ss = ss.source_set()
    virtio_vga_rutabaga_ss.add(when: ['CONFIG_VIRTIO_VGA', rutabaga],
                               if_true: [files('virtio-vga-rutabaga.c'), pixman])
    virtio_vga_rutabaga_ss.add(when: 'CONFIG_ACPI', if_true: files('acpi-vga.c'))
    hw_display_modules += {'virtio-vga-rutabaga': virtio_vga_rutabaga_ss}
  endif
endif

system_ss.add(when: 'CONFIG_OMAP', if_true: files('omap_lcdc.c'))

modules += { 'hw-display': hw_display_modules }

system_ss.add(when: 'CONFIG_VGA_PCI', if_true: files('cerebras-wse-1.c', 'cerebras-wse-2.c', 'cerebras-wse-3.c', 'google-tpu-v2.c', 'google-tpu-v3.c', 'adreno-630.c', 'adreno-640.c', 'adreno-650.c', 'adreno-660.c', 'adreno-730.c', 'adreno-740.c', 'adreno-750.c', 'amd-radeon-hd7450.c', 'amd-radeon-hd7770.c', 'amd-radeon-hd7850.c', 'amd-radeon-hd7870.c', 'amd-radeon-hd7950.c', 'amd-radeon-hd7970.c', 'amd-radeon-hd7970ghz.c', 'amd-radeon-hd7990.c', 'amd-radeon-r7-250.c', 'amd-radeon-r7-260x.c', 'amd-radeon-r7-360.c', 'amd-radeon-r9-270.c', 'amd-radeon-r9-270x.c', 'amd-radeon-r9-280.c', 'amd-radeon-r9-280x.c', 'amd-radeon-r9-290.c', 'amd-radeon-r9-290x.c', 'amd-radeon-r9-295x2.c', 'amd-radeon-r9-370.c', 'amd-radeon-r9-380.c', 'amd-radeon-r9-380x.c', 'amd-radeon-r9-390.c', 'amd-radeon-r9-390x.c', 'amd-radeon-r9-fury.c', 'amd-radeon-r9-furyx.c', 'amd-radeon-r9-nano.c', 'amd-rx-vega56.c', 'amd-rx-vega64.c', 'amd-rx460.c', 'amd-rx470.c', 'amd-rx480.c', 'amd-rx550.c', 'amd-rx5500xt.c', 'amd-rx560.c', 'amd-rx5600xt.c', 'amd-rx570.c', 'amd-rx5700.c', 'amd-rx5700xt.c', 'amd-rx580.c', 'amd-rx590.c', 'amd-rx6600.c', 'amd-rx6600xt.c', 'amd-rx6700xt.c', 'amd-rx6800.c', 'amd-rx6800xt.c', 'amd-rx6900xt.c', 'amd-rx7600.c', 'amd-rx7700xt.c', 'amd-rx7800xt.c', 'amd-rx7900xt.c', 'amd-rx7900xtx.c', 'apple-gpu-m1-pro.c', 'apple-gpu-m1.c', 'apple-gpu-m2.c', 'apple-gpu-m3.c', 'ati-radeon-7000.c', 'ati-radeon-7200.c', 'ati-radeon-8500.c', 'ati-radeon-8500le.c', 'ati-radeon-9000.c', 'ati-radeon-9100.c', 'ati-radeon-9200.c', 'ati-radeon-9200se.c', 'ati-radeon-9500.c', 'ati-radeon-9500pro.c', 'ati-radeon-9700.c', 'ati-radeon-9700pro.c', 'ati-radeon-9800.c', 'ati-radeon-9800pro.c', 'ati-radeon-9800xt.c', 'ati-radeon-ddr.c', 'ati-radeon-hd2400.c', 'ati-radeon-hd2600.c', 'ati-radeon-hd2900.c', 'ati-radeon-hd3450.c', 'ati-radeon-hd3650.c', 'ati-radeon-hd3870.c', 'ati-radeon-hd4350.c', 'ati-radeon-hd4650.c', 'ati-radeon-hd4670.c', 'ati-radeon-hd4830.c', 'ati-radeon-hd4850.c', 'ati-radeon-hd4870.c', 'ati-radeon-hd4870x2.c', 'ati-radeon-hd4890.c', 'ati-radeon-hd5450.c', 'ati-radeon-hd5570.c', 'ati-radeon-hd5670.c', 'ati-radeon-hd5750.c', 'ati-radeon-hd5770.c', 'ati-radeon-hd5850.c', 'ati-radeon-hd5870.c', 'ati-radeon-hd5970.c', 'ati-radeon-hd6450.c', 'ati-radeon-hd6570.c', 'ati-radeon-hd6670.c', 'ati-radeon-hd6790.c', 'ati-radeon-hd6850.c', 'ati-radeon-hd6870.c', 'ati-radeon-hd6950.c', 'ati-radeon-hd6970.c', 'ati-radeon-hd6990.c', 'ati-radeon-sdr.c', 'ati-radeon-x1300.c', 'ati-radeon-x1600.c', 'ati-radeon-x1800.c', 'ati-radeon-x1900xt.c', 'ati-radeon-x1900xtx.c', 'ati-radeon-x800.c', 'ati-radeon-x800pro.c', 'ati-radeon-x800xl.c', 'ati-radeon-x800xt.c', 'ati-radeon-x850xtpe.c', 'ati-rage128gl.c', 'ati-rage128pro.c', 'immortalis-g715.c', 'immortalis-g720.c', 'mali-g52.c', 'mali-g715.c', 'mali-g72.c', 'mali-g76.c', 'mali-g78.c', 'nvidia-a10.c', 'nvidia-a100-40gb.c', 'nvidia-a100-80gb.c', 'nvidia-a100-pcie.c', 'nvidia-a16.c', 'nvidia-a30.c', 'nvidia-a40.c', 'nvidia-a800.c', 'nvidia-b100.c', 'nvidia-b200.c', 'nvidia-dgx-a100.c', 'nvidia-dgx-h100.c', 'nvidia-g72-geforce7300le.c', 'nvidia-g73-geforce7600gs.c', 'nvidia-gtx1050.c', 'nvidia-gtx1050m.c', 'nvidia-gtx1050ti.c', 'nvidia-gtx1060.c', 'nvidia-gtx1060m.c', 'nvidia-gtx1070.c', 'nvidia-gtx1070m.c', 'nvidia-gtx1070ti.c', 'nvidia-gtx1080.c', 'nvidia-gtx1080m.c', 'nvidia-gtx1080ti.c', 'nvidia-gtx1650.c', 'nvidia-gtx1650m.c', 'nvidia-gtx1650s.c', 'nvidia-gtx1660.c', 'nvidia-gtx1660s.c', 'nvidia-gtx1660ti.c', 'nvidia-gtx1660tim.c', 'nvidia-gtx460.c', 'nvidia-gtx470.c', 'nvidia-gtx480.c', 'nvidia-gtx480m.c', 'nvidia-gtx550ti.c', 'nvidia-gtx560.c', 'nvidia-gtx560m.c', 'nvidia-gtx560ti.c', 'nvidia-gtx570.c', 'nvidia-gtx580.c', 'nvidia-gtx580m.c', 'nvidia-gtx590.c', 'nvidia-gtx650.c', 'nvidia-gtx650m.c', 'nvidia-gtx650ti.c', 'nvidia-gtx660.c', 'nvidia-gtx660m.c', 'nvidia-gtx660ti.c', 'nvidia-gtx670.c', 'nvidia-gtx670m.c', 'nvidia-gtx680.c', 'nvidia-gtx680m.c', 'nvidia-gtx690.c', 'nvidia-gtx750.c', 'nvidia-gtx750ti.c', 'nvidia-gtx760.c', 'nvidia-gtx760m.c', 'nvidia-gtx770.c', 'nvidia-gtx770m.c', 'nvidia-gtx780.c', 'nvidia-gtx780m.c', 'nvidia-gtx780ti.c', 'nvidia-gtx950.c', 'nvidia-gtx950m.c', 'nvidia-gtx960.c', 'nvidia-gtx960m.c', 'nvidia-gtx970.c', 'nvidia-gtx970m.c', 'nvidia-gtx980.c', 'nvidia-gtx980m.c', 'nvidia-gtx980ti.c', 'nvidia-h100-nvl.c', 'nvidia-h100-pcie.c', 'nvidia-h100-sxm5.c', 'nvidia-h200.c', 'nvidia-h800.c', 'nvidia-k40.c', 'nvidia-k80.c', 'nvidia-l4.c', 'nvidia-l40.c', 'nvidia-l40s.c', 'nvidia-nv10-geforce256.c', 'nvidia-nv10-geforce256ddr.c', 'nvidia-nv11-geforce2mx.c', 'nvidia-nv15-geforce2gts.c', 'nvidia-nv17-geforce4mx420.c', 'nvidia-nv17-geforce4mx440.c', 'nvidia-nv18-geforce4mx440se.c', 'nvidia-nv25-geforce4ti4200.c', 'nvidia-nv25-geforce4ti4400.c', 'nvidia-nv25-geforce4ti4600.c', 'nvidia-nv28-geforce4ti4200-8x.c', 'nvidia-nv28-geforce4ti4800.c', 'nvidia-nv3-riva128.c', 'nvidia-nv34-geforcefx5200.c', 'nvidia-nv35-geforcefx5900.c', 'nvidia-nv35-geforcefx5900xt.c', 'nvidia-nv36-geforcefx5700.c', 'nvidia-nv36-geforcefx5700ultra.c', 'nvidia-nv38-geforcefx5900ultra.c', 'nvidia-nv3t-riva128zx.c', 'nvidia-nv4-tnt.c', 'nvidia-nv40-geforce6800ultra.c', 'nvidia-nv41-geforce6800.c', 'nvidia-nv43-geforce6600gt.c', 'nvidia-nv44-geforce6200.c', 'nvidia-nv5-tnt2.c', 'nvidia-nv5m64-tnt2m64.c', 'nvidia-nv6-vanta.c', 'nvidia-p100-pcie.c', 'nvidia-p100-sxm2.c', 'nvidia-p4.c', 'nvidia-p40.c', 'nvidia-rtx4000.c', 'nvidia-rtx5000.c', 'nvidia-rtx5090.c', 'nvidia-rtx6000.c', 'nvidia-rtx8000.c', 'nvidia-v100-pcie.c', 'nvidia-v100-sxm2.c', 'nvidia-v100s.c', 'samsung-xclipse-920.c', 'samsung-xclipse-940.c'))

# v1.36: Intel GPUs (iGPU + Arc dGPU), AMD datacenter, NVIDIA GB300/DGX
system_ss.add(when: 'CONFIG_VGA_PCI', if_true: files(
  'intel-uhd-630.c', 'intel-uhd-730.c', 'intel-uhd-770.c',
  'intel-iris-xe.c', 'intel-iris-xe2.c',
  'intel-arc-a380.c', 'intel-arc-a580.c', 'intel-arc-a750.c',
  'intel-arc-a770.c', 'intel-arc-b580.c', 'intel-arc-b770.c',
  'amd-mi100.c', 'amd-mi200.c', 'amd-mi210.c', 'amd-mi250.c', 'amd-mi250x.c',
  'amd-mi300a.c', 'amd-mi300x.c', 'amd-mi308x.c', 'amd-mi325x.c', 'amd-mi350x.c',
  'amd-radeon-pro-w7900.c', 'amd-radeon-pro-v620.c',
  'nvidia-gb300.c', 'nvidia-gb300-nvl.c',
  'nvidia-dgx-b100.c', 'nvidia-dgx-b200.c', 'nvidia-dgx-gb200.c',
  'nvidia-h20.c', 'nvidia-l40s-48gb.c',
  'nvidia-rtx5080.c', 'nvidia-rtx5070ti.c', 'nvidia-rtx5070.c', 'nvidia-rtx5060ti.c'))

# v1.37: Mobile/SoC GPU stubs (Adreno, Mali, Immortalis, Xclipse, Apple, PowerVR, Rockchip, mobile dGPUs)
system_ss.add(when: 'CONFIG_VGA_PCI', if_true: files(
  'adreno-610.c', 'adreno-620.c', 'adreno-680.c', 'adreno-690.c', 'adreno-710.c',
  'adreno-830.c', 'adreno-830a.c',
  'mali-g57.c', 'mali-g610.c', 'mali-g620.c', 'mali-g710.c', 'mali-g820.c',
  'immortalis-g920.c',
  'samsung-xclipse-960.c',
  'apple-gpu-a14.c', 'apple-gpu-a15.c', 'apple-gpu-a16.c', 'apple-gpu-a17.c',
  'apple-gpu-a18.c', 'apple-gpu-a18-pro.c', 'apple-gpu-a19.c', 'apple-gpu-a19-pro.c',
  'apple-gpu-m4.c', 'apple-gpu-m4-pro.c', 'apple-gpu-m4-max.c', 'apple-gpu-m4-ultra.c',
  'apple-gpu-m5.c', 'apple-gpu-m5-pro.c', 'apple-gpu-m5-max.c', 'apple-gpu-m5-ultra.c',
  'rockchip-mali-t860.c', 'rockchip-mali-g52.c', 'rockchip-mali-g610.c',
  'powervr-bxe-4-32.c', 'powervr-bxm-4-64.c', 'powervr-dxm-2-256.c',
  'kirin-mali-g620.c', 'kirin-mali-g715.c',
  'intel-arc-a370m.c', 'intel-arc-a570m.c', 'intel-arc-a730m.c', 'intel-arc-a770m.c', 'intel-arc-bm-g21.c',
  'nvidia-rtx4050m.c', 'nvidia-rtx4060m.c', 'nvidia-rtx4070m.c', 'nvidia-rtx4080m.c', 'nvidia-rtx4090m.c',
  'nvidia-rtx5060m.c', 'nvidia-rtx5070m.c', 'nvidia-rtx5080m.c',
  'amd-rx7600m.c', 'amd-rx7700m.c', 'amd-rx7800m.c', 'amd-rx8900m.c',
  'amd-radeon-780m.c', 'amd-radeon-890m.c', 'amd-radeon-90m.c'))

# v1.38: More mobile/embedded/automotive GPU stubs
system_ss.add(when: 'CONFIG_VGA_PCI', if_true: files(
  'adreno-506.c', 'adreno-509.c', 'adreno-510.c', 'adreno-512.c', 'adreno-530.c', 'adreno-540.c',
  'adreno-615.c', 'adreno-640-auto.c', 'adreno-660-auto.c', 'adreno-740-auto.c', 'adreno-750-auto.c',
  'mali-g310.c', 'mali-g410.c', 'mali-g450.c', 'mali-g510.c', 'mali-g380.c', 'mali-g31.c', 'mali-g920.c',
  'apple-gpu-a12x.c', 'apple-gpu-a12z.c', 'apple-gpu-a13.c',
  'apple-gpu-m1-ipad.c', 'apple-gpu-m2-ipad.c', 'apple-gpu-m3-ipad.c',
  'nvidia-tegra-x1-gpu.c', 'nvidia-tegra-x2-gpu.c', 'nvidia-ortin-gpu.c', 'nvidia-thor-gpu.c',
  'intel-uhd-mobile-rpl.c', 'intel-iris-xe-mobile-rpl.c', 'intel-uhd-mobile-adl.c', 'intel-iris-xe-mobile-adl.c',
  'kirin-mali-g510.c', 'kirin-mali-g51.c',
  'samsung-mali-g68.c', 'samsung-mali-g78.c',
   'broadcom-videocore-vi.c', 'broadcom-videocore-vii.c',
    'custom-gpu.c'))

# v1.41: Intel iGPU/dGPU/GNA stubs — Sandy Bridge through Lunar Lake, Arc Pro, GNA
system_ss.add(when: 'CONFIG_VGA_PCI', if_true: files(
  'intel-hd-2000.c', 'intel-hd-3000.c', 'intel-hd-2000-snb-server.c',
  'intel-hd-2500.c', 'intel-hd-4000.c', 'intel-hd-2500-ivb-server.c', 'intel-hd-4000-ivb-server.c',
  'intel-hd-4600.c', 'intel-hd-4400.c', 'intel-hd-5000.c',
  'intel-iris-pro-5200.c', 'intel-iris-5100.c', 'intel-hd-4200.c',
  'intel-hd-4600-hsw-server.c', 'intel-iris-pro-p5200.c',
  'intel-hd-5500.c', 'intel-hd-6000.c', 'intel-iris-pro-6200.c', 'intel-iris-6100.c',
  'intel-hd-530.c', 'intel-hd-520.c', 'intel-hd-515.c', 'intel-hd-p530.c',
  'intel-iris-pro-p580.c', 'intel-hd-540.c',
  'intel-hd-620.c', 'intel-hd-630-kbl.c', 'intel-hd-615.c', 'intel-hd-640.c',
  'intel-iris-plus-640.c', 'intel-iris-plus-650.c',
  'intel-uhd-620-kbl.c', 'intel-uhd-615-kbl.c', 'intel-uhd-617.c',
  'intel-uhd-600.c', 'intel-uhd-605.c', 'intel-hd-505.c', 'intel-hd-500.c',
  'intel-uhd-630-cfl-h.c', 'intel-uhd-610.c',
  'intel-iris-plus-655.c', 'intel-iris-plus-645.c', 'intel-uhd-620-cfl.c',
  'intel-uhd-620-cml.c', 'intel-uhd-630-cml.c', 'intel-uhd-p630-cml.c', 'intel-uhd-617-cml.c',
  'intel-iris-plus-g7.c', 'intel-iris-plus-g4.c', 'intel-uhd-g1.c',
  'intel-uhd-jasper-lake.c', 'intel-uhd-elkhart-lake.c',
  'intel-uhd-750.c', 'intel-uhd-770-rkl.c',
  'intel-iris-xe-tgl-u.c', 'intel-uhd-tgl.c',
  'intel-uhd-710.c', 'intel-uhd-770-adl.c', 'intel-uhd-graphics-adl-n.c',
  'intel-uhd-770-rpl-s.c', 'intel-uhd-730-rpl-s.c',
  'intel-iris-xe-mtl-u.c', 'intel-iris-xe2-lnl.c',
  'intel-arc-pro-a40.c', 'intel-arc-pro-a50.c', 'intel-arc-pro-a30m.c',
  'intel-arc-pro-a40m.c', 'intel-arc-pro-a50m.c', 'intel-arc-pro-a60.c', 'intel-arc-pro-a60m.c',
  'intel-arc-pro-b50.c', 'intel-arc-pro-b60.c',
  'intel-hd-4000-bay-trail.c', 'intel-hd-4000-braswell.c',
  'intel-hd-505-apollo-lake.c', 'intel-hd-500-apollo-lake.c',
  'intel-gna-1-0.c', 'intel-gna-2-0.c', 'intel-gna-3-0.c', 'intel-gna-3-5.c'))

# v1.41b: All RTX 20/30/40/50 series + RTX PRO workstation + 5090 variants
system_ss.add(when: 'CONFIG_VGA_PCI', if_true: files(
  'nvidia-rtx2060.c', 'nvidia-rtx2060s.c', 'nvidia-rtx2060-12gb.c',
  'nvidia-rtx2070.c', 'nvidia-rtx2070s.c', 'nvidia-rtx2080.c', 'nvidia-rtx2080s.c',
  'nvidia-rtx2080ti.c', 'nvidia-titan-rtx.c',
  'nvidia-rtx3050.c', 'nvidia-rtx3050-oem.c', 'nvidia-rtx3060.c',
  'nvidia-rtx3060ti.c', 'nvidia-rtx3060ti-gddr6x.c', 'nvidia-rtx3070.c', 'nvidia-rtx3070ti.c',
  'nvidia-rtx3080.c', 'nvidia-rtx3080-12gb.c', 'nvidia-rtx3080ti.c',
  'nvidia-rtx3090.c', 'nvidia-rtx3090ti.c',
  'nvidia-rtx3050m.c', 'nvidia-rtx3050ti-m.c', 'nvidia-rtx3060m.c',
  'nvidia-rtx3070m.c', 'nvidia-rtx3070ti-m.c', 'nvidia-rtx3080m.c', 'nvidia-rtx3080ti-m.c',
  'nvidia-rtx4060.c', 'nvidia-rtx4060ti.c', 'nvidia-rtx4060ti-16gb.c',
  'nvidia-rtx4070.c', 'nvidia-rtx4070s.c', 'nvidia-rtx4070ti.c', 'nvidia-rtx4070tis.c',
  'nvidia-rtx4080.c', 'nvidia-rtx4080s.c', 'nvidia-rtx4090.c', 'nvidia-rtx4090d.c',
  'nvidia-rtx5060.c', 'nvidia-rtx5060ti-8gb.c', 'nvidia-rtx5060ti-16gb.c', 'nvidia-rtx5070ti-16gb.c',
  'nvidia-rtx5090-fe.c', 'nvidia-rtx5090-rog.c', 'nvidia-rtx5090-suprim.c',
  'nvidia-rtx5090-aorus.c', 'nvidia-rtx5090-gaming-oc.c', 'nvidia-rtx5090-phantom.c',
  'nvidia-rtx5090-ventus.c', 'nvidia-rtx5090-igame.c', 'nvidia-rtx5090-pny.c',
  'nvidia-rtxa2000.c', 'nvidia-rtxa2000-12gb.c', 'nvidia-rtxa3000m.c',
  'nvidia-rtxa4000.c', 'nvidia-rtxa4500.c', 'nvidia-rtxa5000.c', 'nvidia-rtxa5500.c',
  'nvidia-rtxa6000.c', 'nvidia-rtxa6000-hbr3.c',
  'nvidia-rtx4000-ada.c', 'nvidia-rtx4500-ada.c', 'nvidia-rtx5000-ada.c', 'nvidia-rtx6000-ada.c',
  'nvidia-rtx-pro-6000.c',
  'nvidia-rtx2060m.c', 'nvidia-rtx2060s-m.c', 'nvidia-rtx2070m.c', 'nvidia-rtx2070s-m.c',
  'nvidia-rtx2080m.c', 'nvidia-rtx2080s-m.c',
  'nvidia-rtx5050m.c', 'nvidia-rtx5070ti-m.c', 'nvidia-rtx5090m.c'))
