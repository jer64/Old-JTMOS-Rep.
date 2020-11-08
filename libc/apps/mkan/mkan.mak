# mkan make file
#
default: build

build:
		rm -f *.o
		gcc -c -O2 mkan.c
		gcc -c -O2 mkanUser.c
		gcc -c -O2 mkanProcess.c
		gcc -c -O2 mkanImage.c
		gcc -c -O2 mkanStretch.c
		gcc -c -O2 mkanBMP.c
		gcc -c -O2 mkanPCX.c
		gcc -c -O2 mkanVIC.c
		gcc *.o -o mkan



