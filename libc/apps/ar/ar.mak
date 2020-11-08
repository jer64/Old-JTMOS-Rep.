# =========================================================
# (C) 1997-2003 by Jari Tuominen
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=ar
LIBCDIR=../..
JTMOSSTDLIBCLOC=$(LIBCDIR)/libc/stlibc.o
STDSRC=$(LIBCDIR)/lib/stdapp.src
8TO1=$(LIBCDIR)/tools/8to1
BIN2C=$(LIBCDIR)/tools/bin2c
CC=gcc
CFLAGS=-Wall -w -c \
-O2 -m486 -DCPU=586 \
-I$(LIBCDIR)/include
AR=ar
AS=as
ASM=nasm
ASMFLAGS=-f elf
LD=ld
LDFLAGS=-o $(APPNAME).bin -oformat=binary \
	-Map $(APPNAME).map -T$(STDSRC) \
	-L$(LIBCDIR)/lib -lcjtmos

default: linkit

linkit: application
	@echo ---: Linking out application binary
	$(LD) $(JTMOSSTDLIBCLOC) \
	*.o \
	$(LDFLAGS)

application:
	$(CC) $(CFLAGS) ar.c
	$(CC) $(CFLAGS) huf.c
	$(CC) $(CFLAGS) io.c
	$(CC) $(CFLAGS) maketbl.c
	$(CC) $(CFLAGS) maketree.c
	$(CC) $(CFLAGS) slide.c

clean:
	rm $(APPNAME).o

