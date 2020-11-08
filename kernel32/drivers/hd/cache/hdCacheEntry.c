// ======================================================
// Internal Hard Disk Software Cache/Buffering System
// CHS ONLY CACHE - A SEPERATE CACHE WILL EXIST
// FOR NON-CHS DRIVES IN FUTURE, E.G. LBA32 ACCESS.
// (C) 2003 by Jari Tuominen
//
// hdArrangeFreeEntry might behave strangely (BUG !!).
// ======================================================
#include <stdio.h>
#include "hd.h"
#include "hdCache.h"
#include "hdCacheEntry.h"

//
void hdWipeCache(HCDB *h)
{
	int i,i2,l,c;
	static HCE *sort[1000];
	HCE *e1,*e2;

	//--------------------------------------------------
	// Get amount
	l = h->n_e;

	//--------------------------------------------------
	for(i=0; i<l; i++)
	{
		h->e[i]->isfree = TRUE;
	}
}

// Get more entries free on cache, use prioritizing
// (otherwise all of entries would be freed)
void hdArrangeMoreFreeEntries(HCDB *h)
{
	int i,i2,l,c;
	static HCE *sort[1000];
	HCE *e1,*e2;

	//
//	panic(__FUNCTION__);

	//--------------------------------------------------
	// We need to flush writeback cache first
	hdFlushWriteBack();
	hdWipeCache(h);
	return;

	//--------------------------------------------------
	// Get amount
	l = h->n_e;

	//--------------------------------------------------
	// Get initial list of entries
	for(i=0; i<l; i++)
	{
		if(h->e[i]->writeback)
		{
			panic("WRITE BACK FLAG STILL SET EVEN AFTER FLUSH - DUNNO WHAT TO DO ...");
		}
		sort[i] = h->e[i];
	}

	//--------------------------------------------------
	// Sort according hits
	for(i=0; i<l; i++)
	{
		//
		for(i2=0; i2<(l-1); i2++)
		{
			//
			if(sort[i+1]->hits < sort[i]->hits)
			{
				// Swap entries

				// Get
				e1 = sort[i+0];
				e2 = sort[i+1];
				// Store
				sort[i+0] = e2;
				sort[i+1] = e1;
			}
		}
	}
	
	
	//--------------------------------------------------
	// Deallocate half of the least important entries
	//
	c = h->n_e>>1; if(c==0) c=1;
	for(i=0; i<c; i++)
		sort[i]->isfree = TRUE;
}

// Arrange a new free entry by using various methods
int hdArrangeFreeEntry(void)
{
	int i,i2;
	HCE *h;

        // Check for corruption
        CORCHECK

	//
	while(1)
	{
		// Find for already free entry
		for(i=0; i<hdb->n_e; i++)
		{
			h = hdb->e[i];
			if(h->isfree==TRUE)	return i;
		}

		// Arrange more free entries
		hdArrangeMoreFreeEntries(hdb);
	}
}

// Search for existance of specified track in the cache
HCE *hdFindTrackFromCache(int C, int H)
{
	int i,i2;
	HCE *h;

        // Check for corruption
        CORCHECK

	//
	for(i=0; i<hdb->n_e; i++)
	{
		// Get entry
		h = hdb->e[i];

		//
		if(h->isfree==FALSE)
		{
			if(	h->c==C &&
				h->h==H		)
			{
				return h;
			}
		}
	}
	return NULL;
}



