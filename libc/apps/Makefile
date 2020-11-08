#
# Makefile for all applications in this directory
#
# applist.txt is used by kernel to check out which
# kernels will be featured in the kernel image.
#
default: makeapps

graos:
		./buildGUI.sh

makeapps:
		rm -f *.o
		make -f cat.mak
		make -f clear.mak
		make -f cp.mak
		make -f d642zip.mak
		make -f aud.mak
		#make -f hello.mak
		#make -f example.mak
		make -f filetest.mak
		make -f keytest.mak
		make -f ls.mak
		make -f malloctest.mak
		make -f memconfig.mak
		make -f mv.mak
		make -f picture.mak
		#make -f performance.mak
		make -f plasma.mak
		make -f printtest.mak
		make -f ren.mak
		make -f rawplay.mak
		make -f snake.mak
		make -f sound.mak
		make -f sqa.mak
		make -f sqsh.mak
		#make -f test.mak
		#make -f test2.mak
		#make -f tsr.mak
		make -f vgaplasma.mak
		#make -f wtest.mak
		#make -f wtest2.mak
		#make -f wtest3.mak
		echo *.bin > file.lst

utilsdisk:
		./buildutils.sh

testdisk:
		./buildtest.sh

sel:
		./selectivebuild.sh

clean:
		rm -f *.o
		rm -f *.bin

#

