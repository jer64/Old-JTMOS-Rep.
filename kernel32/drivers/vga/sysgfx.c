/*** System Graphic Routines ***
 *
 * This library is a part of vga.c,
 * vga.c handles the mode changes,
 * this library mostly handles the rest.
 *
 * Like drawing on the screen, 
 * clearing screen, etcz.
 *
 */
#include "sysgfx.h"
#include "vga.h"
#include "basdef.h"
#include "kernel32.h"

//
static int current_vga_plane=-1;

//
void vga_xorpixel_m11(long x,long y)
{
 //
 long ad,ad2,ad3,ad4,i,i2,i3,i4;
 BYTE bits[]={128,64,32,16,8,4,2,1};
 BYTE *dis;

 //
// if(dispadmin_getcurrentmode()!=0x11)return;

 //
 dis=0xa0000;

 //
 if(y<480 && x<640 && x>-1 && y>-1)
 {
  //
  ad=640*y;
  //
  dis[(ad+x)>>3]^=bits[x&7];
 // dis[(ad+x)>>3]^=bits[x&7];
 }

 // 
}

// Example:
// setpalettemap(0, 256, myPaletteData);
//
void setpalettemap(int offs, int amount, BYTE *paldata)
{
	int i,i2;

	// Mask all registers
	outportb(0x3C6, 0xff);

	// Go to offset
	outportb(0x3C8, offs);

	//
	for(i=0,i2=0; i<(amount*3); i++,i2++)
	{
		outportb(0x3C9, paldata[i2]);
	}
}

//
void setpaletteEx(int c, int r,int g,int b)
{
	int i;

	// Mask all registers so we can update any of them.
	outportb(0x03C6, 0xFF);

	// Send RGB.
	outportb(0x3C8, c);
	outportb(0x3C9, r);
	outportb(0x3C9, g);
	outportb(0x3C9, b);
}

//
void setvgaplane(int which)
{
	//
	if(current_vga_plane!=which)
	{
	        setwplane(which & 3);
	        setrplane(which & 3);
		current_vga_plane = which;
	}
}

// SET VIDEO BANK (VGA 640x480 16c...)
void setvbank(int which)
{
	//
	char tab[]={1,2,4,8,16,32,64,128};
 
	//
	outportb(0x3CE,4);
	outportb(0x3CF,which);
// outportw( 0x3ce,0xff08 );
// outportw( 0x3c4,(tab[which&3])<<8 | 2 );
// outportw( 0x3c4,0x0f02 );
// outportw( 0x3ce,0xff08 );
}

//
void vga_clearscreen(void)
{
	//
	clearvscreen();
}

// CLEAR VIDEO SCREEN
// Currently only supports videomode 0x11(640x480x2)
//
void clearvscreen(void)
{
 //
 long i,bank;
 unsigned char *p;

 //
 p=0xa0000;

 // 640x480x16
 if(dispadmin_getcurrentmode()==0x12)
 {
  //
  for(bank=0; bank<4; bank++)
  {
   //
   setvbank(bank);
   
   //
   for(i=0; i<38400; i++) { p[i]=0; }
  }
 }

 // 640x480x2
 if(dispadmin_getcurrentmode()==0x11)
 {
  //
  for(i=0; i<38400; i++)p[i]=0;
 }

 //
}

void vga_waitretrace(void)
{
	BYTE m1;

	// wait until done with vertical retrace
/*	do
	{
		m1=inportb(0x3da);
	}while(m1&8!=0);*/
	// wait until done refreshing
	do
	{
		m1=inportb(0x3da);
	}while((m1&8)==0);
}
