#!/bin/bash
###################################################
## Build graos windowing system
###################################################

# Make disk specific tools
cd ../libc/apps
make graos
cd ../../tools

# Clean up our temp dir
rm -f something/*
# Copy the binaries into the temp dir
cp ../libc/apps/gui/*.bin something
cp ../libc/apps/gui/*.raw something

# Make an archive disk image out of
# the directory's contents
./sqarc something > graos.img
#./nzip graos.img
