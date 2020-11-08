##############################################################################
#
# JTMOS KERNEL SMART MAKEFILE - autogenerates a makefile on the fly.
# (C) 2008 by Jari Tuominen (jari.t.tuominen@gmail.com).
#
##############################################################################

default:
	cd init/xml; make; cd ../..
	./autogen.pl > generated.mak
	make -j 2 -f generated.mak

linkit:
	./autogen.pl > generated.mak
	make linkit -f generated.mak

purelinkit:
	make linkit -f generated.mak

clean:
	@echo ---: Cleaning up kernel modules.
	find . -name \*~* |xargs rm -f
	find . -name \*.o |xargs rm -f

