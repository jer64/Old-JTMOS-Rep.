//--------------------------------------------------------------------------------------------
// threadSwitch.c - thread switching logic.
//--------------------------------------------------------------------------------------------
#include "kernel32.h"
#include "threads.h"
#include "threadSwitch.h"

// Aka ASCPI sleep.
int vmware_sleep_enabled = TRUE;
//
static int SleepTime=2;
//
static int goback=0;

//
void vmware_sleep(void)
{
	//
	if(vmware_sleep_enabled && vmware)
	{
		vmsleep();
	}
}

//--------------------------------------------------------------------------------------------
// Used by ScheduleNextThread and timerhandler.mac.
//
void forceSwitch(void)
{
	int i,x,y;
	TSSEG *t;
	char str[256],str2[256];

	//
	SetToSleep(4);

	//---------------------------------------------------------------------
	// Do we have an ordered switch to a specified thread ?
	if(force_switch_thread_nr && !goback)
	{
		//
		goback = nr_curthread;

		// Switch to ordered thread
		nr_curthread = force_switch_thread_nr;

		//
		thread_idling[nr_curthread] = 0;

		//
		force_switch_thread_nr = 0;

		//
		return;
	}

	// Return back from 
	if(goback)
	{
		//
		goback = 0;
		nr_curthread = goback;
	}

	//---------------------------------------------------------------------
	// Get next valid thread.
	//
	for(i=0; ; )
	{
		//----------------------------------------------------------
		// Get next logical thread number.
		//
		nr_curthread++;

		// Loop if necessary.
		if(nr_curthread>=nr_threads)
		{
			if(i)
			{
			//	printk("SLEEP\n");
				// Switch to idle process.
				nr_curthread = RPID(IdleThread.pid);
				break;
			}
			else
			{
				nr_curthread = 0;
			}
			i++;
		}

		// Skip all dead processes.
		if(thread_states[nr_curthread]==0)
			continue;

		//----------------------------------------------------------
		// Determine whether if it is a valid one to run now.
		//

		// Skip idling processes, decrease their idling counter.
		// See SetToSleep -function in threads.c.
		if(thread_idling[nr_curthread])
		{
			thread_idling[nr_curthread]--;
			continue;
		}

		// Schedule according thread priorities.
		thread_tick[nr_curthread]++;
		if(thread_tick[nr_curthread] >= thread_states[nr_curthread])
		{
			// This process needs to be ran now.

			// Get tick count ready for next time.
			thread_tick[nr_curthread] = 0;

			// 
		}

		//
		t = GetThreadTSS(nr_curthread);
		if(!t->cs || !t->ds || !t->es || !t->fs)
		{
			// Illegal segment detected...
			GetThreadName(nr_curthread, str2);
			sprintf(str, "%s: Illegal segment selector detected for process %d(%s).",
				__FUNCTION__, nr_curthread, str2);
			panic(str);
		}

		// Skip IdleThread process when running on regular loop through threads.
		if( nr_curthread==RPID(IdleThread.pid) )
		{
			if(SleepTime)
			{
				SleepTime--;
				continue;
			}
			SleepTime=2;
		}

		// Loop until we got a running process.
		if(thread_states[nr_curthread]!=0)
			// Got process
			break;
	}

	//		
}
