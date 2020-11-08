# =========================================================
# GRAOS Windowing System 2003
# (C) 1997-2002 by Jari Tuominen(ehc50@kanetti.net)
# =========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=graos
LIBCDIR=../..
JTMOSSTDLIBCLOC=$(LIBCDIR)/libc/stlibc.o
STDSRC=$(LIBCDIR)/lib/stdapp.src
8TO1=$(LIBCDIR)/tools/8to1
BIN2C=$(LIBCDIR)/tools/bin2c
CC=gcc
CFLAGS=-Wall -w -c \
-O2 -mcpu=i486 -DCPU=586 \
-I$(LIBCDIR)/include
AR=ar
AS=as
ASM=nasm
ASMFLAGS=-f elf
LD=ld
LDFLAGS=-oformat=binary -Map $(APPNAME).map -T$(STDSRC) -L../../lib -lcjtmos -o $(APPNAME).bin

default: linkit

linkit: application
	@echo ---: Linking out application binary
	$(LD) $(JTMOSSTDLIBCLOC) *.o $(LDFLAGS)
	ls -l graos.bin

application:
	$(CC) $(CFLAGS) $(APPNAME).c
	$(CC) $(CFLAGS) windowOrder.c
	$(CC) $(CFLAGS) collision.c
	$(CC) $(CFLAGS) config.c
	$(CC) $(CFLAGS) windowMoving.c
	$(CC) $(CFLAGS) windowFunc.c
	$(CC) $(CFLAGS) guiServer.c
	$(CC) $(CFLAGS) guiStart.c
	$(CC) $(CFLAGS) guiEnd.c
	$(CC) $(CFLAGS) guiLoop.c
	$(CC) $(CFLAGS) guifont.c
	$(CC) $(CFLAGS) guifunc.c
	$(CC) $(CFLAGS) guidot.c
	$(CC) $(CFLAGS) guiUpdate.c
	$(CC) $(CFLAGS) guiApp.c
	$(CC) $(CFLAGS) guiArrow.c
	$(CC) $(CFLAGS) guiBlit.c
	$(CC) $(CFLAGS) guiTaskbar.c
	$(CC) $(CFLAGS) guiRect.c
	$(CC) $(CFLAGS) windowDraw.c
	$(CC) $(CFLAGS) framebuf.c
	$(CC) $(CFLAGS) wdb.c
	$(CC) $(CFLAGS) windows.c
	$(CC) $(CFLAGS) guiDesktopDraw.c

clean:
	rm $(APPNAME).o

