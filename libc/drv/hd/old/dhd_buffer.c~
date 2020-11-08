// ===================================================================
// Direct HD Access - TRACK BUFFER MANAGEMENT
// (C) 2001,2002 by Jari Tuominen(jari@vunet.org)
// This is the IDE Hard Disk driver for The JTMOS Operating System.
// ===================================================================
#include "kernel32.h"
#include "int.h"
#include "cpu.h" // nop()
#include "sysglvar.h"
#include "delay.h"
#include "io.h"
#include "dma.h"
#include "dhd.h"
#include "devsys.h"
#include "dhd_detect.h"
#include "dhd_dev.h"
#include "dhd_func.h"
#include "hdparam.h"
#include "dhd_sectorRW.h"
#include "dhd_buffer.h"
#include "corruption.h"

//
#define ISVALIDID(ID)	(ID>=0 && ID<h->db.n_tracks)

//
void dhd_flush(HDSPEC *h)
{
	int i,i2;

	// IF BUFFERING ISN'T ENABLED, RETURN IMMEDIATLY
	if(!HD_BUFFERING_ENABLED)	return;

	BUFDEB(
"dhd_flush: Flushing internal cache(tracks=%d)\n",
		h->db.n_tracks);
	for(i=0; i<h->db.n_tracks; i++)
	{
		if(h->db.track[i].used)
		{
			flushTU(h, i);
			BUFDEB(".");
		}
	}
	BUFDEB("\ndhd_flush: Done\n");
}

//
void dhd_bufferInit(HDSPEC *h)
{
	int i,i2,i3,i4;

	// Reset database
	memset(&h->db, 0, sizeof(TRACKDB));

	//
	h->db.n_tracks = HD_BUFFER_AMOUNT;

	// Allocate buffers
	for(i=0; i<h->db.n_tracks; i++)
	{
		//
		h->db.track[i].accessCount = 0;
		h->db.track[i].buf =
			malloc(h->blocksize*h->sectors + DHD_BUFFER_SAFEBOUNDARY);
		//
		SetCorruptionCheck(h->db.track[i].buf + h->blocksize*h->sectors);
	}
}

// Decreases all access counts that are non-zero.
// Used in prioritizing. All most recently needed
// blocks will be left in the cache this way.
// Even those cache entries which had a high
// access count history will be deallocated
// upon a period of idling.
void dhd_decreaseCounts(HDSPEC *h)
{
	int i,i2,i3,i4;
	BUFTRACK *t;

	// Go through all allocated entries
	for(i=0; i<h->db.n_tracks; i++)
	{
		t = &h->db.track[i];
		if(t->used && t->accessCount)
			t->accessCount--;
	}
}

// Return value:
// -1 = not buffered
// other = buffered to # unit
// the track data.
int cylinderIsBuffered(HDSPEC *h, int cylinder, int headNr)
{
	int i,i2,i3,i4;
	BUFTRACK *t;

	//
	for(i=0; i<h->db.n_tracks; i++)
	{
		t = &h->db.track[i];
		if(t->used
			&&
		   t->c==cylinder
			&&
		   t->h==headNr)
						return i;
	}
	return -1;
}

// Flush specified TRACK UNIT to physical disk
int flushTU(HDSPEC *h, int id)
{
	int rv,try,i,i2;
	BUFTRACK *t;

	//
	if(!ISVALIDID(id))	return 1;

	//
	t = &h->db.track[id];
	BUFDEB(
	"flushTU: Flushing ICU%d CHS %d,%d,%d\n",
		id,
		t->c,
		t->h,
		t->s);

	// Write back if needed
	if(t->writeBack)
	{
		// Try a few times, then give up if not OK
		for(try=0; try<HD_RW_TRIES; try++)
		{
			// Flush all sectors
			rv =	dhd_sectorRW1(h,
					t->buf,
					t->h,
					1,
					t->c,
					h->slave,
					WRITE,
					h->sectors);

			//
			if(!rv)goto noError;

			//
			dhd_recalibrate(h);

			//
			printk("%s: IDE error #10, try=%d, rv=%d, status=%x\n",
				__FUNCTION__,
				try, rv, dhd_GetStatus());
			dhd_TellStatus(h);
		}
	}

	// Flush error
	return 1;

noError:
	// Modify entry after flush
	t->used =	0;
	t->writeBack =	0;
	dhd_decreaseCounts(h);
	t->accessCount+=4;
	BUFDEB("flushTU: Flush completed OK\n");
	return 0;
}

// Frees a track unit
int deallocateTU(HDSPEC *h, int id)
{
	BUFTRACK *t;

	//
	if(!ISVALIDID(id))	return 1;

	// Get PTR
	t = &h->db.track[id];
	// If entry is used, deallocate it as was requested:
	if(t->used)
	{
		// Flush first
		flushTU(h, id);

		// Then remove entry
		t->writeBack =	0;
		t->used =		0;
		t->accessCount =	0;
		t->c =		0;
		t->h =		0;
		t->s =		0;

		// OK:
		return 0;
	}
	else
	{
		// Entry was not allocated at all:
		return 1;
	}
}

