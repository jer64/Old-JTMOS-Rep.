#
# JTMOS MAIN MAKEFILE
#
MAKE=make

default: help

help:
	@echo "======== JTMOS OPERATING SYSTEM ===================================="
	@echo "Following build options are available:"
	@echo ""
	@echo "**** TO BUILD JTMOS, YOU NEED GCC 2.95 ! ****"
	@echo ""
	@echo "make kernel"
	@echo "         Rebuilds kernel and generates system boot disk (jtm32.img)."
	@echo "make linkit"
	@echo "         Links everything, only, by executing this it'll expect you to have a fully-built kernel."

others:
	@echo "make all"
	@echo "         Builds default system boot disk."
	@echo "make appunit"
	@echo "         Rebuilds applications for system boot disk(part of all)."
	@echo "make jlibc"
	@echo "         Rebuilds JTMOS LIBC (part of all)."
	@echo "make clean"
	@echo "         Cleans up backup, object and other temporary files."
	@echo "make arc"
	@echo "         Archive current kernel source(saves .tar.gz at homedir)."
	@echo "make image"
	@echo "         Builds boot disk image out of kernel and utilsdisk.img."

all: sfx jlibc appunit kernel

linkit:
	cd kernel32; make imp; make linkit; cd ..
	./buildimage

image:
	./buildimage
	#./uploadimage

kernel:
	@echo ---: COMPILING EVERYTHING
	date > SpeedCompile
	./compileall
	date >> SpeedCompile

kernel:
	@echo ---: LINKING EVERYTHING
	date > SpeedCompile
	./link
	date >> SpeedCompile

dos:
	cd dosker; make; cd ..

appunit:
	cd tools ; make disk ; cd ..

jlibc:
	@echo ---: COMPILING LIBC
	./compilelibc

clean:
	@echo ---: Cleaning up
	./cleanup

packup:
	@echo ---: Packing up
	./arc

sfx:
	@echo ---: Making self-extracting archive for the kernel
	./compilesfx

disk:
	@echo ---: Making utilities disk image
	./compiledisk

