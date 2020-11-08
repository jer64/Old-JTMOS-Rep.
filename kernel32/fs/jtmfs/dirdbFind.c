//======================================================
// EDCDS DATABASE WALKING FUNCTIONS
// (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
//======================================================
#include "kernel32.h"
#include "dirdb.h"
#include "dirdbAccess.h"
#include "dirdbWalk.h"
#include "dirdbFind.h"

// Search for a regular file or directory.
int dirdb_ffindfile(int dnr,
                const char *fname, int db, DIRWALK *dw)
{
	//
	return dirdb_ffindfileEx(dnr, fname, db, dw, 0);
}

// Search for a deleted file entry.
int dirdb_ffindghostfile(int dnr,
                const char *fname, int db, DIRWALK *dw)
{
	//
	return dirdb_ffindfileEx(dnr, fname, db, dw, 1);
}

// Search for directory name definition,
// which indicates directory name.
// (parameter fname is ignored, use NULL) DIRWALK *dw
int dirdb_ffinddirname(int dnr, const char *name, int db, DIRWALK *dw)
{
	//
	return dirdb_ffindfileEx(dnr, NULL, db, dw, 2);
}

// Search for a specified file or directory.
// ghost=0 means: Search for non-deleted files only.
// ghost=1 means: Search for deleted files only.
// ghost=2 means: Search for directory names only.
// Return value: zero if not found...
int dirdb_ffindfileEx(int dnr,
                const char *fname, int db, DIRWALK *dw,
		int ghost)
{
	static char *p,*p2;
	static DIR *dir;
	static int i,i2;
	static JTMFS_FILE_ENTRY *fe;

	// Get directory
	dir = touchDir(dnr, db, ID_DIRECTORY);
	if(dir==NULL)return 0;

	// Go through all entries and look for requested file
	for(i=0,p=dir->buf; *p; p+=sizeof(JTMFS_FILE_ENTRY))
	{
		//
		fe = p;

		//
		if( fname!=NULL && !strcmp(fe->name, fname) )
		{
			// Ignore this directory name indicator.
			if( fe->type==JTMFS_ID_DIRNAME )
				continue;

			// Ghost filter:
			// If searching for scratched files and entry is not scratched, then skip it.
			if( ghost && fe->length!=-1 )	continue;
			// Disallow using DIRNAME entry, even if it is marked deleted(it is really not :).
			if( fe->type==JTMFS_ID_DIRNAME && ghost!=2 )	continue;
			// If not searching for special entries and file is scratched, then skip it.
			if( !ghost && fe->length==-1 )	continue;

			// Found requested file:
			// --------------------------------------
			if(dw!=NULL)
			{
				// Init dw structure.
				jtmfs_ffirst(dnr,db, dw);
				// Modify structure to point to this find.
				dw->d = dir;
				dw->fe = fe;
			}
			//
			printk("<<%s!>>", fe->name);
			// Return:
			return 1;
		}
		
	}

	// Requested file not found:
	return 0;
}

// Figure out what is the name of the directory at specified location.
// Return value: FALSE if not found, TRUE if found.
int dirdb_GetDirName(int dnr,int db, char *dest_name)
{
	static char *p,*p2;
	static DIR *dir;
	static int i,i2;
	static JTMFS_FILE_ENTRY *fe;

	// Get directory
	dir = touchDir(dnr, db, ID_DIRECTORY);
	if(dir==NULL)
		return FALSE;

	// Go through all entries and look for requested file
	for(i=0,p=dir->buf; *p; p+=sizeof(JTMFS_FILE_ENTRY))
	{
		//
		fe = p;

		//
		if(fe->type==JTMFS_ID_DIRNAME)
		{
			//
			printk("%s: Found DIRNAME entry for %d/%d, content='%s'(0x%x), type=0x%x.\n",
					__FUNCTION__, dnr,db,
					fe->name,fe->name, fe->type);
			printk("size of JTMFS_FILE_ENTRY = %d bytes\n",
				sizeof(JTMFS_FILE_ENTRY));

			// Found requested file:
			// --------------------------------------
			strcpy(dest_name, fe->name);

			// Return:
			return TRUE;
		}

		//
	}

	// Requested file not found:
	return FALSE;
}


