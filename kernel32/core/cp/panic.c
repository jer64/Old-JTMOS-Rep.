/*
 * Kernel panic handler functions.
 * (C) 2001-2005 Jari Tuominen.
 */
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "sb.h"
#include "syssh.h"
#include "ega.h"
#include "driver_sys.h"
#include "zero.h"
#include "null.h"
#include "pci.h"
#include "vga.h"
#include "threads.h"
#include "syssh_applications2.h"
#include "statusline.h"
#include "panic.h"

//
void IRQStatusCritical(void)
{
	int i,i2;

	//
	for(i=0; i<16; i++)
		disable_irq(i);

	// Enable keyboard IRQ(needed for console switches)
	enable_irq(1);

	// Enable IRQs
	enable();
}

////////////////////////////////////////////////////////////////////////////////
// Panic function.
//
void FATALERROR(const char *s)
{
	static char tmp[256];

	//
	strcpy(tmp, s);

	//
        asm("cli");
	asm("movw $0x10,%ax");
	asm("movw %ax,%ss");
	asm("movl $0x7BFF,%esp");

	// Disable all other interrupts except keyboard handler.
	IRQStatusCritical();

	//
	asm("sti");

	// Setup screen
	// If not text mode, setup it
	if( dispadmin_getcurrentmode()!=3 && !EGASAFE )
	{
		setvmode(3);
	}

	// Switch view to first terminal
	SetVisibleTerminal(0);
	SetThreadTerminal(GetCurrentThread(), 0);

	// Reset screen 
	window(0,0, 79,24);

	//
	textcolor(15); textbackground(0);
	printk("\nSystem failure on PID%d (%s).\n",
		GetCurrentThread(),
		thread_name[RPID(GetCurrentThread())]);
	textcolor(7);
	printk("Description: '%s'.\n", tmp);

	//
	ps();

	//
	printk("SS:ESP = 0x%x:0x%x  CS=0x%x DS=0x%x\n",
		GETSS(),GETESP(),
		GETCS(),GETDS());
	//
	printk("System halted - you may press CTRL+ALT+DEL to restart computer. ");

	// Loop
	while(1) { nop(); }
}

