/*
 * ============================================================
 *			VGA GRAPHICS DRIVER
 * Tauron VGA graphics library ported successfully to The JTMOS
 * (C) 2001-2006,2020 by Jari Tuominen.
 * ============================================================
 */

//
#include "kernel32.h"
#include "ega.h"
#include "io.h"
#include "vga.h"
#include "vgapal.h"
#include "sysvga.h"
#include "sysgfx.h" // vga_clearscreen
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h"
#include "dispadmin.h"

//
int graphicModeActive=0;
int visible_before_switch=-1;
int vga_terminal;
Vmode Mode;

// a fix :
#define outport outportw
// the port addresses :
#define SEQ_ADDR 			0x03C4
#define GRACON_ADDR 			0x03CE
#define CRTC_ADDR 			0x03D4






//--------------------------------------------------------------------------------

// Same as Mode 01H
unsigned char mode00h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x67,      0x00,          0x03,0x08,0x03,0x00,0x02,
// CRTC regs
0x2D,0x27,0x28,0x90,0x2B,0xA0,0xBF,0x1F,0x00,0x4F,0x06,0x07,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x1F,0x96,0xB9,0xA3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x08,0x00
};

// Same as Mode 02H
unsigned char mode03h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x67,      0x00,          0x03,0x00,0x03,0x00,0x02,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x55,0x81,0xBF,0x1F,0x00,0x4F,0x0E,0x0F,0x00,0x00,0x00,
0x00,0x9C,0x0E,0x8F,0x28,0x01,0x96,0xB9,0xA3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x08,0x00
};

// Same as Mode 05H
unsigned char mode04h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,          0x03,0x09,0x03,0x00,0x02,
// CRTC regs
0x2D,0x27,0x28,0x90,0x2B,0x80,0x0BF,0x1F,0x00,0x0C1,0x00,0x00,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x00,0x96,0xB9,0x0A2,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x30,0x0F,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x3B,0x3D,0x3F,0x02,0x04,0x06,0x07,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
0x01,0x00,0x03,0x00,0x00
};

unsigned char mode06h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,      0x03,0x01,0x01,0x00,0x06,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x0C1,0x00,0x00,0x00,0x00,0x00,
0x00,0x9C,0x8E,0x8F,0x28,0x00,0x96,0xB9,0x0C2,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x01,0x00,0x01,0x00,0x00
};

unsigned char mode07h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x066,     0x00,          0x03,0x00,0x03,0x00,0x02,
// CRTC regs
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0A,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x0E,0x00,0x0F,0x00,0x00
};

unsigned char mode0Dh[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x063,     0x00,          0x03,0x09,0x0F,0x00,0x06,
// CRTC regs
0x2D,0x27,0x28,0x90,0x2B,0x80,0x0BF,0x1F,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,
0x31,0x9C,0x8E,0x8F,0x14,0x00,0x96,0xB9,0xE3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x05,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x01,0x00,0x0F,0x00,0x00
};

unsigned char mode0Eh[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,          0x03,0x01,0x0F,0x00,0x06,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,
0x59,0x9C,0x8E,0x8F,0x28,0x00,0x96,0xB9,0xE3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x08,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x01,0x00,0x0F,0x00,0x00
};

unsigned char mode0Fh[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x0A2,     0x00,          0x03,0x01,0x0F,0x00,0x06,
// CRTC regs
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x08,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x18,0x00,0x00,
0x0B,0x00,0x05,0x00,0x00
};

unsigned char mode10h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x0A3,     0x00,          0x03,0x01,0x0F,0x00,0x06,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x40,0x00,0x00,0x00,0x00,0x00,
0x00,0x83,0x85,0x5D,0x28,0x0F,0x63,0x0BA,0xE3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x01,0x00,0x0F,0x00,0x00
};

unsigned char mode11h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0xE3,     0x00,          0x03,0x01,0x0F,0x00,0x06,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0B,0x3E,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x59,
0xEA,0x8C,0x0DF,0x28,0x0F,0x0E7,0x004,0x0C3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x08,0x3F,0x3F,0x18,0x18,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x01,0x00,0x0F,0x00,0x00
};

