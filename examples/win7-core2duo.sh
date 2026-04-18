#!/usr/bin/env bash
# Windows 7 on a Core 2 Duo E8400 with 2GB RAM
# Good balance of era-accuracy and performance
bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm \
    -cpu Intel-Core-2-Duo-E8400 \
    -m 2048 \
    -vga std \
    -hda win7.qcow2 \
    -cdrom win7.iso \
    -boot d \
    -nic user \
    -device intel-hd-audio \
    -device qemu-xhci \
    -device usb-tablet
