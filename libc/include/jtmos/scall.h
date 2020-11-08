// JTMOS LIBC system call support
#ifndef __INCLUDED_SCALL_H__
#define __INCLUDED_SCALL_H__

//
#include "syscallcodes.h"
#include "basdef.h"

// Example for calling:
// scall(1, 0,0,0,0 0,0,0);
// (calls relax_thread)

//
extern DWORD scall(DWORD eax,DWORD ebx,DWORD ecx,DWORD edx,
		DWORD esi,DWORD edi,DWORD ebp);

#endif