unsigned char mode12h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0xE3,     0x00,          0x03,0x01,0x0F,0x00,0x06,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0B,0x3E,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x59,
0xEA,0x8C,0x0DF,0x28,0x00,0x0E7,0x04,0xE3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x0F,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x01,0x00,0x0F,0x00,0x00
};

unsigned char mode13h[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,          0x03,0x01,0x0F,0x00,0x0E,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x41,0x00,0x00,0x00,0x00,0x00,
0x00,0x9C,0x0E,0x8F,0x28,0x40,0x96,0xB9,0x0A3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x40,0x05,0x0F,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x41,0x00,0x0F,0x00,0x00
};

unsigned char modeC4[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,          0x03,0x01,0x0F,0x00,0x06,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,
0x9C,0x0E,0x8F,0x28,0x00,0x96,0xB9,0xE3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x40,0x05,0x0F,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x41,0x00,0x0F,0x00,0x00
};

unsigned char modeJ[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0xa3,      0x00,          0x03,0x01,0x03,0x00,0x02,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x55,0x81,0xBF,0x1F,0x00,0x47,0x0E,0x0F,0x00,0x00,0x00,
0x00,0x83,0x85,0x57,0x28,0x1F,0x60,0xB8,0xA3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00
};

unsigned char modeK[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,          0x03,0x01,0x03,0x00,0x02,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x55,0x81,0xBF,0x1F,0x00,0x47,0x0E,0x0F,0x00,0x00,0x00,
0x00,0x9C,0x8E,0x8F,0x28,0x1F,0x96,0xB9,0xA3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00
};

unsigned char modeL[62] = {
// MISC reg,  STATUS reg,    SEQ regs
0x67,0x00,0x03,0x08,0x03,0x00,0x02,
// CRTC regs
0x2D,0x27,0x28,0x90,0x2B,0x0A0,0x0BF,0x1F,0x00,0x47,0x06,0x07,0x00,0x00,0x00,
0x31,0x83,0x85,0x57,0x14,0x1F,0x60,0x0B8,0x0A3,0x0FF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0x0FF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x10,0x11,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00,
};

unsigned char modeM[62] = {
// MISC reg,  STATUS reg,    SEQ regs
0x67,0x00,0x03,0x08,0x03,0x00,0x02,
// CRTC regs
0x2D,0x27,0x28,0x90,0x2B,0x0A0,0x0BF,0x1F,0x00,0x47,0x06,0x07,0x00,0x00,0x00,0x31,
0x9C,0x8E,0x8F,0x14,0x1F,0x96,0x0B9,0x0A3,0x0FF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x10,0x0E,0x00,0x0FF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x14,0x07,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x0C,0x00,0x0F,0x00,0x00,
};



// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int vga_device_call(DEVICE_CALL_PARS)
{
	return 0;
}

// Registers device 'vga'
int vga_register_device(void)
{
 // Check that the device isn't already registered
 if( driver_getdevicebyname("vga") )
 {
  print("cannot register device vga: device already registered");
  return 1;
 }

 // Register the device
 driver_register_new_device("vga",vga_device_call,DEVICE_TYPE_BLOCK);
 
 //
 return 0;
}

// Initialize VGA driver
void vga_init(void)
{
	// Create seperate console for the VGA display
	vga_terminal = NewTerminal();

	// Register device
	vga_register_device();
}

// Note: ReadBIOSfont was not ported due to DOS dependent behavior.
void ReadBIOSfont(int a,int b)
{
	//
}

