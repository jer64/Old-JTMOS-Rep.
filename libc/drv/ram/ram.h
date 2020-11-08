#ifndef __INCLUDED_RAMDISK_H__
#define __INCLUDED_RAMDISK_H__

//
#define RAM_DEV_NAME		"rx"

//
#define RD_DEFAULT_SIZE		1750
#define RD_DEFAULT_BLOCK_SIZE	1024

//
#include <stdio.h>

//
extern int rd_init(void);

// Shall we debug?
#define __RAMDISK_DEBUG__

//
typedef struct
{
	// Amount of blocks used for ramdisk?
	DWORD	blocks;
	// Block unit's size
	DWORD	blocksize;
	// Buffer where all ramdisk data is stored
	BYTE	*buf;
}RAMDISK_IMAGE;

//
extern int rd_getsize(RAMDISK_IMAGE *);
extern int rd_getblocksfree(RAMDISK_IMAGE *);
extern void rd_formatimage(RAMDISK_IMAGE *);
extern void rd_createimage(RAMDISK_IMAGE *jim,
	DWORD blocks,DWORD blocksize);
extern void rd_destroyimage(RAMDISK_IMAGE *jim);
extern int rd_readblock(RAMDISK_IMAGE *im,int which,BYTE *buf);
extern int rd_writeblock(RAMDISK_IMAGE *im,int which,BYTE *buf);
extern void rd_createdefaultramdisk(void);
int rdDevCall(DCPACKET *dc,
        int clientPID, int n_call,
        int p1,int p2,int p3,int p4,
        void *po1,void *po2);

//
extern RAMDISK_IMAGE default_ramdisk;

#endif







