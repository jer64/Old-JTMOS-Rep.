/*
 * ===========================================================
 * fslast.c
 * JTMFS FILE SYSTEM - Seek to last directory entry
 * (C) 2002 by Jari Tuominen
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
#include "fslast.h"

// NOTE: the dirwalking must be already running before calling this!
int jtmfs_seektolastentry(long device_nr, DIRWALK *dw)
{
	int i;

	// JTMFS required
	if(!jtmfat_isJTMFS(device_nr))return 1;

	// It has to be already walking(dw)
	if(!dw->isWalking)
	{
#ifdef JTMFSDEBUG
		print("jtmfs_seektolastentry: logic failure in dirwalk structure\n");
		print("isn't yet walking, cannot use a non-walking structure\n");
		print("(structure is undefined? should contain f.e. the dir's block number)\n");
#endif
		return 2;
	}
	else
	{
		// First to the starting point
		if( jtmfs_ffirst(device_nr,-1,dw) )return;
		// Seek to the last entry available
		for(i=1; ; i++)
		{
			if( jtmfs_fnext(device_nr,dw) )break;
		}
		if((i+10)>=MAX_ENTRIES_PER_DIR)
		{
			// Indicate "directory full".
			return RV_DIRFULL;
		}

#ifdef JTMFSDEBUG
		// Here we are at the last entry
		ReportMessage("seektolastentry: reached last entry, block=%d, offset=0x%x('%c' / asc %d)\n",
				dw->current_block, dw->current_offset,
				dw->buf[dw->current_offset],
				dw->buf[dw->current_offset]);
#endif

		//
		return 0;
	}

	//
	return 0;
}



