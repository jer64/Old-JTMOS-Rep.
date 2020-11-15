//=====================================================================================
// createfat.c - FAT & info block creation.
//
// (C) 2003 by Jari Tuominen
//
// Note: The largest block size the FAT system can handle is 16K,
// Formatting drives with larger than 16K blocks will lead into
// corruption of the file system.
// New: Now also creates more detailed information block,
// which contains specific information about the file system,
// kernel and boot loader configuration.
// The present createfat also automatically detects standard
// BIOS drives, e.g. A:, B:, C:, D: drives and writes
// boot loader on the drive with appropriate configuration.
//=====================================================================================
#include "kernel32.h"
#include "jtmfs.h"
#include "jtmfat.h"
#include "flexFat.h"
#include "createfat.h"

// FAT CREATION
// ------------
//
// - Allocates & writes a boot loader on the drive if is bootable.
// - If drive is bootable, a 512K kernel region is reserved on the drive.
// - Updates boot record.
// - Updates information block.
// - Creates a blank FAT table, JTMOS stylish.
//
// This function makes sure that most of it's writes and
// works get done properly.
//
int jtmfat_createfat(int dnr)
{
	int i,i2,i3,i4,l,data1,v1,v2,
		tick,tick2,trigger,trigger2;
	BYTE *tmp,*p;
	char *s,*s2;
	int bytes_per_entry=4,bdrv;
	BYTE *vali;
	char devname[50];
	JTMFAT_INFOBLOCK *inf;

	//---------------------------------------------------------------------------------------------
	if(jtmfat_ready!=TRUE)
		return 10;

	//
	if(!isValidDevice(dnr))
	{
		printk("jtmfat_createfat: Invalid device %d.\n",
			dnr);
		return 11;
	}

	//
	if( !getsize(dnr) )
	{
		panic("jtmfat_createfat: Size of the specified device is zero blocks.");
		return 12;
	}

	//---------------------------------------------------------------------------------------------
	// Get device name
	driver_getdevicename(dnr, devname);

	//////// FIRST FLUSH CACHE THEN WORK ///////////
	printk("Flushing cache ...\n");
	driver_TriggerCacheFlush();

	//////////////////////////////////////////////////////////////////////////////////////////

	// Convert to BIOS drive.
	bdrv = DetermineBootdrv(devname);

	// Which BIOS drive.
	switch(bdrv)
	{
		//
		case 0:
		case 1:
		printk("%s: is a floppy disk drive in BIOS standards.\n",
			devname);
		break;

		//
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
		printk("%s: is an IDE hard disk drive in BIOS standards.\n",
			devname);
		break;

		//
		default:
		break;
	}



	// -------------------------------------------------------
	// And now we need to store all this information on
	// the bootblock.
	// There is a slight problem, if the disk will be made
	// bootable, it still needs to be JTMOS complaint,
	// or it will not be accessible from JTMOS,
	// so probably whetever a bootdisk will be accessed
	// it'll always return invalid values because of
	// the incorrect bootblock.
	// However, the problem is not that big since,
	// many bootdisks often only contain just the kernel image
	// and perhaps a root ramdisk on it too, or on another disk.
	// Perhaps this problem will be fixed in the future.
	// For example slackware comes with two disks,
	// boot(kernel) disk and the ramdisk image disk.

	// Install boot loader.
	InstallBootLoader(dnr, bdrv);

	// Build information block.
	inf = BuildInfoBlock(dnr, bdrv);
	// Write on drive.
	WriteInformationBlock(dnr, inf);

	// -------------------------------------------------------

	// We need to refresh the drive, to update boot block cache.
	// DiskChange should not be called unless really needed,
	// e.g. it causes current cached FAT to be lost,
	// currently cached directories for the directory to be lost,
	// everything will basically have to be re-read from the disk.
	// Now before calling DiskChange, always a devapi flush should
	// be called, to make sure the cache is flushed properly.
	printk("Refreshing drive ...\n");
	driver_TriggerCacheFlush();
	DiskChange(dnr);

	//
	printk("Detection string(detstr) = '%s'.\n",
		fatsys->bb[dnr]->detstr);

	// ------ ALLOCATE SYSTEM AREA
	// Allocate system area for system use only:
	//
	printk("Allocating system area 0-%d.\n", fatsys->bb[dnr]->sysarea_length);
	l = fatsys->bb[dnr]->sysarea_length;
	FastSetBlocks(dnr, 0,l, 0xFFFFFFF7);
	// Verify.
	for(i=0; i<l; i++)
	{
		// Peek for value.
		if(jtmfat_getblock(dnr,i)!=0xFFFFFFF7)
		{
			// Wrong value detected:
			printk("PASS I ERROR: FAT creation failed, could not write values on FAT table.\n");
			return 1978;
		}
	}
	// ------- SELF-DIAGNOSTIC 1
	//
	printk("System area reserved OK.\n");

	// ------ FREE DATA AREA
	// Free rest of blocks:
	// Define range.
	v1 = fatsys->bb[dnr]->sysarea_length;
	v2 = getsize(dnr);
	// This alittle long, so we have a percent done meter as well.
	printk("Setting blocks free on FAT %d-%d.\n",
		v1,v2);
	FastSetBlocks(dnr, v1,v2, 0);
	// Verify.
	for(i=v1; i<v2; i++)
	{
		// Peek for value.
		if(jtmfat_getblock(dnr,i)!=0)
		{
			// Wrong value detected:
			printk("PASS II ERROR: FAT creation failed, could not write values on FAT table.\n");
			return 1978;
		}
	}

	// RELEASE A BLOCK FOR ROOT DIR...
	// However, we will need to release the first block of the root directory
	// for use(createdirectory will expect this from us)
	printk("Releasing a block from FAT for rootdir on block #%d.\n", jtmfat_getrootdir(dnr));
	jtmfat_setblock(dnr, jtmfat_getrootdir(dnr), 0);
	//
	driver_TriggerCacheFlush();

	//
	return 0;
}

