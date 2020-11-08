#!/bin/bash
#
# Only limited amount of utilities
# can be put on the system disk.
# JTMOS operating system system partition on
# boot disk can hold upto 800K data,
# compression is used to make more utilities
# fit on the boot disk.
#

#
./bm.sh

#
rm -f *.o
rm -f *.bin
#cd gui ; make ; cd ..
#cd bwbasic ; make ; cd ..
#cp bwbasic/*.bin .
#cp gui/*.bin

## System utilities
##################################################
make -f sqa.mak
make -f nzip.mak
make -f edit.mak
#make -f rt.mak
#make -f sqsh.mak
#make -f ls.mak
#make -f mkdir.mak

## File manager
##################################################
make -f filer.mak

## Amusement
##################################################
make -f snake.mak
make -f vgaplasma.mak
make -f picture.mak
#make -f plasma2.mak

## Diagnostic utilities
##################################################
#make -f otest.mak
#make -f pdump.mak
#make -f net.mak
#make -f cwdtest.mak
#make -f sscanf_test.mak
#make -f fpuhard.mak
#make -f fputest.mak

# Task launching stresstest.
#make -f runner.mak
# Disk drive performance test.
#make -f performance.mak

## Installers
##################################################
#make -f install.mak
#make -f setup.mak

## Miscellaneous
##################################################
#make -f idler.mak
#make -f talk.mak
#make -f listen.mak

## Graos applications
##################################################
#make -f wintest.mak
#make -f wintest2.mak
make -f winshell.mak
make -f taskbar.mak

## Some tools
##################################################
# Real-time process displayer.
#make -f top.mak
# Process displayer.
make -f ps.mak
# File copying utility.
#make -f cp.mak
# Executable files validation utility.
make -f val.mak
# File removal utility.
make -f rm.mak
#make -f clear.mak
#make -f mdir.mak

## Multimedia
##################################################
#make -f aud.mak
#make -f sound.mak

#make -f writing.mak
#make -f mkfs.mak
#make -f ren.mak
#make -f cat.mak
#cp tekstia.txt tekstia.bin

# Archiver
#cd ar ; make clean ; make ; cd ..
#cp ar/ar.bin .
##make -f lar.mak

## MEMORY STUFF
#make -f memconfig.mak

# Compress
#./comutils.sh
