#ifndef __INCLUDED_MM_H__
#define __INCLUDED_MM_H__

//
#define MAPAD_MM_DB		0xF2000000

//
#include <stdio.h>

// To debug or not to debug
//#define MMPRINT		printk
#define MMPRINT		//

// Page size for this system
#define PAGESIZE	4096

// The bigger allocation database,
// the more entries it can have.
// There's currently no limitation,
// so allocating too many entries will
// lead into instability, TODO,
// easy to fix.
// The database size must be atleast
// size of two megabytes, this is
// because of the memory bitmap,
// that virtually maps four gigabytes
// of RAM in 4 KB units.
#define ALLOCATION_DATABASE_SIZE	1024*1024*1

// PTRs
extern char *MSTART,*MEND,*REND;

// Memory Descriptor(ME)
typedef struct
{
	// INFO REGION
	// PTR to the beginning of the region
	char *p;
	// Process ID of the process which owns this memory region
	int PID;
	// Start page for the region
	int page;
	// Length of the region in pages
	int pages;
	// Fixed length of the region in bytes
	int bytes;
	// 1=Region is free, 0=Region is reserved
	int isFree;

	// Address where the memory is mapped.
	DWORD mapad;

	// DEBUG REGION
	char name[20];	// name of the function which called kmalloc
}MD;

//
typedef struct
{
	// PTRs to the descriptors of the allocated memory blocks
	MD **allocated;
	// How many blocks currently allocated in the system?
	int n_allocated;
	// Max. entries
	int max_allocated;
	// Free page list, a single bit = 4K page
	BYTE *bitmap;
	int l_bitmap;
}MM;
extern MM *p_mm;

//
extern int mallocOK;

// Function prototypes
void mmInit(void *region, int length);

//
extern int mm_debug;

//
#include "mmBit.h"
#include "mmEntry.h"
#include "paging.h"

#endif


