/*
 * ===========================================================
 * JTMFS FILE SYSTEM - Add blocks for a dir entry
 * (C) 2002-2003 by Jari Tuominen
 * ===========================================================
 */
//
#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
// ---- #include "fileapi.h"
#include "ramdisk.h"
#include "jtmfs.h"
#include "fswalk.h"
#include "fsaddblocks.h"

// jtmfs_add_blocks_for_entry
//
// new: now checks for available space,
// if it is insufficient, we'll just
// return an error, a non-zero value.
//
int jtmfs_add_blocks_for_entry(int device_nr,
		char *fname, int dblock, int amount)
{
	static int z1;
	static DWORD i,i2,i3,i4,db,block,new_block,amount_to_allocate,retval;
	static DIRWALK dw;

	//
	if(  !amount || amount<0 || amount>=getsize(device_nr) 
		|| !isValidDevice(device_nr)  )
	{
		dprintk("%s: ERROR amount=%d, illegal amount.\n",
			__FUNCTION__, amount);
		return 10;
	}

	// No error as default.
	retval=0;
	// Init DIRWALK structure
	InitDW(&dw);

	//
	amount_to_allocate=amount;

	// Find the file
	dprintk("%s: jtmfs_ffindfile -call\n", __FUNCTION__);
	if( !jtmfs_ffindfile(device_nr, fname, dblock, &dw) )
	{
		// Not found?
		dprintk("%s: File '%s' not found\n",
			__FUNCTION__, fname);
		return 1;
	}

	// Does it have already a data block?
	if(!dw.fe->FirstDataBlock)
	{
		// No first data block allocated yet here:
		// =======================================
		if(glvar.debugMsg)
			dprintk("%s has no data allocated on it yet!\n", fname);
	 	// We have to do one less now
		amount_to_allocate--;
		// We don't have one yet, 
		// so we straight away start the allocation:
		block = jtmfat_getfreeblock(device_nr); if(block<=0)return 10;
		dw.fe->FirstDataBlock = block; // define first data block
		dw.fe->LastDataBlock = block; // (this one is same, because there is only one)
		dw.fe->length += amount*getblocksize(device_nr); // file size(length in bytes) realization
		// Then write file entry back to the disk(..)
		jtmfs_ffwritelocation(device_nr, &dw);
		// Set "end of chain" at first block
		jtmfat_setblock(device_nr, block, 0xFFFFFFF8);
	}
	else
	{
		//dprintk("%s has first data block on %d\n", fname, dw.fe->FirstDataBlock);
		// First data block allocated already here:
		// ========================================
		block = dw.fe->FirstDataBlock;
		// We have already a data block, just update the size information:
		dw.fe->length += amount*getblocksize(device_nr); // file size(length in bytes) realization
		// Then write file entry back to the disk(..)
		jtmfs_ffwritelocation(device_nr,&dw);
	}

	// OPTIMIZED ON 17.07.2002
	// Added use of more modern FirstDataBlock & LastDataBlock.
	block = dw.fe->LastDataBlock;

	// Add more blocks to the chain
	for(i=0; i<amount_to_allocate; i++)
	{
		z1 = DExpandChain(device_nr, block);
		if( z1<=0 )
		{
			retval=3;
			break;
		}
		db=jtmfat_getblock(device_nr, block);
		block=db;
	}

	// Write directory file entry back on disk
	dw.fe->LastDataBlock = block;
	jtmfs_ffwritelocation(device_nr, &dw);
	//dprintk("BLOCKS %d-%d\n", dw.fe->FirstDataBlock, dw.fe->LastDataBlock);

	//
	return retval;
}

