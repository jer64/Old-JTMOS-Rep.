// JTMOS sbrk
#include <stdio.h>

//
static void *curptr = NULL;

//
void *sbrk(int req)
{
	void *p;

	// First call?
	if(curptr==NULL)
		curptr = GetProgramEnd();

	// Get pointer
	p = curptr;
	// Move ptr
	curptr += req;
	return p;
}

