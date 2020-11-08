#
#
# aud
#
# AUTOMATICALLY GENERATED MAKEFILE
# GENERATED BY makgen.pl 1.0.2004
#
APPNAME=aud
JTMOSSTDLIBCLOC=../libc/stlibc.o
STDSRC=../lib/stdapp.src
8TO1=TO1
BIN2C=../tools/bin2c
CC=gcc
CFLAGS=-Wall -w -c -O -march=i486 -DCPU=i586 -I../include
AR=ar
AS=as
ASM=nasm
ASMFLAGS=-f elf
LD=ld
LDFLAGS=-oformat=binary -Map $(APPNAME).map -T$(STDSRC) -L../lib -lcjtmos -o $(APPNAME).bin

default: linkit

		
linkit: application
	@echo ---: Linking out binary: aud.bin 
	$(LD) $(JTMOSSTDLIBCLOC) aud.o  $(LDFLAGS)

application:
	
	$(CC) $(CFLAGS) aud.c

clean:
	rm -f aud.o 
	
	
