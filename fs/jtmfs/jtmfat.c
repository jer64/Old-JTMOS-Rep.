/*
 * ========================================================================
 * jtmfat.c - JTMOS FAT is part of the JTMOS File System(JTMFS) package
 * This file contains only parts of JTMFAT, see flexFat.* for more.
 * (C) 2001-2005 by Jari Tuominen(jari.tuominen@kanetti.fi).
 * ========================================================================
 */
 
/*
 * [FAT]       (FILE ALLOCATION TABLE)
 * |
 * V
 * [DATA AREA] (FILESYSTEM/OTHER DATA)
 *
 *
 *
 * FAT TABLE(32-bit)
 *
 * 00000000		empty place
 * FFFFFFF1-FFFFFFF6	bad track marking
 * FFFFFFF7		system reserved area(BOOTSECTOR & FAT ..)
 * FFFFFFF8		used to mark end of a file chain
 * FFFFFFFF		standard marker for end of a file chain
 * (!) Last cluster of a file always
 * contains a value of FFFFFFF8-FFFFFFFF.
 *
 * Ramdisks have got a standard sector
 * size of 1KB, 3.5" 1.44M floppys 512(½KB) bytes.
 */

//
#include "kernel32.h"
#include "rtc.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
//
#include "jtmfs.h"
// ---- #include "fileapi.h"
#include "ramdisk.h"
#include "jtmfat.h"

//
int findfreeblock_last_block_nr=-1,
	EntranceGetrootdir=0;
//
char *TMPSPACE1=NULL, *BOOT_TMP_BUF=NULL;
//
JTMFATSYS *fatsys;
// Set to TRUE after jtmfat is ready to be used
int jtmfat_ready=FALSE;
//
USE_DEBUGTRAP

//
static void DebugTrap(int dnr, const char *funcStr)
{
	char *str,*str2;
	int i,i2;
	BYTE *p;

	//
}

// Calculate 32-bit checksum out of the whole FAT table
DWORD GetFATChecksum32(CFAT *f)
{
	int i,i2,l;
	BYTE *p;
	DWORD sum;

	//
	p = f->buf;  l = f->l_buf;
	for(i=0,sum=0; i<l; i++) { sum+=(p[i]+1); }
	return sum;
}

// 0 =			OK
// 1 =			ERROR
// OTHER NUMBERS =	MISC. ERROR
int read_fat(int dnr)
{
	//
	DEBUGLINE
	return 0;
}

// Called by kernel cache flusher process only!:
int write_fat(int dnr)
{
	//
	return 0;
}

int allocate_fat(int dnr)
{
	//
	return 0;
}

// Touch on FAT it becomes alive...
int touch_fat(int dnr)
{
	//
	return 0;
}

// HAS TO BE CALLED FIRST BEFORE USING JTMFAT
int jtmfat_init(void)
{
	static int i,i2,i3,i4;


	//
	if(TMPSPACE1==NULL)	TMPSPACE1 = malloc(1024*64);

	//
	dprintk("%s: JTMFAT Initializing ... ", __FUNCTION__);
	//getch();
	DEBUGLINE
	
	//
	

	// ----------------------------------------------
	// GENERIC INIT
	// ----------------------------------------------
	//
	dprintk("1 ");
	//getch();
	fatsys = malloc(sizeof(JTMFATSYS));
	//
	dprintk("2 ");
	//getch();
	fatsys->virgin=1;
	dprintk("3 ");
	//getch();
	SetMemoryName(fatsys, "fatsys:structure");

	// Clear all the heck out first, clear table
	dprintk("4 ");
	//getch();
	memset((void*)fatsys, 0, sizeof(JTMFATSYS));
	dprintk("\n");
	//getch();


	// --------------------------------------------------------------
	//
	//
	dprintk("%s: Allocating sector buffers for all block devices:\n",
		__FUNCTION__);
	//getch();
	for(i=0; i<driver_nr(); i++)
	{
		// This is a block device?
		if(getsize(i)>=1)
		{
			fatsys->bb[i] = malloc(1024*32);
		}
	}
	dprintk("%s: Sector buffer allocation (COMPLETED OK).\n",
		__FUNCTION__);
	//getch();

	// ----------------------------------------------
	//
	//   Temporary buffer
	//
	dprintk("%s: Allocating fatsys->tbuf temporary buffer\n", __FUNCTION__);
	//getch();
	fatsys->tbuf = malloc(1024*64);
	SetMemoryName(fatsys->tbuf, "fatsys:tbuf");
	// ----------------------------------------------

	// Now we enable the jtmfat, after this, calls will work.
	dprintk("Enabling FAT system.\n");
	//getch();
	jtmfat_ready=TRUE;

	// Init FlexFAT
	dprintk("%s: Calling flexFatInit FLEX FAT Init.\n", __FUNCTION__);
	//getch();
	flexFatInit();
	
	dprintk("%s: We are done here, returning back to the caller.\n", __FUNCTION__);
	//getch();
	
	//
//	addPitHandler(jtmfatChecker);
}

