/*
 * =====================================================================
 *   malloc.c - JTMOS LIBC DYNAMICAL MEMORY ALLOCATION
 *   (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
 * =====================================================================
 */

//--------------------------------------------------------------------------------

// Are we ready for optimizing malloc?
#define MALLOC_OPTIMIZE_POSITIVE
//
#include "basdef.h"
#include "malloc.h"

// Are we debugging this library ?
///#define DEBUG_MALLOC

//
void malloc_store_info(MCHUNK *i,
			 DWORD *next_chunk,
			 DWORD *last_chunk,
			 DWORD size,
			 BYTE isfree)
{
	//
	i->next_chunk = next_chunk;
	i->last_chunk = last_chunk;
	i->size       = size;
	i->isfree     = isfree;
}

//-------------------------------------------------------------------------------
// size,length
// Note: FIRST OF ALL, BEFORE USING ANY OF OTHER FUNCTIONS, RUN THIS FIRST!
void malloc_init(DWORD s,DWORD l)
{
	//
	MCHUNK *p;

	// I:  setup initial rules, beginning of allocable memory and length of it
	MALSTRU.start= s;
	MALSTRU.length=l;

	// II: copy empty info block at the beginning
	p = (DWORD)MALSTRU.start;
	malloc_store_info( p,  NULL,NULL,MALSTRU.length-sizeof(MCHUNK),TRUE );
}

//
void malloc_create_free_chunk(MCHUNK *chunk)
{
	//
	chunk->next_chunk = 0;
	chunk->last_chunk = 0;
	chunk->size       = 0;
	chunk->isfree     = TRUE; 
}

//
void malloc_create_hunt(HUNT *h)
{
	// Give initial values for a hunt
	h->where            = MALSTRU.start;
	h->end              = FALSE;
	// ..
	malloc_create_free_chunk(  &h->chunk  );
}

// Gets the MCHUNK chunk from the "where" location
void malloc_hunt(HUNT *h)
{
	//
	DWORD i,i2,ad,ad2,ad3,ad4,ad5,ad6;
	DWORD *p;
	MCHUNK *m;
 
	// Lets get the chunk
	m = h->where;

	// Get the chunk from memory location at 'where'( referred by (MCHUNK *)m )
	h->chunk.next_chunk = m->next_chunk;
	h->chunk.last_chunk = m->last_chunk;
	h->chunk.size       = m->size;
	h->chunk.isfree     = m->isfree;
 
	//
	if(!h->chunk.next_chunk) h->end=TRUE;
}

/*
 * Jumps to next chunk in the memory,
 * and gets the MCHUNK chunk
 */
BYTE malloc_hunt_jump(HUNT *h)
{
 //
 DWORD i,i2,ad,ad2,ad3,ad4,ad5,ad6;
 DWORD *p;
 MCHUNK *m;
  
 // The last chunk? (next_chunk must be zero)
 if( !h->chunk.next_chunk )
 {
  //
  h->end=TRUE;
  return TRUE;
 }
 else
 {
  //
  h->end=FALSE;
  // Lets get next chunk
  m = h->chunk.next_chunk; 
 
  // Change the position
  h->where = h->chunk.next_chunk; 

  // Copy it to the present
  h->chunk.next_chunk = m->next_chunk;
  h->chunk.last_chunk = m->last_chunk;
  h->chunk.size       = m->size;
  h->chunk.isfree     = m->isfree;
  
  //
  return FALSE;
 }
}

/*
 * This function makes a new chunk on a place
 * where is an unused chunk with size of atleast
 * same as l or larger.
 * If size of the present chunk is larger
 */
void malloc_reuse_chunk(void *p,long l)
{
 //
 MCHUNK *m,*m2,*mz; // mz=temp. chunk
 DWORD original_size,chunk_2nd_size,i,i2;
 
#ifdef DEBUG_MALLOC
 //
 printf("\nmalloc_reuse_chunk called: ");
 printf("p = 0x"); p32((DWORD)p); printf(",");
 printf("l = 0x"); p32((DWORD)l); printf("\n");
 waitkey();
#endif
 
 
 // duplicate the pointer, but with MCHUNK -type
 m=p;
  
 // Get original chunk size 
 original_size = m->size; 
 
 // Resize chunk to new size
 m->size = l;
 
 // Mark chunk as reserved
 m->isfree = FALSE;
 
 
 // The new chunk isn't same size as the previous?
 if(  original_size > l  )
 {
#ifdef DEBUG_MALLOC
  //
  printf("* match: original_size > l\n");
  waitkey();
#endif
   
  // determine the amount of free memory left for CHUNK-II's data
  chunk_2nd_size = (original_size - l) - sizeof(MCHUNK);
  
  // Just to make sure...
  if( chunk_2nd_size < (sizeof(MCHUNK)+1) )
	{ 
		// TODO: a fatal error handler
		//FATALERROR("malloc_reuse_chunk() error #1");
	}
  
  // create MCHUNK pointer at the secondary memory chunk thats being created
  m2 = ((DWORD)  ( ((DWORD)m) + (l + sizeof(MCHUNK)) ));
  
#ifdef DEBUG_MALLOC
  //
  printf("creating mchunk-II at 0x"); p32( (DWORD)m2 ); printf("\n");
  waitkey();
#endif

  // Setup MCHUNK-II
  m2->next_chunk = m->next_chunk;
  m2->size = chunk_2nd_size;
  // it'll be a free chunk(non-allocated)
  // mark it as free
  m2->isfree = TRUE;
  
  // Fix MCHUNK-I's next_chunk to point at MCHUNK-II(not at MCHUNK-xx)
  m->next_chunk = m2;

  /*
   * void malloc_store_info(MCHUNK *i,
   *			 DWORD *next_chunk,
   *			 DWORD *last_chunk,
   *			 DWORD size,
   *			 BYTE isfree)
   */
 
  // Last chunk? -> Create empty chunk after it then.
  if( !m2->next_chunk )
  {
   // Create empty chunk after Chunk-II(status=free,size=amount of memory left)
   mz = (((DWORD) m2)+sizeof(MCHUNK)+m2->size);
   i = MALSTRU.length - (((DWORD)mz) - ((DWORD)MALSTRU.start)); // get remaining memory
   if(i<0)
	{
		//FATALERROR("malloc_reuse_chunk failure -- out of memory -- memory range exceeded(I)");
	}

   //
   malloc_store_info( mz, NULL,NULL, i, TRUE );
  }
 }
 else
 {
  /* We didn't create a secondary chunk to satisfy the allocation.
   * If this chunk was the last chunk(next_chunk==NULL), we will create an empty
   * chunk after it.
   */
#ifdef DEBUG_MALLOC
  printf("match: 'else'\n");
#endif
  // Last chunk? -> Create empty chunk after it then
  // (which will eventually tell the amount of the memory that is remaining).
  m=p;
  //
  if( !m->next_chunk )
  {
   // Create empty chunk after Chunk-II(status=free,size=amount of memory left)
   mz = ((DWORD) m) + (sizeof(MCHUNK) + m->size);
   i = MALSTRU.length - (((DWORD)mz) - ((DWORD)MALSTRU.start)); // get remaining memory
   if(i<0)
   { 
	// TODO :
	//FATALERROR("malloc_reuse_chunk failure -- out of memory -- memory range exceeded(II)");
   }
   malloc_store_info( mz, NULL,NULL, i, TRUE );
  }
 }
 
}

