default: build

build:
		rm -f *.o
		gcc -c -O2 vidpak.c
		gcc -c -O2 vidcomp.c
		gcc -c -O2 vidfunc.c
		gcc *.o -o vidpak