// Obviously this function setups a videomode :-)
void SetMode(const unsigned char *p)
{
   //
   long i;

 
   // Send the first two regs.
   outportb( MISC_ADDR,		p[0] );
   outportb( STATUS_ADDR,	p[1] );
   
 
   // Send the seq. regs!
   for(i=0; i<5; i++)
   {
    //
    outportb( SEQ_ADDR+0, i      );
    outportb( SEQ_ADDR+1, p[2+i] );
   }
   
 
   // Clear protection bits!
   outportw( CRTC_ADDR, ( 0x0e00 & 0x7f00 ) | 0x11 );
 
   
   // Send CRTC regs.
   for(i=0; i<25; i++)
   {
    //
    outportb( CRTC_ADDR+0, i );
    outportb( CRTC_ADDR+1, p[2+5+i] );
   }
 
   
   // Send GRAPHICS regs.
   for(i=0; i<9; i++)
   {
    //
    outportb( GRACON_ADDR+0, i );
    outportb( GRACON_ADDR+1, p[2+5+25+i] );
   }
 
   
   // Make a dummy read from STATUS_ADDR.
   inportb(STATUS_ADDR);
   
   // Send ATTRCON regs.
   for(i=0; i<21; i++)
   {
    // Read ATTRCON_ADDR+0 once.
    inportb( ATTRCON_ADDR+0 );
    // Send data.
    outportb( ATTRCON_ADDR+0, i );
    outportb( ATTRCON_ADDR+0, p[2+5+25+9+i] );
   }
   
   //
   outportb( ATTRCON_ADDR+0, 0X20 );
}

