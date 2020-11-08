/**** JTMOS KERNEL - (C) 1999-2008 by Jari Tuominen. ***
 *
 *  KERNEL FIRST ROUTINE
 *
 */

// Required includes
#include "kernel32.h"	// Preferences for kernel32(init)
#include "keropt.h"	// Not included in kernel32.h !

//
static WORD *buf = 0xB8000;

// ^^================ BOOT UP =================================^^
// NOTICE: Keep this function on top of the code.
// it has to be in top of the file when Boot sector calls it.
// boot_up = int/llint.asm boot_up subroutine,
// which'll call up this C function.
__attribute__ ((__noreturn__)) void _boot_up(void)
{
	// Change stack location.
	asm("movl $0x90000,%esp");

	//
	//(WORD*)buf[0] = 0xFCE2;
	//while(1);

	//
	EGASAFE =		TRUE;
	KINIT = 		FALSE;
	KernelInitPassed = 	FALSE;
	mallocOK = 		FALSE;

	//
	screens[0].buf = 0xB8000;

	// Jump to the main part of the KERNEL32.
	KernelInit();

	// This function shouldn't ever be called, logically :-)
	halt();
}

