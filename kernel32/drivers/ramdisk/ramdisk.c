/*
 * ramdisk.c
 * RAM DISK BLOCK DEVICE DRIVER.
 * (C) 2002-2008 by Jari Tuominen.
 *
 * Changed ram disk device name to "ram:", much simplier now.
 *
 * 1/2003: Added ram disk service
 * ------: Service adding frozen for now, will fix some other stuff first
 */

//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "ramdisk.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...

// Default ram disk which will be
// created upon initialization of the ram disk device.
RAMDISK_IMAGE default_ramdisk;
int ram_dev_mounted=1;

//
void rd_createdefaultramdisk(void)
{
	RAMDISK_IMAGE *i;

	//
	i=&default_ramdisk;

	//
	printk("Initializing ram disk device(%dK)\n",
		RD_DEFAULT_SIZE * RD_DEFAULT_BLOCK_SIZE/1024);

	// Create the device, requires
	// at least 16 Mb of RAM installed.
	if( GetRamAmount() >= (1024*1024*16) )
	{
		//
		rd_createimage(i, RD_DEFAULT_SIZE, RD_DEFAULT_BLOCK_SIZE);
		//
		printk("Ramdisk block device has been initialized.\n");
	}
	else
	{
		printk("This computer has a very low amount of total RAM available.\n");
		printk("Not enough memory for ram disk.\n");
	}
}


// Old stylish device call wrapper, direct access!
int rdDevCall(DCP)
{
	static SREQ ss;

	// Convert to SREQ structure
	ss.n_call = n_call;
	ss.p1 = p1;
	ss.p2 = p2;
	ss.p3 = p3;
	ss.p4 = p4;
	ss.po1 = po1;
	ss.po2 = po2;
	// Give out to handler
	return rdHandler(&ss);
}

// Note: To see what does 'DEVICE_CALL_PARS'(DCP) mean,
// check driver_api.h.
//
int rdHandler(SREQ *s)
{
	DWORD *size;
	int flags;

	// If unmounted, we won't answer anything until mounted again.
	if(s->n_call!=DEVICE_CMD_MOUNT && !ram_dev_mounted)
		return 0;

	// Store old state & disable interrupts
	flags = get_eflags();
	disable();

	//
	switch(s->n_call)
	{
                case DEVICE_CMD_MOUNT:
                ram_dev_mounted=1;
                break;
                case DEVICE_CMD_UNMOUNT:
                ram_dev_mounted=0;
                break;

		//
		case DEVICE_CMD_INIT:
		break;
		//
		case DEVICE_CMD_SHUTDOWN:
		break;
		//
		case DEVICE_CMD_READBLOCK:
		rd_readblock(&default_ramdisk, s->p1,s->po1);
		break;
		//
		case DEVICE_CMD_WRITEBLOCK:
		rd_writeblock(&default_ramdisk, s->p1,s->po1);
		break;
		//
		case DEVICE_CMD_GETSIZE:
		size = s->po1;
		*size = rd_getsize(&default_ramdisk);
		break;
		//
		case DEVICE_CMD_GETBLOCKSIZE:
		size = s->po1;
		*size = default_ramdisk.blocksize;
		break;
		//
		case DEVICE_CMD_SEEK:
		break;
		//
		case DEVICE_CMD_GETCH:
		break;
		//
		case DEVICE_CMD_PUTCH:
		break;
		//
		case DEVICE_CMD_TELL:
		break;
		//
		default:
		if(s->extension_wrapper_function!=NULL)
			((int(*)(void *))s->extension_wrapper_function)(s);
		break;
	}

	//
	set_eflags(flags);
	return 0;
}

// Registers device 'ramdisk'
int rd_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("ram") )
	{
		panic("Cannot register device ram: device already registered.");
	}

	// Register the device
	driver_register_new_device("ram", rdDevCall, DEVICE_TYPE_BLOCK);
	// Create alias ramdisk:
	driver_register_new_deviceEx( "ramdisk", rdDevCall,0,0,
		DEVICE_TYPE_BLOCK,
		driver_getdevicenrbyname("ram") );
 
	return 0;
}

//
int rd_init(void)
{
	//
	ramdisk_init();
}

//
int ramdisk_init(void)
{
	//
	rd_register_device();
	rd_createdefaultramdisk();
	return TRUE;
}

// rd_createimage
// imageptr, blocks, blocksize
void rd_createimage(RAMDISK_IMAGE *jim,
	DWORD blocks,DWORD blocksize)
{
	int i;

	//
	memset(jim, 0, sizeof(RAMDISK_IMAGE));

	// 
	jim->blocks=		blocks;
	jim->blocksize=	blocksize;

	//
	printk("ramdisk image creation: ");
	printk("blocks="); pd32(blocks); printk(", ");
	printk("blocksize="); pd32(blocksize); printk("\n");
 
	// allocate some memory for the disk image
//	jim->buf = malloc(blocks*blocksize);
	jim->buf = malloc(blocks*blocksize);
 
	// check if it worked
	if( illegal(jim->buf) )
	{
		FATALERROR("rd_createimage, memory error, out of memory?");
	}
	else
	{
		for(i=0; i<(blocks*blocksize); i++) { jim->buf[i]=0; }
	}
}

//
void rd_destroyimage(RAMDISK_IMAGE *jim)
{
	//
	if(jim->buf)
	{
		free( jim->buf );
	}
	else print("rd_destroyimage: null pointer reference detected");
}

// rd_readblock
// note: return value = amount of blocks read
int rd_readblock(RAMDISK_IMAGE *im,int which,unsigned char *buf)
{
	int i,offs;

	// 
	if(which < im->blocks)
	{
		//
		offs = which*im->blocksize;
		// routine check for a senseful blocksize
		if(im->blocksize>32767 || im->blocksize<512)
		{
			print("rd_readblock: non-sense blocksize detected, ");
			print("blocksize="); pd32(im->blocksize); print("\n");
			panic("cannot continue");
		}

		//
		for(i=0; i<im->blocksize; i++) { buf[i] = im->buf[i+offs]; }
		return 1;
	}
	else
	{
		print("rd_readblock: requested block # exceeds disk boundary, read error!\n");
		print("block=#"); pd32(which); print(" limit="); pd32(im->blocks); print("\n");
		panic("cannot continue");
	}
}

// Note: return value = amount of blocks wrote
int rd_writeblock(RAMDISK_IMAGE *im,int which,unsigned char *buf)
{
	int i,offs;

	// 
	if(which < im->blocks)
	{
		offs = which*im->blocksize;
		// routine check for a senseful blocksize
		if(im->blocksize>32767 || im->blocksize<512)
		{
			print("rd_writeblock: non-sense blocksize detected, ");
			print("blocksize="); pd32(im->blocksize); print("\n");
			getch();
		}
		//
		for(i=0; i<im->blocksize; i++) { im->buf[i+offs] = buf[i]; }
		return 1;
	}
	else
	{
		print("rd_writeblock: requested block # exceeds disk boundary, write error!\n");
		print("block=#"); pd32(which); print(" limit="); pd32(im->blocks); print("\n");
		return 0;
	}
}

// Returns size of the ramdisk in blocks
int rd_getsize(RAMDISK_IMAGE *im)
{
	char str[256];

	//
	if(im->blocks >= (1024*512))
	{
		sprintf(str, "RAM disk has weird amount of blocks !! (%d)", im->blocks);
		panic(str);
	}
	return im->blocks;
}

// Returns size of a single block unit(in bytes)
int rd_getblocksize(RAMDISK_IMAGE *im)
{
	//
	return im->blocksize;
}

