#!/usr/bin/env bash
# NVIDIA multi-GPU setup: RTX 5090 primary + 3 secondary GPUs
# Shows 4 GPU adapters in device manager
bin/qemu-system-x86_64 -L share/qemu \
    -machine q35,accel=kvm \
    -cpu host \
    -m 8192 \
    -vga none \
    -device nvidia-rtx5090,gpu-count=4 \
    -hda win10.qcow2 \
    -nic user
