// ==========================================================
// statusline.c - Status line program - Shows some basic info
// (C) 2002-2003 by Jari Tuominen
// ==========================================================
#include "kernel32.h"
#include "threads.h"
#include "statusline.h"
#include "sysglvar.h"
#include "rtc.h" // rtc_getdate

THREAD StatusLine;
int statusline_terminate=0;

//
void statusline_thread(void)
{
	SCREEN s;
	int i,oldX,oldY;
	char str[400],str2[400],str3[400];
	DATE d;

	//
	InitScreen(&s);

	// Define window area
	swindow(&s, 0,0,79,0);
	// NO HARDWARE CURSOR UPDATE(messes everything :))
	s.updcur=0;

	// Normalize our running priority
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	// Statusline main loop
	while(1)
	{
		// Get date
		rtc_getdate(&d);

		// Build string
		sprintf(str, " %s 1.0.%x  %dK RAM free  %d processes                                                   ",
				JTMOSBETANAME,
				BUILD_NR,
				malloc_getmemoryfree()/1024,
				GetRunningThreadsCount());
		sprintf(str2, "%d:%d:%d",
				d.hour, d.minute, d.second);
		strcpy(str+79-strlen(str2), str2);

		// Print string
		stextcolor(&s, 0);  stextbackground(&s, 7);
		sclrscr(&s); sgotoxy(&s,0,0);
		sgotoxy(&s,0,0); writetext(&s, str);

		// Wait until next update
		for(i=GetTickCount(); 
			(GetTickCount()-i)<250; )
		{
			//
			SwitchToThread();
		}

		// Terminate request received?
		if(statusline_terminate)break;
	}

	//
	ExitThread(0);
}

void start_statusline(void)
{
	int flags;

	//
	flags = get_eflags();
	disable();

	//
	ThreadCreateStack(&StatusLine,1024*16); 
	StatusLine.pid =
	        create_thread(StatusLine.stack, StatusLine.l_stack, statusline_thread);

	// max. 31 characters
	IdentifyThread(StatusLine.pid, "statusline");

	//
	set_eflags(flags);
}


