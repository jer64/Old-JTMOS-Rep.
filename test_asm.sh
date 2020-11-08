#!/bin/bash
nasm -f bin test.asm
ndisasm -b 32 test