// Build information block for a specified drive
// on system boot block and return PTR to it.
JTMFAT_INFOBLOCK *BuildInfoBlock(int dnr, int bdrv)
{
	int i,i2,i3,i4,l,data1,v1,v2,
		tick,tick2,trigger,trigger2;
	BYTE *tmp,*p;
	char *s,*s2;
	int bytes_per_entry=4;
	BYTE *vali;
	JTMFAT_INFOBLOCK *inf;

	// BUILD INFO BLOCK, SPECIFY PARAMETERS
	// ------------------------------------

	//
	if(getsize(dnr)<10)
	{
		return NULL;
	}

	//
	inf = fatsys->bb[dnr];
	memset(inf, 0, sizeof(JTMFAT_INFOBLOCK));

	// Define boot area size in blocks.
	inf->bootblock_size = (FAT_BOOT_SECTION_LEN_K*1024)/getblocksize(dnr);

	// If we're building a system disk for hda, we also add
	// 512K (1024 sectors) kernel image space.
	if( (bdrv&0xF0)==0x80 )
	{
		// Reserve kernel image region.
		inf->bootblock_size += KERNEL_LEN_K*1024/getblocksize(dnr);

		// Specify kernel location.
		inf->kernel_offs = FAT_BOOT_SECTION_LEN_K*1024/getblocksize(dnr);
	}
	else
		// No kernel on this drive.
		inf->kernel_offs = 0;

	// FAT LENGTH (blocks)
	// *X because one entry takes X bytes.
	// Now we need to determine how many blocks the whole table for
	// N sectors will take in blocks, we will add one/or more to it,
	// so it will take at least the last possible half block space too.
	inf->fat_length =
		((getsize(dnr)*bytes_per_entry)
			/ getblocksize(dnr)) + 2;
					// 2 blocks for safety (minimum = 1)
	// Max. FAT entry value allowed.
	inf->max_val = getsize(dnr);

	// SYSTEM AREA LENGTH (blocks).
	inf->sysarea_length =
		inf->bootblock_size + inf->fat_length + 10;

	// FAT TABLE OFFSET (blocks)
	// FAT begins after the boot block(offset=block offset=block #).
	inf->fat_offs = inf->bootblock_size;

	// DATA AREA OFFSET (blocks)
	// Data area begins after the FAT area.
	// (IS THE ROOT DIRECTORY AREA! -OR- _OPTIONALLY_ SOMETHING ELSE)
	// data_offs = offset to the root directory.
	inf->data_offs =
		inf->fat_length + inf->bootblock_size + 9;
						// 9 is a fixed number, see +10 above

	// Store identifier.
	strcpy(inf->detstr, "JTMFS");

	//////////////////////////////////////////////////////////////////////////////////////////
	// Display values
/*	ReportMessage("[:- FAT creation: fat_length=%d, sysarea_length=%d,
		fat_offs=%d, data_offs=%d]\n",
		inf->fat_length,
		inf->sysarea_length,
		inf->fat_offs,
		inf->data_offs);*/

	//
	s = inf->detstr;
	ReportMessage("Detection string(detstr) = '%s'.\n",
	               s);

	// Error check
	if(
		inf->fat_length>getsize(dnr) ||
	        inf->sysarea_length>getsize(dnr) ||
	        inf->fat_offs>getsize(dnr) ||
	        inf->data_offs>getsize(dnr))
	{
		//
		printk("FATAL LOGIC FAILURE: FAT CREATION FAILED, INVALID PARAMETERS DETECTED!\n");
		return NULL;
	}

	//
	return inf;
}

