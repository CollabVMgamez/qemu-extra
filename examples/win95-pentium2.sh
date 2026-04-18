#!/usr/bin/env bash
# Windows 95 on a Pentium II 233MHz with 64MB RAM
# Requires: Win95 ISO image
bin/qemu-system-x86_64 -L share/qemu \
    -machine pc_p4era,accel=tcg \
    -cpu Intel-Pentium-II-233 \
    -m 64 \
    -vga cirrus \
    -hda win95.qcow2 \
    -cdrom win95.iso \
    -boot d \
    -nic user \
    -soundhw sb16
