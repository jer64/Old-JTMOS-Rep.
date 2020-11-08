// FILE NAME TRANSLATION FUNCTIONS FOR JTMFS
// (C) 2002 Jari Tuominen(jari.tuominen@kanetti.net)
// =================================================

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
#include "fsadd.h"
#include "fsdos.h"
#include "fname_translation.h"

// Returns device number, -1 if no one defined.
int tell_fname(const char *fname, char *parsedfname)
{
	static int i,i2,i3,i4,hasdev,l;
	static char strdev[50];

	l = strlen(fname);
	for(i=0,hasdev=0; i<l; i++)
	{
		if( fname[i]==':' )
		{
			// Copy found device to strdev
			for(i2=0; i2<i; i2++)
				strdev[i2] = fname[i2];
			strdev[i2]=0;

			// Convert device string to an
			// interger based device identificator
			hasdev = driver_getdevicenrbyname(strdev);
			if(hasdev==-1)hasdev=0;

			// ----------------------------------
			// Get the file name
			// skip past the ':' character
			i++;

			//
			goto nores;
		}
	}
	i=0;
nores:

	// Copy the file name
	for(i2=i,i3=0; i2<l; i2++,i3++)
		parsedfname[i3] = fname[i2];
	parsedfname[i3]=0;

	//
/*	printk("tell_fname: Resolved following, devnr = %d, fname = '%s'\n",
		hasdev, parsedfname);*/

	// Return possible device number
	return hasdev;
}
