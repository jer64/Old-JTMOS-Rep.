#ifndef __MALLOC_H__
#define __MALLOC_H__

//
#include "basdef.h"
#include "calloc.h"

//
#define malloc1		malloc
#define free1 		free

//
typedef struct
{
	// Start of malloc memory, length of memory allocation region.
	DWORD start,length;
}MALLOC;
MALLOC MALSTRU;

// 6 bytes,  3 DWORDs
typedef struct
{
	DWORD next_chunk;
	DWORD last_chunk;
	DWORD size;
	BYTE isfree;
}MCHUNK;

//
typedef struct
{
	// Where we are currently
	DWORD where;
	// End of hunt? (FALSE/TRUE)
	BYTE end;
 
	//
	MCHUNK chunk;
}HUNT;

//
void malloc_meminfo(void);
void malloc_store_info(MCHUNK *, DWORD *,DWORD *,DWORD,BYTE);
void malloc_init(DWORD,DWORD);
void malloc_create_free_chunk(MCHUNK *);
void malloc_create_hunt(HUNT *);
void malloc_hunt(HUNT *);
BYTE malloc_hunt_jump(HUNT *);
void malloc_reuse_chunk(void *,long);
void *malloc(DWORD);
void free(void *);
DWORD malloc_getsize(void *);
BYTE malloc_isfree(void *);
DWORD malloc_nextchunk(void *);
DWORD malloc_lastchunk(void *);
DWORD malloc_getamountfree(void);

#endif
