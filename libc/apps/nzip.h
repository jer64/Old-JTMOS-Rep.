#ifndef __INCLUDED_NZIP_H__
#define __INCLUDED_NZIP_H__

//
#define MIN_LOOKBACK	16
// #define MIN_CHUNK	16
#define CLEVEL1		// Normal
//#define CLEVEL2	// Slow

// 64 & 15 = good one
// == MEDIUM COMPRESSION ==
#ifdef CLEVEL1
#define COMP_STEP	63 // 0,1,2,3,7,15,...
#define MAX_LOOK	1024*128
#endif

// == HEAVY COMPRESSION ==
#ifdef CLEVEL2
#define COMP_STEP	7 // 0,1,2,3,7,15,...
#define MAX_LOOK	1024*128
#endif

//
#define REACHING	100 // how many bytes to reach near current offset at max.
#define IND_STEP	1023 // Indicator showing step
#define MAX_UNSIZE	1024*1024*2 // Max. file size for uncompression
				// (depacking buffer size)
#define REMOVE_OLD	// remove older file after process
#define ENABLE_ADVPAK	// enable advanced compression(recommended to be enabled)
#define ENABLE_CHRPAK	// enable char packing
#include <stdio.h>

#ifdef UNCOMPRESS_ALL
#include <jtmos/filefind.h>
#include "sqa.h"
#endif

#define BYTE	unsigned char
#define WORD	unsigned short int
#define DWORD	unsigned long int

//
typedef struct
{
	// Uncompressed data
	unsigned char *buf;
	int l_buf;
	int offs;
}CO;

//
typedef struct
{
	int method;
	int offs;
}SIM;

//
#define CODE_CHARPACK	0xFCE3
#define CODE_ADVPAK	0xFCE4
#define CODE_END	0xFCEE
#define CODE_FIX	0xFCEF

#endif

