/* 
 * delay.c - delay function
 *
 * new: now saves CPU time while sleeping
 * (implemented threads)
 *
 * (C)2002 by Jari Tuominen
 */
#include "pit.h"   
#include "io.h" 
#include "sysglvar.h"
#include "basdef.h"
#include "int.h"
#include "delay.h"
#include "ega.h"
#include "kernel32.h"

/*void xsleep(DWORD duration)
{
	//
	int i,i2,i3;

	//
	glvar.timer[5] = 0;

	//
	for( ; ;  )
	{
		// Check the timer
		if( glvar.timer[5] > duration )break;
		// Spare CPU time for others
		SwitchToThread();
	}
}*/

// Sleep specified amount of milliseconds
//
void delay(DWORD durationMSEC)
{ 
	//
	int t;

	//
	t = GetTickCount();
	for(; (GetTickCount()-t)<durationMSEC; ) { SwitchToThread(); }
}

//

