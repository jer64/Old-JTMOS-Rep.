#!/bin/bash
#####################################################
# PRODUCE UTILITIES DISK 1
# utilsdisk.img
#####################################################

# Make LIBC.
cd ../libc/libc
make
cd ../../tools

# Make drivers.
cd ../libc/drv
make
cd ../../tools

# Make disk specific tools
cd ../libc/apps
make clean
make utilsdisk
cd ../../tools

# Clean up our temp. dir.
rm -f something/*
# Copy device drivers
cp ../libc/drv/*.bin something
cp ../libc/drv/*.cfg something
cp ../libc/docs/*.txt something
# Useful programs
#cp ../misc/compress.biz something/compress.bin
#cp ../misc/bwbasic.biz something/bwbasic.bin
# Copy the binaries + help files into the temp dir
cp ../libc/apps/*.bin something
cp ../libc/apps/*.hlp something
cp ../libc/apps/boot.sh something
cp ../libc/apps/help.sh something
cp ../libc/apps/setup.sh something
#cp ../libc/apps/*.raw something
#cp ../libc/apps/brandy/src/*.bin something
#cp ../libc/apps/gui/*.bin something
#cp ../libc/apps/gui/*.raw something

# Make an archive disk image out of
# the directory's contents.
./sqarc something > _utilsdisk.img

######################################
# Build compressed disk.
######################################

# Wipe temp. files.
rm -f comp/*

# Build unz
cd ../libc/apps
make -f unz.mak
cd ../../tools

# Build disk with compressed image.
# Remark any unneeded programs.
#cp something/nzip.bin comp
#cp something/sqa.bin comp
#cp something/sqsh.bin comp
#cp ../libc/apps/rt.bin comp	# ramdisk store/restore
cp ../libc/apps/unz.bin comp	# automatic unzip/unarchive

# Compress image.
./nzip _utilsdisk.img
# Copy into final compression folder.
cp _utilsdisk.img.nz comp/install.img.nz
# Copy graos there too.
####cp graos.img.nz comp
# Produce squirrel archive.
./sqarc comp > utilsdisk.img
cp utilsdisk.img ..
echo "Maximum size for utilsdisk.img is 800K"
echo "current size is:"
ls -l -s -k utilsdisk.img
# Remove temp. files.
#rm -f comp/*

# Produce boot disk image (combination).
cd ..
make image
cd tools

