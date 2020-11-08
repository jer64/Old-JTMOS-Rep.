#ifndef __INCLUDED_HDCACHE_H__
#define __INCLUDED_HDCACHE_H__

//
#include "hd.h"
#include "hdCacheConfig.h"

// Enable this line below only if REALLY neccessary,
// it considerably slows all write operations.
//#define HD_SLOW_VERIFY_WRITE

//
#define CORCHECK hdCacheCheckBuffers1(__FUNCTION__, __LINE__);

//
typedef struct
{
	// TRACK BUFFER
	BYTE *buf;
	int l_buf;
	// PHYSICAL LOCATION IN CHS FORMAT
	int c,h,s;
	// IS FREE OR NOT?		[TRUE / FALSE]
	int isfree;
	// Write back required?		[TRUE / FALSE]
	int writeback;
	// How many faults have been happended
	int fault;
	// How many hits this entry got
	int hits;
}HCE;	// HCE = HD Cache Entry

//
typedef struct
{
	HCE **e;
	int n_e;
}HCDB;	// HCD = HD Cache Database
extern HCDB *hdb;

//
extern int hdCacheActive;

//
#include "hdCacheAccess.h"
#include "hdCacheEntry.h"
#include "hdCacheFlush.h"

#endif
