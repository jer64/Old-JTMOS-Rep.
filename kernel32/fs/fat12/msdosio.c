// MS-DOS disk format access implementation.
// Currently only supports the FAT12 read-only.
#include "kernel32.h"
#include "msdosmetadata.h"
#include "filedes.h" // kernel32/fs/filedes...

//
/***

FAT12 BOOT SECTOR

     Offset  Size		Description

	00   3bytes	jump to executable code
	03   8bytes	OEM name and version
	0B   word	bytes per sector
	0D   byte	sectors per cluster (allocation unit size)
	0E   word	number of reserved sectors (starting at 0)
	10   byte	number of FAT's on disk
	11   word	number of root directory entries (directory size)
	13   word	number of total sectors (0 if partition > 32Mb)
	15   byte	media descriptor byte  (see MEDIA DESCRIPTOR)
	16   word	sectors per FAT
	18   word	sectors per track  (DOS 3.0+)
	1A   word	number of heads  (DOS 3.0+)
	1C   word	number of hidden sectors  (DOS 3.0+)
	20   dword	(DOS 4+) number of sectors if offset 13 was 0
	24   byte	(DOS 4+) physical drive number
	25   byte	(DOS 4+) reserved
	26   byte	(DOS 4+) signature byte (29h)
	27   dword	(DOS 4+) volume serial number
	2B  11bytes	(DOS 4+) volume label
	36   8bytes	(DOS 4+) reserved**/

// MSDOSIO for each device.
static MSDOSIO *msdosio[10000];
static int n_msdosio = 10000;

//
static int allocatestructure(FILEDES *f)
{
	//
	if(f->dnr < 0 || f->dnr > 9999 )
	{
		return 1;
	}
	msdosio[f->dnr] = malloc(sizeof(MSDOSIO));
	memset(msdosio[f->dnr], 0, sizeof(MSDOSIO));
	return 0;
}

//
static int mounted(FILEDES *f)
{
	//
	return msdosio[f->dnr]->mounted;
}

//
static int load_buffer(FILEDES *f)
{
	//
	msdosio[f->dnr]->bufsiz = 1024*1440;
	msdosio[f->dnr]->buf = malloc(msdosio[f->dnr]->bufsiz);
	readblocks(f->dnr,0,msdosio[f->dnr]->buf, 2880);
	return 0;
}

//
static int msdos_convertfat12todwordarray(FILEDES *f)
{
	int i;

	// Root directory at 0x1400 by default.
	// FAT12 at 0x1400 by default.

	// Convert from 12-bit buffer to 32-bit buffer.
	msdosio[f->dnr]->l_fat = 1440; // applying 1K blocks
	// Allocate FAT.
	msdosio[f->dnr]->fat =
		malloc(msdosio[f->dnr]->l_fat*sizeof(DWORD));
	// Conversion code:
	for(i=0; i<msdosio[f->dnr]->l_fat; )
	{
		msdosio[f->dnr]->fat[i++] =
			msdosio[f->dnr]->buf[i+0]
		| (msdosio[f->dnr]->buf[i+1]&0x0F<<8);
		msdosio[f->dnr]->fat[i++] =
			(msdosio[f->dnr]->buf[i+1]&0xF0>>4)
		| (msdosio[f->dnr]->buf[i+2]<<4);
	}

	//
	return 0;
}

//
int _msdos_readdir(int fd, char *buf, int count)
{
	FILEDES *f;
	int i,i2;

        //
        f = get_filedesptr(fd);
        if(f==NULL)
        {
                printk("%s/%s: Invalid file descriptor '%d'.\n",
			__FUNCTION__, __FILE__, fd);
                return -1;
        }

	//
	for(i=0x2600,i2=0; ; i++)
	{
		buf[i2++] = msdosio[f->dnr]->buf[i];
		if(!msdosio[f->dnr]->buf[i]) break;
	}
	return i2;
}

// FILE INPUT
int _msdos_read(int fd, const char *fname,
        char *buf, int count, int offset)
{
	FILEDES *f;

        //
        f = get_filedesptr(fd);
        if(f==NULL)
        {
                printk("%s/%s: Invalid file descriptor '%d'.\n",
			__FUNCTION__, __FILE__, fd);
                return -1;
        }

	//
}

//
int _msdos_write(int fd, const char *fname,
        char *buf, int count, int offset)
{
	FILEDES *f;

        //
        f = get_filedesptr(fd);
        if(f==NULL)
        {
                printk("%s/%s: Invalid file descriptor '%d'.\n",
			__FUNCTION__, __FILE__, fd);
                return -1;
        }

	//
}

//
int msdos_loadbootsector(FILEDES *f)
{
	msdosio[f->dnr]->l_bootsector = 512;
	readblock(f->dnr, 0, msdosio[f->dnr]->bootsector);
}

// Called after open function call.
int msdos_mount_drive(int fd)
{
	FILEDES *f;

        //
        f = get_filedesptr(fd);
        if(f==NULL)
        {
                printk("%s/%s: Invalid file descriptor '%d'.\n",
			__FUNCTION__, __FILE__, fd);
                return -1;
        }

	//
	if(mounted(f)) {
		return 1;
	}

	//
	printk("%s/%s: loading boot sector\n",
		__FUNCTION__, __FILE__);
	msdos_loadbootsector(f);
	printk("%s/%s: done loading boot sector\n",
		__FUNCTION__, __FILE__);

	// Load disk image.
	printk("%s/%s: Buffering 1440K of data from a device\n",
		__FUNCTION__, __FILE__);
	load_buffer(f);
	printk("%s/%s: Buffering completed.\n",
		__FUNCTION__, __FILE__);
	// Init fat.
	// For easier reading put in 32-bit at f->fat.
	msdos_convertfat12todwordarray(f);
	//
	f->db = msdos_scanrootblock(f->dnr);
}

// TODO: not yet implemented
static int flush_buffer(FILEDES *f)
{
	//...
}

//
int msdos_unmount_drive(FILEDES *f)
{
	//
	flush_buffer(f);
}

//
int msdos_scanrootblock(FILEDES *f)
{
	// By default root block is fixed of position on a 1.44 Mb FAT12 disk floppy.
	// Move the head to root directory.
	return f->db = 0x2600/getblocksize(f->dnr);
}

//
int _msdos_open(FILEDES *f, const char *name, int flags)
{
        // At the moment we support only FAT12 and
        // 1.44 Mb devices.
	if(flags&O_DIRECTORY)
	{
	        if(!mounted(f))
	        {
	                msdos_mount_drive(f);
	        }
	}
	else
	{
	        if(!mounted(f))
	        {
	                msdos_mount_drive(f);
	        }
	}

	//
}

//
int _msdos_close(FILEDES *f)
{
	//

}

//

