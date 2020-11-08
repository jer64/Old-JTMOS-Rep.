// ====================================================================================
// driver_devcall.c
// (C) 2002-2004,2017 by Jari Tuominen(jari.t.tuominen@gmail.com).
// Added special by-passing rights for cache manager threads.
// if( (GetThreadType()&1) ) { ... // Cache Manager Process }
// ====================================================================================
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
#include "driver_api.h"
#include "driver_sys.h"
#include "driver_lock.h"
#include "driver_rw.h" // readblock...
#include "driver_devcall.h"
#include "driver_remdevcall.h"
#include "_virtualfilesystem_devcall.h"

// Returns 0 if device is not valid.
int device_call(long device_nr,
		long n_function,
		long par1,long par2,long par3,long par4,
		void *p1,void *p2)
{
	int retval;
	void *ptr;
	DEVICE_ENTRY *de;
	static char str[100];

	//
	DEBLINE

	//
	if(!isValidDevice(device_nr))	return 0;

	// Multithread Handling
	//
	// Wait until current operation is finished.
	if( (GetThreadType(GetCurrentThread())&1) ) goto past1;
	while(device_call_busy) { SwitchToThread(); }
past1:

	//
	disable();
	device_call_busy=1;

	//
	de=driver_getdevice(device_nr);
	// Skip if device does not exist (is free).
	if(de==NULL || de->isfree)
		return -123;

	//
	switch(n_function)
	{
		case DEVICE_CMD_OPEN:
		case DEVICE_CMD_CREAT:
		case DEVICE_CMD_CLOSE:
		case DEVICE_CMD_READ:
		case DEVICE_CMD_WRITE:
		case DEVICE_CMD_LSEEK:
		case DEVICE_CMD_READDIR:
		case DEVICE_CMD_MKDIR:
		case DEVICE_CMD_UNLINK:
		case DEVICE_CMD_CHDIR:
		case DEVICE_CMD_GETCWD:
		case DEVICE_CMD_MKFS:
		ptr = (DWORD)virtual_file_system_device_call;
		break;
		//
		case DEVICE_CMD_READBLOCK:
		// For statistics:
		de->reads++;
		// Get jump offset
		ptr = (DWORD)de->device_call;
		break;
		//
		case DEVICE_CMD_WRITEBLOCK:
		// For statistics:
		de->writes++;
		// Get jump offset
		ptr = (DWORD)de->device_call;
		break;
		//
		default:
		// Get jump offset
		ptr = (DWORD)de->device_call;
		break;
	}

	// Check jump [offset] pointer
	if(ptr==NULL && (de->type&DEVICE_TYPE_NONKERNELDRV)==0)
	{
		//
		return 0;
	}

	// Enable interrupts
	enable();

	// Wait until the device is free
	if( (GetThreadType(GetCurrentThread())&1) ) goto past2;
	while( de->busy ) { SwitchToThread(); }
past2:

	// Reserve the device
	de->busy=1;

	//
	device_call_busy=0;

	// Is it a non-kernel mode driver?
	if( (de->type&DEVICE_TYPE_NONKERNELDRV) && 1==0 )
	{
		// REMOTE DEVICE CALL
		// 1) Non-kernel mode device driver call,
		// will use messages. *remdevcall*
		retval = remoteDeviceCall(device_nr, de->pid, n_function,
					par1,par2,par3,par4, p1,p2);
	}
	else
	// Call kernel mode
	if(de->cs==0)
	{
		// 2) Kernel mode device driver call.
		// (KERNEL LOCAL CALL)
		retval = (*(int (*)(DEVICE_CALL_PARS)) ptr)(n_function,par1,par2,par3,par4,p1,p2);
	}
	else
	{
		// 3) Custom mode device driver call
		// Far call, applications/drivers.
		// (NOT IMPLEMENTED YET, USE MESSAGES INSTEAD.).
		retval = fardcall(ptr,de->cs,
			n_function,par1,par2,par3,par4,p1,p2);
	}

	// We are done, unlock the device.
	de->busy=0;

	//
	return retval;
}

