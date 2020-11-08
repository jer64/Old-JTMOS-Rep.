// fixal.c
#include "kernel32.h"
#include "fixal.h"

//
#define DIST 16

//
void fixalChangeChunkOwner(void *ptr, int pid)
{
	void *p;

	//
	p = ptr-DIST;
	ChangeChunkOwner(p, pid);
}

// Reallocate the buffer if it is corrupted
void *fixalReallocateOnCorruption(void *buf, int l_buf)
{
	void *newp;

	// Default to old buffer
	newp = buf;

	//
	if( fixalCheck(buf, l_buf)==FALSE )
	{
		// Allocate new
		newp = fixal(l_buf);
		// Reallocation
		// Free old buffer
		fixalFree(buf);
	}

	//
	return newp;
}

// Dellocate fixal dyn.mem. region 
void fixalFree(void *p)
{
	//
	return free(p-DIST);
}

// Check guarded memory region
// TRUE = guards are in place
// FALSE = guards have been altered (memory corruption)
int fixalCheck(void *pp, int amount)
{
	char *p;
	DWORD *dp;

	// Check guard #1
	dp = pp-DIST;
	if(*dp!=FIXAL_GUARD)
	{
		//
		printk("FIRST GUARD POINT CHECK INDICATES CORRUPTION\n");
		printk("%x != %x\n",  *dp, FIXAL_GUARD);
		return FALSE;
	}

	// Check guard #2
	dp = pp+amount;
	if(*dp!=FIXAL_GUARD)
	{
		//
		printk("SECOND GUARD POINT CHECK INDICATES CORRUPTION\n");
		printk("%x != %x\n",  *dp, FIXAL_GUARD);
		return FALSE;
	}

	// All OK:
	return TRUE;
}

// guard allocate
void *fixal1(int amount, const char *func)
{
	char *p;
	DWORD *dp;

	// void *malloc1(int count, const char *func)
	// Allocate enough data
	p = malloc1(amount+DIST*2, func);

	// Set guard variables
	// Guard #1
	dp=p; *dp=FIXAL_GUARD;

	// Guard #2
	dp=p+amount+DIST; *dp=FIXAL_GUARD;

	// Return pointer to data
	return p+DIST;
}


