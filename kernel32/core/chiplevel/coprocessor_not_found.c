// coprocessor_not_found.c - FPU support for JTMOS kernel.
#include <stdio.h>
#include "coprocessor_not_found.h"

// -1 if no process has been using the FPU yet.
static int fpu_last_user_pid = -1;
// Indiciates how many times coprocessor_not_found have been called.
int mathHandling=0;

// Save last FPU user's (if any) FPU registers,
// and restore(or +create if none) current FPU registers.
void coprocessor_not_found(void)
{
	//
	mathHandling++;

	// Are we the same process as the one
	// who owns the current FPU registers?
	if(fpu_last_user_pid)
	{
		// Aha, we are the "fpu_last_user_pid" -process,
		// therefore we don't have to save & restore FPU registers.

		// Here we just return back, it's okay.
		return;
	}

	// Need to save last user's FPU registers?
	if(fpu_last_user_pid>=0)
	{
		// Got last user PID.

		// Save last user process FPU registers.
		fpu_save(thread_fpuregs[fpu_last_user_pid]);
	}

	// Do we have registers storage yet for this process?
	if(thread_fpuregs[GetCurrentThread()]==NULL)
	{
		// Nope.

		// We allocate one for it then.
		thread_fpuregs[GetCurrentThread()] = malloc(100);

		// Store zeroes on these.
		memset(thread_fpuregs[GetCurrentThread()], 0, sizeof(FPUREGS));
	}

	// Restore FPU registers.
	fpu_restore(thread_fpuregs[GetCurrentThread()]);

	// This process becomes last user.
	fpu_last_user_pid = GetCurrentThread();

	// From here on, the FPU should be loaded with current process FPU registers . .
}

//


