@echo off
@rem mkan make file
@rem
                del *.o
                gcc -c -O2 mkan.c
                gcc -c -O2 mkanUser.c
                gcc -c -O2 mkanProcess.c
                gcc -c -O2 mkanImage.c
                gcc -c -O2 mkanStretch.c
                gcc -c -O2 mkanBMP.c
                gcc -c -O2 mkanPCX.c
                gcc -c -O2 mkanVIC.c
		gcc *.o -o mkan
                copy mkan.exe %WINDIR%