// Set correct values on a boot record.
void BuildBootRecord(int dnr, BOOTRECORD *br, int bdrv)
{
	int i;

	// Store offset to "kernel image on HD".
	// Used by HD boot loader to locate kernel
	// on the hard disk.
	i = FAT_BOOT_SECTION_LEN_K*1024;
	br->boot_len_k = i;
	// Define drive CHS parameters
	// (we should make a call to hard disk driver
	//  to figure out the correct parameters here!)
	br->cylinders =	895;
	br->heads =	5;
	br->sectors =	55;
	// Store boot drive variable.
	br->bootdrv = bdrv;
	// Store boot sector signature.
	br->signature = 0xAA55;
}

// Determine which BIOS drive the device name refers to.
int DetermineBootdrv(const char *devname)
{
	int bd;

	// Determine BIOS boot drive.
	if(!strcmp(devname,"hda"))
		bd = 0x80;
	else
	if(!strcmp(devname,"hdb"))
		bd = 0x81;
	else
	if(!strcmp(devname,"hdc"))
		bd = 0x82;
	else
	if(!strcmp(devname,"hdd"))
		bd = 0x83;
	else
	if(!strcmp(devname,"floppy"))
		bd = 0;
	else
	if(!strcmp(devname,"floppy2"))
		bd = 1;
	else
	{
		// Default to floppy A: if unknown
		// (boot sector loader will be not used on
		//  this media anyway, probably!)
		bd = 0xFF;
		printk("Note: This is a non-BIOS drive, therefore it does not support system boot up feature.\n");
	}

	//
	return bd;
}

// Update information block on drive.
void WriteInformationBlock(int dnr, JTMFAT_INFOBLOCK *inf)
{
	BYTE *tmp,*p;
	int i,i2,i3,i4;

	//
	tmp = malloc(65536);

	// Read block.
	readblock(dnr, FAT_INFO_BLOCK_OFFS/getblocksize(dnr), tmp);
	//
	p=inf;
	// Copy info block data to tmp.
	i2 = FAT_INFO_BLOCK_OFFS % getblocksize(dnr);
	for(i=0; i<sizeof(JTMFAT_INFOBLOCK); i++)tmp[i+i2]=p[i];

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	printk("%s: writeblock(DNR=%d, %d, tmp=0x%x)\n",
		__FUNCTION__,
		dnr, 0, tmp);
	// Write info block data on disk(block #0).
	if( writeblock(dnr, FAT_INFO_BLOCK_OFFS/getblocksize(dnr), tmp) )
	{
		//
		printk("FAT creation failed: Could not write boot block.\n");
		return 1987;
	}
	else
	{
		printk("Boot block written succesfully.\n");
	}

	//
	free(tmp);
}

// Read information block from drive.
void ReadInformationBlock(int dnr, JTMFAT_INFOBLOCK *inf)
{
	BYTE *tmp,*p;
	int i,i2,i3,i4;

	//
	tmp = malloc(16384);

	// Read block.
	readblock(dnr, FAT_INFO_BLOCK_OFFS/getblocksize(dnr), tmp);
	//
	p=inf;
	// Copy info block data to tmp.
	i2 = FAT_INFO_BLOCK_OFFS % getblocksize(dnr);
	for(i=0; i<sizeof(JTMFAT_INFOBLOCK); i++)p[i] = tmp[i+i2];

	//
	free(tmp);
}

