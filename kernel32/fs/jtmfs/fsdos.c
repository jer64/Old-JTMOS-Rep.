// ==============================================================================================
// fsdos.c - DOS compatibility functions
//
// JTMOS file I/O application implementation for applications and medium level system functions
// DOS compatibility functions. For easy access.
//
// findfirst find MUST be ended with close(fd);, so that the file descriptors gets dellocated.
//
// (C) 2002-2003 Jari Tuominen(jari.tuominen@kanetti.net)
// ==============================================================================================
#include "kernel32.h"
#include "jtmfat.h"	// getfreeblock
#include "jtmfs.h"	// comes with fileapi as well
#include "ramdisk.h"	// default_ramdisk
#include "fsdos.h"
#include "filedes.h"	// From kernel32/fs (the generic file system, and its File System Support Submodules)

// Copy to DOSish FFBLK structure from JTMOS file entry.
void FE2BLK(JTMFS_FILE_ENTRY *fe, FFBLK *blk)
{
	// Get file name.
	strcpy(blk->ff_name, fe->name);
	// Get length.
	blk->ff_fsize = fe->length;
	blk->ff_attrib = fe->type;
	// Rest are not supported, yet.
	blk->ff_fdate = 0;
	blk->ff_ftime = 0;
}

// Copy to DOSish FFBLK structure from JTMOS file entry.
void NAME2BLK(const char *fname, FFBLK *blk, int sz)
{
	// Get file name.
	strcpy(blk->ff_name, fname);
	// Get length.
	blk->ff_fsize = sz;
	// Rest are not supported yet.
	blk->ff_attrib = 0;
	blk->ff_fdate = 0;
	blk->ff_ftime = 0;
}

// jtmfs_findfirst
//
// Returns file descriptor(int) if OK, below or equal to 0
// if no matching files found or on error.
int jtmfs_findfirst(int dnr, int db, const char *path, FFBLK *blk)
{
	int fd;
	FILEDES *d;
	int rv;

	//
	dprintk("%s: new_filedes\n", __FUNCTION__);
	fd = new_filedes(); // Get new file descriptor
	dprintk("%s: get_filedesptr\n", __FUNCTION__);
	d = get_filedesptr(fd); // Get jtm_filedes -structure pointer
	if(d==NULL)
		return -1;

	// Try searching for exact match (device or file)
	if(jtmfs_Fexist(dnr, path, db)
		|| driver_getdevicenrbyname(path)!=-1)
	{
		//
		NAME2BLK(path, blk, jtmfs_GetFileSize(dnr, path, db));
		blk->_ff_reserved[0] = 1; // single
		return fd;
	}
	else
	{
		//
		blk->_ff_reserved[0] = 0; // single
	}

	//
	dprintk("%s: jtmfs_ffirst\n", __FUNCTION__);
	// first to the starting point
	if( (rv=jtmfs_ffirst(dnr, db, &d->dw)) )
	{
		// Not found.
		dprintk("%s: No file(s) found\n", __FUNCTION__);
		FreeFD(fd);
		return -rv;
	}
	else
	{
		// Found:
		dprintk("%s: File(s) found, fd=%d\n",
			__FUNCTION__, fd);
		FE2BLK(d->dw.fe, blk);
		return fd;
	}
}

// findnext
// findnext(p->dnr, p->db, p->v1, p->par1)
//
// Returns 0 if OK, non-zero if end of directory on error.
int jtmfs_findnext(int dnr, int db, int fd, FFBLK *blk)
{
	FILEDES *d;
	int r;

	// Check for DESPTR
	d = get_filedesptr(fd); // Get jtm_filedes structure pointer
	if(d==NULL)
	{
		dprintk("%s: bad file descriptor(%d)\n",
			__FUNCTION__, fd);
		return -3;
	}

	//
	if(blk->_ff_reserved[0]) // single
		// End of directory, no more files
		return -2;

	//
	if( (r=jtmfs_fnext(dnr, &d->dw)) )
	{
		//
		dprintk("%s: jtmfs_fnext returns %d\n",
			__FUNCTION__, r);

		// Entry received & End of dir.
		//FE2BLK(d->dw.fe, blk);
		return r;
	}
	else
	{
		// Entry received.
		FE2BLK(d->dw.fe, blk);
		return 0;
	}
}

