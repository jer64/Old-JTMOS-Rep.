// ================================================================
// REMOVABLE DEVICE SUPPORT
// DiskChange is called by various functions/programs.
// It ejects all cache, buffers on the specified drive.
// (C) 2002-2004,2017 by Jari Tuominen (jari.t.tuominen@gmail.com.
// ===============================================================
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
#include "driver_diskchange.h"

// Wrapper
void diskchange(int dnr)
{
	//
	diskchangednr(dnr);
}

// Wrapper
void DiskChange(int dnr)
{
	//
	diskchangednr(dnr);
}

// Called when the specified device had a disk change
// (used by driver).
// All functions this function calls MUST BE IRQ safe,
// which means that these functions must be suitable
// to be called from IRQ handlers.
void diskchangednr(int WHICH_DNR)
{
	// TODO
}

