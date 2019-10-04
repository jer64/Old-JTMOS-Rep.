// ============================================================================
// Memory management system - Free memory bitmap operations.
// (C) 2002-2005 by Jari Tuominen.
// ============================================================================
#include <stdio.h>
#include "mm.h"
#include "mmBit.h"

//
BYTE mmOrBits[8]={1,2,4,8, 16,32,64,128};
BYTE mmAndBits[8]={255-1, 255-2, 255-4, 255-8,
		255-16, 255-32, 255-64, 255-128};

//
int malloc_getamountfree(void)
{
	return malloc_getmemoryfree();
}

// Returns total amount of memory free in bytes
// at the present moment of calling.
int malloc_getmemoryfree(void)
{
	int i,i2,freePages,allocatedPages,l,c;

	//
	// Search through all pages for suitable
	// contigous area of free space.
	l = 1024*1024;
	for(i=0,c=0,freePages=0,allocatedPages=0;
		i<l; i++)
	{
		// Got free page?
		if( !mmBitGet(i) )
				freePages++;
			else
				allocatedPages++;
	}

	// Return amount of memory free in bytes:
	return freePages*PAGESIZE;
}

// Returns the start page number which contains
// contigously the specified amount of free pages.
int mmFindFreePages(int _pages)
{
	int i,i2,c,sp,l,pages;

	//
	pages = _pages+10;

	// Search through all pages for suitable
	// contigous area of free space.
	l = 1024*1024;
	for(i=1,c=0,sp=0; i<l; i++)
	{
		if( !mmBitGet(i) )
		{
			// If first page then specify it as SP
			if(!c)	sp = i;
			// Increase count of free pages found
			c++;
			// If enough pages found return
			// the SP(start page) to the caller:
			if(c>=pages)
				// Found satisfying amount of free pages!
				return sp;
		}
		else
			c=0;
	}

	// No free pages found!
	return 0;
}

//
int mmFreeRegionAD(char *p_s, char *p_e)
{
	DWORD s,e;

	//
	s = p_s; s=s/PAGESIZE;
	e = p_e; e=e/PAGESIZE;
	return mmFreeRegion(s,e);
}

//
int mmReserveRegionAD(char *p_s, char *p_e)
{
	DWORD s,e;

	//
	s = p_s; s=s/PAGESIZE;
	e = p_e; e=e/PAGESIZE;
	return mmReserveRegion(s,e);
}

// Free specified memory region
int mmFreeRegion(DWORD s, DWORD e)
{
	return mmSetRegion(s,e, 0);
}

// Reserve specified memory region
int mmReserveRegion(DWORD s, DWORD e)
{
	return mmSetRegion(s,e, 1);
}

// Fill a region in the bitmap to a defined state(0 or 1)
int mmSetRegion(DWORD startPage, DWORD endPage, int STATE)
{
	DWORD i,i2,l, pages;

	// Set area state
	for(i=startPage; i<(endPage); i++) // +1 ??
		BIT(i, STATE);

	//
	return 0;
}

//



