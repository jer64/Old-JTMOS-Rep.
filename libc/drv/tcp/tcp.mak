#==========================================================
# (C) 1997-2005 by Jari Tuominen(jari@vunet.org).
#==========================================================
# Just copy this make file and then change APPNAME to
# your application's name.
#
APPNAME=tcp
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
	$(CC) $(CFLAGS) icmp.c
	$(CC) $(CFLAGS) ip.c
	$(CC) $(CFLAGS) socket.c
	$(CC) $(CFLAGS) SendPacket.c
	$(CC) $(CFLAGS) ConnectHost.c
	$(CC) $(CFLAGS) tcp.c
	$(CC) $(CFLAGS) tcpchksum.c
	$(CC) $(CFLAGS) tcpDev.c
	$(CC) $(CFLAGS) tcpService.c
	$(CC) $(CFLAGS) tcpConfig.c
	$(CC) $(CFLAGS) ts.c
	$(CC) $(CFLAGS) main.c

clean:
	rm $(APPNAME).o

