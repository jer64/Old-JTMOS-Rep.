/*
 * pit.c - programmable interrupt timer
 *
 * (C) 2001-2005 by Jari Tuominen.
 *
 * Programmable Interrupt Chip handler library.
 * Basically we need a PIC to do anything nice
 * with the OS, so here's one of the most
 * needed part of the entire OS ;-)
 */

//
// Update: finally got rid of the AT&T standard
// assembly code, quickly translated it into NASM,
// now it seems to compile, and it'll be possible
// to start coding task switching code with NASM
// standard ASM by now, great.
//

#include "kernel32.h"
#include "pit.h"
#include "io.h"
#include "sysglvar.h"
#include "basdef.h"
#include "int.h"
#include "delay.h"
#include "floppy.h" // fd_int1c

//
PIT *pit;
//
static int lala=0;

//
extern void pit_ISR(void);

//
void Sleep(int _count)
{
	MilSleep(_count);
}

//
void runPitHandlers(void)
{
	//
#ifndef IMPLEMENT_MULTIPLE_PIT_HANDLERS
	return;
#endif

	//
	int i;

	// Call other handlers
	for(i=0; i<pit->n_handlers; i++)
	{
		//
		if(pit->handler[i]!=NULL)
			(*(void (*)(void)) pit->handler[i])();
	}
}

//---------------------------------------------------------------------------------
// Init programmable interrupt timer
//
void InitTimer(void)
{
        // Let's enable timer:

	//
	disable();
        // 1) Setup a safe timer frequency at the first place
        init_pit(SAFE_TIMER_FREQUENCY, 0);
        // 2) Setup handler for the PIT timer interrupt
        setint(M_VEC,
                pit_low, GIMMIE_CS(),
                D_INT + D_PRESENT + D_DPL3);
        // 3) Enable PIT timer
//        enable_irq(0);
}

/*
The timer output frequency is obtained by dividing the 8253 master clock
frequency of 1,193,180 Hz by a 16 bit divisor.  (A zero divisor is treated as
65,536.  This is the value programmed by the BIOS, to give a system timer of
18.2065 Hz.)  The timer is programmed by sending a control byte to port 43h,
followed by a divisor word (low byte, high byte) to port 4Xh where X is the
timer being programmed.

If the system timer (counter 0) is modified, an Int 8 handler must be written to
call the original Int 8 handler 18.2 times per second, otherwise some system
functions (eg time of day) may be upset.  When it does call the original Int 8
handler it must NOT send and EOI to the 8259 for the timer interrupt, since the
original Int 8 handler will send the EOI also.

Mitch

default 0x1000(16x BIOS default)
*/
void init_pit(long h, unsigned char channel)
{
	//
	DWORD temp=0;
	int i;

	// Initialize PIT structure
	pit = malloc(sizeof(PIT));
	memset(pit, 0, sizeof(PIT));
	
	//
	temp = h;
	
	// -----------------------------------------------------------------
	PUSHFLAGS;
	disable();

	//
	outportb(TMR_CTRL, (channel*0x40) + TMR_BOTH + TMR_MD3);
	outportb((0x40+channel), (unsigned char) temp);
	outportb((0x40+channel), (unsigned char) (temp>>8));

	// Init all tick counters
	init_counters();

	// Determine how many ticks is a millisecond and a second
	// in the current tick interval.
	initPitMeasure();

	//
	POPFLAGS;
	// -----------------------------------------------------------------
}

// Makes up precalculations,
// e.g. how many ticks is one second
// at the current tick rate.
// To delay a millisecond, the amount
// of ticks needed to delay a single second
// is divided by 100, then this amount
// of ticks is waited and
// a millisecond has passed.
void initPitMeasure(void)
{
	int t1,t2;

	//
	sleep(1);
	t1=GetTickCount2(); sleep(1); t2=GetTickCount2();
	pit->secondInTicks =		t2-t1;
	pit->millisecondInTicks =	pit->secondInTicks/1000;
	// Fix zeroes
	if(!pit->secondInTicks)		pit->secondInTicks = 1;
	if(!pit->millisecondInTicks)	pit->millisecondInTicks = 1;
	printk("%s: %d ticks per millisecond.\n",
		__FUNCTION__, pit->millisecondInTicks);
}

// Sleep a specified amount of milliseconds(1/1000 seconds).
void MilSleep(int periodMilliseconds)
{
	int i,t1,t2;

	//
	t1 = GetTickCount2();
	while( (GetTickCount2()-t1)<(pit->millisecondInTicks*periodMilliseconds) )
	{
		SwitchToThread();
	}
}

//
unsigned int pit_getchannel(unsigned char channel)
{
	//
	unsigned int x=0;
	
	//
	outportb(TMR_CTRL, (channel*0x40) + TMR_LATCH);
	x = inportb(0x40+channel);
	x += (inportb(0x40+channel) << 8);
	return x;
}

//----------------------------------------------------------------
// PIT interrupt handler (timer interrupt handler)
//
void pit_handler(void)
{
	int i,kert;
	char *p;

	// test...
/*	p = 0xB8000;
	for(i=0; i<50; i++)
	{
		p[i]++;
	}*/

	//
	ThreadTimeOut();

	// Increase all timers
	for(i=0; i<50; i++) { glvar.timer[i]++; }

	//
	runPitHandlers();

	// Timer #10 is always equal to milliseconds,
	// so that one tick in timer #10 means one millisecond.
	if(glvar.timer[12]>=pit->millisecondInTicks)
	{
		// Increase the millisecond counter
		// (used by e.g. GetTickCount -function).
		glvar.timer[12] = 0;
		glvar.timer[10]++;

		// Call floppy disk drive handler function every millisecond
		fd_int1c();
	}

	// This is done in ASM handler timerhandler.mac:
//	outportb(0x20, 0x20);
}

//
void init_counters(void)
{
	//
	int i;

	//
	for(i=0; i<50; i++) { glvar.timer[i]=0; glvar.dbtimer[i]=0; }
}

// Add new PIT handler
void addPitHandler(void *ptr)
{
	int i;

	//
	if(pit->n_handlers==0)
		pit->n_handlers++;

	//--------------------------------------------------
	// Look for empty entry, if not found,
	// add one more entry.
	for(; ; pit->n_handlers++)
	{
		//
		if(pit->n_handlers>=N_MAX_PIT_HANDLERS)
		{
			//
			panic("addPitHandler: MAXIMUM NUMBER OF PIT HANDLERS EXCEEDED");
		}

		//--------------------------------------------------
		for(i=0; i<pit->n_handlers; i++)
		{
			//
			if(pit->handler[i]==NULL)
			{
				//
				pit->handler[i]=ptr;
				return;
			}
		}
	}
}

// Remove a PIT handler
void removePitHandler(void *ptr)
{
	int i;

	//
	for(i=0; i<pit->n_handlers; i++)
	{
		//
		if(pit->handler[i]==ptr)
		{
			//
			pit->handler[i]=NULL;
			return;
		}
	}
}

//


