// ======================================================
// Internal Hard Disk Software Cache/Buffering System
// CHS ONLY CACHE - A SEPERATE CACHE WILL EXIST
// FOR NON-CHS DRIVES IN FUTURE, E.G. LBA32 ACCESS.
// (C) 2003 by Jari Tuominen
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

	        // Check for corruption
	        if( fixalCheck(e->buf, e->l_buf)==FALSE )
	        {
	                //
	                hdCacheShowInfo();
	                //
		//	sprintf(str, "Found corruption in HD-CACHE, function=%s, line=%d, PID=%d(%s)",
		//		func, line, GetCurrentThread(), thread_name[GetCurrentThread()]);
		//	panic(str);
		}
	}
}

// Show info about corruptions
void hdCacheShowInfo(void)
{
	int i,cor;
	HCE *e;

	//
	for(i=0,cor=0; i<hdb->n_e; i++)
	{
		// Get entry PTR
		e = hdb->e[i];

		// Skip free entries
		if(e->isfree)
			continue;

		// Check for corruption
		if( fixalCheck(e->buf, e->l_buf)==FALSE )
		{
			// Report
			printk("HD cache entry %d is corrupted\n",
				i);
			cor++;
		}
	}

	//
	printk("%d corrupted entries detected.\n",
		cor);
	printk("Cache has %d entries.\n",
		hdb->n_e);
}

//-------------------------------------------------------------------------------
// Makes the specified track available in the cache
HCE *hdLoadNewTrack(int C, int H)
{
	int id;
	HCE *h;

	// Check for corruption
	CORCHECK

	// Arrange one free cache entry
	id = hdArrangeFreeEntry();

	// Get entry PTR
	h = hdb->e[id];

	// Check for corruption
	CORCHECK

	// Check for corruption
	if( fixalCheck(h->buf, h->l_buf)==FALSE )
	{
		//
		hdCacheShowInfo();
		//
		panic("HD DRIVER CACHE BUFFER CORRUPTED");
	}

	// Check for corruption
	CORCHECK

	// Mark it as used
	h->isfree = FALSE;
	// Define location
	h->c = C;
	h->h = H;
	h->s = 1; // Note: Sector # is not needed, but set to 1 to be sure
	// Hit count to zero
	h->hits = 0;

	// Check for corruption
	CORCHECK

	// Load to buffer
	// - First check that buffer can handle it
	if(  (chd->sectors*chd->blocksize) <= h->l_buf  )
	{
		//
		hdSectorRW(chd,		// CURRENT HD
			h->buf,h->l_buf,
			h->h,1,h->c,
			chd->slave,
			READ,
			chd->sectors);
	}
	else	printk("%s: TRACK BUFFER INSUFFICIENT ALLOCATION", __FUNCTION__);

	// Check for corruption
	CORCHECK

	// Return PTR to it
	return h;
}

// Gives hits to specified track
BYTE *hdCacheAccess(int C, int H)
{
	int i,i2;
	HCE *h;

	// Check for corruption
	CORCHECK

	// Does it exist in cache already?
	h = hdFindTrackFromCache(C,H);
	if(h==NULL)
	{
		// Check for corruption
		CORCHECK
		// It doesn't, therefore let's cache it
		h = hdLoadNewTrack(C,H);
	}

	// Check for corruption
	CORCHECK

	// Check for corruption
	if( fixalCheck(h->buf, h->l_buf)==FALSE )
	{
		// Check for corruption
		CORCHECK
		//
		hdCacheShowInfo();
		//
		panic("HD DRIVER CACHE BUFFER CORRUPTED");
	}

	// Check for corruption
	CORCHECK

	// Return hits structure PTR
	return h;
}

// RW hits to cache
int hdCacheRW(int C, int H, int S,  int rw,
		BYTE *buf,int l_buf)
{
	HCE *h;
	int offs;
	char str[256];

	// Check for corruption
	CORCHECK

	// ----------------------------------------------------------
	// Param validity check first
	if( !isValidCHS(chd, C,H,S) )		return 1;

	// Check for corruption
	CORCHECK

	// ----------------------------------------------------------
	// Get hits to it
	h = hdCacheAccess(C, H);

	// Check for corruption
	CORCHECK

	// Check for corruption
	if( fixalCheck(h->buf, h->l_buf)==FALSE )
	{
		// Check for corruption
		CORCHECK
		//
		hdCacheShowInfo();
		//
		panic("HD DRIVER CACHE BUFFER CORRUPTED");
	}

	// Check for corruption
	CORCHECK

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

	// Check for corruption
	CORCHECK

	// ----------------------------------------------------------
	// Conduct operation
	// - First check that offset does not exceed limits
	if(  offs>=0 && (offs+chd->blocksize)<=h->l_buf  )
	{
		// Check for corruption
		CORCHECK
		//
		if(rw==READ)
		{
			// Check for corruption
			CORCHECK
			// Copy from cache buffer
			memcpy(buf, h->buf+offs, chd->blocksize);
			// Check for corruption
			CORCHECK
		}
		else
		if(rw==WRITE)
		{
			// Check for corruption
			CORCHECK
			// Copy to cache buffer
			memcpy(h->buf+offs, buf, chd->blocksize);
			// Set write back signature
			h->writeback = TRUE;
			// Check for corruption
			CORCHECK
		}
		else
		{
			//
			panic("yep hd driver cache");
		}
		// Always increase hits counter
		h->hits++;
		// Check for corruption
		CORCHECK
	}
	else	printk("%s: Offset exceeds buffer boundaries, offs=0x%x\n",
			__FUNCTION__, offs);

	//
	return 0;
}




