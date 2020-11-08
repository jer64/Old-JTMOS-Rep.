// ==================================================
// FDC DISK DRIVER CACHE SYSTEM
// Normalizes disk access performance to MSDOS level
// and above. This driver achieves to perform
// as well as any other OS floppy driver.
// (C) 2002 by Jari Tuominen
// ==================================================
#include "kernel32.h"
#include "fdc.h"
#include "fdcCache.h"
// 9
// 18
// 18*2
#define PER_BUFFER	(18*2)
//#define PER_BUFFER	2

/******************************************************/
BYTE *giveBlock(int which, int readIt)
{
	int i,ofs;
	FDCENT *ent;
	ent = &fdc.ent[0];

	// We already have some block?
	// Make sure it's allocated.
	if(ent->isfree==0)
	{
		// Check it's location, does it match?
		if(which>=ent->s && which<ent->e)
			// Bingo, we have it here.
			goto weHaveIt;
	}

	// -------------------------------------------
	// Flush old track
	if(ent->writeback)
	{
		fdcFlushCacheEnt(ent);
	}

	//
	dprintk("%s: Cache miss for block %d (area %d-%d) isfree=%d\n",
		__FUNCTION__,
		which,
		ent->s, ent->e,
		ent->isfree);

	// -------------------------------------------
	// We have to read it (PER_BUFFER sectors):
	// Determine aligned block offset from requested block(which)
	i = which; i=i/PER_BUFFER; i=i*PER_BUFFER;
	// Determine cache entry
	ent->writeback =	0;
	ent->isfree =		0;
	ent->s =		i;
	ent->e =		i+PER_BUFFER;
	dprintk("%s:  Entry: %d-%d  Which=%d\n",
		__FUNCTION__,
		ent->s, ent->e,
		which);
	// Commence read
	// This is really commenced even for writes, really neccesary!
//	if(readIt)
		fd_read_blocks(i, ent->buf, PER_BUFFER);

	// ------------------------------------------
weHaveIt:
	// Flag with writeback for writes:
	if(!readIt)
		ent->writeback = 1;
	// Determine offset
	ofs = (which - ent->s);
	return ent->buf + ofs*512;
}

// Flush contents of the FDC buffers to the drive
void fdcFlushCacheEnt(FDCENT *ent)
{
	if(ent->writeback)
	{
		fd_write_blocks(ent->s, ent->buf, PER_BUFFER);
		ent->writeback=0;
	}
}

// Clear floppy disk drive cache
void fdcFlushCache(void)
{
	FDCENT *ent;
	ent = &fdc.ent[0];

	fdcFlushCacheEnt(ent);
}

// Clear floppy disk drive cache
void fdcClearCache(void)
{
	FDCENT *ent;
	ent = &fdc.ent[0];

	ent->isfree=1;
}

/******************************************************/
// Read single block
int fd_read_block(int block, BYTE *buf)
{
	memcpy(buf, giveBlock(block,1), 512);
}

// Write single block
int fd_write_block(int block, BYTE *buf)
{
	memcpy(giveBlock(block,0), buf, 512);
}
/******************************************************/



