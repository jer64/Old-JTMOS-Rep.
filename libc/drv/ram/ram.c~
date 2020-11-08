/*
 * RAMDISK BLOCK DEVICE DRIVER
 * (C) 2002-2003 by Jari Tuominen
 *
 * Changed ramdisk device name to "ram:", much simplier now.
 *
 * 1/2003: Added ramdisk service
 * ------: Service adding frozen for now, will fix some other stuff first
 */

//
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "ram.h"
#include "ramService.h"

// Request 2048K of RAM from the init process.
SYSMEMREQ(APPRAM_2048K)

// Default ramdisk which will be
// created upon initialization of the ramdisk device.
RAMDISK_IMAGE default_ramdisk;
int ram_dev_mounted=1;

//
static int curpid=0;
static DCPACKET *curdc;

//
void rd_createdefaultramdisk(void)
{
	RAMDISK_IMAGE *i;

	//
	i=&default_ramdisk;

	//
	printf("Initializing ramdisk device\n");

	// Create the device
	rd_createimage(i, RD_DEFAULT_SIZE, RD_DEFAULT_BLOCK_SIZE);
		// 2880 blocks, blocksize is 512 bytes

	//
	printf("Ramdisk block device has been initialized.\n");
}


// Old stylish device call wrapper, direct access!
int rdDevCall(DCPACKET *dc,
	int clientPID, int n_call,
	int p1,int p2,int p3,int p4,
	void *po1,void *po2)
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
	return rdHandler(dc,clientPID, &ss);
}

// Note: To see what does 'DEVICE_CALL_PARS'(DCP) mean,
// check devapi.h.
//
int rdHandler(DCPACKET *dc,int clientPID,
	SREQ *s)
{
	DWORD *size;
	int flags;
	int sz;

	//
	curdc=dc;
	curpid=clientPID;

	// If unmounted, we won't answer anything until mounted again.
	if(s->n_call!=DEVICE_CMD_MOUNT && !ram_dev_mounted)
		return 0;

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
		sz = rd_getsize(&default_ramdisk);
                globalcopy(GetCurrentThread(),&sz,
                        curpid,size,
                        4);
		break;
		//
		case DEVICE_CMD_GETBLOCKSIZE:
		size = s->po1;
		sz = default_ramdisk.blocksize;
                globalcopy(GetCurrentThread(),&sz,
                        curpid,size,
                        4);
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
		break;
	}

	//
	return 0;
}

// Registers device 'ramdisk'
int rd_register_device(void)
{
	// Register the device
	registerNewDevice(RAM_DEV_NAME,
		NULL,0,
		GetCurrentThread(), DEVICE_TYPE_NONKERNELDRV,
		0);
	return 0;
}

//
int rd_init(void)
{
	//
	rd_register_device();
	rd_createdefaultramdisk();
	return 0;
}

// rd_createimage
// imageptr, blocks, blocksize
void rd_createimage(RAMDISK_IMAGE *jim,
	DWORD blocks,DWORD blocksize)
{
	int i;

	// 
	jim->blocks=		blocks;
	jim->blocksize=	blocksize;

	//
	printf("ramdisk image creation: ");
	printf("blocks="); pd32(blocks); printf(", ");
	printf("blocksize="); pd32(blocksize); printf("\n");
 
	// allocate some memory for the disk image
	jim->buf = malloc(blocks*blocksize);
 
	// check if it worked
	if(!jim->buf)
	{
		printf("rd_createimage, memory error, out of memory?");
		abort();
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
	else printf("rd_destroyimage: null pointer reference detected");
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
			printf("rd_readblock: non-sense blocksize detected, ");
			printf("blocksize="); pd32(im->blocksize); printf("\n");
			panic("cannot continue");
		}

                // Copy chunk to client's memory space
                globalcopy(GetCurrentThread(),im->buf+offs,
                        curpid,buf,
                        im->blocksize);
		//
		//for(i=0; i<im->blocksize; i++) { buf[i] = im->buf[i+offs]; }
		return 1;
	}
	else
	{
		printf("rd_readblock: requested block # exceeds disk boundary, read error!\n");
		printf("block=#"); pd32(which); printf(" limit="); pd32(im->blocks); printf("\n");
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
			printf("rd_writeblock: non-sense blocksize detected, ");
			printf("blocksize="); pd32(im->blocksize); printf("\n");
			getch();
		}
		//
                // Copy chunk to client's memory space
                globalcopy(curpid,buf,
			GetCurrentThread(),im->buf+offs,
                        im->blocksize);
	//	for(i=0; i<im->blocksize; i++) { im->buf[i+offs] = buf[i]; }
		return 1;
	}
	else
	{
		//
		printf("rd_writeblock: requested block # exceeds disk boundary, write error!\n");
		printf("block=#"); pd32(which); printf(" limit="); pd32(im->blocks); printf("\n");
		return 0;
	}
}

// Returns size of the ramdisk in blocks
int rd_getsize(RAMDISK_IMAGE *im)
{
	//
	return im->blocks;
}

// Returns size of a single block unit(in bytes)
int rd_getblocksize(RAMDISK_IMAGE *im)
{
	//
	return im->blocksize;
}

