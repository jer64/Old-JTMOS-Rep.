/************************************************************************
 *                                                                	*
 * JTMOS - Device/File System/Block Device -Driver Fusion Solution	*                                                          *
 * APPLICATION INTERFACE FOR DEVICE DRIVER CALLS			*
 * (C) 2001-2005,2017-2020 by Jari Tuominen(jari.t.tuominen@gmail.com). *
 *                                                                 	*
 ************************************************************************/

// NOTES
// Whole FAT read/write functions are now implementing
// _readblock _writeblock instead of direct reads.
// This prevents cacheflusher process from jamming.

//
//#include "basdef.h"
//#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "driver_api.h"
#include "driver_sys.h"
#include "driver_lock.h"
#include "driver_rw.h" // readblock...
#include "driver_devcall.h"
#include "driver_init.h"

//
char *devCmdNames[]={
	"nothing",
        "init","shutdown","readblock","writeblock",
	"obsolete",
        "getsize","getblocksize","seek","getch",
        "putch","tell","flush","mount","unmount",
        "receivepacket","transmitpacket",
        "opensocket","closesocket","readsocket","writesocket",
        "ping",
	"open",
	"creat",
	"close",
	"read",
	"write",
	"lseek",
	"readdir",
	"mkdir",
	"unlink",
	"chdir",
	"getcwd",
	"mkfs",
	"sync",
	"rmdir",
        "*"
        };

// Used by driver_FindCacheEntry
int dtravel=0;
//
// Busy indicator variable
char device_call_busy=0;
// Lock variable
int driver_lock[100]={0};
// Debug variable, indicates the function # which
// is currently running, when DEVAPI is locked
// (Read: Is in use by some specific process).
int driver_db=0;
// Name of the function(string)
char driver_dbFuncStr[100]={ 0 };

// Note: special functions are seperated with stars.
//
/********* DEBUG VARIABLE HANDLING *******************/
char *driver_getDebugVar(void)
{
	return driver_dbFuncStr;
}

//-------------------------------------------------------------------------------------
void DumpDC(DCPACKET *dc, char *func)
{
	// Report
	printf("%s: received message %s (%d), %d,%d,%d,%d, %x,%x\n",
		func,
		devCmdNames[dc->n_function],
		dc->n_function,
		dc->par1,dc->par2,dc->par3,dc->par4,
		dc->p1,dc->p2);
}

//-------------------------------------------------------------------------------------
void IllegalPacket(DCPACKET *dc)
{
	// Report
	printf("%s: %d, %d,%d,%d,%d, %x,%x\n",
		__FUNCTION__,
		dc->n_function,
		dc->par1,dc->par2,dc->par3,dc->par4,
		dc->p1,dc->p2);
}

// Returns the size of the device, which is counted in blocks
int getsize(int device_nr)
{
	int actual_size;

	//
	if( !isValidDevice(device_nr) )return 0;
	actual_size=0;

	//
	device_call(device_nr,
			DEVICE_CMD_GETSIZE,
			0,0,0,0,
			&actual_size,NULL);

	//
	return actual_size;
}

// Returns the blocksize in the specified device
int getblocksize(int device_nr)
{
	int actual_size;

	actual_size=0;

	if( !isValidDevice(device_nr) )return 0;

	device_call(device_nr,
			DEVICE_CMD_GETBLOCKSIZE,
			0,0,0,0,
			&actual_size,NULL);

	// NOTE: size 1 means error, of course and functions should handle this :)
	if(!actual_size)
	{
		actual_size=1;
	}

	return actual_size;
}

// Change device
// Returns isJTMFS true or not
int chdev(int device_nr)
{
	if( !isValidDevice(device_nr) )return FALSE;
	return TRUE;
}

//
int driver_unmountDNR(int dnr)
{
	//
        device_call(dnr,
                        DEVICE_CMD_FLUSH,
                        0,0,0,0,
                        0,NULL);
        device_call(dnr,
                        DEVICE_CMD_UNMOUNT,
                        0,0,0,0,
                        0,NULL);
}

//
int driver_mountDNR(int dnr)
{
	//
	device_call(dnr,
			DEVICE_CMD_MOUNT,
			0,0,0,0,
			0,NULL);
}


// Checks whether if a drive is ready and
// has a healthy file system.
int isDriveReady(int dnr)
{
	int r,rv;

	//
	return 0;
}

// Commit a disk change
// All device buffers and caches are deallocated,
// and prepared for a re-read.
int diskChange(int dnr)
{
	//
	return 0;
}




