#!/bin/bash
img jtm32.cfg jtm32.img
ls -la jtm32.img
md5sum   jtm32.img
md5sum   jtm32.img > jtm32.md5