// Updates info block for the drive
// (loads a new one from drive to the drive's info block buffer).
int jtmfat_LoadNewInfoBlock(int dnr)
{
	void *tmp,*p;
	int i,i2,i3;

	// Allocate on demand.
	if(fatsys->bb[dnr]==NULL)
	{
		//
		fatsys->bb[dnr] = malloc(getblocksize(dnr));
	}

	//
	tmp = TMPSPACE1;
	//
	ReportMessage("%s: Reading updated info block from drive %d.\n",
		__FUNCTION__, dnr);
	// Read info block(first block on the block device).
	readblock(dnr, FAT_INFO_BLOCK_OFFS/getblocksize(dnr), tmp);

	// Copy the info part to storage.
	p =	fatsys->bb[dnr];
	i2 =	sizeof(JTMFAT_INFOBLOCK);
	i3 =	FAT_INFO_BLOCK_OFFS%getblocksize(dnr);
	memcpy(p, tmp+i3, i2);

	// Here here.
	if( !strncmp(fatsys->bb[dnr]->detstr, "JTMFS", 5) )
	{
		printk("Block device #%d is not JTMFS-formatted.\n",
			dnr);
		fatsys->isJTMFS[dnr] = TRUE;
	}
	else
		fatsys->isJTMFS[dnr] = FALSE;

	//
	return 0;
}

// Loads up data to fatsys->bb[dnr]
// Returns non-zero if the drive is not
// JTMFS formatted.
int _jtmfat_loadinfoblock(const char *caller, int dnr)
{
	static BYTE *tmp,*p;
	static int i,i2;

	//
	dprintk("%s was called by %s\n",
		__FUNCTION__, caller);
	DEBUGLINE

	//------------------------------------------------------
	// FAT system must be active
	if(jtmfat_ready!=TRUE)return 1;

	//------------------------------------------------------
	// Check device
	if(!isValidDevice(dnr))
	{
		printk("%s: Invalid device %d.\n", 
			__FUNCTION__, dnr);
		return 2;
	}

	//------------------------------------------------------
	// Get temporary data buffer PTR
	tmp = TMPSPACE1;
	if(tmp==NULL)
		panic("loadinfoblock TMPSPACE1==NULL.");

	// Allocate on demand
	if(fatsys->bb[dnr]==NULL)
	{
		//
		jtmfat_LoadNewInfoBlock(dnr);
	}

	//
	dprintk("%s: Leaving this function.\n",
		__FUNCTION__);
	return fatsys->isJTMFS[dnr];
}

// Return value:
// Non-zero=Success, 0=Failure
//
int _jtmfat_chdev(const char *caller, long dnr)
{
	//
	FFCHECK

	//-----------------------------------------------------------------------------------
	//
	if(jtmfat_ready!=TRUE)return 0;

	//
	if(!isValidDevice(dnr))
	{
		dprintk("jtmfat_chdev: Invalid device %d\n",
			dnr);
		return 2;
	}

	//-----------------------------------------------------------------------------------
	// Note: This function is called really often,
	// adding debug traces to this function
	// will lead into a big flow of debug text.
	dprintk("%s was called by %s\n",
		__FUNCTION__, caller);

	//
	FFCHECK
	// ALWAYS load info block,
	// because this function is not the fast access function,
	// this function's job is to do the update of the info block.
	jtmfat_loadinfoblock(dnr);
	//
	FFCHECK
	//
	dprintk("%s: leaving this function\n",
		__FUNCTION__);
	
	//
	return fatsys->isJTMFS[dnr];
}

