// =====================================================
// dirdbAccess.c - EDCDS DATABASE ACCESS FUNCTIONS.
// (C) 2002-2005 by Jari Tuominen.
// =====================================================
#include "kernel32.h"
#include "dirdb.h"
#include "dirdbAccess.h"
#include "dirdbWalk.h"

//
#define ddprintk dprintk
//#define ddprintk printk

//
int roll=0;

// This function is called upon disk
// changes(removable storage devices).
void dirdb_DiskChange(int dnr)
{
	//
	dirdb_clearCacheForDNR(dnr);
}

// Clears DIRDB cache for a specified DNR
// This funtion is called by e.g. IRQ handlers,
// therefore it must be kept quick and simple.
void dirdb_clearCacheForDNR(int dnr)
{
	DIR *d;
	int i,i2,i3,i4;
	int flags;

	//
	DIRDEBUG

	// Find all DNR's entries and deallocate these
	for(i=0; i<dirdb.n_entries; i++)
	{
		//
		d = &dirdb.dir[i];

		//
		if( d->dnr==dnr )
		{
			d->isfree = 1;
			d->dnr = -1;
			d->db = -1;
		}
	}
}

//
void dirdb_clearCacheForDBEx(int dnr, int db)
{
	DIR *d;

	//
	DIRDEBUG

	// Check validity
	if(dnr<0 || db<0)return;

	// Type 0 means = Any
	if( (d=dbGiveDir(dnr, db, 0)) )
	{
		d->isfree = 1;
		d->dnr = -1;
		d->db = -1;
	}
}

//
void dirdb_clearCacheForDB(int dnr, int db)
{
	//
	DIRDEBUG

	// Check validity
	if(!isValidDevice(dnr) || db<0)
		return;

	//
	if(dbGiveDir(dnr,db,0)) // Type 0 = Any
	{
		FlushAll(); // ?????????? Is this allowed?
		dirdb_clearCacheForDBEx(dnr, db);
	}
}

//
int getAlreadyFreeDBEntry(void)
{
	static int i,i2,i3,i4,id,req;

	//
	DIRDEBUG

	// Warning: a possible endless loop.
	i4=roll&3; roll++;
retry:
	//
	for(i=0; i<dirdb.n_entries; i++)
	{
		if(dirdb.dir[i].isfree)
			return i;
	}

	// Already too much allocated?
	if(dirdb.n_entries>=MAX_DIRS_CACHED/2)
	{
		// Determine a fair requirement, 1/3 of space must be free
		req = dirdb.n_entries/3; if(!req)req++;
		// Make some space, deallocate ~10 entries
		for(i=0,i2=0; i<dirdb.n_entries && i2<req; i++)
		{
			if(!dirdb.dir[i].isfree && !dirdb.dir[i].flushRequired
				&& ((i+i4)&3)==0)
			{
				free(dirdb.dir[i].buf);
				dirdb.dir[i].l_buf=0;
				dirdb.dir[i].isfree=1;
				i2++;
			}
		}

		//
		i4++;
		if(i4>=10)	goto alloc;
		goto retry;
	}

alloc:
	// In cases when we afford to suggest allocation, we do so:
	return -1;
}

//
int reserveNewDBEntry(void)
{
	static int i;

	//
	DIRDEBUG

	//
	if(dirdb.n_entries>=dirdb.total)
	{
		panic("reserveNewDBEntry: TODO #1.");
	}

	//
	i = dirdb.n_entries;
	dirdb.n_entries++;
	return i;
}

// New: now deallocates a few entries if none is found.
int getFreeDBEntry(void)
{
	static int i,id;

	//
	DIRDEBUG

	//
	id = getAlreadyFreeDBEntry();
	if(id==-1)
	{
		// Reserve new entry
		id = reserveNewDBEntry();
	}
	return id;
}

// touchDir dnr, db
//
// This function "touches" a directory on the database,
// if it does not exist on the database then it'll get it,
// so it is there on the fly, this makes it look for
// the caller like the directory would've been 
// there all the time.
DIR *touchDir(int dnr, int db, int type)
{
	static DIR *d;

	//
	DIRDEBUG

	//
	dirdbCorruptionCheck();

	// If not valid device, just return NULL
	if(!isValidDevice(dnr))return NULL;

	// Check for corruption
	FFCHECK

	//
	if( (d=dbGiveDir(dnr, db, type))==NULL )
	{
		//
		ddprintk("%s: Caching directory %d/%d\n",
			__FUNCTION__,
			dnr,db);
		return copyDirToDB(dnr,db, type);
	}

	//
	return d;
}

// Returns NULL if dir is not in the database.
DIR *dbGiveDir(int dnr, int db, int type)
{
	static int i;
	static DIR *d;

	// If not valid device, just return NULL
	if(!isValidDevice(dnr))		return NULL;

	//
	FFCHECK
	DIRDEBUG

	//
	ddprintk("%s: Searching through cache (%d entries) for %d/%d\n",
		__FUNCTION__,
		dirdb.n_entries,
		dnr,db);

	//
	for(i=0; i<dirdb.n_entries; i++)
	{
		d = &dirdb.dir[i];
		if(!d->isfree
			&&
		   d->dnr==dnr
			&&
		   d->db==db
			&&
		   (d->type==type || type==0)) // type 0 = Any type
		{
			ddprintk("%s: Found entry %d,%d\n",
				__FUNCTION__,
				d->dnr, d->db);
			return d;
		}
	}
	ddprintk("%s: Entry %d,%d not found\n",
		__FUNCTION__,
		dnr, db);
	return NULL;
}

