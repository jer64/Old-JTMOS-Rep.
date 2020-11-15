##############################################################################
#
# INCORONATIONDB - Smart Auto Make
# (C) 2020 by Jari Tuominen (jari.t.tuominen@gmail.com).
#
##############################################################################

default:
	./autogen.pl > generated.mak
	make -j 4 -f generated.mak

linkit:
	./autogen.pl > generated.mak
	make linkit -f generated.mak

purelinkit:
	make linkit -f generated.mak

clean:
	@echo ---: Cleaning up kernel modules.
	find . -name \*~* |xargs rm -f
	find . -name \*.o |xargs rm -f

