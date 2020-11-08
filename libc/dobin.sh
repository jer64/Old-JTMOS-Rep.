#!/bin/bash
cd libc
make clean
make
cd ..
cd apps
rm *.bin
./selectivebuild.sh
cd ..
cd binutils
make sel
make arc
cd ..
echo Done.
