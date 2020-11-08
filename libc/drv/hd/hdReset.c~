//==============================================================
// hdReset.c
// (C) 2003 by Jari Tuominen
//==============================================================
#include <stdio.h>
#include "hd.h"
#include "hdReset.h"

// Reset IDE hard disk
void hdReset1(void)
{
	int i,irq;

	//
	irq=HD_ENABLE_IRQS;
	//irq=0;

	//
	HDDEBUG
	// Trigger and release reset -line
	outportb(0x3F6, HD_RESET_BIT|irq); for(i=0; i<10000; i++) { } // |2 for IRQs
	outportb(0x3F6, 0|irq); for(i=0; i<10000; i++) { }
}

// Does following things:
// 1) Reset & init line
// 2) Init line
// 3) Wait completion
void hdReset(HD *h)
{
	//
	HDDEBUG
	hdReset1();
	hdWait(h);
}



