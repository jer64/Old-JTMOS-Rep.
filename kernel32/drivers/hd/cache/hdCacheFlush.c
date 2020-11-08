// ======================================================
// Internal Hard Disk Software Cache/Buffering System
// CHS ONLY CACHE - A SEPERATE CACHE WILL EXIST
// FOR NON-CHS DRIVES IN FUTURE, E.G. LBA32 ACCESS.
// (C) 2003-2004 by Jari Tuominen
// ======================================================
#include <stdio.h>
#include "hd.h"
#include "hdCache.h"
#include "hdCacheFlush.h"

//
static BYTE *tmp=NULL;
static int l_tmp=0;

// Flush everything to disk
void hdFlushWriteBack(void)
{
	int i,x,l;
	HCE *h;
	
	// Ignore requests when in LBA mode.
	if(chd->lbaMode)
	    return;

	// Allocate temp. buffer if neccessary
	if(tmp==NULL)
	{
		//
		l_tmp = chd->sectors*512+8192;
		tmp = malloc(l_tmp);
	}

	// Go through all entries and look for writeback(s) and
	// write all back to disk when neccesary to do so.
	for(i=0; i<hdb->n_e; i++)
	{
		// Get entry PTR
		h = hdb->e[i];

		// Skip free entries
		if(h->isfree)
			continue;

		// Write back if needed
		if(h->writeback==TRUE)
		{
			// Ignore broken entries
			if(h->c<0 || h->h<0 ||
				h->c>=65536 || h->h>=65536)
			{
				//
				h->writeback = FALSE;
				h->c = 0;
				h->h = 0;
				h->s = 1;
				panic("Illegal CHS detected...");
				printk("%s: ILLEGAL CHS detected!\n",
					__FUNCTION__);
				goto past;
			}

rewrite:
			//------------------------------------------
			// Write on disk
			hdSectorRW(chd,		// CURRENT HD
				h->buf,h->l_buf,
				h->c,h->h,1,
				chd->slave,
				WRITE,
				chd->sectors);

			//------------------------------------------
			memset(tmp,0x55,l_tmp);
			// Verify write.
			hdSectorRW(chd,		// CURRENT HD
				tmp,l_tmp,
				h->c,h->h,1,
				chd->slave,
				READ,
				chd->sectors);

			//
			for(x=0; x<(chd->sectors*512); x++)
			{
				if(h->buf[x] != tmp[x])
				{
					panic("IDE HD WRITE ERROR");
					goto rewrite;
				}
			}

			//------------------------------------------
			// Write OK:
			// Mark as written
past:
			h->writeback = 0;

			// Demand re-read
			h->isfree = TRUE;
			h->c = -1;
			h->h = -1;
			h->s = -1;
		}
	}
}


