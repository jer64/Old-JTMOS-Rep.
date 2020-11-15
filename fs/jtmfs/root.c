//=============================================================
// Root device functions - root.c
// (C) 2003 by Jari Tuominen
//=============================================================
#include "kernel32.h"
#include "jtmfat.h" // getfreeblock
#include "jtmfs.h"
#include "ramdisk.h" // default_ramdisk
#include "fsdos.h"
#include "fname_translation.h" // tell_fname
#include "root.h"
#include "mount.h"

// Returns current root device number
int getRootDNR(void)
{
	return jtmfsAccess.dnr;
}

// MOUNT ROOT
// ----------
//
// Return values' meaning:
// 0=success, other values=error
// (1=drive does not contain a valid JTMFS file system)
//
int mount_root(int dnr)
{
	// Not valid device?
	if( !isValidDevice(dnr) ) return 1;
	jtmfsAccess.dnr=dnr;
	return 0;
}

// UNMOUNT ROOT
// ------------
//
// 0=success, other values=error
//
int umount_root(void)
{
	// Already unmounted?
	if(jtmfsAccess.dnr==-1)return 0;

	// Undo the registration of a root device.
	// Special notification:
	// device #0 does not exist,
	// all references to device #0 always mean
	// device "nothing".
//	jtmfsAccess.dnr=0;
	jtmfsAccess.dnr = driver_getdevicenrbyname("ram");

	//
	return 0;
}

