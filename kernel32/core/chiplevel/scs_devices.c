// =======================================================
// scs.c - JTMOS system call services
// (C) 2002 by Jari Tuominen(jari@vunet.org).
// OBSOLETE....
// =======================================================
//
#include "kernel32.h"
#include "systemcalls.h" // aka scs.h
#include "scconf.h" // contains only defines, works with nasm
#include "postman.h" // POSTMANCMD, etc..
#include "ega.h"
#include "threads.h"

// ebx = name of the device
// Return value: handle of the device(DWORD)
DWORD scall_getdevnrbyname(SYSCALLPAR)
{
	char *s;

	//
	s = A2K(scall_ebx);
	return driver_getdevicenrbyname(s);
}

// ebx=devnr, ecx=blocknr, edx=pointer to buffer
DWORD scall_readblock(SYSCALLPAR)
{
	char *s;

	//
	s = A2K(scall_edx);
	return readblock(scall_ebx, scall_ecx, s);
}

// ebx=devnr, ecx=blocknr, edx=pointer to buffer
DWORD scall_writeblock(SYSCALLPAR)
{
	char *s;

	//
	enable();
	s = A2K(scall_edx);
	return writeblock(scall_ebx, scall_ecx, s);
}

