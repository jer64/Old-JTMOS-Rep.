//------------------------------------------------------------
// ega.c - EGA VIDEO DRIVER
// (C) 2000-2007 by Jari Tuominen(jari@vunet.org).
//-----------------------------------------------------------
#include "ega.h"
#include "sysglvar.h"
#include "kernel32.h"
#include "macrodef.h"
#include "cpu.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "printf.h"
#include "string.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h"
#include "terminal.h"
#include "ega_ansi.h" // ansi code support
#include "term.h"
#include "vesa.h"

//
#define IF_VISIBLE	if( thread_terminal[RPID(GetCurrentThread())]==visible_terminal )

// Safe mode EGA init so that kernel panics work at the start up
// even without the dynamic memory allocation system(malloc).
// (obsolete)
void EGASafeMode(void)
{
	//
	dump_all_to_com1=FALSE;
	terminalLockPID=0;
	puts_busy=0;
	visible_terminal=0;
	saved_terminal=-1;

	//
	initscreenstructure(&screens[0]);
}

// For 80x25 EGA
void EgaSelectWindow(int which)
{
	//
	EgaSetViewStart(which*8);
	visible_terminal=which;
	locatetxtcursor( &screens[visible_terminal] );
}

//
void EgaSetViewStart(WORD ad)
{
	//
	outportw(0x3D4,(0x0c)|((ad>>8&255)<<8));
	outportw(0x3D4,(0x0c)|((ad&255)<<8));
}

// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int ega_device_call(DEVICE_CALL_PARS)
{
	//
	switch(n_call)
	{
		//
		case DEVICE_CMD_INIT:
		break;
		//
		case DEVICE_CMD_SHUTDOWN:
		break;
		//
		case DEVICE_CMD_READBLOCK:
		break;
		//
		case DEVICE_CMD_WRITEBLOCK:
		break;
		//
		case DEVICE_CMD_GETSIZE:
		break;
		//
		case DEVICE_CMD_GETBLOCKSIZE:
		break;
		//
		case DEVICE_CMD_SEEK:
		break;
		//
		case DEVICE_CMD_GETCH:
		break;
		//
		case DEVICE_CMD_PUTCH:
		break;
		//
		case DEVICE_CMD_TELL:
		break;
		//
		default:
		break;
	}
 
	//
	return 0;
}

// Registers device 'ega'
int ega_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("ega") )
	{
		print("cannot register device ega: device already registered");
		return 1;
	}

	// Register the device
	driver_register_new_device("ega", ega_device_call, DEVICE_TYPE_CHAR);
 
	//
	return 0;
}

//
int ega_init(void)
{
	//
	ega_register_device();
	return 0;
}

// New: Now adds current visible screen offset
// to the cursor location.
//
void ega_locatetxtcursor(SCREEN *s)
{
	int ad,ad2,ad3,ad4;

	//
	ad2=s->copy; ad2-=0xb8000;

	//
	if(s->updcur)
	{
		//
		ad = (s->width*s->cury + s->curx) + (ad2>>1);
		outvgareg(0x0e,ad>>8);
		outvgareg(0x0f,ad&0xff);
	}
}

// EGA/terminal system initialization
void EgaConsoleInit(void)
{
	// Define that we want now to see the output at EGA HW as well
	if(!vesa_enabled)
		VisibleTerminals_SetHWCopy(1);
}

//
