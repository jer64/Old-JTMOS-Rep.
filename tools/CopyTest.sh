#!/bin/bash

# Make LIBC
cd ../libc/libc
make
cd ../../tools

# Make disk specific tools
cd ../libc/apps
make clean
make testdisk
cd ../../tools

# Clean up our temp dir
rm -f something/*
# Copy the binaries into the temp dir
cp ../libc/apps/*.bin something

# Make an archive disk image out of
# the directory's contents
./sqarc something > testdisk.img
