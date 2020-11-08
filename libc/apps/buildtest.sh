#!/bin/bash
#
echo "Building programs for the test disk ..."
rm -f *.o
rm -f *.bin

# Build test programs
make -f filetest.mak
make -f float_test.mak
make -f keytest.mak
make -f malloctest.mak
make -f printtest.mak
make -f sscanf_test.mak
make -f test.mak
make -f test2.mak
make -f wtest.mak
make -f wtest2.mak
make -f wtest3.mak

