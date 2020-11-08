#!/bin/bash
cd apps
rm -f *.o
cd ..
cd libc
rm -f *.o
cd ..
cd binutils
make clean
cd ..

