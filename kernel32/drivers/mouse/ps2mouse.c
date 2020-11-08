//==================================================================
// PS/2 mouse driver for JTMOS.
//
// (C) 2002-2008,2017 by Jari Tuominen(jari.t.tuominen@gmail.com).
//
// It might sound alittle odd, but both PS/2 mouse and keyboard
// work through the INTEL 8042 keyboard controller.
// This is why we also program/read mouse via the controller.
//
// -- PORTS --------------------------------------------------
// 0x60 8042 Keyboard input/output buffer register
// 0x61 8042 system control port (for compatibility with 8255)
// 0x64 8042 keyboard command/status register
//
// Note: keyboard and PS/2 mouse use same I/O system.
//==================================================================
/*
 * Supports graphic modes: 640x480x2		(0x11)
 *                         80x25 textmode	(0x03)
 *
 * Support means arrow displaying for the specified mode,
 * in future perhaps more.
 *
 * User should call ps2mouse_modechange function to give
 * new parameters for the mouse driver to work with,
 * like f.e. screen metrics(width,height,bpp).
 *
 * UPDATE:
 * The PS/2 mouse driver seems to be somewhat working now.
 * Still needs some testing to make sure.
 */

// #define __PS2MOUSEKVIIK__
#include "basdef.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "ega.h"
#include "syssh.h"
#include "io.h"
#include "floppy.h"
#include "dma.h"
#include "cpu.h"
#include "mouse.h"
#include "ps2mouse.h"
#include "kernel32.h"
#include "dispadmin.h"
#include "sysgfx.h"
#include "driver_sys.h"
#include "delay.h"

//#define PS2MOUSE_DEBUG_PORTWRITE
//
int ps2mouse_enabled=0;
int ps2mouse_counter=0,ps2mouse_first_time,
	PS2MOUSE_TEST_OK=0;

//
void ps2mouse_delay(void)
{
	int i,t;

	//
	for(i=0,t=GetSeconds(); i<1000 && (GetSeconds()-t)<2; i++)
	{
		inportb(0x80); nop();
	}
}

//
int ps2mouse_kbwrite(int value)
{
	int i,i2,i3,i4,d,t;

	//
	ps2mouse_delay();

	// When using PS/2 mouse we should wait here
	// for kbwaitmousebufferfull, so
	// it should be manually called before using
	// this function when commanding PS/2 mouse.
	for(t=GetSeconds(); (inportb(0x64)&0x20)==0x20; )
	{
		if( (GetSeconds()-t)>=2 )
			// Time-out error.
			return 1;
		inportb(0x60); //
		inportb(0x80);
	}

	// Wait until output buffer is empty.
	for(t=GetSeconds(); (inportb(0x64)&0x02)==0x02; )
	{
		if( (GetSeconds()-t)>=2 )
			// Time-out error.
			return 2;
		inportb(0x60); //
		inportb(0x80);
	}

	// write the data
	outportb(0x60,value); for(i4=0; i4<10; i4++)inportb(0x80);
	ps2mouse_delay();
	return 0;
}

// non-zero will be an error
int ps2mouse_kbcmd(int cmd)
{
	int i;

	//
	ps2mouse_delay();
	// cmd_send
	outportb(0x64,cmd); 
	ps2mouse_delay();
	return 0;
}

// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int ps2mouse_device_call(DEVICE_CALL_PARS)
{
	//
	switch(n_call)
	{
		//
		case DEVICE_CMD_INIT:
		break;	

		//
		default:
		break;
	}
	return 0;
}

// Registers device 'ps2mouse'
int ps2mouse_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("ps2mouse") )
	{
		//
		print("cannot register device ps2mouse: device already registered");
		return 1;
	}

	// Register the device
	driver_register_new_device("ps2mouse",ps2mouse_device_call,DEVICE_TYPE_CHAR);
	return 0;
}

// If value==-1, nothing will be written after
// sending the mouse command
//
// usage: ps2mouse_writemouse([Mouse CMD Number],[set_to_value, optional]);
//
int ps2mouse_writemouse(BYTE mousecmd, long value)
{
	int i;

	//
#ifdef PS2MOUSE_DEBUG_PORTWRITE
	printk("Write mouse(%x,%x); ",
		mousecmd, value);
#endif

	// Wait until output buffer is empty, so we can write to it
	ps2mouse_kbcmd(0xd4);
	ps2mouse_delay();
	outportb(0x60,mousecmd);
	ps2mouse_delay();

	// Only if we were requested to write.
	if(value==-1)
	{
		// Wait until the operation is done
#ifdef PS2MOUSE_DEBUG_PORTWRITE
		printk("Done.\n");
#endif
		return 0;
	}

	//
	if( ps2mouse_kbwrite(value) )
		return 1;

#ifdef PS2MOUSE_DEBUG_PORTWRITE
	printk("Done.\n");
#endif

	//
	return 0;
}

