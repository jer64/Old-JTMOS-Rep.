# =========================================================
# (C) 1997-2002 by Jari Tuominen(ehc50@kanetti.net)
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=ram
JTMOSSTDLIBCLOC=../../libc/stlibc.o
STDSRC=../../lib/stdapp.src
CC=gcc
CFLAGS=-w -c \
-O2 -m486 -DCPU=586 \
-I. -Icache -I../../include
AR=ar
AS=as
ASM=nasm
ASMFLAGS=-f elf
LD=ld
LDFLAGS=-oformat=binary -o $(APPNAME).bin \
	-Map $(APPNAME).map -T$(STDSRC) \
	-L../../lib -lcjtmos

default: linkit

linkit: application
	@echo ---: Linking out application binary
	$(LD) $(JTMOSSTDLIBCLOC) *.o $(LDFLAGS)

application:
	$(CC) $(CFLAGS) ramService.c
	$(CC) $(CFLAGS) ram.c

clean:
	rm $(APPNAME).o

