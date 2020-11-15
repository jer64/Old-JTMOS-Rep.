//
#ifndef __INCLUDED_FILEAPI_DOS_H__
#define __INCLUDED_FILEAPI_DOS_H__

//
#include "basdef.h"
#include "dirtype.h"

//
#define FA_RDONLY	JTMFS_ID_READONLY	// 1
#define FA_HIDDEN	JTMFS_ID_HIDDEN		// 2
#define FA_SYSTEM	JTMFS_ID_SYSTEM		// 4
#define FA_LABEL	JTMFS_ID_DIRNAME	// 8
#define FA_DIREC	JTMFS_ID_DIRECTORY	// 16
#define FA_ARCH		JTMFS_ID_ARCHIVE	// 32

//
typedef struct
{
	DWORD _ff_reserved[5];
	DWORD ff_attrib;
	DWORD ff_ftime;
	DWORD ff_fdate;
	DWORD ff_fsize;
	char ff_name[260];
}FFBLK;

//
int jtmfs_findfirst(int dnr, int db, const char *path, FFBLK *blk);
int jtmfs_findnext(int dnr, int db, int fd, FFBLK *blk);

#endif

