//=====================================================
// DISPADMIN - JTMOS DISPLAY ADMINISTRATION SYSTEM
// -----------------------------------------------
//
// (C) 2001-2003 by Jari Tuominen
//
// This program is a heart of JTMOS display
// handling, it is used to figure out which
// video mode is currently set,
// also VGA/EGA drivers call disp.admin.
// to inform of mode changes.
//=====================================================
#include "kernel32.h"
#include "dispadmin.h"
#include "sysgfx.h"

//
static DISPADMIN ds;

// Defines current video mode
void dispadmin_reportmodechange(int vmode)
{
	//
	ds.vmode = vmode;
	return;
}

// Returns current video mode
int dispadmin_getcurrentmode(void)
{
	//
	return ds.vmode;
}

//
DISPADMIN *dispadmin_getstr(void)
{
	//
	return &ds;
}

// XORPIXEL FOR MODE11
void dispadmin_xorpixel_m11(int x,int y)
{
	//
	vga_xorpixel_m11(x,y);
}

//
void dispadmin_xorpixel(int x,int y)
{
	//
	switch(ds.vmode)
	{
		//
		case 0x11:
		dispadmin_xorpixel_m11(x,y);
		break;
	}
}
