// ======================================================
// Internal Hard Disk Software Cache/Buffering System
// CHS ONLY CACHE - A SEPERATE CACHE WILL EXIST
// FOR NON-CHS DRIVES IN FUTURE, E.G. LBA32 ACCESS.
// (C) 2003-2004 by Jari Tuominen
// ======================================================
#include <stdio.h>
#include "hd.h"
#include "hdCache.h"
#include "hdCacheEntry.h"

//-------------------------------------------------------------------------------
void hdCacheCheckBuffers(void)
{
	//
	hdCacheCheckBuffers1("[unknown (detected upon task switch)]", 0);
}

// Check HD cache buffers for corruption
void hdCacheCheckBuffers1(const char *func, int line)
{
	int i;
	HCE *e;
	static char str[256],str2[256];

	// Cache active yet?
	if(hdCacheActive==FALSE)
		// Nope
		return;

	//
	for(i=0; i<hdb->n_e; i++)
	{
		// Get entry PTR
		e = hdb->e[i];

		// Skip free entries
		if(e->isfree)
			continue;
	}
}

// Show info about corruptions
void hdCacheShowInfo(void)
{
	//
}

//-------------------------------------------------------------------------------
// Makes the specified track available in the cache
HCE *hdLoadNewTrack(int C, int H)
{
	int id;
	HCE *h; // HD cache entry

	// Arrange one free cache entry
	id = hdArrangeFreeEntry();

	// Get entry PTR
	h = hdb->e[id];

	// Mark it as used
	h->isfree = FALSE;
	// Define location
	h->c = C;
	h->h = H;
	h->s = 1; // Note: Sector # is not needed, but set to 1 to be sure
	// Hit count to zero
	h->hits = 0;

	// Load to buffer
	// - First check that buffer can handle it
	if(  (chd->sectors*chd->blocksize) <= h->l_buf  )
	{
		//
		hdSectorRW(chd,		// CURRENT HD
			h->buf,h->l_buf,
			h->c,h->h,1,
			chd->slave,
			READ,
			chd->sectors);
	}
	else	printk("%s: TRACK BUFFER INSUFFICIENT ALLOCATION", __FUNCTION__);

	// Return PTR to it
	return h;
}

// Gives hits to specified track
BYTE *hdCacheAccess(int C, int H)
{
	int i,i2;
	HCE *h;

	// Does it exist in cache already?
	h = hdFindTrackFromCache(C,H);
	if(h==NULL)
	{
		// It doesn't, therefore let's cache it
		h = hdLoadNewTrack(C,H);
	}

	// Return hits structure PTR
	return h;
}

// RW hits to cache
int hdCacheRW(int C, int H, int S,  int rw,
		BYTE *buf,int l_buf)
{
	HCE *h; // HD cache entry
	int offs;
	char str[256];

	// ----------------------------------------------------------
	// Param validity check first
	if( !isValidCHS(chd, C,H,S) )		return 1;

	// ----------------------------------------------------------
	// Get hits to it
	h = hdCacheAccess(C, H);

	// Check
	if(S<=0 || S>=(chd->sectors+1))
	{
		// Illegal sector
		sprintf(str, "hdCacheAccess: illegal sector %d (legal boundary 1-%d)",
			S, chd->sectors);
		panic(str);
	}

	// ----------------------------------------------------------
	// Determine offset to cache buffer by sector value
	offs = ((S-1)%chd->sectors)*chd->blocksize;

	// ----------------------------------------------------------
	// Conduct operation
	// - First check that offset does not exceed limits
	if(  offs>=0 && (offs+chd->blocksize)<=h->l_buf  )
	{
		//
		if(rw==READ)
		{
			// Direct read.
		/*	hdSectorRW(chd,
				buf,chd->blocksize,
				C,H,S,
				chd->slave,
				READ,
				1);*/

			// Copy from cache buffer
			memcpy(buf, h->buf+offs, chd->blocksize);
		}
		else
		if(rw==WRITE)
		{
		/*	// Direct read.
			hdSectorRW(chd,
				buf,chd->blocksize,
				C,H,S,
				chd->slave,
				WRITE,
				1);*/
			// Copy to cache buffer
			memcpy(h->buf+offs, buf, chd->blocksize);
			// Set write back signature
			h->writeback = TRUE;
		}
		else
		{
			//
			panic("yep hd driver cache");
		}
		// Always increase hits counter
		h->hits++;
	}
	else	printk("%s: Offset exceeds buffer boundaries, offs=0x%x\n",
			__FUNCTION__, offs);

	//
	return 0;
}




