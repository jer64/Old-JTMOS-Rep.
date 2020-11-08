// vplayGraph.c - Video player graphical functions (calls to 16-bit BIOS)
#include <stdio.h>
#include <dpmi.h>
#include "vplayGraph.h"

// Wait screen retract
void waitsd()
{
 unsigned char m1;
 do
 {
  m1=inportb(0x3da);
 }while(m1&8!=0);
 do
 {
  m1=inportb(0x3da);
 }while((m1&8)==0);
}

void setpalette(unsigned char _color,
     unsigned char _rrr,unsigned char _ggg,unsigned char _bbb)
{
	outportb(0x3c8,_color);
	outportb(0x3c9,_rrr);
	outportb(0x3c9,_ggg);
	outportb(0x3c9,_bbb);
}

// Setup a palette with 256 scales of grey
void greyscale256(void)
{
	int i;

	//
	for(i=0; i<256; i++)
		setpalette(i, i>>3,i>>3,i>>3);
}

//
void openGraphics(void)
{
	int i;

	//
	setvmode(0x13);
	greyscale256();
}

//
void closeGraphics(void)
{
	//
	setvmode(3);
}

// Copy video frame to video memory
void vf(char *__videoframe)
{
	//
	dosmemput(__videoframe, 64000, 0xA0000);
}

// Setup video mode
void setvmode(unsigned _mode)
{
	//
	__dpmi_regs regs;
	regs.x.ax=_mode;
	__dpmi_int(0x10,&regs);
}



