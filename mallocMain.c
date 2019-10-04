/*
 * =====================================================================
 *   mallocMain.c - malloc main.
 *   (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
 * =====================================================================
 */
     
//--------------------------------------------------------------------------------
     
// Are we ready for optimizing malloc?
#define MALLOC_OPTIMIZE_POSITIVE
//
#include <stdio.h>
#include "basdef.h"
#include "malloc.h"

//
void free(void *p)
{
	//
	return scall(SYS_kfree,  p,0,0, 0,0,0);
}

//
void *malloc(DWORD l)
{
	//
	return scall(SYS_kmalloc,  l,0,0, 0,0,0);
}
   
/*
 * MALLOC
 *
 * What it does :
 *  1) Validates the requested amount of memory,
 *     if it is illegal, return NULL straight away
 *     to conserve CPU usage.
 *  2) Begins a search through all memory chunks,
 *     searchs for empty chunk
 *  3) If the allocated chunk was the last chunk,
 *     then make sure that an empty chunk is left behind,
 *     
 */
void *__malloc(DWORD l)
{
	//
	HUNT hunt;
	void *p;

#ifdef DEBUG_MALLOC
	//
	printf("\nmalloc called: ");
	printf("length of requested allocation = "); pd32(l); printf("\n"); 
	waitkey();
#endif
 

	// Quick check for validity.
	// Explanation: if total amount of memory is less than
	//              the requested amount of memory, then return NULL immediatly.
	if( l >= (MALSTRU.length-256) )
	{
		printf("malloc: warning: malloc received request of %d bytes which is too much.\n",
			l);
		printf("malloc: total memory available = %d bytes\n", MALSTRU.length);
		return NULL;
	}
  
	// This loop searches for a suitable free memory chunk.
	for( malloc_create_hunt(&hunt); ; )
	{
		// Run a single step
		malloc_hunt(&hunt);

#ifdef DEBUG_MALLOC  
		//
		printf("malloc_hunt:\n");
		printf("location = 0x"); p32(hunt.where); printf("\n");
		printf("hunt.chunk.next_chunk = 0x"); p32(hunt.chunk.next_chunk); printf("\n");
		printf("hunt.chunk.last_chunk = 0x"); p32(hunt.chunk.last_chunk); printf("\n");
		printf("hunt.chunk.isfree = "); pd32(hunt.chunk.isfree); printf("\n");
		printf("hunt.chunk.size = "); pd32(hunt.chunk.size); printf("\n");
		waitkey();
#endif

		// Check size of the current chunk
		// (The extra 100 bytes is used as our warranty
		// to keep the malloc system working properly)
		if( hunt.chunk.isfree==TRUE  &&  (hunt.chunk.size==l || hunt.chunk.size>(l+(sizeof(MCHUNK)*2)))  )
		{
			// Aha, so the chunk is free, lets (re)use it!
			p = (DWORD)hunt.where;
			malloc_reuse_chunk( p, l );
			break;
		}
  
		// End of hunt reached without any success? -> Return NULL
		if( malloc_hunt_jump(&hunt)==TRUE )
		{
			// TODO:
			///FATALERROR("out of memory");
		}
	}
 
	// Return correct address(we did have success)
	p = (DWORD) (hunt.where + sizeof(MCHUNK));
	// Clear the allocated area(this is a very kind thing to do, indeed)
#ifndef __MALLOC_DOESNT_CLEAR_ARRAYS__
	memset(p,0,l);
#endif
	// Finally return the pointer
	return p;
}



//
void __free(void *p)
{
	//
	MCHUNK *m;
 
	// Get pointer to the chunk (its sizeof(MCHUNK) bytes minus offset from
	//                           the memory location of allocated data)
	m=(((DWORD)p) - sizeof(MCHUNK));

	// Mark as free.
	m->isfree = TRUE;

#ifdef MALLOC_OPTIMIZE_POSITIVE
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// call memory optimizer -- this will eventually give us more memory, if the memory is fr
	malloc_optimize();
#endif
}

