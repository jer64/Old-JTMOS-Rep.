/* ===============================================================
 * sb.c - Sound blaster support - Device interface part.
 *
 * (C) 2001-2002,2017 by Jari Tuominen(jari.t.tuominen@gmail.com)
 * ===============================================================
 */
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "dma.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "sb.h"
#include "sb_dev.h"
//
#include "driver_sys.h"

//
int sb_device_call(DEVICE_CALL_PARS)
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

//
int sb_register_device(void)
{
	//
	if( device("sb")>0 )return 1;
 
	//
	driver_register_new_device("sb", sb_device_call, DEVICE_TYPE_CHAR);
 
	//
	return 0;
}

