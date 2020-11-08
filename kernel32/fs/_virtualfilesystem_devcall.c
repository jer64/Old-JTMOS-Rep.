// ==========================================================================$
// _virtualfilesystem_devcall.c
// (C) 2017 by Jari Tuominen(jari.t.tuominen@gmail.com).
// ==========================================================================$
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

// virtual_file_system_device_call
// ===============================
// WORKS ON A MOUNTED DEVICE, WHICH APPEARS AS A DRIVER_API -DEVICE.
// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h. It is the device call parameters standard.
//
int virtual_file_system_device_call(DEVICE_CALL_PARS)
{
        DWORD *size;
        int i,i2,i3,i4;
        static char buf[512],*tmp;
        int flags;

        //
        switch(n_call)
        {
		// MANY TODOS!
                case DEVICE_CMD_OPEN:
		break;

                case DEVICE_CMD_CREAT:
		break;

                case DEVICE_CMD_CLOSE:
		break;

                case DEVICE_CMD_READ:
		break;

                case DEVICE_CMD_WRITE:
		break;

                case DEVICE_CMD_LSEEK:
		break;

                case DEVICE_CMD_READDIR:
		break;

                case DEVICE_CMD_MKDIR:
		break;

                case DEVICE_CMD_UNLINK:
		break;

                case DEVICE_CMD_CHDIR:
		break;

                case DEVICE_CMD_GETCWD:
		break;

                case DEVICE_CMD_MKFS:
                break;

		default:
		// Unhandled. New feature?
		break;
	}

	//
	return 0;
}
