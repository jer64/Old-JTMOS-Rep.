/////////////////////////////////////////////////////////////////////////////////////////
// format.c
/////////////////////////////////////////////////////////////////////////////////////////
#include "kernel32.h"
#include "jtmfs.h"
#include "jtmfat.h"
#include "flexFat.h"
#include "format.h"

//-----------------------------------------------------------------------------------------
// What it does do :
//	- Creates an empty FAT
//	- Reserves system area on FAT
//	- Creates a basic root directory
int jtmfs_formatdrive(int device_nr)
{
	int nr_blocks,blocksize;
	char str[100];

	//----------------------------------------------------
	if(!isValidDevice(device_nr))
	{
		printk("%s/%s/%d: Invalid device number: number #%d.",
			__FUNCTION__, __FILE__, __LINE__,
			device_nr);
		return 10;
	}

	//----------------------------------------------------
	// Inform cache system first.
	DiskChange(device_nr);

	//
	driver_getdevicename(device_nr, str);
	printk("Formatting drive %s: ...\n",
		str);
	//
	nr_blocks = getsize(device_nr);
	blocksize = getblocksize(device_nr);
	//
	FFCHECK

	//
	printk("Drive has %d blocks, each block consists of %d bytes\n",
		nr_blocks, blocksize);

	//
	if(!nr_blocks)
	{
		//
		printk("Device is not suitable for a file system.\n");
		return 1;
	}

	// Make sure we are using correct device:
	jtmfat_chdev(device_nr);
	// Do the job:
	jtmfat_createfat(device_nr);
	jtmfs_createrootdir(device_nr);

	// Update drive contents at the end.
	FlushAll();

	//
	printk("Format Complete.\n");
	return 0;
}