void SetVideoMode(int mode)
{
   //
   int val;

   //
   Mode.mode = mode;
   
   //
   if (mode == MODE00H)                        // 40 x 25 x 16
   {
      SetMode((unsigned int)&mode00h);
      setpalette16();
      ReadBIOSfont(6,16);

      Mode.width = 40;
      Mode.height = 25;
      Mode.width_bytes = 1000;
      Mode.colors = 16;
      Mode.attrib = TVU_TEXT;
   }
   else if (mode == MODE03H)                   // 80 x 25 x 16
   {
      SetMode((unsigned int)&mode03h);
      setpalette16();
      ReadBIOSfont(6,16);
      setup_systemfont();
      clrscr();
      setpalette16();
      VisibleTerminals_SetHWCopy(1);
	if(saved_terminal!=-1)
   	{
		SetVisibleTerminal(saved_terminal);
		saved_terminal=0;
	}

      Mode.width = 80;
      Mode.height = 25;
      Mode.width_bytes = 2000;
      Mode.colors = 16;
      Mode.attrib = TVU_TEXT;
   }
   else if (mode == MODE04H)                   // 320 x 200 x 4
   {
      SetMode((unsigned int)&mode04h);
      setpalette4();
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 320;
      Mode.height = 200;
      Mode.width_bytes = 8192;
      Mode.colors = 4;
      Mode.attrib = TVU_GRAPHICS;
   }
   else if (mode == MODE06H)                    // 640 x 200 x 2
   {
      SetMode((unsigned int)&mode06h);
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 640;
      Mode.height = 200;
      Mode.width_bytes = 8192;
      Mode.colors = 2;
      Mode.attrib = TVU_GRAPHICS;
   }
   else if (mode == MODE07H)                    // 80 x 25 x 2
   {
      SetMode((unsigned int)&mode07h);

      Mode.width = 80;
      Mode.height = 25;
      Mode.width_bytes = 2000;
      Mode.colors = 2;
      Mode.attrib = TVU_TEXT | TVU_MONOCHROME;
   }
   else if (mode == MODE0DH)                    // 320 x 200 x 16
   {
      SetMode((unsigned int)&mode0Dh);
      setpalette16();
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 320;
      Mode.height = 200;
      Mode.width_bytes = 8000;
      Mode.colors = 16;
      Mode.attrib = TVU_GRAPHICS | TVU_PLANAR;
   }
   else if (mode == MODE0EH)                    // 640 x 200 x 16
   {
      SetMode((unsigned int)&mode0Eh);
      setpalette16();
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 640;
      Mode.height = 200;
      Mode.width_bytes = 16000;
      Mode.colors = 16;
      Mode.attrib = TVU_GRAPHICS | TVU_PLANAR;
   }
   else if (mode == MODE0FH)                    // 640 x 350 x 2
   {
      SetMode((unsigned int)&mode0Fh);
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 640;
      Mode.height = 350;
      Mode.width_bytes = 28000;
      Mode.colors = 2;
      Mode.attrib = TVU_GRAPHICS | TVU_MONOCHROME;
   }
   else if (mode == MODE10H)                    // 640 x 350 x 16
   {
      SetMode((unsigned int)&mode10h);
      setpalette16();
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 640;
      Mode.height = 350;
      Mode.width_bytes = 28000;
      Mode.colors = 16;
      Mode.attrib = TVU_GRAPHICS | TVU_PLANAR;
   }
   else if (mode == MODE11H)                    // 640 x 480 x 2
   {
	saved_terminal = visible_terminal;
	SetVisibleTerminal(0);

      SetMode((unsigned int)&mode11h);
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 640;
      Mode.height = 480;
      Mode.width_bytes = 38400u;
      Mode.colors = 2;
      Mode.attrib = TVU_GRAPHICS | TVU_PLANAR;

      vga_clearscreen();
   }
   else if (mode == MODE12H)                    // 640 x 480 x 16
   {
	saved_terminal = visible_terminal;
	SetVisibleTerminal(0);

      SetMode((unsigned int)&mode12h);
      setpalette16();
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 640;
      Mode.height = 480;
      Mode.width_bytes = 38400u;
      Mode.colors = 16;
      Mode.attrib = TVU_GRAPHICS | TVU_PLANAR;

      vga_clearscreen();
   }
   else if (mode == MODE13H)                    // 320 x 200 x 256
   {
	saved_terminal = visible_terminal;
	SetVisibleTerminal(0);

      SetMode((unsigned int)&mode13h);
      setpalette256();
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 320;
      Mode.height = 200;
      Mode.width_bytes = 64000u;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS;
   }
   else if (mode == CHAIN4)                     // unchained 320 x 200 x 256
   {
      SetMode((unsigned int)&modeC4);
      setpalette256();
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 320;
      Mode.height = 200;
      Mode.width_bytes = 16000;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_X)                     // unchained 320 x 240 x 256
   {
      SetMode((unsigned int)&modeC4);
      VisibleTerminals_SetHWCopy(0);

      outportb(MISC_ADDR,0xE3);
      // turn off write protect
      outport(CRTC_ADDR,0x2C11);
      // vertical total
      outport(CRTC_ADDR,0x0D06);
      // overflow register
      outport(CRTC_ADDR,0x3E07);
      // vertical retrace start
      outport(CRTC_ADDR,0xEA10);
      // vertical retrace end AND wr.prot
      outport(CRTC_ADDR,0xAC11);
      // vertical display enable end
      outport(CRTC_ADDR,0xDF12);
      // start vertical blanking
      outport(CRTC_ADDR,0xE715);
      // end vertical blanking
      outport(CRTC_ADDR,0x0616);

      setpalette256();
      Mode.width = 320;
      Mode.height = 240;
      Mode.width_bytes = 19200;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_A)                     // unchained 320 x 350 x 256
   {
      SetMode((unsigned int)&modeC4);
      VisibleTerminals_SetHWCopy(0);

      // turn off double scanning mode
      outportb(CRTC_ADDR,9);
      outportb(CRTC_ADDR+1,inportb(CRTC_ADDR+1) & ~0x1F);
      // change the vertical resolution flags to 350
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0x80);
      // turn off write protect
      outport(CRTC_ADDR,0x2C11);
      // vertical total
      outport(CRTC_ADDR,0xBF06);
      // overflow register
      outport(CRTC_ADDR,0x1F07);
      // vertical retrace start
      outport(CRTC_ADDR,0x8310);
      // vertical retrace end AND wr.prot
      outport(CRTC_ADDR,0x8511);
      // vertical display enable end
      outport(CRTC_ADDR,0x5D12);
      // start vertical blanking
      outport(CRTC_ADDR,0x6315);
      // end vertical blanking
      outport(CRTC_ADDR,0xBA16);

      setpalette256();
      Mode.width = 320;
      Mode.height = 350;
      Mode.width_bytes = 28000u;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_B)                     // unchained 320 x 400 x 256
   {
      SetMode((unsigned int)&modeC4);
      VisibleTerminals_SetHWCopy(0);
      // turn off double scanning mode
      outportb(CRTC_ADDR,9);
      outportb(CRTC_ADDR+1,inportb(CRTC_ADDR+1) & ~0x1F);
      // change the vertical resolution flags to 400
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0x40);

      setpalette256();
      Mode.width = 320;
      Mode.height = 400;
      Mode.width_bytes = 32000;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_C)                     // unchained 320 x 480 x 256
   {
      SetMode((unsigned int)&modeC4);
      VisibleTerminals_SetHWCopy(0);

      // turn off double scanning mode
      outportb(CRTC_ADDR,9);
      outportb(CRTC_ADDR+1,inportb(CRTC_ADDR+1) & ~0x1F);
      // change the vertical resolution flags to 480
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0xC0);
      // turn off write protect
      outport(CRTC_ADDR,0x2C11);
      // vertical total
      outport(CRTC_ADDR,0x0D06);
      // overflow register
      outport(CRTC_ADDR,0x3E07);
      // vertical retrace start
      outport(CRTC_ADDR,0xEA10);
      // vertical retrace end AND wr.prot
      outport(CRTC_ADDR,0xAC11);
      // vertical display enable end
      outport(CRTC_ADDR,0xDF12);
      // start vertical blanking
      outport(CRTC_ADDR,0xE715);
      // end vertical blanking
      outport(CRTC_ADDR,0x0616);

      setpalette256();
      Mode.width = 320;
      Mode.height = 480;
      Mode.width_bytes = 38400u;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_D)                     // unchained 360 x 200 x 256
   {
      SetMode((unsigned int)&mode13h);
      VisibleTerminals_SetHWCopy(0);

      // Turn off Chain 4
      outport(SEQ_ADDR,0x0604);
      // Activate a synchronous reset
      outport(SEQ_ADDR,0x0100);
      // Select 28 mhz pixel clock
      outportb(MISC_ADDR,0xE7);
      // Release synchronous reset
      outport(SEQ_ADDR,0x0300);

      // change the vertical resolution flags to 400
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0x40);

      // turn off write protect
      outport(CRTC_ADDR,0x2C11);

      //
      outport(CRTC_ADDR,0x6B00);
      outport(CRTC_ADDR,0x5901);
      outport(CRTC_ADDR,0x5A02);
      outport(CRTC_ADDR,0x8E03);
      outport(CRTC_ADDR,0x5E04);
      outport(CRTC_ADDR,0x8A05);
      outport(CRTC_ADDR,0x0008);
      outport(CRTC_ADDR,0xC009);
      outport(CRTC_ADDR,0x000A);
      outport(CRTC_ADDR,0x000B);
      outport(CRTC_ADDR,0x000C);
      outport(CRTC_ADDR,0x000D);
      outport(CRTC_ADDR,0x000E);
      outport(CRTC_ADDR,0x000F);
      outport(CRTC_ADDR,0xAC11);
      outport(CRTC_ADDR,0x2D13);
      outport(CRTC_ADDR,0x0014);
      outport(CRTC_ADDR,0xE317);
      outport(CRTC_ADDR,0xFF18);

      //
      setpalette256();
      Mode.width = 360;
      Mode.height = 200;
      Mode.width_bytes = 18000u;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_E)                     // unchained 360 x 240 x 256
   {
      //
      SetMode((unsigned int)&mode13h);
      VisibleTerminals_SetHWCopy(0);

      // Turn off Chain 4
      outport(SEQ_ADDR,0x0604);
      // Activate a synchronous reset
      outport(SEQ_ADDR,0x0100);
      // Select 28 mhz pixel clock
      outportb(MISC_ADDR,0xE7);
      // Release synchronous reset
      outport(SEQ_ADDR,0x0300);

      // change the vertical resolution flags to 480
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0xC0);

      // turn off write protect
      outport(CRTC_ADDR,0x2C11);

      //
      outport(CRTC_ADDR,0x6B00);
      outport(CRTC_ADDR,0x5901);
      outport(CRTC_ADDR,0x5A02);
      outport(CRTC_ADDR,0x8E03);
      outport(CRTC_ADDR,0x5E04);
      outport(CRTC_ADDR,0x8A05);
      outport(CRTC_ADDR,0x0D06);
      outport(CRTC_ADDR,0x3E07);
      outport(CRTC_ADDR,0x0008);
      outport(CRTC_ADDR,0xC009);
      outport(CRTC_ADDR,0x000A);
      outport(CRTC_ADDR,0x000B);
      outport(CRTC_ADDR,0x000C);
      outport(CRTC_ADDR,0x000D);
      outport(CRTC_ADDR,0x000E);
      outport(CRTC_ADDR,0x000F);
      outport(CRTC_ADDR,0xEA10);
      outport(CRTC_ADDR,0xAC11);
      outport(CRTC_ADDR,0xDF12);
      outport(CRTC_ADDR,0x2D13);
      outport(CRTC_ADDR,0x0014);
      outport(CRTC_ADDR,0xE715);
      outport(CRTC_ADDR,0x0616);
      outport(CRTC_ADDR,0xE317);
      outport(CRTC_ADDR,0xFF18);

      setpalette256();
      Mode.width = 360;
      Mode.height = 240;
      Mode.width_bytes = 21600;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_F)                     // unchained 360 x 350 x 256
   {
      SetMode((unsigned int)&mode13h);
      VisibleTerminals_SetHWCopy(0);

      // Turn off Chain 4
      outport(SEQ_ADDR,0x0604);
      // Activate a synchronous reset
      outport(SEQ_ADDR,0x0100);
      // Select 28 mhz pixel clock
      outportb(MISC_ADDR,0xE7);
      // Release synchronous reset
      outport(SEQ_ADDR,0x0300);

      // change the vertical resolution flags to 350
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0x80);

      // turn off write protect
      outport(CRTC_ADDR,0x2C11);

      outport(CRTC_ADDR,0x6B00);
      outport(CRTC_ADDR,0x5901);
      outport(CRTC_ADDR,0x5A02);
      outport(CRTC_ADDR,0x8E03);
      outport(CRTC_ADDR,0x5E04);
      outport(CRTC_ADDR,0x8A05);
      outport(CRTC_ADDR,0xBF06);
      outport(CRTC_ADDR,0x1F07);
      outport(CRTC_ADDR,0x0008);
      outport(CRTC_ADDR,0x4009);
      outport(CRTC_ADDR,0x000A);
      outport(CRTC_ADDR,0x000B);
      outport(CRTC_ADDR,0x000C);
      outport(CRTC_ADDR,0x000D);
      outport(CRTC_ADDR,0x000E);
      outport(CRTC_ADDR,0x000F);
      outport(CRTC_ADDR,0x8310);
      outport(CRTC_ADDR,0x8511);
      outport(CRTC_ADDR,0x5D12);
      outport(CRTC_ADDR,0x2D13);
      outport(CRTC_ADDR,0x0014);
      outport(CRTC_ADDR,0x6315);
      outport(CRTC_ADDR,0xBA16);
      outport(CRTC_ADDR,0xE317);
      outport(CRTC_ADDR,0xFF18);

      setpalette256();
      Mode.width = 360;
      Mode.height = 350;
      Mode.width_bytes = 31500;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_G)                     // unchained 360 x 400 x 256
   {
      SetMode((unsigned int)&mode13h);
      VisibleTerminals_SetHWCopy(0);

      // Turn off Chain 4
      outport(SEQ_ADDR,0x0604);
      // Activate a synchronous reset
      outport(SEQ_ADDR,0x0100);
      // Select 28 mhz pixel clock
      outportb(MISC_ADDR,0xE7);
      // Release synchronous reset
      outport(SEQ_ADDR,0x0300);

      // change the vertical resolution flags to 400
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0x40);

      // turn off write protect
      outport(CRTC_ADDR,0x2C11);

      outport(CRTC_ADDR,0x6B00);
      outport(CRTC_ADDR,0x5901);
      outport(CRTC_ADDR,0x5A02);
      outport(CRTC_ADDR,0x8E03);
      outport(CRTC_ADDR,0x5E04);
      outport(CRTC_ADDR,0x8A05);
      outport(CRTC_ADDR,0x0008);
      outport(CRTC_ADDR,0x4009);
      outport(CRTC_ADDR,0x000A);
      outport(CRTC_ADDR,0x000B);
      outport(CRTC_ADDR,0x000C);
      outport(CRTC_ADDR,0x000D);
      outport(CRTC_ADDR,0x000E);
      outport(CRTC_ADDR,0x000F);
      outport(CRTC_ADDR,0xAC11);
      outport(CRTC_ADDR,0x2D13);
      outport(CRTC_ADDR,0x0014);
      outport(CRTC_ADDR,0xE317);
      outport(CRTC_ADDR,0xFF18);

      setpalette256();
      Mode.width = 360;
      Mode.height = 400;
      Mode.width_bytes = 36000u;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_H)                     // unchained 360 x 480 x 256
   {
      SetMode((unsigned int)&mode13h);
      VisibleTerminals_SetHWCopy(0);

      // Turn off Chain 4
      outport(SEQ_ADDR,0x0604);
      // Activate a synchronous reset
      outport(SEQ_ADDR,0x0100);
      // Select 28 mhz pixel clock
      outportb(MISC_ADDR,0xE7);
      // Release synchronous reset
      outport(SEQ_ADDR,0x0300);

      // change the vertical resolution flags to 480
      outportb(MISC_ADDR,(inportb(0x3CC) & ~0xC0) | 0xC0);

      // turn off write protect
      outport(CRTC_ADDR,0x2C11);

      outport(CRTC_ADDR,0x6B00);
      outport(CRTC_ADDR,0x5901);
      outport(CRTC_ADDR,0x5A02);
      outport(CRTC_ADDR,0x8E03);
      outport(CRTC_ADDR,0x5E04);
      outport(CRTC_ADDR,0x8A05);
      outport(CRTC_ADDR,0x0D06);
      outport(CRTC_ADDR,0x3E07);
      outport(CRTC_ADDR,0x0008);
      outport(CRTC_ADDR,0x4009);
      outport(CRTC_ADDR,0x000A);
      outport(CRTC_ADDR,0x000B);
      outport(CRTC_ADDR,0x000C);
      outport(CRTC_ADDR,0x000D);
      outport(CRTC_ADDR,0x000E);
      outport(CRTC_ADDR,0x000F);
      outport(CRTC_ADDR,0xEA10);
      outport(CRTC_ADDR,0xAC11);
      outport(CRTC_ADDR,0xDF12);
      outport(CRTC_ADDR,0x2D13);
      outport(CRTC_ADDR,0x0014);
      outport(CRTC_ADDR,0xE715);
      outport(CRTC_ADDR,0x0616);
      outport(CRTC_ADDR,0xE317);
      outport(CRTC_ADDR,0xFF18);

      setpalette256();
      Mode.width = 360;
      Mode.height = 480;
      Mode.width_bytes = 43200u;
      Mode.colors = 256;
      Mode.attrib = TVU_GRAPHICS | TVU_UNCHAINED;
   }
   else if (mode == MODE_I)                     // 640 x 400 x 16
   {
      //
      SetMode((unsigned int)&mode10h);
      VisibleTerminals_SetHWCopy(0);
      
      //
      val = inportb( 0x03cc );
      val = (val & 0x3f) | 0x40;
      //
      outportb( 0x03c2, val&0xff );

      //
      outportw( CRTC_ADDR, 0x9c10 );
      outportw( CRTC_ADDR, 0x8311 );
      outportw( CRTC_ADDR, 0x8f12 );
      outportw( CRTC_ADDR, 0x9615 );
      outportw( CRTC_ADDR, 0xb916 );

      //
      setpalette16();
      Mode.width = 640;
      Mode.height = 400;
      Mode.width_bytes = 32000;
      Mode.colors = 16;
      Mode.attrib = TVU_GRAPHICS | TVU_PLANAR;
   }
   else if (mode == MODE_J)                    // 80 x 43 x 16
   {
      SetMode((unsigned int)&modeJ);
      ReadBIOSfont(3,8);
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 80;
      Mode.height = 43;
      Mode.width_bytes = 3440;
      Mode.colors = 16;
      Mode.attrib = TVU_TEXT;
   }
   else if (mode == MODE_K)                    // 80 x 50 x 16
   {
      SetMode((unsigned int)&modeK);
      ReadBIOSfont(3,8);
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 80;
      Mode.height = 50;
      Mode.width_bytes = 4000;
      Mode.colors = 16;
      Mode.attrib = TVU_TEXT;
   }
   else if (mode == MODE_L)                    // 40 x 43 x 16
   {
      SetMode((unsigned int)&modeL);
      ReadBIOSfont(3,8);
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 40;
      Mode.height = 43;
      Mode.width_bytes = 4000;
      Mode.colors = 16;
      Mode.attrib = TVU_TEXT;
   }
   else if (mode == MODE_M)                    // 40 x 50 x 16
   {
      SetMode((unsigned int)&modeM);
      ReadBIOSfont(3,8);
      VisibleTerminals_SetHWCopy(0);

      Mode.width = 40;
      Mode.height = 50;
      Mode.width_bytes = 4000;
      Mode.colors = 16;
      Mode.attrib = TVU_TEXT;
   }
}

