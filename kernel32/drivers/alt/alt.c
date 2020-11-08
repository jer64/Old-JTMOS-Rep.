/*** ALT: BLOCK DEVICE BASED ON HDA: DEVICE ***
 *	SPLIT PARTITIONING FEATURE
 *
 *	(C) 2002 by Jari Tuominen(jari@vunet.org)
 */

//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "buildnr.h"
// ---- #include "fileapi.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
#include "alt.h"

// Device number of the floppy: device
static int altdev_hda;

//
// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int altdev_device_call(DEVICE_CALL_PARS)
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
		readblock1(altdev_hda, (p1%2000)+0, po1);
		break;
		//
		case DEVICE_CMD_WRITEBLOCK:
		writeblock1(altdev_hda, (p1%2000)+0, po1);
		break;
		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		*size=2000;
		break;
		// return 0, it's not a block device
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		*size=512;
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
int altdev_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("alt") )
	{
		printk("cannot register device alt: device already registered\n");
		return 1;
	}

	// Register the device
	driver_register_new_device("alt", altdev_device_call, DEVICE_TYPE_BLOCK);
	//
	altdev_hda = driver_getdevicenrbyname("hda");
	return 0;
}

//
int altdev_init(void)
{
	//
	altdev_register_device();

	//
	return 0;
}

