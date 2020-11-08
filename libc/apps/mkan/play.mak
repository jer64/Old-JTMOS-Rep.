# mkan make file
#
default: build

build:
		rm -f *.o
		gcc -c -O2 play.c
		gcc -c -O2 mkanUser.c
		gcc -c -O2 mkanProcess.c
		gcc -c -O2 mkanImage.c
		gcc -c -O2 mkanStretch.c
		gcc -c -O2 mkanBMP.c
		gcc -c -O2 mkanPCX.c
		gcc -c -O2 mkanVIC.c
		gcc -c -O2 vplayGraph.c
		gcc mkanUser.o mkanProcess.o mkanImage.o mkanStretch.o\
		mkanBMP.o mkanPCX.o mkanVIC.o play.c vplayGraph.o -o play



