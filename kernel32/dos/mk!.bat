@echo off
echo MAKE KERNEL - By Jari Tapio Tuominen in 1999
del %1.bin
del %1.o
gcc -w -c %1.c -Wall -O
ld %1.o -o %1.bin -oformat binary -T%1.src
