# =========================================================
# (C) 1997-2002 by Jari Tuominen(ehc50@kanetti.net)
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=slip
JTMOSSTDLIBCLOC=../../libc/stlibc.o
STDSRC=../../lib/stdapp.src
CC=gcc
CFLAGS=-w -c \
-O2 -march=i486 \
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
	$(CC) $(CFLAGS) slipService.c
	$(CC) $(CFLAGS) slipConfig.c
	$(CC) $(CFLAGS) slipLogin.c
	$(CC) $(CFLAGS) slipStack.c
	$(CC) $(CFLAGS) slipMain.c
	$(CC) $(CFLAGS) slipDev.c
	$(CC) $(CFLAGS) slip.c

clean:
	rm $(APPNAME).o

