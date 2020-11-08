#
ASM=nasm
ASMFLAGS=-f bin
CC=gcc
CCFLAGS=-O -o

all:
	$(CC) $(CCFLAGS) img img.c
	cp img ..

clean:
	rm *.o
