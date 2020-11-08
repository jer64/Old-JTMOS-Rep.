# =========================================================
# (C) 1997-2002 by Jari Tuominen(ehc50@kanetti.net)
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=rt
JTMOSSTDLIBCLOC=../libc/stlibc.o
STDSRC=../lib/stdapp.src
8TO1=../tools/8to1
BIN2C=../tools/bin2c
CC=gcc
CFLAGS=-Wall -w -c \
-O2 -m486 -DCPU=586 \
-I../include
AR=ar
AS=as
ASM=nasm
ASMFLAGS=-f elf
LD=ld
LDFLAGS=-o $(APPNAME).bin -oformat=binary \
	-Map $(APPNAME).map -T$(STDSRC) \
	-L../lib -lcjtmos

default: linkit

linkit: application
	@echo ---: Linking out application binary
	$(LD) $(JTMOSSTDLIBCLOC) $(APPNAME).o $(LDFLAGS)

application:
	$(CC) $(CFLAGS) $(APPNAME).c

clean:
	rm $(APPNAME).o

