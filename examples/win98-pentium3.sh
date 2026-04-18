#!/usr/bin/env bash
# Windows 98 SE on a Pentium III 500MHz with 128MB RAM
# Requires: Win98 SE ISO image
bin/qemu-system-x86_64 -L share/qemu \
    -machine pc_p4era,accel=tcg \
    -cpu Intel-Pentium-III-500 \
    -m 128 \
    -vga cirrus \
    -hda win98se.qcow2 \
    -cdrom win98se.iso \
    -boot d \
    -nic user \
    -soundhw sb16
