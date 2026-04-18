#!/usr/bin/env bash
# Retro gaming: DOS on a 486DX2-66 with 16MB RAM and Sound Blaster 16
# Perfect for early 90s DOS games
bin/qemu-system-x86_64 -L share/qemu \
    -machine pc,accel=tcg \
    -cpu Intel-486DX2-66 \
    -m 16 \
    -vga cirrus \
    -hda dosgames.qcow2 \
    -nic user \
    -soundhw sb16
