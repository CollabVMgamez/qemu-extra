#!/usr/bin/env bash
# Windows XP on a Pentium 4 Northwood 2.4GHz with 512MB RAM and DDR
# This is the classic WinXP experience configuration
bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm \
    -cpu Intel-Pentium-4-2400 \
    -m 512 \
    -vga std \
    -hda winxp.qcow2 \
    -cdrom winxp.iso \
    -boot d \
    -nic user \
    -device intel-hd-audio \
    -device qemu-xhci \
    -device usb-tablet
