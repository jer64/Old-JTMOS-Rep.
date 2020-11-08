ASM=nasm
ASMFLAGS=-f bin
CC=bcc

default: dosker

dosker:
	../boot32/kchecksum dosker32.bin > kchecksum.inc
	$(ASM) $(ASMFLAGS) dosker.asm -o dosker.com
