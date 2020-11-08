# =========================================================
# (C) 1997-2002 by Jari Tuominen(ehc50@kanetti.net)
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=nzip
JTMOSSTDLIBCLOC=../libc/stlibc.o
STDSRC=../lib/stdapp.src
8TO1=../tools/8to1
BIN2C=../tools/bin2c
CC=gcc
CFLAGS=-Wall -w -c \
-O2 -m486 -DCPU=586 \
-I../include -DUNCOMPRESS_ALL
AR=ar
AS=as
ASM=nasm
ASMFLAGS=-f elf
LD=ld
LDFLAGS=-o unz.bin -oformat=binary \
	-Map $(APPNAME).map -T$(STDSRC) \
	-L../lib -lcjtmos

default: linkit

linkit: application
	@echo ---: Linking out application binary
	$(LD) $(JTMOSSTDLIBCLOC) $(APPNAME).o $(APPNAME)Main.o sqa.o $(LDFLAGS)

application:
	$(CC) $(CFLAGS) $(APPNAME).c
	$(CC) $(CFLAGS) $(APPNAME)Main.c
	$(CC) $(CFLAGS) sqa.c

clean:
	rm $(APPNAME).o

