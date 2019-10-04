// =================================================================================
// mmAlloc.c - kmalloc and kfree functions.
// (C) 2002-2005 by Jari Tuominen.
// Note: malloc defaults to current thread as the owner of the allocated block.
// TODO: needs to be multi-thread safe.
// =================================================================================
#include <stdio.h>
#include "mm.h"
#include "mmAlloc.h"

//
char free_caller[100]={0,};
char malloc_caller[100]={0,};

// ----------- Wrappers I -------------------------------------

// Obsolete functions:
void malloc_optimize(void) { }
void malloc_dumpchunkinfoEx(void) { }
void malloc_restfree(void) { }

// Affect process memory load.
void AffectProcessML(int pid, int affection)
{
	//
	if( (GetThreadML(pid)+affection)>=0 )
	{
		SetThreadML(pid, GetThreadML(pid)+affection);
	}
}

//
void *malloc1(int count, const char *_property_name)
{
	if(jtmos_debug)		strcpy(malloc_caller, _property_name);
	AffectProcessML(GetCurrentThread(), (count/4096)+1);
	if(mm_debug)
	{
		printk("%s called malloc with request count of %d bytes\n",
			_property_name, count);
	}
	return kmalloc(count, _property_name);
}

//
void free1(void *ptr, const char *_property_name)
{
	//
	if(jtmos_debug)		strcpy(free_caller, _property_name);
	if(mm_debug)	printk("%s called free with request to free PTR 0x%x\n",
		_property_name, ptr);
	kfree(ptr);
}

// ----------- Pause ------------------------------------------
void mmPause(void)
{
	int t;

	//
	ESCLOCK();
}

// ----------- Wrappers II ------------------------------------

// No support for this function yet
void SetMemoryName(void *ptr, const char *name)
{
	//
	
}

// Wrapper
void *kmalloc(int l, const *_property_name)
{
	//
	return _kmalloc(l+PAGESIZE, GetCurrentThread(), _property_name);
}

// Wrapper
void kfree(void *ptr)
{
	// AVOID FREEING(TEST)
	_kfree(ptr);
}

// ----------- Functions -------------------------------------

// Deallocate all memory allocated by the specified process.
// ADDED UNIPID SUPPORT 7/2003.
void malloc_freeprocmem(int unipid)
{
	int i,flags;
	MD *m;

	//
	UNIF(unipid) {} else { return; }

	// Save state & disable interrupts
	flags = get_eflags();
	disable();

	// Go through all entries and search for matches
	for(i=0; i<p_mm->n_allocated; i++)
	{
		// Get structure
		m = p_mm->allocated[i];

		// Skip free entries
		if(m->isFree)	continue;

		// Check whether if the PID does match
		if(m->PID==unipid && m->p)
		{
			// OK got a match,
		        // unallocate&remove entry:
		        mmRemoveEntry(m);
		}
	}

//	MMPANIC("FREEPROCMEM WAS IMPLEMENTED!");

	// Restore state
	set_eflags(flags);
}

// Return size of the allocated memory chunk.
int malloc_getsize(void *PTR)
{
	MD *m;

	// Find entry
	if( (m=mmGiveMD(PTR))==NULL )
		// Entry not found
		return 0;

	// Return the fixed size(size in bytes)
	return m->bytes;
}

// Change allocated memory region's owner.
int ChangeChunkOwner(void *PTR, int unipid)
{
	MD *m;

	// Find entry
	if( (m=mmGiveMD(PTR))==NULL )
		// Entry not found
		return 0;

	//
	AffectProcessML(m->PID, -((m->bytes/4096)+1));
		
	// Set new owner
	m->PID = unipid;

	//
	AffectProcessML(m->PID, ((m->bytes/4096)+1));
	return m->PID;
}

// Allocate memory and assign it
// under the specified PID's ownership.
// _property_name = debug property name for the allocation chunk
void *procmalloc(DWORD l, int unipid, const char *_property_name)
{
	//
	return _kmalloc(l, unipid, _property_name);
}

