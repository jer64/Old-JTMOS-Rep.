/*
 *============================================================
 * fsfind.c
 * JTMFS FILE SYSTEM - File finder functions
 * (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
 *============================================================
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
#include "fsfind.h"
#include "DirDB.h"

// jtmfs_ffindfile dnr,fname,db,dw
//
// 0=not found
// 1=found
// dw = your DIRWALK structure, contains the block and offset to the entry
// then after it has found it, if found.
//
// NEW: now ignores the ghost file entries(deleted).
//
int jtmfs_ffindfile(int dnr,
		const char *fname,int _db,DIRWALK *dw)
{
	//
	dprintk("%s: %d/%d, search for '%s'\n",
			__FUNCTION__,
			dnr,
			_db,
			fname);
	dsleep(1);
	//waitkey();

	//
	return dirdb_ffindfile(dnr, fname, _db, dw);
}

// Resolves a directory name of a directory.
// In other words, converts a DNR/DB to name string.
int jtmfs_ResolveDirName(int dnr,int db,DIRWALK *dw)
{
	//
	return dirdb_ffinddirname(dnr,NULL,db,dw);
}

// jtmfs_ffindghostfile dnr,fname,db,dw
//
// 0=not found
// 1=found
// dw = your DIRWALK structure, contains the block and offset to the entry
// then after it has found it, if found.
//
//
int jtmfs_ffindghostfile(int dnr,const char *fname,int db,DIRWALK *dw)
{
	//
	return dirdb_ffindghostfile(dnr,fname,db,dw);
}

// _jtmfs_ffindfirstghostfile dnr,db,dw
// 0=not found
// 1=found
// dw = your DIRWALK structure, contains the block and offset to the entry
// then after it has found it, if found.
int jtmfs_ffindfirstghostfile(int dnr,int db,DIRWALK *dw)
{
	//
	if(!dw)return 0;

	//
	InitDW(dw);

	// first to the starting point
	if(jtmfs_ffirst(dnr,db,dw))return 3;

	// ????: Shouldn't we check the 1st entry first? TODO?
	return jtmfs_ffindnextghostfile(dnr, db, dw);
}

// Used after ffindfirstghostfile is called.
// Finds next ghost entry, and returns 1 if one is found,
// all other return values mean that no ghost entry is found.
//
int jtmfs_ffindnextghostfile(int dnr,int db,DIRWALK *dw)
{
	//
	if(!dw)return 0;
	if(!dw->fe)return 0;

	//
	while(1)
	{
		// Make sure that the file is deleted:
		if( dw->fe->length==-1 && dw->fe->type!=JTMFS_ID_DIRNAME )return 1;
		// Load next entry
		if( jtmfs_fnext(dnr,dw) )return 0;
	}

	//
	return 0;
}

