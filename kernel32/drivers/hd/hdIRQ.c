// ===================================================================
// hdIRQ.c:	High level IDE interrupt handler mechanism
// hdInt.asm:	Low level interrupt handler
// ===================================================================
#include <stdio.h>
#include "hd.h"
#include "hdIRQ.h"

void hdIRQ14(void)
{   
	outportb(0x20,0x20);
	outportb(0xA0,0x20); 
}

void hdIRQ15(void)
{
	outportb(0x20,0x20);
	outportb(0xA0,0x20);
}

void dhd_setuphdirqs(void)
{
	// IRQ 14: IDE0
	setint(S_VEC+(14-8), hdLowIRQ14, SEG_CODE32, 0x8e00);
	// IRQ 15: IDE1
	setint(S_VEC+(15-8), hdLowIRQ15, SEG_CODE32, 0x8e00);

	// Enable IRQs 14 and 15 in PIC #2
	outb_p(inb_p(0x21)&0xfb,0x21);
	outb(inb_p(0xA1)&0xbf,0xA1);
}



