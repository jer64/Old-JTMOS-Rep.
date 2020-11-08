// ======================================================
// Internal Hard Disk Software Cache/Buffering System
// CHS ONLY CACHE - A SEPERATE CACHE WILL EXIST
// FOR NON-CHS DRIVES IN FUTURE, E.G. LBA32 ACCESS.
// (C) 2003 by Jari Tuominen
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
				goto past;
			}

			//------------------------------------------
repeat:
			// Write on disk
			hdSectorRW(chd,		// CURRENT HD
				h->buf,h->l_buf,
				h->h,1,h->c,
				chd->slave,
				WRITE,
				chd->sectors);

#ifdef HD_SLOW_VERIFY_WRITE
			// Read back
			hdSectorRW(chd,		// CURRENT HD
				tmp,h->l_buf,
				h->h,1,h->c,
				chd->slave,
				READ,
				chd->sectors);

			// Compare
			l = chd->sectors*512;
			for(x=0; x<l; x++)
				if(tmp[x]!=h->buf[x])
				{
					h->fault++;
					printk("%s: write fault, CHS=%d,%d,X\n",
						__FUNCTION__,
						h->c, h->h);
					goto repeat;
				}
#endif

			//------------------------------------------
			// Write OK:
			// Mark as written
past:
			h->writeback = 0;

			// Demand re-read
			h->isfree = 1;
		}
	}
}


