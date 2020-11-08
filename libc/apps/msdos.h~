#ifndef __INCLUDED_MSDOS_H__
#define __INCLUDED_MSDOS_H__

//
#define FALSE	0
#define TRUE	1
#ifndef BYTE
#define BYTE unsigned char
#endif

//
typedef struct
{
	// Jump instruction.
	BYTE JMP[3];

	// OEM name and version.
	BYTE oem[8];

	// Bytes per sector.
	WORD bytesPerSector;

	// Sectors per cluster.
	BYTE sectorsPerCluster;

	// Number of reserved sectors.
	WORD reservedSectors;

	// Number of FAT tables on drive.
	BYTE numberOfFATs;

	// Number of root directory entries.
	WORD numberOfRootDirEntries;

	// Number of total sectors.
	WORD totalSectors;

	// Media descriptor byte.
	// (0xF0 for 1.44M floppy)
	BYTE mediaDescriptor;

	// Number of sectors per FAT.
	// (FAT length)
	WORD sectorsPerFAT;

	// Sectors per track.
	WORD sectorsPerTrack;

	// Number of heads.
	WORD numberOfHeads;

	// Number of hidden sectors.
	WORD numberOfHiddenSectors;

	// Total number of sectors.
	// (for size > 32Mb)
	DWORD totalSectors;

	// Physical drive number.
	BYTE physicalDriveNumber;

	// Reserved byte.
	BYTE reserved;

	// Extended boot signature.
	// (0x29)
	BYTE extendedBootSignature;

	// Binary volume ID.
	DWORD volumeID;

	// Volume label.
	char label[11];

	// Reserved.
	BYTE reserved2[8];

	// Bootstrap loader code.
	BYTE bootstrap[0];
}DOSBOOTSECTOR;

// MSDOS FAT FS 8+3 File Entry
// Modified from: http://www.cs.rochester.edu/u/gchunt/vfat.html
typedef struct
{
	// short 8.3 names:
	char name[8];		// file name
	char ext[3];		// file extension
	BYTE attr;		// attribute byte
	BYTE lcase;		// Case for base and extension
	BYTE ctime_ms;		// Creation time, milliseconds
	WORD ctime;		// Creation time
	WORD cdate;		// Creation date
	WORD adate;		// Last access date
	BYTE reserved[2];	// reserved values (ignored)
	WORD time;		// time stamp
	WORD date;		// date stamp
	WORD start;		// starting cluster number
	DWORD size;		// size of the file
}FFE; // FAT File Entry (FFE)
  
#endif