// Allocate a memory region.
// _property_name = debug property name for the allocation chunk
void *_kmalloc(int _bytes, int unipid, const char *_property_name)
{
	int i,i2,
		startPage,endPage,
		pages,bytes;
	void *p,*rp;
	MD *m;
	int flags,piddi;
	static char str[256];

	//
	piddi = unipid;

	// We only serve when MM has been first initialized.
	if(!mallocOK)	return NULL;

	// Allocation request in bytes must be
	// atleast equal to PAGESIZE bytes or larger:
	bytes = _bytes;
	if(bytes<PAGESIZE)	bytes = PAGESIZE;
	// Make pagesize alignment
	i=bytes; bytes=bytes/PAGESIZE; bytes=bytes*PAGESIZE;
	if(bytes<i)	bytes+=PAGESIZE;

	// Set return PTR to NULL as default.
	rp = NULL;

	// This must be done safely.
	flags = get_eflags();
	disable();

	// -------- Allocate I -------------------------------------------

	// Determine how many 4 KB pages are needed.
	pages = bytes/PAGESIZE; pages++;

	// Find enough pages to satisfy allocation request.
	startPage = mmFindFreePages(pages);
	if(!startPage)
	{
	/*	//
		sprintf(str, "%s: [REQUEST FOR %d BYTES(%d MB) OF MEMORY]: out of memory\n",
			_property_name, bytes, bytes/(1024*1024));
		panic(str); */

		// No free pages available.
		rp = NULL;
		goto past;
	}
	// Determine end page.
	endPage = startPage+pages;

	// -------- Allocate II ------------------------------------------

	// Convert the page offset to a pointer.
	p = startPage*PAGESIZE;

	// Reserve region.
	mmReserveRegion(startPage, startPage+pages);

	// -------- Allocate III -----------------------------------------

	// Add entry to the entry database
	// [start page] [amount of pages] [fixed length in bytes]
	if( (m=mmAddEntry(startPage, pages, bytes))==NULL )
	{
		MMPANIC("kmalloc: Unhandled situation 2.");
	}

	// Specify owner of this memory region.
	m->PID = piddi;

	// Specify it as reserved.
	m->isFree = 0;

	// Define amount of pages.
	m->pages = pages;

	// Define start page.
	m->page = startPage;

	// Define direct PTR (linear memory).
	m->p = p;

	// Copy debug property 'name'.
	strncpy(m->name, _property_name, 18);

	// ---------------------------------------------------------------
	// Map the memory.
	m->mapad = pg_mapmemory(startPage, pages);

	//
	if(!m->mapad)
	{
		panic("mmAlloc.c: mapad equals NULL!");
	}

	// ---------------------------------------------------------------
	// Set new return PTR.
	rp = m->mapad;

	// -------- Allocate IV ------------------------------------------
past:
	// Restore state.
	set_eflags(flags);
	return rp;
}

// Free a memory region
void _kfree(void *ptr)
{
	int pages,flags;
	MD *m;

	//----------------------------------------------------------------------
	//
	if(!mallocOK)		return;

	// This must be done safely
	flags = get_eflags();
	disable();

	// -------- Free I -----------------------------------------------

	// Find entry
	if( (m=mmGiveMD(ptr))==NULL )
	{
		//
		return;
	}

	// Skip if it is free
	if(m->isFree)	return;

	// Refuse to deallocate anything allocated
	// by any anonymous process.
	// This prevents instability problems.
	if(m->PID==0)	return;

	// -------- Free II ----------------------------------------------
	// Entry is validated.

	// Unmap the memory.
	pg_unmapmemory(m->mapad, m->pages);

	//
	AffectProcessML(m->PID, -((m->bytes/PAGESIZE)+1));

	// Remove entry from the entry database(also affects the bitmap).
	mmRemoveEntry(m);

	// -------- Free III ---------------------------------------------
	// Restore state
	set_eflags(flags);
	return;
}

//


