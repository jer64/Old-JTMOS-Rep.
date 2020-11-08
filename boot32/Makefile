#
ASM=nasm
ASMFLAGS=-f bin
CC=gcc

all:
	@echo ---: getting checksum of the kernel image
	$(CC) -o blocksum blocksum.c
	$(CC) -o kchecksum kchecksum.c
	#./blocksum
	./kchecksum ../kernel32/kernel32.bin > kchecksum.inc
	@echo ---: compiling kernel loader
	$(ASM) $(ASMFLAGS) boot.asm
	$(ASM) $(ASMFLAGS) nonsysdisk.asm
	mv boot boot.bin
	mv nonsysdisk nonsysdisk.bin
	ls -la boot.bin
	ls -la nonsysdisk.bin

clean:
	rm boot.bin
	rm boot
	rm boot.o
