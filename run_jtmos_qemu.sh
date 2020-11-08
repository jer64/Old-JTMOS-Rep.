#!/bin/bash
# -curses
# qemu-system-i386
# kvm
qemu-git.qemu-i386 -fda jtm32.img -m 512 -hda hd_jtmos.img -boot a
