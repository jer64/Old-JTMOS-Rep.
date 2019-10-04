// =================================================================
// mmEntry.c - Entry database management functions.
// (C) 2002-2005 by Jari Tuominen.
// =================================================================
#include <stdio.h>
#include "mm.h"
#include "mmEntry.h"

// We just set it as free, I don't know what
// else I should make it do at the present moment.
void mmRemoveEntry(MD *m)
{
	// Free region from free memory bitmap
	mmFreeRegion(m->page, m->page+m->pages);

	//
//	m->PID = 1010;
	memset(m, 0, sizeof(MD));
	m->PID = -1;
	m->isFree=1;
}

// Search through database for the specified
// memory region's memory descriptor(MD *),
// if one is not found then it'll return NULL.
MD *mmGiveMD(void *ptr)
{
	int i;
	DWORD ad;

	//
	ad = ptr;

	// Search through chunk database for the requested PTR
	for(i=0; i<p_mm->n_allocated; i++)
	{
		// Skip free entries
		if(p_mm->allocated[i]->isFree)	continue;

		// Does this entry contain the specified PTR?
		if(p_mm->allocated[i]->mapad==ad)
			return p_mm->allocated[i];
	}

	// If noting found, return NULL:
	return NULL;
}

// This is the main allocation function
// in this part of the MMSYS.
// Provides a new entry, by either using already
// existing free entry, or by adding a new entry
// to the entry database.
MD *mmGetNewEntry(void)
{
	MD *m;

	// ------- Way I ------------------------------------
	// Find already free entry
	m = mmFindFreeEntry();

	// ------- Way II -----------------------------------
	// No free entries found? => Add a new one:
	if(m==NULL)
	{
		MMPRINT("%s: No already free entries found, allocating a new one\n",
			__FUNCTION__);
		m = mmNewEntry();
	}

	// Return the entry to the caller
	MMPRINT("%s: m = 0x%x\n", __FUNCTION__, m);
	return m;
}

// Allocate one new entry,
// returns NULL if allocation failed
// otherwise the PTR to the entry.
MD *mmNewEntry(void)
{
	int i;

	// Check limit
	if(p_mm->n_allocated>=p_mm->max_allocated)
	{
		// Out of entries
		return NULL;
	}

	// Increase amount of allocated entries
	i = p_mm->n_allocated; p_mm->n_allocated++;

	// Reserve it
	p_mm->allocated[i]->isFree=0;

	// Return the new entry PTR to the caller
	return p_mm->allocated[i];
}

// Searches through the present chunk list for a free entry,
// if not found, it'll return NULL, otherwise it'll
// return PTR to the entry.
MD *mmFindFreeEntry(void)
{
	int i;

	// Go through all entries
	for(i=0; i<p_mm->n_allocated; i++)
	{
		// This is a free entry?
		if(p_mm->allocated[i]->isFree)
			// Found free entry, return the entry's PTR to the caller:
			return p_mm->allocated[i];
	}

	// No free entry found
	return NULL;
}

// Determine chunk owner
int GetChunkOwner(void *p)
{
	MD *m;

	//
	if( (m = mmDetermineOwner(p))==NULL )
		return 0;
	return m->PID;
}

// Searches through all curretly allocated memory chunks
// and tries to figure out who owns the memory pointed
// out by the specified pointer "void *p".
// Return value: PID of the owner, zero if none found.
MD *mmDetermineOwner(void *p)
{
	int i;
	void *p1,*p2;
	MD *m;

	// Go through all entries
	for(i=0; i<p_mm->n_allocated; i++)
	{
		//
		m = p_mm->allocated[i];

		// This is a free entry?
		if(m->isFree==FALSE)
		{
			// Define START and END of the region area.
			p1 = m->mapad;
			p2 = m->mapad + m->bytes;

			// Is specified pointer within this region?
			if( p>=p1 && p<p2 )
			{
				return m;
			}
		}
	}

	// Not found.
	return NULL;
}

// Add new entry
void *mmAddEntry(int sp, int pages, int bytes)
{
	MD *m;

	//
	if(!mallocOK)
		return NULL;

	// ------------- Add entry I ---------------------------------
	// Get a new entry
	m = mmGetNewEntry();
	if(m==NULL)
	{
		MMPANIC("mmAddEntry: Out of entries(unhandled situation 1).");
		return NULL;
	}

	// ------------- Add entry II --------------------------------
	// Reserve entry
	m->isFree = 0;
	// Define starting page
	m->page = sp;
	// Define amount of pages
	m->pages = pages;
	// Define fixed length in bytes
	m->bytes = bytes;
	// Define real memory PTR from the page start address
	m->p = sp*PAGESIZE;

	//
	return m;
}


