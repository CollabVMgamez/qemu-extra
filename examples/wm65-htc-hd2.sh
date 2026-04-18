#!/usr/bin/env bash
# Windows Mobile 6.5 on HTC HD2 (QSD8250 Scorpion)
bin/qemu-system-aarch64 -L share/qemu \
    -machine htc-hd2 \
    -cpu qsd8250 \
    -m 256 \
    -nographic \
    -kernel wm65-nk.bin
