#!/usr/bin/env bash
# Windows CE 6.0 on ARM Versatile Express (Cortex-A9)
# For WinCE kernel development/testing
bin/qemu-system-aarch64 -L share/qemu \
    -machine wince-vexpress-a9 \
    -cpu cortex-a9 \
    -m 256 \
    -nographic \
    -kernel wince-nk.bin
