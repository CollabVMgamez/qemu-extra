#!/usr/bin/env bash
# MIPS Malta board with 24Kf CPU (Linux/MIPS development)
bin/qemu-system-mips64 -L share/qemu \
    -machine malta \
    -cpu 24Kf \
    -m 256 \
    -nographic \
    -kernel vmlinux \
    -initrd rootfs.cpio.gz \
    -nic user
