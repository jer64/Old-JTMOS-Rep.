/*
 *============================================================
 * JTMFS FILE SYSTEM - Add directory entry.
 * (C) 2002-2003 by Jari Tuominen
 *============================================================
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

// jtmfs_adddirentry
// -----------------
//
// Adds a file/directory entry to the specified directory.
//
// Description:
//      It scrolls through existing directory tree,
//      hunting for empty place, if empty place is
//      found, it will use it,
//      if no empty place is found, it will create
//      one at the end of the directory tree,
//      if needed it will also expand the directory
//      size by one block so it gains needed extra
//      space for the new directory entry.
//
//      BTW, to detect an unused directory entry,
//      try: "if length == -1 then its empty"
//
//      Note: Unused directory entries are also
//      called "ghosts".
//
//
// This function adds a new directory entry
// on a directory specified by the block number.
//
// dnr,db,type
//
// db = defines on which block the directory is located at
//
// TODO: There is still some problems with the seektolastentry using in this function,
//        and/or the problem maybe related much to the code within this function itself as well.
// UPDATE: Not anymore, besides there might occur new problems afterwards, so I keep the TODO still.
//
int jtmfs_adddirentry(int dnr,int db,JTMFS_FILE_ENTRY *fe)
{
	DIRWALK dw,temp_dw;
	int cb,i,i2,i3,i4,expand;
	char *p;
	JTMFS_FILE_ENTRY *entry;
	char *src,*otp;

	// Force magicNumber
	fe->magicNumber = 0xFCE2E37B;

	// Inform
	dprintk("%s: dnr=%d/db=%d,fe=0x%x\n",
		__FUNCTION__,
		dnr, db, fe);
	dsleep(1);

	//
	InitDW(&dw); InitDW(&temp_dw);

	// TODO: add a dnr validity check here
	// Routine check for null pointers
	if(fe==NULL)
	{
		dprintk("jtmfs_adddirentry: error, NULL pointer reference for fe\n");
		return 111;
	}

	//
	if( !strlen(fe->name)  )
	{
		dprintk("silly error: filename's length is zero\n");
		return 110;
	}

	//
	dprintk("%s: Finding file '%s'\n",
		__FUNCTION__, fe->name);
	// Make sure that the filename is not yet used: 
	if( jtmfs_ffindfile(dnr, fe->name, db, &temp_dw) )
	{
		//
		ReportMessage("%s: File already exists(%s).\n",
			__FUNCTION__, fe->name);

		// Bugfix: Remembered the deallocation
		EndDirWalk(&temp_dw);
		// Cannot create - Already exists!
		return 10;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	expand=0;

	//
	dprintk("%s: Finding ghost file '%s'.\n",
		__FUNCTION__, fe->name);
	// extern int jtmfs_ffindfirstghostfile(int dnr,int block_nr,DIRWALK *dw);
	//
	// First we check for a ghost file entry, which we could use instead:
	if(  jtmfs_ffindfirstghostfile( dnr,db,&dw)==1  )
	{
		// Rearrangemable ghost entry found:
		// now we just pass over "the allocation of a new entry slot".
		dprintk("%s: Found deleted entry(%s)\n",
			__FUNCTION__, fe->name);
	}
	else
	{
		// If nothing appeared to be there, we just look for
		// the last entry and add a new one there:
		InitDW(&dw);

		//
		dprintk("%s: ffirst\n", __FUNCTION__);
		// Jump to first block of the chain
		if( jtmfs_ffirst(dnr,db,&dw) )
		{
			// Bugfix2: Remembered the deallocation
			EndDirWalk(&dw);
			return 145;
		}

		//
		dprintk("%s: seektolastentry.\n", __FUNCTION__);
		// Find the last entry.
		if( jtmfs_seektolastentry(dnr, &dw) )
		{
			// Directory full, don't add now.
			return 1005;
		}

		//
		dprintk("%s: stepover\n", __FUNCTION__);
		// (adddirentry -function)
		// Jump over the last entry, if we break the block boundary,
		// we will allocate one more block for the directory chain.
/*		if( jtmfs_stepover(dnr, &dw) )
		{
			// Error in modern walking.
			return 1004;
		}
		else
		{
			// Ah, we still have got space for this entry within the
			// current directory's block, we don't have to do anything
			// additionally:
			//
		}*/

	}

	//
	dprintk("%s: ffupdatefe.\n",
		__FUNCTION__);
	//
	jtmfs_ffupdatefe(&dw);
	// ----- We got a slot to copy the entry on, so here we do it then:  
	// Now copy the entry to it's place
	//memcpy( dw.fe, fe, sizeof(JTMFS_FILE_ENTRY) );
	otp=dw.fe; src=fe; i=sizeof(JTMFS_FILE_ENTRY);
	for(; i; i--,src++,otp++)*otp=*src;

#ifdef JTMFSDEBUG
	//
	dprintk("ADDING DIR ENTRY: offs=%x, block=%d, name='%s', entry=%d bytes\n",
			dw.current_offset, dw.current_block, dw.fe->name,
			sizeof(JTMFS_FILE_ENTRY));

	//
	for(p=dw.d->buf; *p; p+=sizeof(JTMFS_FILE_ENTRY))
	{
		entry=p;
		dprintk("0x%x: name='%s', length=%d bytes\n",
			p,
			entry->name, entry->length);
	}
	entry=p;
	dprintk("0x%x: name='%s', length=%d bytes\n",
		p,
		entry->name, entry->length);
	dsleep(1);
//	waitkey();
#endif

	dprintk("%s: ffwritelocation\n",
			__FUNCTION__);
	// Then write to the disk(..)
	jtmfs_ffwritelocation(dnr, &dw);

	// End walking, free the dirwalker
	EndDirWalk(&dw);

	// We have done a fine job, now we can return.
	return 0;
}


