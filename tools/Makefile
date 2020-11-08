#
ASM=nasm
ASMFLAGS=-f bin

default:
	@echo ""
	@echo "           *** Tools for fools ***"
	@echo ""
	@echo "all"
	@echo "		Compile everything"
	@echo "tools"
	@echo "		Compiles all tools that"
	@echo "		are required for JTMOS"
	@echo "		development, building and so on."
	@echo "disk"
	@echo "		Generates utilities disk"
	@echo "		image by performing various jobs."
	@echo "		Saved at utilsdisk.img"
	@echo "disk2"
	@echo "		Generates utilities disk 2"
	@echo "		Saved at utilsdisk2.img"
	@echo "bwbasic"
	@echo "		Generates Bywater Basic disk"
	@echo "		Saved at bwbasic.img"
	@echo "web"
	@echo "		Generates default webpage disk"
	@echo "		for OravaHTTPD WEB Server."
	@echo "web2"
	@echo "		Generates other files web disk"
	@echo "graos"
	@echo "		Generates GRAOS GUI disk."
	@echo "test"
	@echo "		Make test programs disk."
	@echo "xmlparse"
	@echo "		XML-parser for kernel and applications."

util:
	make graos
	make disk
	cd .. ; make image ; cd tools

tools:
	@echo ---: compiling tools
	gcc -O2 -o bin2c bin2c.c
	gcc -O2 -o 8to1 8to1.c
	gcc -O2 -o sqarc sqarc.c
	gcc -O2 -o nzip nzip.c nzipMain.c
	gcc -O2 -o dif dif.c
	gcc -O2 -o jg jg.c

disk:
	./utils.sh
	./UploadUtils.sh

disk2:
	./CopyUtils2.sh
	./UploadUtils2.sh

bwbasic:
	./CopyBwbasic.sh
	./UploadBwbasic.sh

web:
	./makehttpd.sh

web2:
	./makehttpdother.sh

graos:
	./graos.sh
	#./UploadGraos.sh

test:
	./CopyTest.sh
	./UploadTest.sh

nz:
	./nzip.sh

all:
	./allmake.sh

xmlparse:
	./buildxmlparse.sh

clean:
	rm *.o
