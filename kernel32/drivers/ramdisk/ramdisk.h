#ifndef __INCLUDED_RAMDISK_H__
#define __INCLUDED_RAMDISK_H__

//
#include "kernel32.h"

//
extern int ramdisk_init(void);

// Shall we debug?
#define __RAMDISK_DEBUG__

/* 
 * Ramdisk Disk Image - A Concrete Part of The Block Device called "ramdisk".
 *
 * In this case, what is in the diskimage usually?
 * A disk image is a concept of FAT and a data area.
 * The data area contains the file datas, and a directory structure.
 * FAT contains the information which tells how those above were allocated,
 * so fat is above dirs and files. It must be obeyed, in order to keep up
 * a healthy disk system, that services it's purpose.
 */
typedef struct
{
	// Amount of blocks used for ramdisk?
	DWORD	blocks;
	// Block unit's size
	DWORD	blocksize;
	// Buffer where all ramdisk data is stored
	BYTE		*buf;
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
int rdDevCall(DCP);

//
extern RAMDISK_IMAGE default_ramdisk;

//
#include "ramser.h"

#endif







