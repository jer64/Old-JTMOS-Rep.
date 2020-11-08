#
ASM=nasm
ASMFLAGS=-f bin

all:
	@echo ---: compiling dummy kernel
	$(ASM) $(ASMFLAGS) dker.asm
	mv dker dker.bin
	ls -la dker.bin

clean:
	rm dker.bin
	rm dker
	rm dker.o
