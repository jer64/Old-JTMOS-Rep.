// --------------------------------------------------------
// fdc_dev.c - Device call handler
// (C) 2002 by Jari Tuominen
// Device call = A gateway to access the floppy driver,
// used by lowest level kernel drive access routines.
// --------------------------------------------------------
#include "basdef.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "ega.h"
#include "syssh.h"
#include "io.h"
#include "fdc.h"
#include "fdc2_dev.h"
#include "dma.h"
#include "kernel32.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h"

#define FD_THIS_DRIVE	1

//
int fdc_dev2_mounted=1;

// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int floppy2_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;
	int i,i2,i3,i4;
	static char buf[512],*tmp;
	int flags;

        // If unmounted, we won't answer anything until mounted again.
        if(n_call!=DEVICE_CMD_MOUNT && !fdc_dev2_mounted)
                return 0;

	// Store old state & disable interrupts
	flags = get_eflags();
#ifdef FDC_DRIVER_SAFEMODE
	disable();
#endif

	//
	switch(n_call)
	{
		case DEVICE_CMD_FLUSH:
		fdcFlushCache();
		fdcClearCache();
		break;
		case DEVICE_CMD_MOUNT:
		fdc_dev2_mounted=1;
		fdcClearCache();
		break;
		case DEVICE_CMD_UNMOUNT:
		fdc_dev2_mounted=0;
		fdcClearCache();
		break;
		//
		case DEVICE_CMD_INIT:
		break;
		//
		case DEVICE_CMD_SHUTDOWN:
		break;
		//
		case DEVICE_CMD_READBLOCK:
		fd_selectdrive(FD_THIS_DRIVE);
		memset(po1, 0, 512);
		fd_read_block(p1,po1);
		break;
		//
		case DEVICE_CMD_WRITEBLOCK:
		fd_selectdrive(FD_THIS_DRIVE);
		for(i=0; i<8; i++)
		{
			fd_write_block(p1,po1);
			// verify
			fd_read_block(p1,buf);
			// compare
			tmp=po1;
			for(i2=0,i3=0; i2<512; i2++)
			{
				if(buf[i2]!=tmp[i2]){i3=1; break;}
			}
			// We are clear
			if(!i3)return 0;

			// Reset on every fourth try
			if(!(i&3)==3)
				fd_reset();

			// Write three times:
			fd_write_block(p1,po1);
			fd_write_block(p1,po1);
			fd_write_block(p1,po1);

			// Continue... :
		}
		// Failure:
		dprintk("fdc.c: write failed after %d tries\n", i);
		break;
		//
		case DEVICE_CMD_GETSIZE:
		fd_selectdrive(FD_THIS_DRIVE);
		size=po1;
		size[0]=2880;
		break;
		//
		case DEVICE_CMD_GETBLOCKSIZE:
		fd_selectdrive(FD_THIS_DRIVE);
		size=po1;
		size[0]=512;
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
	set_eflags(flags);

	//
	return 0;
}

// Registers device 'floppy'
int floppy2_register_device(void)
{
	int dnr;

	// Check that the device isn't already registered
	if( driver_getdevicebyname("floppy2") )
	{
		panic("cannot register device floppy: device already registered\n");
	}

	// Register the device
	driver_register_new_device("floppy2", floppy2_device_call, DEVICE_TYPE_BLOCK);

	// Disable read cache
	dnr = driver_getdevicenrbyname("floppy2");
	driver_disableDAPIReadCache(dnr);

	//
	return 0;
}