//
int kbread1(void)
{
	return inportb(0x60);
}

//
void ps2mouse_drawgraphicalarrow(int t_x,int t_y)
{
	int x,y,i,i2,i3,i4;

	//
	for(y=0,i=0; y<32; y++)
	{
		//
		for(x=0; x<32; x++,i++)
		{
			//
			if(mouse_arrow[i]==206)
			            vga_xorpixel_m11(x+t_x,y+t_y);
			else
			if(mouse_arrow[i]==0)
			{
				if( (x+y)&1 )
				vga_xorpixel_m11(x+t_x,y+t_y);
		 	}
		}
		//
	}
}

//
void ps2mouse_drawarrow(void)
{
	int ad,ad2,ad3,ad4,i,i2,i3,i4;

	//
	if( !mouse_isVisible() )return;

	// Don't redraw it if the location hasn't changed.
	// (This shouldn't be done if the arrow is animated,
	//  which is not yet a supported feature.)
	if(mouse.x == mouse.lx
		&&
	    mouse.y == mouse.ly)
	{
		return;
	}

	//
	switch(dispadmin_getcurrentmode())
	{
		// 80x25
		case 3:
		if(!ps2mouse_first_time)
		{
			ad=mouse.ly*80+mouse.lx;
			storemark(mouse.ch,((ad)<<1)+1 );
		} 

		//
		ad=mouse.y*80+mouse.x;
		mouse.ch=getmark( (ad<<1)+1 );
		storemark( (( getmark((ad<<1)+1) &0x0f)|0x40)^PS2MOUSE_XORCOLOR, (ad<<1)+1 );
		break;

		// 640x480
		case 0x11:
		if(!ps2mouse_first_time)
		{
			ps2mouse_drawgraphicalarrow(mouse.lx,mouse.ly);
		}
		ps2mouse_drawgraphicalarrow(mouse.x,mouse.y);
		break;

		//
		default:
		break;
	}
}

/*
 PS/2 IRQ HANDLER (IRQ #12)

 The standard PS72 mouse sends movement
 (and button) information to th the host
 using following 3-byte packet

 BYTE1:
 	Bit7		Bit6		Bit5		Bit4		Bit3		Bit2		Bit0
	Y overflow	X overflow	Y sign bit	X sign bit	Middle Btn	Right Btn	Left Btn

 BYTE2:
	X Movement

 BYTE3:
	Y Movement


 */
void ps2mouse_irqhandler12(void)
{
	//
	char *p;
	long ad,ad2;

	//
	PS2MOUSE_TEST_OK=1;
 
	//
#ifdef __PS2MOUSEKVIIK__
	printk("[kviik!]");
#endif

	// disable keyboard temporarily
	ps2mouse_kbcmd(0xad);

	//
	if(ps2mouse_first_time)
	{
		kbread1(); kbread1(); kbread1();
	}

	 /*
		BIT7: [Always 0]
		BIT6: Mode(scaling), 0 = 1:1, 1 = 2:1
		BIT5: Enable(?)
		BIT4: Scaling
		BIT3: [Always 0]
		BIT2: Left button
		BIT1: Middle button
		BIT0: Right button
	  */
	// button change
	mouse.buttons=kbread();

	// modifier-x
	mouse.mx=kbread();

	// modifier-y
	mouse.my=kbread();

	// store last coordinates
	mouse.lx=mouse.x;
	mouse.ly=mouse.y;
	// update to new ones
	mouse.x+=(mouse.mx);
	mouse.y+=-(mouse.my);

	// Keep mouse cursor location inside the specified frame area.
	if(mouse.x <= mouse.frame.x1) mouse.x=mouse.frame.x1;
	if(mouse.x >= mouse.frame.x2) mouse.x=mouse.frame.x2;
	if(mouse.y <= mouse.frame.y1) mouse.y=mouse.frame.y1;
	if(mouse.y >= mouse.frame.y2) mouse.y=mouse.frame.y2;

	//
	// pd32(mouse.x); printk(","); pd32(mouse.y); printk(", buttons=0x");
	// p8(mouse.buttons); printk("\n");

	//
	if(mouse.isVisible)
		ps2mouse_drawarrow();

	// Restore keyboard.
	ps2mouse_kbcmd(0xAE);

	//
	ps2mouse_first_time=0;

	//
	outportb(0x20,0x20);
	outportb(0xA0,0x20);
}

//
void ps2mouse_setupirqs(void)
{
	//
	disable();
	// IRQ12 PS/2 mouse
	setint(S_VEC+(12-8),ps2mouse_asmirqhandler12,SEG_CODE32,0x8e00);

	// Enable IRQ 12
	enable_irq(12);
	enable();
}

