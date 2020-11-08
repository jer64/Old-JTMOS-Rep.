/*
 * ===========================================================
 * fswalk.c - JTMFS FILE SYSTEM DIRECTORY WALKING CODE
 * (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
 * ===========================================================
 */
//
#include "kernel32.h"
#include "sysmem.h"
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
#include "dirdb.h"

// Steps over a dir entry
// (Moves to next entry)
// return value: returns nonzero if attempting to exceed the block boundary
// (means, we need to load in another block.. or etc..)
// Special note: IT DOES NOT RETURN NONZERO ON AN EMPTY DIR ENTRY..
//               ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int jtmfs_stepover(int dnr, DIRWALK *dw)
{
	int i,i2;

	//
	return dirdb_stepover(dnr, dw);
}


// jtmfs_fnext
// walks to next dir entry and receives it
// note: will return nonzero on error or on end of dir
// new: also updates the dw->fe pointer correctly now!
// new2(FIX): returns nonzero on an empty dir entry 
// new3: loads new block if one exists
//
// Valid Return Values are:
// ------------------------
// 1=End of dir and chain.
// 2=End of dir(entry is zero based)
// 0=New entry loaded
//
int jtmfs_fnext(int dnr,
		DIRWALK *dw)
{
	//
	return dirdb_fnext(dnr, dw);
}

// Ends the ffirst/fnext,
// frees the allocated resources, etc.
void jtmfs_fend(DIRWALK *dw)
{
        //
        dw->fe=NULL;
        dw->current_block = -1;
        dw->loaded_block = -1;
}

// jtmfs_ffirst
// walks to the first dir entry and receives it
// note: will return nonzero on error
//
// new feature: if dirblock == -1  then  the dirwalking will be resetted
// 	back to the starting block dw->start_block
//
int jtmfs_ffirst(int dnr,
		int dirblock, DIRWALK *dw)
{
	// JTMFS required
	if( !jtmfat_isJTMFS(dnr) )return 10;

	//
	DEBUGLINE

	//
	FFCHECK
	// TODO: Add a dnr validity check here
	// null pointer detector.
	if(dw==NULL)
	{
		//
		dprintk("jtmfs_ffirst: Error, dw == NULL\n");
		return 1;
	}

	//
	FFCHECK
	DEBUGLINE
	// Make sure that the requested block is allocated
	if( dirblock!=-1 && !jtmfat_getblock(dnr, dirblock) )
	{
		//
		dw->current_block=-1;
		dw->isWalking=0;
		dprintk("jtmfs_ffirst: Non-existant block (%d)\n", dirblock);
		dprintk("dirblock=%d, FAT reads=%d\n",
			dirblock,
			jtmfat_getblock(dnr, dirblock));
		return 1;
	}

	//
	DEBUGLINE

	//
	return dirdb_ffirst(dnr, dirblock, dw);
}


