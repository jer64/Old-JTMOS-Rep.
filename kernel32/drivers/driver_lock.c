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

int daLock=0;

//
void driver_directAccessLockOn(void)
{
	int flags;

	//
	flags = get_eflags();
	disable();
	while(daLock) { SwitchToThread(); }
	daLock=1;
	set_eflags(flags);
}

void driver_directAccessLockOff(void)
{
	daLock=0;
}

void driver_forceSetLock(int lockNr, int what)
{
	driver_lock[lockNr] = what;
}

int driver_getLock(int lockNr)
{
	return driver_lock[lockNr];
}

// ------------------------------------------
// Fix: If the lock owner is same as the caller
// of this function, it'll not try to re-lock it.
void driver_lockOn(int lockNr)
{
	// Disable interrupts, this IS critical.
	disable();

	// Special grant?
	if(driver_lock[lockNr]==GetCurrentThread())
			goto specialGrant;

	// Wait until we get it locked
	while(driver_lock[lockNr]) { SwitchToThread(); }
	driver_forceSetLock( lockNr, GetCurrentThread() );
specialGrant:
	enable();
}

//
void driver_lockOff(int lockNr)
{
	//
	driver_lock[lockNr]=0;
}

