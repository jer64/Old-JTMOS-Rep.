// ===========================================================================
// centralprocessInit.c - Initializes the kernel central process.
// (C) 2002-2003 by Jari Tuominen
// ===========================================================================
#include "kernel32.h"
#include "centralprocess.h"
#include "centralprocessInit.h"

//
THREAD centralprocessThread;

// Creates the central process -task.
void centralprocessInit(void)
{
	//
	ThreadCreateStack(&centralprocessThread, 1024*16);
	centralprocessThread.pid = 
		create_thread(centralprocessThread.stack,
			centralprocessThread.l_stack, centralprocess_run);

	// max. 31 characters
	IdentifyThread(centralprocessThread.pid, "CP");
}


