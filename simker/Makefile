CFLAGS=-c -g3 -I. -Ijtm32 -w
CC=gcc

default: all

all:
	$(CC) $(CFLAGS) jtmfs/createfat.c
	$(CC) $(CFLAGS) jtmfs/dirdbAccess.c
	$(CC) $(CFLAGS) jtmfs/dirdb.c
	$(CC) $(CFLAGS) jtmfs/dirdbCorruption.c
	$(CC) $(CFLAGS) jtmfs/dirdbFind.c
	$(CC) $(CFLAGS) jtmfs/dirdbInformation.c
	$(CC) $(CFLAGS) jtmfs/dirdbWalk.c
	$(CC) $(CFLAGS) jtmfs/flexCorruption.c
	$(CC) $(CFLAGS) jtmfs/flexFat.c
	$(CC) $(CFLAGS) jtmfs/fname_translation.c
	$(CC) $(CFLAGS) jtmfs/format.c
	$(CC) $(CFLAGS) jtmfs/fsaddblocks.c
	$(CC) $(CFLAGS) jtmfs/fsadd.c
	$(CC) $(CFLAGS) jtmfs/fsfind.c
	$(CC) $(CFLAGS) jtmfs/fslast.c
	$(CC) $(CFLAGS) jtmfs/fswalk.c
	$(CC) $(CFLAGS) jtmfs/isJTMFS.c
	$(CC) $(CFLAGS) jtmfs/jtmfat.c
	$(CC) $(CFLAGS) jtmfs/jtmfsAccess.c
	$(CC) $(CFLAGS) jtmfs/jtmfsAccessInit.c
	$(CC) $(CFLAGS) jtmfs/jtmfs.c
	$(CC) $(CFLAGS) jtmfs/jtmfsPath.c
	$(CC) $(CFLAGS) jtmfs/mount.c
	$(CC) $(CFLAGS) jtmfs/ReadChain.c
	$(CC) $(CFLAGS) jtmfs/root.c
	$(CC) $(CFLAGS) jtmfs/WriteChain.c
	$(CC) $(CFLAGS) jtmfs/writeDir.c
