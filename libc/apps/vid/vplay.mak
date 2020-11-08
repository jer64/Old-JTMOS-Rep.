default: build

build:
		rm -f *.o
		gcc -c -O2 vplay.c
		gcc -c -O2 vplayGraph.c
		gcc -c -O2 vplayView.c
		gcc *.o -o vplay
