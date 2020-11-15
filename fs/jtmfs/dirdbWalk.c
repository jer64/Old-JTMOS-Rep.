// =====================================================
// EDCDS DATABASE WALKING FUNCTIONS
// (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
// =====================================================
#include "kernel32.h"
#include "dirdb.h"
#include "dirdbAccess.h"
#include "dirdbWalk.h"

// Marks directory currently being assigned
// as one that contains changed data within.
int dirdb_flushRequirement(DIRWALK *dw)
{
	DIRDEBUG

	//
	if(dw!=NULL && dw->d)
	{
		dw->d->flushRequired=1;
	}
	else
	{
		printk("%s: CANNOT SETUP FLUSH REQUIREMENT BIT\n",
			__FUNCTION__);
		printk("%s: dw=0x%x, dw->d=0x%x (either one is NULL)\n",
			__FUNCTION__,
			dw, dw->d);
		dsleep(1);
	}
	return 0;
}

// Steps over a dir entry
// (Moves to next entry)
// Return value: Returns nonzero
// if attempting to exceed the block boundary.
int dirdb_stepover(int dnr, DIRWALK *dw)
{
	char *e,*p;

	//
	if(dw==NULL || !dw->isWalking)		return 2;

	//
	DIRDEBUG

	// Exceeding allowed amount of entries?
	if(dw->entry>=MAX_ENTRIES_PER_DIR)
	{
		// Already skipped too far!:
		return 1;
	}

	// Determine end
	e = dw->d->buf; e+=dw->d->l_buf;
	// Advance
	p = dw->fe;
	p+=sizeof(JTMFS_FILE_ENTRY);
	dw->fe=p;
	dw->current_offset+=sizeof(JTMFS_FILE_ENTRY);

	// Next entry #
	dw->entry++;

	// End of area?
	if(p>=e)	return 2;

	//
	return 0;
}

// Valid Return Values are:
// ------------------------
// 1=End of dir and chain.
// 2=End of dir(entry is zero based)
// 0=New entry loaded
//
// Returns end on last entry,
// the last entry is delivered with
// the return value 2,
// meaning last entry on the directory.
int dirdb_fnext(int dnr, DIRWALK *dw)
{
	char *p,*e;
	JTMFS_FILE_ENTRY *fe;

	//
	if(dw==NULL || !dw->isWalking)		return 2;

	//
	DIRDEBUG

	// Try to step over, if not possible then return end
	if( dirdb_stepover(dnr, dw) ) return 2;

	// -> Check what location contains(CHECK 1/2)
	// End on following conditions:
	// - file name is empty
	// - magicNumber mistach
	if(dw->fe->name[0]==0 || dw->fe->magicNumber!=0xFCE2E37B)
	{
		// End of directory.
		return 2;
	}

/*	// Check what next location contains II
	p=dw->fe; p+=sizeof(JTMFS_FILE_ENTRY); fe=p;
	if(fe->name[0]==0)
	{
		return 2;
	}*/

	// OK
	return 0;
}

// Not needed to be emulated.
/* void dirdb_fend(DIRWALK *dw) { } */

// walks to the first dir entry and receives it
// note: will return nonzero on error
//
// new feature: if dirblock == -1 then
//	the dirwalking will be resetted
//      back to the starting block dw->start_block
//
int dirdb_ffirst(int dnr,
                int dirblock, DIRWALK *dw)
{
	int block;

	//
	if(!isValidDevice(dnr) || dw==NULL)
		return 1;

	//
	if(dirblock==-1)
	{
		block = dw->start_block;
	}
	else	block = dirblock;

	//
	DIRDEBUG

	// Get directory
	dw->d = touchDir(dnr, block, ID_DIRECTORY);

	// Go to the first entry of the directory
	dw->fe = dw->d->buf;
	dw->start_block = block;
	dw->current_block = block;

	// Reset offset
	dw->current_offset = 0;

	// Setup walking mode
	dw->isWalking=1;

	// Entry to 0
	dw->entry = 0;

	// Return OK
	return 0;
}

//