// jtmfat_getinfoblock =	Fast access to boot block of DNR(for FATGET/FATSET)
// jtmfat_chdev =		Slow access to boot block
// Get handle to the boot block of the specified DNR
JTMFAT_INFOBLOCK *jtmfat_getinfoblock(int dnr)
{
	// Let's load new boot block.
	if( fatsys->bb[dnr]==NULL)
		jtmfat_LoadNewInfoBlock(dnr);

	// Access cache entry only.
	// (will be NULL if called by flexFat before activation!!!!!)
	return fatsys->bb[dnr];
}

// Same as getrootdir1, but this one
// tries to retrieve the rootdir twice
// with two other ways.
int _jtmfat_getrootdir(const char *caller, int dnr)
{
	int block;

	//
	dprintk("%s was called by %s\n",
		__FUNCTION__, caller);

	//
	FFCHECK

	//
	EntranceGetrootdir=1;

	//
	DEBUGLINE

	//
	if(!isValidDevice(dnr))
	{
		//
		EntranceGetrootdir=0;
		printk("%s: Invalid device %d\n",
			__FUNCTION__, dnr);
		return 2;
	}

	//
	block = jtmfat_getrootdir1(dnr);
	if(!block)
	{
		// Try retrying twice by first trying to chdev
		printk("%s: Boot block = zero ??\n",
			__FUNCTION__);
	}

	//
	EntranceGetrootdir=0;

	//
	return block;
}

// Sets specified drive to "removable drive" -mode.
// Causes some minimal performance loss, but provides
// support for removable drives and first of all support
// for disk change.
void RemovableDevice(int dnr)
{
	//
	fatsys->bootinf[dnr].isRemovable = TRUE;
}

// Returns the block # where the root directory resides.
int jtmfat_getrootdir1(int dnr)
{
	int i;

	//
	DEBUGLINE

	//
	if(!isValidDevice(dnr))
	{
		printk("jtmfat_getrootdir1: Invalid device %d\n",
			dnr);
		return -1988;
	}

	//
	if(jtmfat_ready!=TRUE)
		return -1987;

	// Change device if it is not yet the requested one
	if( fatsys->bootinf[dnr].isRemovable )
	{
		// For removable drive:
		if( (fatsys->bb[dnr]==NULL || !fatsys->bb[dnr]->data_offs || fatsys->isJTMFS[dnr]==FALSE) ||
			(GetSeconds() - fatsys->bootinf[dnr].t)>=2 )
		{
			// Update reload time.
			fatsys->bootinf[dnr].t = GetSeconds();
			// Let's load new boot block.
			jtmfat_LoadNewInfoBlock(dnr);
		}
	}
	else
	{
		// For non-removable drive:
		if(fatsys->bb[dnr]==NULL || !fatsys->bb[dnr]->data_offs || fatsys->isJTMFS[dnr]==FALSE)
		{
			// Let's load new boot block.
			jtmfat_LoadNewInfoBlock(dnr);
		}
	}

	//
	dprintk("%s: Root directory at block %d on drive %d.\n", 
		__FUNCTION__,
		fatsys->bb[dnr]->data_offs,
		dnr);

	//
	return fatsys->bb[dnr]->data_offs;
}

