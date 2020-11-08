//-----------------------------------------------------------------------------
// mouse.c - GENERIC MOUSE DRIVER.
// (C) 2006 Jari Tuominen.
//-----------------------------------------------------------------------------
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
#include "32x32_arrow.h"
#include "dispadmin.h"
#include "sysgfx.h"
#include "driver_sys.h"
#include "delay.h"

//
MOUSE mouse;

//
int mouse_isVisible(void)
{
        return mouse.isVisible;
}


// SET MOUSE FRAME (X1,Y1)-(X2,Y2)
void mouse_setframe(long x1,long y1,long x2,long y2)
{
        mouse.frame.x1=x1;
        mouse.frame.y1=y1;
        mouse.frame.x2=x2;
        mouse.frame.y2=y2;
}

// SET MOUSE POSITION (X,Y)
void mouse_setposition(long x,long y)
{
	//mouse_first_time=1;

        mouse.x=x;
        mouse.y=y;
        mouse.lx=-1;
        mouse.ly=-1;
}

//
// Wrappers:
void mouse_modeChange(int vmode)
{
        switch(vmode)
        {
                // 80x25 textmode, 2 colors
                case 3:
                mouse_setframe(0,0,79,24);
                mouse_setposition(40,12);
                break;

                // 640x480, graphic mode,       2 colors
                // --                           16 colors
                // 320x200, graphic mode,       256 colors
                default:
                case 0x11:
                case 0x12:
                case 0x13:
                mouse_setframe(0,0,639,479);
                mouse_setposition(320,240);
                break;
        }

	// Report to different mouse drivers specifically
	ps2mouse_modeChange(vmode);
}

// Some aliases
void mouse_showmouse(void)
{
        mouse_showcursor();
}
// Some aliases
void mouse_hidemouse(void)
{
        mouse_hidecursor();
}
//

//
void mouse_hidecursor(void)
{
        mouse.isVisible=0;
}

//
void mouse_showcursor(void)
{
        mouse.isVisible=1;
}