// Copy [file or directory] to database
DIR *copyDirToDB(int dnr, int db, int type)
{
	DIR *d;
	static int i,i2,i3,i4;

	DIRDEBUG

	//
	if( !isValidDevice(dnr) )	return NULL;

	// Check if it is there already
	if( (d=dbGiveDir(dnr, db, type))!=NULL )
	{
		return d;
	}

	// Find free entry in database
	i = getFreeDBEntry();

	// Get the structure PTR
	d = &dirdb.dir[i];

	// Set it as reserved
	d->isfree=0;
	// Specify location
	d->dnr =	dnr;
	d->db =		db;
	// Specify type
	d->type =	type;

	// Clear the buffer first
	memset(d->buf, 0, d->l_buf);

	// Read directory/file contents
	readDir(d);

	// Return directory PTR
	return d;
}

//
int db_ReadDirectory(int dnr,int db,
		BYTE *buf,int l_buf)
{
	BYTE *tmp;
	int l_tmp;
	int i,i2,i3,i4,ad,ad2,lba,x,bsz;

	// Device configuration...
	if(!isOK(dnr)) { return 0; }
	bsz = getblocksize(dnr);

	//
	tmp = malloc(65536);

	//
	for(lba=db,ad=0; ; )
	{
		//
		readblock(dnr,lba,tmp);

		//
		for(x=0; x<bsz && ad<l_buf; x++)
		{
			buf[ad++] = tmp[x];
		}

		//
		if(ad>=l_buf) break;

		//
		lba = jtmfat_getblock(dnr,lba);
		if( jtmfs_LastBlock(lba) ) break;
	}

	//
}


// Reads directory into a DIR* structure.
int readDir(DIR *d)
{
	int blocks_read;

	DIRDEBUG

	//
	if(d==NULL)
		return 1;

	//
	ddprintk("%s: Reading directory dnr=%d, db=%d, buf=0x%x\n",
		__FUNCTION__,
		d->dnr, d->db,
		d->buf);

	// For files, do some modifications:
	if(d->type==ID_FILE)
	{
		// Make some space for a file
		if(d->l_buf < MAX_CACHED_FILE_SIZE)
		{
			if(d->buf!=NULL)	free(d->buf);
			d->buf = malloc(1024*128);
			d->l_buf = 1024*128;
		}
	}

	// Read directory from chain
	memset(d->buf, 0, d->l_buf);
	blocks_read = db_ReadDirectory(d->dnr,d->db, d->buf,d->l_buf);
	// d->buf, d->dnr, d->db, d->l_buf, 0);
	//	jtmfs_ReadFileChain

	// Define amount of blocks cached
	d->blocks = blocks_read;

	// Determine amount of entries directory has
	if(d->type=ID_DIRECTORY)
	{
		d->n_entries = countEntries(d);
		ReportMessage("%s: %d entries detected for this directory.\n",
			__FUNCTION__,
			d->n_entries);
		dsleep(1);
	}

	//
	return 0;
}

// Flush directory database as needed
int dirdbFlush(void)
{
	int i,i2,c;
	DIR *d;

	DIRDEBUG

	// Wait until ready
	disable();
	while(1)
	{
		// Some prg currently using db?
		if(dirdb.state!=1)break;
		SwitchToThread();
	}
	// Save old state
	dirdb.l_state = dirdb.state;
	// Set state as "flush"
	dirdb.state = 2;
	//
	enable();

	// Flush all dirs that are needed to
	for(i=0,c=0; i<dirdb.n_entries; i++)
	{
		d = &dirdb.dir[i];
		if(!d->isfree
			&&
		    d->flushRequired)
		{
			// Flush only if it is allocated, otherwise report it
			if(!d->isfree)
				writeDir(d);
				else
				ddprintk("%s: WARNING! Can't flush freed entry.\n",
					__FUNCTION__);
			c++;
			d->flushRequired=0;
		}
	}

	//
	ddprintk("%d dir(s) flushed.\n", c);

	// Restore state
	dirdb.state = dirdb.l_state;

	// OK
	return 0;
}

// countEntries d
//
// This function determines how many
// directory entries there are
// and returns the amount as an integer.
int countEntries(DIR *d)
{
	int i;
	char *p,*e;
	JTMFS_FILE_ENTRY *fe;

	DIRDEBUG

	//
	if(d==NULL)	return 0;

	// Determine location of end of the buffer
	e = d->buf; e+=d->l_buf;
	// Count entries
	for(p=d->buf,i=0; ; p+=sizeof(JTMFS_FILE_ENTRY),i++)
	{
		// Get fe PTR
		fe = p;

		// Unexpected end?
		if(p>=e)break;

		// Last entry reached?
		if(!fe->name[0])
			return i;

		//
		if(fe->type==JTMFS_ID_DIRNAME)
		{
			printk(">>>> \"%s\" <<<<\n", fe->name);
		//	sleep(10);
		}

		// If magicNumber is wrong, we assume this directory
		// is entirely broken, we will therefore report 0 files.
		// The caller of this function should understand
		// "Return Value 0" as an indication of the fact that
		// the directory is corrupted, and therfore should be
		// ignored and/or fixed.
		if(fe->magicNumber!=0xFCE2E37B)	
		{
			// Corrupted directory detected:
			return 0;
		}
	}
	return 0;
}

//