// Set kernel checksum value on master boot record.
void SetKernelChecksum(int dnr, WORD checksum)
{
	BYTE *tmp;
	BOOTRECORD *br;
	int i,i2,i3,i4;

	// Allocate temporary memory.
	tmp = malloc(1024*64);
	memset(tmp, 0, 1024*64);
	br = tmp;

	//
	readblock(dnr, 0, tmp);
	tmp[512-11] = checksum&255;
	tmp[512-10] = checksum>>8&255;
	writeblock(dnr, 0, tmp);

	//
	free(tmp);
}

// Install boot loader.
void InstallBootLoader(int dnr, int bdrv)
{
	BYTE *tmp,*p;
	int i,i2,i3,i4;

	// Allocate temporary memory.
	tmp = malloc(1024*64);
	memset(tmp, 0, 1024*64);

	// Write boot loader on disk.
	// --------------------------

	// It's parameters will be changed later on this code.
	memcpy(tmp, boot_bin, boot_bin_length);

	// Correct 16-bit kernel checksum.
	// (we don't know of any kernel yet, so we set it to zero).
	tmp[512-11] = 0;
	tmp[512-10] = 0;

	// Store offset to "kernel image on HD".
	// Used by HD boot loader to locate kernel
	// on the hard disk.
	i = FAT_BOOT_SECTION_LEN_K*1024;
	tmp[512-9] = i &255;
	tmp[512-8] = i >>8;
	// Define drive CHS parameters
	// (we should make a call to hard disk driver
	//  to figure out the correct parameters here!)
	tmp[512-7] =	895 &255;
	tmp[512-6] =	895 >>8;
	tmp[512-5] =	5;
	tmp[512-4] =	55;
	// Store boot drive variable.
	tmp[512-3] = bdrv;
	// Store boot sector signature.
	tmp[512-2] = 0x55;
	tmp[512-1] = 0xAA;

	// Write boot loader: Write block by block on disk.
	for(i=(boot_bin_length/getblocksize(dnr))+1,p=tmp,i2=0; i; p+=getblocksize(dnr),i2++,i--)
	{
		writeblock(dnr, i2, p);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// FREE TMP
	free(tmp); 
}

// Writes kernel image on specified drive.
void WriteKernel(int dnr, void *ker)
{
	int i,i2,l;
	void *p;

	// Write kernel on specified drive.
	l = (1024*512) / getblocksize(dnr);
	i2 = (FAT_BOOT_SECTION_LEN_K*1024) / getblocksize(dnr);
	ReportMessage("Writing kernel on system drive . . ");
	for(i=0,p=ker; i<l; i++,p+=getblocksize(dnr))
	{
		writeblock(dnr, i+i2, p);
	}
	ReportMessage("\n");
	ReportMessage("%d blocks written.\n",
		i);
}

// Determine 16-bit kernel checksum.
WORD DetermineKernelChecksum(BYTE *buf, int len)
{
	WORD sum;
	int i;

	//
	for(i=0,sum=0; i<len; i++)
		sum+=buf[i];
	return sum;
}

// Copy kernel from drive SRC to drive DST.
// (currently source drive can be only the 1.44M system boot disk!!)
int CopyKernel(const char *src, const char *dst)
{
	int dnr,sdnr,ad;
	int i,i2,i3,i4,l;
	void *p,*ker;

	// Get output drive.
	dnr = driver_getdevicenrbyname(dst);
	if(dnr<=0)
		return 1;

	// Get source drive.
	sdnr = driver_getdevicenrbyname(src);
	if(sdnr<=0)
		return 2;

	//------------------------------------------------------------------------
	//
	ad = (FAT_BOOT_SECTION_LEN_K*1024) / getblocksize(dnr);

	// Allocate temporary buffer for kernel image.
	ker = malloc(1024*512);
	memset(ker, 0, 1024*512);

	// Read kernel from source.
	l = (1024*512) / 512;
	ReportMessage("Reading 512K kernel from boot floppy . . ");
	for(i=0,p=ker; i<l; i++,p+=512)
	{
		if( readblock(sdnr, i+18, p) )
			break;
	}
	ReportMessage("\n");

	// Write kernel on drive.
	WriteKernel(dnr, ker);

	// Set new kernel checksum for boot loader.
	SetKernelChecksum(dnr, DetermineKernelChecksum(ker, 1024*512));

	// Free temporary space.
	free(ker);

	//
	return 0;
}



