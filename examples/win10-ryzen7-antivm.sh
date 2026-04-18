#!/usr/bin/env bash
# Windows 10 on a Ryzen 7 7700X with 8GB RAM and fake 32GB SMBIOS
# Uses anti-VM features: hides KVM, spoofs BIOS vendor/board
bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm,fake-ram-mb=32768,hide-kvm-features=on,hide-hv-signature=on,spoof-bios-vendor="American Megatrends International LLC",spoof-bios-version="2.17.19",spoof-board-vendor="ASUSTeK COMPUTER INC.",spoof-board-product="PRIME X670E-PRO" \
    -cpu AMD-Ryzen-7-7700X \
    -m 8192 \
    -smp 8 \
    -vga std \
    -hda win10.qcow2 \
    -nic user \
    -device intel-hd-audio \
    -device qemu-xhci \
    -device usb-tablet
