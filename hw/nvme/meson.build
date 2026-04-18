system_ss.add(when: 'CONFIG_NVME_PCI', if_true: files('ctrl.c', 'dif.c', 'ns.c', 'subsys.c', 'nguid.c'))

# v1.52: NVMe SSD stubs
system_ss.add(when: 'CONFIG_PCI', if_true: files(
  'samsung-990-pro.c', 'samsung-990-evo.c', 'samsung-990-evo-plus.c',
  'samsung-980-pro.c', 'samsung-980.c', 'samsung-870-evo-plus.c',
  'wd-black-sn850x.c', 'wd-black-sn770.c', 'wd-black-sn770m.c', 'wd-blue-sn580.c',
  'crucial-t700.c', 'crucial-t500.c', 'crucial-p5-plus.c',
  'kingston-fury-renegade-g5.c', 'kingston-fury-renegade.c', 'kingston-nv3.c',
  'skhynix-platinum-p41.c', 'skhynix-gold-p31.c',
  'sabrent-rocket-4-plus-g.c', 'seagate-firecuda-540.c', 'corsair-mp700.c'))