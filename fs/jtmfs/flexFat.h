#ifndef __INCLUDED_FLEXFAT_H__
#define __INCLUDED_FLEXFAT_H__

//
#include "jtmfat.h"

// (in bytes)
#define MAX_FAT_SIZE		(1024*1024*64)
//
#define INITIALIZED		0x136FD2DA

//
#define N_MAX_FATS		100

// FlexFat's errors will be always displayed due to it's importancity.
#define FLEXER(ERRNO, EXPLANATION) printk("Flex error(%s) #%d\n", __FUNCTION__, ERRNO); \
printk(EXPLANATION); \
//panic(EXPLANATION);

//
#define FLFGUARD		0xFCE2E37B

//
#define READ	0
#define WRITE	1

//
// ------------------------------------------------------------------
// Flex FAT
typedef struct
{
	// Is OK(ready)? (TRUE OR FALSE)
	int isOK;

	// Buffer properties
	// -----------------
	// Guard DWORD (0xFCE2E37B)
	DWORD guard;
	// PTR to data buffer(DWORD PTR for 32-bit FAT)
	DWORD *buf;
	// Amount of entries (DWORD) on this FAT
	int amount;
	// Lenght of the buffer
	int l_buf;
	// Map of which blocks to write back on disk
	BYTE *writemap;
	// --
	int l_writemap;
	// DNR/BNR of the currently cached region of the FAT
	int dnr, bnr;
	// This buffer contains currently something that
	// has to be written back on disk?
	int writeBack;

	// Drive properties
	// ----------------
	// Block size for this DNR
	int bs;

	//
	DWORD status;

	//
	DWORD max_val;

	// Boot block(contains block number # to the FAT table)
	JTMFAT_INFOBLOCK *bb;
}FLEFAT;

//
typedef struct
{
	// FAT structures for hundred devices
	FLEFAT FAT[N_MAX_FATS];
}FLEFATSYS;

// Function prototypes
void flexFlushFAT(int dnr);
void flexDiskChange(int dnr);
void clearFatContents(FLEFAT *f);
void jtmfatInformation1(FLEFAT *e, const char *name);
void jtmfatInformation(void);
int _flexFatGetBlock(const char *caller, int dnr,int X);
int _flexFatSetBlock(const char *caller, int dnr,int X,int VALUE);
int FastSetBlocks(int dnr, int x1, int x2, int what);
void AnalyzeFAT(FLEFAT *f);

//
#define FastGetBlock(dnr,which) _FastGetBlock(__FUNCTION__, dnr,which)
#define FastSetBlock(dnr,which,what) _FastSetBlock(__FUNCTION__, dnr,which,what)
//int _FastGetBlock(char *fun, int dnr, int which);
//int _FastSetBlock(char *fun, int dnr, int which, DWORD what);
void flexHealthCheck(void);
void flexRWFAT(FLEFAT *f, int rw);

//
extern char flextmp[256];
extern int flexActive;

// Flex FAT main structure
extern FLEFATSYS *fle;

//
#include "flexCorruption.h"

//
void AnalyzeFATs(void);

#endif


