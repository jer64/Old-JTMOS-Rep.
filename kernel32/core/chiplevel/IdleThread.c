//=====================================================
// JTMOS THREADS - IdleThread.
// (C) 2001-2005 by Jari Tuominen.
//=====================================================
#include "cpu.h"
#include "kernel32.h"
#include "threads.h"
#include "IdleThread.h"

//
DWORD IdleThreadCounter=0;
//
THREAD IdleThread;

// Creates an idle thread
void init_IdleThread(void)
{
	//
	ThreadCreateStack(&IdleThread,1024*16);
	IdleThread.pid = 
		create_thread(IdleThread.stack,
			IdleThread.l_stack, IdleThreadProgram);

	// max. 31 characters.
	IdentifyThread(IdleThread.pid, "CPU_IDLE");

	// Idle thread should be immortal.
	Immortal(IdleThread.pid);
}

//
void IdleThreadProgram(void)
{
	int a,x,y;
	int ram,hd;

	// Loop and spend time
	for(IdleThreadCounter=0; ; IdleThreadCounter++)
	{
		//
		vmware_sleep();
		SwitchToThread();
	}
}

//