// Searches for a free block and returns it
// NEW: Optimized to remember location for last found
// free block, it'll continue search on the last position.
// It is much possible that it'll find another empty
// block after it, or it is atleast much more probable
// to find than when starting from the beginning at every
// try.
// When the function is called first time, it'll not
// use the findfreeblock_last_block_nr -variable,
// although it'll save the block number to it after
// it has ran through the operation.
//
// Fixed: Won't accept block #0 as a free one ever.
//
// Returns -1 on disk full.
//
int jtmfat_getfreeblock1(long dnr)
{
	int i,i2,amount;

	// ------------------------------------
	DEBUGLINE

	//
	if(jtmfat_ready!=TRUE)
		return -3;

	//
	if( !isValidDevice(dnr) )
	{
		dprintk("jtmfat_getfreeblock1: Invalid device %d\n",
			dnr);
		return 0;
	}

	// ------------------------------------
	DEBUGTRAP

	//
	amount = getsize(dnr);
	if(amount<10)return -2;

	//
	if	(findfreeblock_last_block_nr!=-1
		&&
		findfreeblock_last_block_nr>0)
	{
		i = findfreeblock_last_block_nr;
	}
	else
	{
		//
		i = 0;
	}

	//
	for(i2=0; i2<amount; i2++,i++)
	{
		//
		if(i<=0 || i>=amount)
		{
			i=1;
		}

		//
		if( !jtmfat_getblock(dnr,i) )
		{
			findfreeblock_last_block_nr=i;
			return i;
		}
	}

	//
	findfreeblock_last_block_nr=i;

	// if not found, it returns a zero.
	return -1;
}

//
long jtmfat_getfreeblock(long dnr)
{
	int b;

	//
	DEBUGLINE

	//
	if(!isValidDevice(dnr))
	{
		dprintk("jtmfat_getfreeblock: Invalid device %d\n",
			dnr);
		return 0;
	}

	// ------------------------------------
	DEBUGTRAP

	//
	b = jtmfat_getfreeblock1(dnr);
	return b;
}

//------------------------------------------------------------
//
long jtmfat_isJTMFS(int dnr)
{
	//---------------------------------------
	// Couple checks
	if(jtmfat_ready!=TRUE)
	{
		printk("jtmfat isn't ready yet!\n");
		return FALSE;
	}

	//
	if(!isValidDevice(dnr))
		return FALSE;

	//---------------------------------------
	// Perform
	jtmfat_chdev(dnr);

	//
	return fatsys->isJTMFS[dnr];
}

// jtmfat_getblocksfree
// --------------------
// Returns the amount of blocks free
// Return value: below zero on error, zero on success
long jtmfat_getblocksfree(long dnr)
{
	static int i,i2,i3,i4,amount,n_free,n_allocated;

	//
	DEBUGLINE

	//
	if( !isValidDevice(dnr) )
	{
		dprintk("jtmfat_getblocksfree: Invalid device %d\n",
			dnr);
		return -1;
	}

	// ------------------------------------
	DEBUGTRAP

	// Get drive size
	amount=getsize(dnr);
	// Do sanity check
	if(amount<10)return 1;

	// Find free block
	for(i=0,n_allocated=0,n_free=0; i<amount; i++)
	{
		if( jtmfat_getblock(dnr,i) )
		{
			n_allocated++;
		}
		else
		{
			n_free++;
		}
	}

	//
	return n_free;
}

// jtmfat_getblocksallocated
// -------------------------
// Returns the amount of blocks free
// Return value: below zero on error, zero on success
long jtmfat_getblocksallocated(long dnr)
{
	static int i,i2,i3,i4,
		amount,n_free,n_allocated;

	//
	DEBUGLINE

	//
	if(!isValidDevice(dnr))
	{
		dprintk("jtmfat_getblocksallocated: Invalid device %d\n",
			dnr);
		return -1;
	}

	//
	jtmfat_chdev(dnr);
	if(!fatsys->isJTMFS[dnr])
	{
		dprintk("jtmfat_getblocksallocated: Non-JTMFS FS\n");
		return -1;
	}

	// ------------------------------------
	DEBUGTRAP

	//
	amount=getsize(dnr);

	//
	for(i=0,n_allocated=0,n_free=0; i<amount; i++)
	{
		if( jtmfat_getblock(dnr,i) )
		{
			n_allocated++;
		}
		else
		{
			n_free++;
		}
	}

	//
	return n_allocated;
}

int jtmfat_deactivateFatDNR(int dnr)
{
	//
	if(!isValidDevice(dnr))return 1;

	DEBUGLINE

	//
	fatsys->fat[dnr]->isActive=0;
	return 0;
}

//
void _jtmfatInformation(void)
{
	//
}

//




