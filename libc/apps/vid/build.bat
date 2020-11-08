@echo off
		rm -f *.o
		gcc -c -O2 vplay.c
		gcc -c -O2 vplayGraph.c
		gcc -c -O2 vplayView.c
		gcc *.o -o vplay

		rm -f *.o
		gcc -c -O2 vidpak.c
		gcc -c -O2 vidcomp.c
		gcc -c -O2 vidfunc.c
		gcc *.o -o vidpak

		copy vidpak.exe %WINDIR%
                copy vplay.exe %WINDIR%
