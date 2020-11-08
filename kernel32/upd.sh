#!/bin/bash
echo -----------------------------------------------------------------------
echo Compiling ASM
# compile the assembly language library(various functions)
nasm -f elf asm/aslib.asm
mv asm/aslib.o .

# compile the math functions
gcc -c cos.S -o cos.o
gcc -c sin.S -o sin.o

echo Compiling C++
comopt1=-c
comopt2=-W -O
gcc $comopt1 scanmem.c	$comopt2
gcc $comopt1 malloc.c	$comopt2
gcc $comopt1 sysansi.c	$comopt2
gcc $comopt1 sysapp.c	$comopt2
gcc $comopt1 sysfile.c	$comopt2
gcc $comopt1 sysgfx.c	$comopt2
gcc $comopt1 sysglvar.c	$comopt2
gcc $comopt1 sysmisc.c 	$comopt2
gcc $comopt1 system.c	$comopt2
gcc $comopt1 sysusr.c	$comopt2
gcc $comopt1 sysvga.c	$comopt2
gcc $comopt1 syskeyb.c	$comopt2
gcc $comopt1 syssh.c 	$comopt2
gcc $comopt1 io.c	$comopt2
gcc $comopt1 int.c 	$comopt2
gcc $comopt1 kernel32.c	$comopt2
gcc $comopt1 cmos.c	$comopt2
gcc $comopt1 pit.c	$comopt2
gcc $comopt1 string.c	$comopt2
gcc $comopt1 jtmfs.c	$comopt2
gcc $comopt1 foo.c	$comopt2
gcc $comopt1 modes.c	$comopt2
gcc $comopt1 dma.c	$comopt2 
gcc $comopt1 floppy.c	$comopt2

ls *.o

echo Linking
ld kernel32.o cos.o sin.o \
modes.o cmos.o aslib.o sysansi.o sysapp.o sysfile.o sysgfx.o \
sysglvar.o sysmisc.o system.o sysusr.o sysvga.o io.o int.o \
pit.o syskeyb.o syssh.o string.o jtmfs.o foo.o \
malloc.o scanmem.o dma.o floppy.o \
-o kernel32.bin -oformat binary -Tkernel32.src

echo KERNEL32 binary ready :
ls -la kernel32.bin 
echo Done.






