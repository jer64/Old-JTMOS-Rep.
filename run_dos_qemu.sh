#!/bin/bash
qemu -cdrom ~/fdbasecd.iso -m 64 -hda hd_jtmos.img -boot c
