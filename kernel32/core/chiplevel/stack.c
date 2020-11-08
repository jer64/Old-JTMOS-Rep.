//=============================================================
// Stack renderation
// (C) 2003 by Jari Tuominen
//=============================================================
#include "kernel32.h"
#include "int.h" // unipid.h
#include "stack.h"

// Render a stack for thread
void *RenderStack(char *esp, DWORD pc,
	int unipid, WORD ss, WORD cs, WORD ds)
{
	int flags,i;
	DWORD *d;
	WORD *w;
	char *p;

	// Get good flags, with IF set (interrupts enabled)
	flags = get_eflags() | (1<<9);

	// Get stack PTR
	p = esp;

	// Store EFLAGS
	p-=4; d=p; *d=flags;
	// Store CS
	p-=2; w=p; *w=cs;
	// Store EIP
	p-=4; d=p; *d=pc;
	// Store 8 general registers (simulate pushad)
	for(i=0; i<8; i++)
	{
		p-=4; d=p; *d=0;
	}

	// Return new ESP
	return p;
}




