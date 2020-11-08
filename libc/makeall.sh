#!/bin/bash
#
# LIBC
echo "COMPILING JTMOS LIBC"
cd libc
rm *.so
rm *.a
make
make headers
cd ..

# APPS
echo "COMPILING JTMOS BINARY UTILITIES/APPLICATIONS"
cd apps
rm *.o
make
cd ..

# DRIVERS
echo "COMPILING DRIVERS"
cd drv
rm *.o
make
cd ..

#
