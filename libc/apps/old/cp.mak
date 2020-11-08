# =========================================================
# (C) 1997-2002 by Jari Tuominen.
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=cp
JTMOSDIR=../../../jtm
JTMOSSTDLIBCLOC=$(JTMOSDIR)/libc/libc/stlibc.o
STDSRC=$(JTMOSDIR)/libc/lib/stdapp.src
8TO1=$(JTMOSDIR)/tools/8to1
BIN2C=$(JTMOSDIR)/tools/bin2c
CC=gcc
CFLAGS=-Wall -w -c \
-O2 -m486 -DCPU=586 \
-I$(JTMOSDIR)/libc/include -nostdlib -nostdinc++ -nostdinc -fno-builtin
AR=ar
AS=as
ASM=nasm
ASMFLAGS=-f elf
LD=ld
LDFLAGS=-o $(APPNAME).bin -oformat=binary \
	-Map $(APPNAME).map -T$(STDSRC) \
	-L$(JTMOSDIR)/libc/lib -lcjtmos

default: linkit

linkit: application
	@echo ---: Linking out application binary
	$(LD) $(JTMOSSTDLIBCLOC) $(APPNAME).o $(LDFLAGS)

application:
	$(CC) $(CFLAGS) $(APPNAME).c

clean:
	rm $(APPNAME).o

