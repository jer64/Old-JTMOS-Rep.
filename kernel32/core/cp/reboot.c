/****************************************
 * reboot.c - System Reboot Functions	*
 * (C) 1999-2008 by Jari Tuominen	*
 ****************************************/

// Required includes
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "syssh.h"
#include "floppy.h"
#include "buildnr.h"
#include "syssh.h"
#include "ega.h"
#include "driver_sys.h"
#include "zero.h"
#include "null.h"
#include "pci.h"
#include "vga.h"
#include "threads.h"
#include "syssh_applications2.h"
#include "statusline.h" // statusline_terminate trigger
#include "panic.h"
#include "reboot.h"

int rebootCommenced=0;

//
// system_reset1
//
// Immediate system reset.
// Warning: No writeback disk cache flushes are made,
// data loss possible.
//
void system_reset1(void)
{
	int t;

	//
	TripleFault();

	// The actual code that is used.

	// Wait until keyboard buffer is empty.
	for(t=GetSeconds(); inportb(0x64) & 0x02 && (GetSeconds()-t)<2;)
		inportb(0x60);
	// Send reset command.
	// 0xD1 = turn CPU Off,  0xFE = reset
	outportb(0x64, 0xFE);
	// Halt CPU.
	asm("cli;hlt");
}

// Alias for system_reset.
void SystemReboot(void)
{
	system_reset();
}

/*
 * This function resets computer.
 * Used by e.g. CTRL+ALT+DEL handler.
 */
void system_reset(void)
{
	DWORD data1;

	// On failure reboot immediatly
	if(rebootCommenced)
		system_reset1();

	//
	rebootCommenced = TRUE;

	// Enable IRQs
	enable();

	// Ask statusline to stop updating statusline & exit
	statusline_terminate = 1;

	// Setup screen
	SetVisibleTerminal(0);
	SetThreadTerminal(GetCurrentThread(), 0);
        window(0,0, 79,24);
	textcolor(15); textbackground(3);
	clrscr();
	// Switch to first window

	// Inform user & do neccesary actions
	textcolor(15);
	printk("\n\n");
	printk("    JTMOS is now preparing to restart the computer.\n");
	printk("    If system does not reboot within 5-10 seconds,\n");
	printk("    then you may need to shutdown or reset your computer manually.\n");
	printk("    There is a known bug in JTMOS regarding rebooting.\n");
	printk("    Flushing buffers ... ");

	// Flush disk cache
	// .....

	// Shutdown all devices
	printk("    Shutting down all devices ... ");
	driver_ShutdownAllDevices();

	//
	printk("\n");
	printk("    Restarting computer ");

	// Disable interrupts
	disable();
	// Do immediate reset
	system_reset1();
}

