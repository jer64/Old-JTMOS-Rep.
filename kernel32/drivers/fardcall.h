//
#ifndef __INCLUDED_FARDCALL_H__
#define __INCLUDED_FARDCALL_H__

//
int fardcall(DWORD eip, DWORD cs,
	DWORD r1,DWORD r2,
	DWORD r3,DWORD r4,
	DWORD r5,DWORD r6,
	DWORD r7);

#endif

//

