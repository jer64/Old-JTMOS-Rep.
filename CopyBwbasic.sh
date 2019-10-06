#!/bin/bash

# Make LIBC
cd ../libc/libc
make
cd ../../tools

# Make disk specific tools
cd ../libc/apps

# BWBASIC
cd bwbasic
make
cd ..

cd ../../tools

# Clean up our temp dir
rm -f something/*
# Copy the binaries into the temp dir
cp ../libc/apps/bwbasic/*.bin something
cp ../libc/apps/bwbasic/seltests/*.bas something

# Make an archive disk image out of
# the directory's contents
./sqarc something > bwbasic.img