void setpal(int color, BYTE r, BYTE g, BYTE b)
{
	outportb( 0x03c8, color );
	outportb( 0x03c9, r );
	outportb( 0x03c9, g );
	outportb( 0x03c9, b );
}

// Just a wrapper
void setpalette(int c, int r, int g, int b)
{
	setpal(c, r,g,b);
}

void setpalette4(void)
{
	setpal( 0,  0,  0,  0);
	setpal( 1,  0, 42, 42);
	setpal( 2, 42,  0, 42);
	setpal( 3, 63, 63, 63);
}

void setpalette16(void)
{
	int i,j;
	BYTE stdegapal[]={
		0,0,0,
		0,0,170,
		0,170,0,
		0,170,170,
		170,0,0,
		170,0,170,
		170,85,0,
		170,170,170,
		85,85,85,
		85,85,255,
		85,255,85,
		85,255,255,
		255,85,85,
		255,85,255,
		255,255,85,
		255,255,255
	};

	//panic("SETPALETTE16");
	for(i=0; i<16; i++)
	{
		setpalette(i,
			stdegapal[i*3+0]>>2,
			stdegapal[i*3+1]>>2,
			stdegapal[i*3+2]>>2);
	}
}

//
void setpalette256(void)
{
	int i,j;
   
	for(i=0,j=0; i<768; i+=3)
	{
		setpal(j, Pal[i]>>2, Pal[i+1]>>2, Pal[i+2]>>2);
		j++;
	}
}

// Set video mode.
void setvmode(int which)
{
	//
	if(which!=3)
		visible_before_switch = GetVisibleTerminal();

	//
	dispadmin_reportmodechange(which);
	if(vesa_frame_buf==NULL)
		SetVideoMode(which);
	mouse_modeChange(which);
	if(which!=3)
		graphicModeActive=1;
		else
		graphicModeActive=0;
}

