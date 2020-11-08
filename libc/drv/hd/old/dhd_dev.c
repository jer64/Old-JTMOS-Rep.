// =========================================================================
// dhd_dev.c - HD device access interface
// (C) 2002 by Jari Tuominen
// =========================================================================
#include "int.h"
#include "cpu.h" // nop()
#include "sysglvar.h"
#include "kernel32.h"
#include "delay.h"
#include "io.h"
#include "dma.h"
#include "dhd.h"
#include "devsys.h"
#include "dhd_detect.h"
#include "dhd_dev.h"
#include "dhd_buffer.h"

//
int dhd_dev_mounted=1;

// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check devapi.h.
//
int dhd_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;
	int flags,i,i2;
	char *p;

        // If unmounted, we won't answer anything until mounted again.
        if(n_call!=DEVICE_CMD_MOUNT && !dhd_dev_mounted)
                return 0;

	//
	switch(n_call)
	{
                case DEVICE_CMD_MOUNT:
                dhd_dev_mounted=1;
                break;
                case DEVICE_CMD_UNMOUNT:
                dhd_dev_mounted=0;
                break;

		//
		case DEVICE_CMD_INIT:
		break;
		//
		case DEVICE_CMD_SHUTDOWN:
		break;

		// ------------------------------------------------------------
		case DEVICE_CMD_READBLOCK:
		if(p1<0 || p1>=CurrentHD->n_blocks)	break;

		// p1 =		(integer)	which block
		// po1 = 	(PTR)		buffer
		flags = get_eflags();
		DISABLE();
		for(i=0,i2=p1*CurrentHD->units,p=po1;
			i<CurrentHD->units; i++,p+=512,i2++)
		{
			dhd_blockrw(CurrentHD, p,i2,0,READ);
		}
		set_eflags(flags);
		break;

		// ------------------------------------------------------------
		case DEVICE_CMD_WRITEBLOCK:
		// Validity check
		if(p1<0 || p1>=CurrentHD->n_blocks)	break;

		// p1 =		(integer)	which block
		// po1 = 	(PTR)		buffer
		flags = get_eflags();
		DISABLE();
		for(i=0,i2=p1*CurrentHD->units,p=po1;
			i<CurrentHD->units; i++,p+=512,i2++)
		{
			dhd_blockrw(CurrentHD, p,i2,0,WRITE);
		}
		set_eflags(flags);
		break;
		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		*size  =  CurrentHD->n_blocks / CurrentHD->units;
		break;
		//
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		*size  =  CurrentHD->blocksize * CurrentHD->units;
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
		case DEVICE_CMD_FLUSH:
		dhd_flush(CurrentHD);
		break;
		//
		default:
		break;
	}
	return 0;
}

// Registers device 'hda'
int dhd_register_device(void)
{
	// Check that the device isn't already registered
	if( devsys_getdevicebyname("hda") )
	{
		panic("cannot register device hda: device already registered");
	}

	// Register the device
	devsys_register_new_device("hda",dhd_device_call,DEVICE_TYPE_BLOCK);
	return 0;
}
