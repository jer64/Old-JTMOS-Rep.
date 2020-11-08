/**************************************************************
 * sysh_callapp.c - Application Binary Launching Code
 * SYSTEM MAINTENANCE SHELL aka SMSH or SYSSH or Kernel Shell
 * (C) 2002-2003 by Jari Tuominen
 * SMSH is a debugging interface to the kernel.
 *************************************************************/
#include "kernel32.h"
#include "basdef.h"             // DWORD,WORD,BYTE,FALSE,TRUE,Etc..
#include "ega.h"
#include "string.h"
#include "foo.h"
#include "sysglvar.h"
#include "ramdisk.h"
#include "vga.h"
#include "sysmem.h"
#include "cmos.h"
#include "floppy.h"
#include "keyb.h"
#include "int.h"
//#include "hexdump.h"
#include "cpu.h"
#include "buildnr.h"
#include "ps2mouse.h"           // mouse stuff
#include "pci.h"                // pci_probe
//#include "devsys.h"

//
#define STACKSIZE	16384

// 
void callapp(void *appcode)
{
	// 
	callappex(appcode, 1, GetCurrentTerminal());
}

//
void callapp1(void *appcode, char waitUntilExits)
{
	//
	int term;

	// Select terminal...
	if (!waitUntilExits)
		term = 3;
	else
		term = 0;

	//
	callappex(appcode, waitUntilExits, term);
}

// 
void callappex(void *appcode, char waitUntilExits, char term)
{
	// ------------------------------------------------------------------
	static char	*appstack;
	static int	key1,
			pr,
			key;
	static char	str[256];

	// ------------------------------------------------------------------
	// Create app's stack(Later will be changed to thread's ownership).
	// This makes it possible that the thread stack will be
	// automatically deallocated upon termination of it(done at
	// create_thread function).
	//
	appstack = malloc(STACKSIZE*2);


	// Create app's thread
	disable();
	key1 = create_thread(appstack, STACKSIZE, appcode);
	if(strlen(cmdpar[0]) < 28)
	{
		IdentifyThread(key1, cmdpar[0]);
	}
	else
	{
		IdentifyThread(key1, "[string too long]");
	}
	// Select a safe priority level.
	SetThreadPriority(key1, THREAD_PRIORITY_NORMAL);
	// Select terminal 1(0-x)
	SetThreadTerminal(key1, term);
	// DEFINE DEVICE & DIR
	SetThreadDNR(key1, GetCurrentDNR());
	SetThreadDB(key1, GetCurrentDB());

	// No termination now at the beginning
	syskey_terminate = 0;
	pr = 1;

	// Schedule once
	SwitchToThread();
	enable();

	//
	if(!waitUntilExits)	return;

	// We need to disable IRQs to act as a monitorate process
	disable();
	// Wait until application exits
	while(1)
	{
		// 
		pr = GetThreadPriority(key1);

		// Thread execution ended?
		if(pr==0)	break;

		// Terminate key pressed?
		if(syskey_terminate)
		{
			// Flag the termination as done
			syskey_terminate = 0;
			// Terminate the application
			KillThread(key1);
			// Restore videomode if needed
			if(dispadmin_getcurrentmode()!=3)
			{
				//
				setvmode(3);
				// Report to console
				printk("\nsyskey_terminate: killed %d\n", key1);
				// 
				break;
			}
		}

		// 
		SwitchToThread();
	}
	return;
}

// Alternative callappex
void callappex2(void *appcode, char waitUntilExits, char term)
{
	THREAD t;

	//
	ThreadCreateStack(&t, 1024*16);
	t.pid =
		create_thread(t.stack,
			t.l_stack, appcode);
	SetThreadTerminal(t.pid, term);

	// max. 31 characters
	IdentifyThread(t.pid, "program");
}


// ps - report process status
//
void ps(void)
{
	int i, i2, i3, i4,fr;
	char str[256],str2[256];

	//
	printk("Process list\n");
	printk("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�\n");
	//
	fr = malloc_getmemoryfree();
	printk("%d KB (%d MB) RAM free\n",
		fr/1024,
		fr/(1024*1024));
	//
	StretchPrint("PID       ",	10);
	StretchPrint("CPU spend ",	10);
	StretchPrint("Priority  ",	10);
	StretchPrint("Process name    ",	16);
	StretchPrint("Memory usage    ",	16);
	printk("\n");

	//
	for(i = 0; i < nr_threads; i++)
	{
		// Non-existing task? => Skip
		if(!thread_states[i])
			continue;

		//
		sprintf(str, "%d", i); StretchPrint(str, 10);
		sprintf(str, "%d", thread_CPUspending[i]); StretchPrint(str, 10);
		sprintf(str, "%d", thread_states[i]); StretchPrint(str, 10);
		if(thread_name[i]!=NULL)
			strcpy(str, thread_name[i]);
			else
			strcpy(str, "[Untitled]");
		StretchPrint(str, 16);

		// Memory load.
		sprintf(str, "%dK", thread_ml[i]*4096/1024);
		StretchPrint(str, 16);

		//
		printk("\n");

		//
	}
}
