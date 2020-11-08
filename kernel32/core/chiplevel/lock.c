//
#include "kernel32.h"
#include "lock.h"

//-----------------------------------------------------------------------------------------------
// LOCK SYSTEM

//
static int lock[1000];

// Wait until a lock is released & lock it
void Lock(int lockID)
{
	int flags;

	//
	if(!Multitasking)
		return;

	//
	flags = get_eflags();
	disable();
	while(lock[lockID])
		SwitchToThread();
	// Reset lock back to 1 (we reuse it)
	lock[lockID] = 1;
	set_eflags(flags);
}

// Unlock lock
void Unlock(lockID)
{
	//
	if(!Multitasking)
		return;

	//
	lock[lockID] = 0;
}
