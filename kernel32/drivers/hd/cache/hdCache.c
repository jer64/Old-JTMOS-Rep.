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
#include "hdCacheFlush.h"
#include "hdCacheAccess.h"

// Hard disk cache DataBase structure
HCDB *hdb;
int hdCacheActive=FALSE;

// INITIALIZE SOFTWARE HARD DISK CACHE SYSTEM
void hdCacheInit(void)
{
	int i,i2,l;
	HCE *h;

	//
	printk("HD cache init.\n");

	// Allocate main structure
	hdb = malloc(sizeof(HCDB));
	// Initialize structure
	memset(hdb, 0, sizeof(HCDB));

	// Define amount of cache entries
	//hdb->n_e = N_HD_CACHE;
	//
	l = (chd->sectors*chd->blocksize+1024*4);
	hdb->n_e = (MAX_CACHE_SIZE_K*1024)/l;

	// Allocate PTR table
	hdb->e = malloc(8*hdb->n_e);

	// Allocate structures
	for(i=0; i<hdb->n_e; i++)
	{
		// Allocate HD Cache Entry structure
		hdb->e[i] = malloc(sizeof(HCE));

		// Initialize structure
		memset(hdb->e[i], 0, sizeof(HCE));

		// Get entry
		h = hdb->e[i];
		// Allocate neccesary buffer
		h->l_buf = chd->sectors*chd->blocksize; // !
		h->buf = malloc(h->l_buf);
		// Initialize buffer contents
		memset(h->buf, 0, h->l_buf);
		// Set free
		h->isfree = TRUE;
	}

	//
	printk("Allocated %dK of HD cache space for %d cylinders.\n",
		(chd->sectors*chd->blocksize*hdb->n_e)/1024,
		chd->cylinders);

	// Show info about corruptions
	hdCacheShowInfo();

	// Set cache active
	hdCacheActive = TRUE;

	// Add debug trap
//	addPitHandler(hdCacheCheckBuffers);
}




