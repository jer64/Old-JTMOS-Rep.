#!/bin/bash

# Make LIBC
cd ../libc/libc
make
cd ../../tools

# Make disk specific tools
cd ../libc/apps

# Applications
make clean
make

# Brandy
#cd brandy/src
#make
#cd ../..

# BWBASIC
cd bwbasic
make
cd ..

cd ../../tools

# Clean up our temp dir
rm -f something/*
# Copy the binaries into the temp dir
#cp ../libc/apps/brandy/src/*.bin something
#cp ../libc/apps/bwbasic/*.bin something
cp ../libc/apps/sqa.bin something
cp ../libc/apps/sqsh.bin something
cp ../libc/apps/ls.bin something
cp ../libc/apps/edit.bin something
#cp ../libc/apps/*.raw something
#cp ../libc/apps/gui/*.bin something
#cp ../libc/apps/gui/*.raw something

# Make an archive disk image out of
# the directory's contents
./sqarc something > utilsdisk2.img

