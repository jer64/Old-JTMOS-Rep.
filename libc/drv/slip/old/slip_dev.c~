/*** SLIP CHAR DEVICE ***
 * SERIAL LINE PROTOCOL
 * (C) 2002 by Jari Tuominen(jari@vunet.org)
 * Programmed according to RFC1055.
 * Feel free to spread and use this code.
 */

//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
// ---- #include "fileapi.h"
#include "ramdisk.h"
#include "jtmfat.h"
#include "jtmfs.h"
//
#include "devsys.h" // devsys_register_new_device, ...
#include "devapi.h" // readblock,writeblock,getsize,getblocksize, ...
#include "slip.h"

//
// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check devapi.h.
//
int slip_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;

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
		size=po1;
		size[0]=1;
		break;
		// return 0, it's not a block device
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		size[0]=0;
		break;
		//
		case DEVICE_CMD_SEEK:
		break;
		//
		case DEVICE_CMD_GETCH:
		size=po1;
		size[0]=0;
		break;
		//
		case DEVICE_CMD_PUTCH:
		size=po1;
		size[0]=0;
		break;
		//
		case DEVICE_CMD_TELL:
		size=po1;
		size[0]=0;
		break;
		//
		default:
		break;
	}

	//
	return 0;
}

// Registers device
int slip_register_device(void)
{
	// Check that the device isn't already registered
	if( devsys_getdevicebyname("slip") )
	{
		printk("cannot register device slip: device already registered");
		return 1;
	}

	// Register the device
	devsys_register_new_device("slip",slip_device_call,DEVICE_TYPE_CHAR);

	//
	return 0;
}

//
int slip_init(void)
{
	//
	printk("slip.c: JTMOS serial line protocol driver\n");

	//
	slip_register_device();

	//
	return 0;
}

