#!/usr/bin/env bash
# Windows XP on VIA C7 Eden thin client with TPU
bin/qemu-system-x86_64 -L share/qemu \
    -machine pc,accel=tcg \
    -cpu VIA-C7-1500 \
    -m 256 \
    -vga cirrus \
    -hda via-thinclient.qcow2 \
    -nic user \
    -device google-edge-tpu-coral-pcie
