//==========================================================================
// jtmfsPath.c - Path to DNR/DB conversion functions.
// (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
// DNR =	Device #
// DB =		Directory block #
//==========================================================================
#include "kernel32.h"
#include "jtmfat.h" // getfreeblock
#include "jtmfs.h" // comes with jtmfsAccess as well
#include "ramdisk.h" // default_ramdisk
#include "fsdos.h"
#include "fname_translation.h" // tell_fname
#include "root.h"
#include "jtmfsAccess.h"
#include "jtmfsPath.h"

// Resolves DNR/DB from a file name containing a path,
// also stores path and name in seperate strings.
int resolveFileNamePath(const char *filepath,
			int *dnr,int *db,
			RESOLVE *r)
{
	char *p,*s,*e;
	int i,i2,l,detected;

	// File name contains a path?
	if( strstr(filepath,"/") )
	{
		// Grep the path
		strcpy(r->path, filepath);
		for(i=strlen(r->path)-1; i!=0; i--)
		{
			if(r->path[i]=='/')
			{
				r->path[i]=0;
				break;
			}
		}

		// Grep the file name.
		// Skip '/' charcter.
		i++;
		l = strlen(filepath);
		// get name
		for(i2=0; i<l; i++,i2++)
		{
			r->name[i2] = filepath[i];
		}
		r->name[i2]=0;

		//
		resolvePath(r->path, dnr,db);

		// Declare that we did get something
		detected=1;
	}
	else
	{
		// Default to "no path resolved"
		detected=0;
		strcpy(r->path, "");
		strcpy(r->name, filepath);
	}
	return detected;
}

// Resolves a plain path into DNR/DB values
//
// resolvePath("/", &dnr,&db);
int resolvePath(const char *path, int *dnr,int *db)
{
	int odnr,odb;

	// Validity check
	if(path==NULL || dnr==NULL || db==NULL)return 1;

	// Save old location
	odnr = GetCurrentDNR();
	odb = GetCurrentDB();

	// Resolve path
	jtmfs_chdir(path);
	// Store resolved DNR/DB values
	*dnr = GetCurrentDNR();
	*db = GetCurrentDB();

	// Restore old location
	SetThreadDNR(GetCurrentThread(), odnr);
	SetThreadDB(GetCurrentThread(), odb);

	//
	return 0;
}

//