// SET MOUSE UPDATE RATE
// returns back the rate if succesful
int ps2mouse_setrate(BYTE rate)
{
	int i;

	disable();
	for(i=0; i<1; i++)
	{
		ps2mouse_writemouse(0xF3,-1);
		ps2mouse_writemouse(rate,-1);
	}
	enable();

	//
	mouse.rate = rate;

	//
	return rate;
}

/*
	Valid values for ps2mouse_setresolution
	0x00	1 count/mm
	0x01	2 count/mm
	0x02	4 count/mm
	0x03	8 count/mm
 */
// SET MOUSE RESOLUTION
// returns zero on success
int ps2mouse_setresolution(int value)
{
	disable();
	ps2mouse_writemouse(0xe8,-1);
	ps2mouse_writemouse(value,-1);
	enable();
	mouse.resolution = value;

	return 0;
}

// Returns non-zero on failure.
int ps2mouse_hardwareinit(void)
{
	int d,d2,v,v2;
	int key;

	// Here we init the PS/2 mouse,
	// disabling irqs is neccesary.
	ps2mouse_setupirqs();

 disable();

	// Reset keyboard
	// ps2mouse_kbcmd(0xff);
	// Enable keyboard scanning
	// kbcmd(0xf4);

	// 
	d=0;
	d|=KBD_MODE_KCC;
	d|=KBD_MODE_KBD_INT;
	d|=KBD_MODE_MOUSE_INT;

	// Write mode bits
	printk("1-");
	ps2mouse_kbcmd(0x60);
	if( ps2mouse_kbwrite(d) )
		return 1;

	// Enable mouse interface
	printk("2-");
	ps2mouse_kbcmd(0xA8);

	// Write mouse/Enable auxility device
	printk("3-");
	ps2mouse_writemouse(0xF4,-1);

	// Go into stream mode
	printk("4-");
	ps2mouse_writemouse(0xEA,-1);

	//
	printk("5-");
	ps2mouse_setrate(PS2MOUSE_DEFAULT_RATE);

	//
	printk("6-");
	ps2mouse_setresolution(PS2MOUSE_DEFAULT_RESOLUTION);

	// Enable keyboard
//	ps2mouse_kbcmd(0xf4);

	//
	printk("7.\n");
	printk("Keyboard status 0x64 reads now = 0x%x\n", inportb(0x64));

	enable();
	return 0;
}

// APPLY VIDEOMODE CHANGE(Gives new params.)
// (Software should call this before enabling mouse cursor)
//
// Sets 2:1(0xe7) scaling for 80x25(3), and 1:1(0xe6) for all other resolutions.
//
//
//
//
//
void ps2mouse_modeChange(int mode1)
{
	if(!ps2mouse_enabled)return;

	//
	switch(mode1)
	{
		// 80x25 textmode, 2 colors
		case 3:
		disable();
		// disable keyboard temporarily(must do)
		//  kbcmd(0xad);
		ps2mouse_writemouse(0xe7,-1);
		ps2mouse_setrate(PS2MOUSE_DEFAULT_RATE);
		// enable keyboard again
		//  kbcmd(0xae);
		ps2mouse_setresolution(PS2MOUSE_DEFAULT_RESOLUTION);
		enable();
		break;

		// 640x480, graphic mode,	2 colors
		// --				16 colors
		// 320x200, graphic mode,	256 colors
		case 0x11:
		case 0x12:
		case 0x13:
		default:
		disable();
		ps2mouse_setrate(PS2MOUSE_DEFAULT_RATE2);
		ps2mouse_writemouse(0xe6,-1);
		ps2mouse_setresolution(PS2MOUSE_DEFAULT_RESOLUTION2);
		enable();
		break;
	}
	return;
}

// INIT MOUSE
void ps2mouse_init(void)
{
	int i,found;

	//
	if(ps2mouse_enabled)
	{
		//
		printk("PS/2 mouse driver is already enabled.\n");
		return;
	}

	//
	printk("PS/2 mouse driver 1.0\n");

	//
	printk("Pass #1\r");

	// set safe defaults(will be adjusted to right soon after this)
/*	mouse_setframe(0,0,79,24);
	mouse_setposition(40,12);
	// mouse_hidecursor();
	
*/
	mouse_showcursor();

	//
	printk("Pass #2\r");
	ps2mouse_register_device();

	//
	printk("Pass #3\r");
	for(i=0; i<PS2MOUSE_RETRIES; i++)
	{
		//
		found = ps2mouse_hardwareinit();
		if(!found)break;
		printk(".");
	}

	//
	printk("Pass #4\r");
	ps2mouse_modeChange(dispadmin_getcurrentmode());

	//
	printk("Pass #5\r");
	// After setup, we could show it
	// mouse_hidecursor();

	// Prevent from running the setup again.
	ps2mouse_enabled=1;
}

//