//
DWORD malloc_getsize(void *p)
{
 //
 MCHUNK *m;
 
 //
 m=(p-sizeof(MCHUNK));
 //
 return m->size;
}

//
BYTE malloc_isfree(void *p)
{
 //
 MCHUNK *m;
 
 //
 m=((void *) (p-sizeof(MCHUNK)));
 //
 return m->isfree;
}

//
DWORD malloc_nextchunk(void *p)
{
 //
 MCHUNK *m;
 
 //
 m=((void *) (p-sizeof(MCHUNK)));
 //
 return m->next_chunk;
}

//
DWORD malloc_lastchunk(void *p)
{
 //
 MCHUNK *m;
 
 //
 m=((void *) (p-sizeof(MCHUNK)));
 //
 return m->last_chunk;
}

//
DWORD malloc_getamountfree(void)
{
 //
 HUNT h;
 long i,i2,i3,i4,allocated,avail,largest;

 //
 for( largest=0,i=0,malloc_create_hunt(&h); ; i++)
 {
  //
  malloc_hunt(&h);

  //
  if( h.chunk.isfree == TRUE )
  {
   //
   avail += h.chunk.size;
  }
  else
  {
   //
   allocated += h.chunk.size;
  }

  // Get the largest one
  if( h.chunk.size>largest )
  {
   largest=h.chunk.size;
  }

  //
  malloc_hunt_jump(&h);
  //
  if(h.end==TRUE)break;
 }

 // avail, allocated, largest
 //
 return largest;
}

//
void malloc_dumpchunkinfo(void)
{
 //
 HUNT h;
 long i,i2,i3,i4;
 
 //
 printf("MALLOC REPORT - memory chunk information\n");
 
 //
 for( i=0,malloc_create_hunt(&h); ; i++)
 {
  //
  malloc_hunt(&h);
  
  //
  if( h.chunk.isfree == TRUE )
  {
   //
   printf("free      ");
  }
  else
  {
   //
   printf("allocated ");
  }

  // 'chunk #n: '
  printf("chunk #"); pd32(i); printf(": ");
  // 'n bytes at 0xNnNnNnNn '
  pd32(h.chunk.size); printf(" bytes at 0x"); p32(h.where);
  // '(n/n)\n'
  printf("("); p32(h.chunk.next_chunk); printf("/"); p32(h.chunk.last_chunk); printf(")\n");
  
  //
  malloc_hunt_jump(&h);
  //
  if(h.end==TRUE)break;
 }
 
}

//
void malloc_optimize(void)
{
 //
 MCHUNK *m;
 HUNT h;
 DWORD i,i2,start,end,length;
 BYTE suggest_new_round;
 
 //
 for( ; ; )
 {
  //
  for( suggest_new_round=FALSE,i=0,length=0,malloc_create_hunt(&h); ; )
  {
   //
   malloc_hunt(&h);
   
   //
   if(h.end==FALSE && h.chunk.isfree==TRUE)
   {
    if(!i) start = h.where;
    i++;
   }
   else
   {
    //
    if(h.end==TRUE && h.chunk.isfree==TRUE) { i++; }
     
    //
    end = h.where;
    
    // Can we merge those mchunks? (if atleast 2 chunks!)
    if(i>=2)
    {
     /* create one united chunk, which will have
      * got space of all those previously seperated
      * memory chunks.
      */
     m=start;
     
     //
     if(h.end==TRUE)
     {
      //
      end=h.where + (h.chunk.size+sizeof(MCHUNK));
      //
      m->next_chunk = 0;
      m->size = (end - start) - sizeof(MCHUNK);
      break;
     }
     else
     {
      m->next_chunk = end;
      m->size = (end - start) - sizeof(MCHUNK);
      suggest_new_round = TRUE;
      break;
     }
    }
    
    // 
    i=0;
   }
   
   //
   if( malloc_hunt_jump(&h) == TRUE )break;
  }
  
  //
  if( suggest_new_round == FALSE )break;
 }
}

//

