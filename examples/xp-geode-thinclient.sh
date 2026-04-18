#!/usr/bin/env bash
# Embedded XP on AMD Geode LX 800MHz thin client
# Common in industrial/embedded XP deployments
bin/qemu-system-x86_64 -L share/qemu \
    -machine pc,accel=tcg \
    -cpu AMD-Geode-LX-800 \
    -m 256 \
    -vga cirrus \
    -hda thinclient.qcow2 \
    -nic user
