# =========================================================
# (C) 1997-2002 by Jari Tuominen(ehc50@kanetti.net)
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=hd
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
LDFLAGS=-o $(APPNAME).bin -oformat=binary \
	-Map $(APPNAME).map -T$(STDSRC) \
	-L../../lib -lcjtmos

default: linkit

linkit: application
	@echo ---: Linking out application binary
	$(LD) $(JTMOSSTDLIBCLOC) *.o $(LDFLAGS)

application:
	$(CC) $(CFLAGS) cache/hdCache.c
	$(CC) $(CFLAGS) cache/hdCacheAccess.c
	$(CC) $(CFLAGS) cache/hdCacheEntry.c
	$(CC) $(CFLAGS) cache/hdCacheFlush.c
	$(CC) $(CFLAGS) chs.c
	$(CC) $(CFLAGS) hd.c
	$(CC) $(CFLAGS) hdService.c
	$(CC) $(CFLAGS) hdBlockRW.c
	$(CC) $(CFLAGS) hdDetect.c
	$(CC) $(CFLAGS) hdDev.c
	$(CC) $(CFLAGS) hdMisc.c
	$(CC) $(CFLAGS) hdRW.c
	$(CC) $(CFLAGS) hdReset.c
	$(CC) $(CFLAGS) low.c

clean:
	rm $(APPNAME).o