// Force deallocate some entry
int forceDeallocateSomeTU(HDSPEC *h)
{
	int least,which,i,i2,i3,i4;
	BUFTRACK *t;

	//
	printk("%s: Finding entry to deallocate\n",
		__FUNCTION__);

	// Find the least used
	for(i=0,least=1000000,which=0; // Always free first entry as default
			i<h->db.n_tracks; i++)
	{
		t = &h->db.track[i];
		if(t->accessCount < least
			&&
		   t->used) {
				which = i;
				least =	t->accessCount;
			}
	}

	// Found one?
	if(which==-1)
	{
		// Function must ALWAYS return one entry, because
		// this function is the last backbone.
		// Logic error:
		panic("IDE HD DRIVER: 'forceDeallocateSomeTU' LOGIC FAILURE");
	}

	//
	printk("%s: Found entry = %d\n",
		__FUNCTION__, which);

	// Deallocate it
	deallocateTU(h, which);
	return which;
}

// Get free track unit
int getFreeTU(HDSPEC *h)
{
	int i,i2,i3,i4;
	BUFTRACK *t;

	//
	for(i=0; i<h->db.n_tracks; i++)
	{
		t = &h->db.track[i];
		if( t->used )
				return i;
	}
	return -1;
}

int forceGetFreeTU(HDSPEC *h)
{
	int id;
	BUFTRACK *t;

	//
looppi:
	id = getFreeTU(h);
	if(ISVALIDID(id))
	{
		// Get PTR
		t = &h->db.track[id];
		// Allocate it
		t->writeBack=0;
		t->used=1;
		t->accessCount=0;
		// After that the caller
		// must define C/H/* on it...
		return id;
	}

	//
	printk("%s: Cache full, prioritizing required ...\n", __FUNCTION__);

	// Get one entry free
	return forceDeallocateSomeTU(h);
}

// Buffer one specified cylinder at the cache
int bufferCylinder(HDSPEC *h, int headNr, int sectorNr,
		int cylinder, int driveNr, int rw)
{
	int id,try,rv;
	BUFTRACK *t;

	//
	id = forceGetFreeTU(h);
	if(!ISVALIDID(id))
	{
		panic("dhd_buffer.c: bufferCylinder internal error");
		return 1;
	}

	// READ ALL SECTORS OF A SINGLE CYLINDER INTO BUFFER
	BUFDEB("bufferCylinder: Reading whole cylinder at CHS %d,%d,%d\n",
			cylinder, headNr, 1);
	for(try=0; ; try++)
	{
		//
		t = &h->db.track[id];

		//
		if( GetCorruptionCheck(t->buf
				+ h->blocksize*h->sectors) ) {
				panic("dhd_buffer.c: Corruption trap #1");
			}

		//
		rv = dhd_sectorRW1(h,
			t->buf, headNr,1,
			cylinder,
			driveNr,
			READ,
			h->sectors);

		//
		if(!rv)goto noError;

		//
		printk("flushTU: IDE error #11, try=%d, rv=%d, status=%x\n",
			try, rv, dhd_GetStatus());
		if( (try%10)==9 )
		{
			dhd_TellStatus(h);
			dhd_recalibrate(h);
		}
	}
	return 1;

noError:
	//
	t->c =			cylinder;
	t->h = 			headNr;
	t->used =		1;
	t->writeBack =		0;
	t->accessCount =	0;
	BUFDEB("bufferCylinder: Cylinder %d has been read OK\n",
			cylinder);
	return 0;
}

// Get one block
int bufferGetBlock(HDSPEC *h,
		BYTE *buf, int headNr, int sectorNr,	
		int cylinder, int driveNr, int rw)
{
	int unit,i,i2,i3,i4;
	BUFTRACK *t;

	//
	BUFDEB("bufferGetBlock called CHS %d,%d,%d\n",
		cylinder, headNr, sectorNr);

	// Check whether if we have it in the cache?
	unit = cylinderIsBuffered(h, cylinder, headNr);
	if(!ISVALIDID(unit))	return 1;

	// Copy requested block
	t = &h->db.track[unit];
	memcpy(buf, t->buf+((sectorNr-1)*h->blocksize), 
			h->blocksize);

	// Increase access counter for this entry
	dhd_decreaseCounts(h);
	t->accessCount+=4;

	//
	return 0;
}

int bufferPutBlock(HDSPEC *h,
		BYTE *buf, int headNr, int sectorNr,	
		int cylinder, int driveNr, int rw)
{
	int unit,i,i2,i3,i4;
	BUFTRACK *t;

	//
	BUFDEB("bufferPutBlock CHS %d,%d,%d\n",
			cylinder, headNr, sectorNr);

	//
	unit = cylinderIsBuffered(h, cylinder, headNr);
	if(!ISVALIDID(unit))	return 1;

	//
	t = &h->db.track[unit];
	memcpy(		t->buf+((sectorNr-1)*h->blocksize),
			buf,
			h->blocksize);
	dhd_decreaseCounts(h);
	t->accessCount+=4;
	t->writeBack=1;
	return 0;
}


