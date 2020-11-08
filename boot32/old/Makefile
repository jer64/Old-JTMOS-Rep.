#
ASM=nasm
ASMFLAGS=-f bin

all:
	@echo ---: getting checksum of the kernel image
	gcc -o kchecksum kchecksum.c
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
