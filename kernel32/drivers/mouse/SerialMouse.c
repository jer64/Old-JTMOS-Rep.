// ===============================================
// SerialMouse.c - Serial Mouse Driver
// (C) 2002-2006 by Jari Tuominen
//
// COM1: 0x3F8, IRQ4
// COM2: 0x2F8, IRQ3
//
// Implements:
// - 'ps2mouse'-structure
// - ps2mouse_setframe -function
// ===============================================
#include "kernel32.h"
#include "mouse.h"
#include "ps2mouse.h" // ps2mouse structure
#include "SerialMouse.h"
#include "SerialMouseDev.h"
#include "serial.h" // serial_get

//
THREAD MouseThread;

// Mouse data receiver process(uses new buffers)
void MouseThreadProgram(void)
{
	//
	while(1)
	{
		//
		HandleSerMouse(0); // << CURRENTLY ON COM1 PORT ONLY
		SwitchToThread();
	}
}

// Creates an idle thread
void init_MouseThread(void)
{
        //
        ThreadCreateStack(&MouseThread,1024*16);
        MouseThread.pid =
                create_thread(MouseThread.stack,
                        MouseThread.l_stack, MouseThreadProgram);

        // max. 31 characters
        IdentifyThread(MouseThread.pid, "KSERMOUSE");
}

// Obsolete:
void SerialMouse_irqhandler4(void)
{
/*	//panic("(MOUSE) IRQ4 COM1");
	SerialMouse_irqhandler(0);
	// Mark interrupt as "done."
	outportb(0x20, 0x20);*/
}

// Obsolete:
void SerialMouse_irqhandler3(void)
{
/*	//panic("(MOUSE) IRQ3 COM2");
	SerialMouse_irqhandler(1);
	// Mark interrupt as "done."
	outportb(0x20, 0x20);*/
}

//
void LimitToRectArea(int *x, int *y,
		int x1,int y1,int x2,int y2)
{
	// Limit within frame window
	if( *x < x1  )
	{
		*x = x1;
	}
	if( *y < y1 )
	{
		*y = y1;
	}
	if( *x >= x2 )
	{
		*x = x2;
	}
	if( *y >= y2 )
	{
		*y = y2;
	}
}

// Handler serial mouse function
void HandleSerMouse(int nCom)
{
	int b,c,i,i2,accel;
	int inbyte, port;
	SERMOUSE *s;

	//
	port = serialport[nCom].ad;
	s = &mouse.serial[nCom];

	// AUTOMATIC DIVISOR VALUE CALCULATOR
	if(mouse.resolution == PS2MOUSE_DEFAULT_RESOLUTION)
	{
		// 80x25
		s->div = 1; // 4
	}
	else
	if(mouse.resolution == PS2MOUSE_DEFAULT_RESOLUTION2)
	{
		// 640x480
		s->div = 1;
	}
	else
		s->div = 1;

	// Acceleration
	accel = sermouse_acceleration;

//	printk("MOUSE TRIGGER %x\n", port);
	s->signal=0xFF;

	/* Get the port byte */
	for(; ; )
	{
		//
		while( (inbyte = serial_get(nCom))==-1 )
		{
			SwitchToThread();
		}

		// Make sure we are properly "synched"
		if( (inbyte&64) )	s->bytenum=0;
		if( s->bytenum>95 )
					s->bytenum=0;

		/* Store the byte and adjust bytenum */
		s->combytes[s->bytenum]=inbyte;
		s->bytenum++;
	
		/* Have we received all 3 bytes? */
		if(s->bytenum>=3)
		{
			/* Yes, so process them */
			s->dx= 
				((s->combytes[0] & 0x3) << 6) +
				(s->combytes[1]&0x3F);
			s->dy= 
				((s->combytes[0] & 0xC) << 4) +
				(s->combytes[2]&0x3F);
			if(s->dx>=128) s->dx-=256;
			if(s->dy>=128) s->dy-=256;
			s->x += ((s->dx)/s->div)*accel;
			s->y += ((s->dy)/s->div)*accel;
			if(s->x<0)s->x=0;
			if(s->y<0)s->y=0;

			//
			LimitToRectArea(&s->x, &s->y,
			mouse.frame.x1, mouse.frame.y1,
			mouse.frame.x2, mouse.frame.y2);

			// Last
			mouse.lx = mouse.x;
			mouse.ly = mouse.y;
			mouse.x += (s->dx)/s->div;
			mouse.y += (s->dy)/s->div;
			LimitToRectArea(&mouse.x, &mouse.y,
			mouse.frame.x1, mouse.frame.y1,
			mouse.frame.x2, mouse.frame.y2);

			// In serial mouses, 0 means pressed down.
			// This is why we XOR the values.
			s->button1=(s->combytes[0]&32);
			s->button2=(s->combytes[0]&16);
			s->button3=(s->combytes[0]&64);

			// Translate button states
			// -----------------------	
			// BUTTON 1
			if(s->button1)
				mouse.buttons |= 1;
				else
				mouse.buttons &= 0xffff - 1;
			// BUTTON 2
			if(s->button2)
				mouse.buttons |= 2;
				else
				mouse.buttons &= 0xffff - 2;
			// BUTTON 3
			if(s->button3)
				mouse.buttons |= 4;
				else
				mouse.buttons &= 0xffff - 4;
	
			/* And start on first byte again */
			s->bytenum=0;

			// ------------------------------
			// DRAW WHEN LOCATION CHANGED
			if(mouse.isVisible)
				ps2mouse_drawarrow();
			ps2mouse_first_time=0;
		}
	}


	// -- End of Serial Mouse Handler -------------
}

//
int SerialMouse_SetDivisor(int nCom, int div)
{
	SERMOUSE *s;
	s = &mouse.serial[nCom];

	s->div = div;
	return 0;
}

// nCom = 0 or 1(COM1 or COM2)
void SerialMouse_setupirqs(void)
{
	int i;
	SERMOUSE *s;

	// Init for all serial COM ports
	for(i=0; i<4; i++)
	{
		s = &mouse.serial[i];
	
		// Reset variables
		s->bytenum =	0;
		s->signal =	0;
		// Reset coordinates
		s->x =		0;
		s->y =		0;
		s->dx =		0;
		s->dy =		0;
		// Moving divisor(larger value=slower movement)
		SerialMouse_SetDivisor(i, 8);
	}

	// New method:
	init_MouseThread();
	
/*	disable();
	// IRQ4
	setint(M_VEC+4,
		SerialMouse_asmirqhandler4,
		SEG_CODE32, 0x8E00);
	// IRQ3
	setint(M_VEC+3,
		SerialMouse_asmirqhandler3,
		SEG_CODE32, 0x8E00);

	// Enable IRQ for COMx port
	disable_irq(4); disable_irq(3);
	if(ENABLE_MOUSE_COM1)	enable_irq(4);
	if(ENABLE_MOUSE_COM2)	enable_irq(3);
	enable();*/
}

