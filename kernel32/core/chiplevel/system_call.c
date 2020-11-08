// =====================================================
// int 0x7E - JTMOS system call & FD database init.
// (C) 2001-2003 by Jari Tuominen
// =====================================================
#include "kernel32.h"
#include "system_call.h"
#include "int.h"
#include "systemcalls.h"
#include "filedes.h" // init_filedes - inits filedes database system

// System calls status.
int scallsIsWorking=FALSE;

// Install system call handler.
// (enables system calls)
void scall_init(void)
{
	int gate_entry;

	//
/*	gate_entry = gdt_NewEntry();
	SetCallGate(gate_entry, scall_handler,SEG_CODE32,
		128+ 64+32+ 8+4, // access: present(128), DPL3(64+32), 8+4(type=call gate)
		0);
	setint(SCALL_INT_NR, 0, gate_entry, 0xEE00);
*/

	//
	setint(SCALL_INT_NR, scall_handler, SEG_CODE32, 0xEE00);

	// Init only
	// see scconf.h
//	setint(SCALL_INT_NR, scall_handler, SEG_CODE32, 0xEE00);

	//
	setint(0x7E, NULL, SEG_CODE32, 0xEE00);

	// Set to 1 if systemcalls are ment to be enabled
	scallsIsWorking = TRUE;

	// Init file descriptor database system
	init_filedes();

	// Report
	if(scallsIsWorking)
		print("System calls are enabled.\n");
	else
		print("System calls are disabled.\n");
}

//

