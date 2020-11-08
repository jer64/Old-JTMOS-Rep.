// jtmfsAccessInit.c
#include "kernel32.h"
#include "jtmfat.h" // getfreeblock
#include "jtmfs.h" // comes with fileapi as well
#include "ramdisk.h" // default_ramdisk
#include "fsdos.h"
#include "fname_translation.h" // tell_fname
#include "root.h"
#include "jtmfsAccessInit.h"

// Initializes kernel fileapi
//
void jtmfsAccessInit(void)
{
	// Init structure
	memset(&jtmfsAccess, 0, sizeof(JTMFSACCESS));

	// Path
	jtmfsAccess.path = malloc(1024*4);
	strcpy(jtmfsAccess.path,"");

	// Temporary buffer for block access
	// Must be larger than any possible drive's block size could be
	jtmfsAccess.rwtmp =	malloc(1024*64);
	jtmfsAccess.rwtmp2 =	malloc(1024*64);

	//
	jtmfsAccess.dnr = driver_getdevicenrbyname("ram");
}


