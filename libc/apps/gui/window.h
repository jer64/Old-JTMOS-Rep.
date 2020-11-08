#ifndef __INCLUDED_WINDOW_H__
#define __INCLUDED_WINDOW_H__


// Frame buffer structure
typedef struct
{
	// Free: TRUE or FALSE
	char isfree;

	// Frame buffer location inside
	// the window client area
	int x,y;

	// Frame buffer width, height
	int width,height;

	// Bits per pixel
	int bpp;

	// PTR to the frame buffer
	BYTE *buf;

	// Frame buffer type
	int type;

	// Terminal ID
	int terID;

	// 32-bit checksum values for each scanline
	// in the text terminal.
	// (only used in text terminals)
	DWORD chksum[50];

	// Refresh table for all text lines
	DWORD rtab[50];

	// Last cursor position
	int lcx,lcy;

	// Current cursor position
	int cx,cy;
}FRAMEBUF;

// Button structure
typedef struct
{
	// Properties
	char isfree;
	int pressed;
	int type;	// WINDOW_CLOSE_BUTTON, EXIT_ON_CLICK
	char caption[100];
	char refresh;
	// Coordinates
	// (not always used)
	int x1,y1,x2,y2;
}BUTTON;

// Button structure
typedef struct
{
	// Window ID  ( same as wdb.w[ID] )
	int id;
	// Added to the size of the actual window
	int borderWidth;
	// Used by buttons mostly
	int pressed;
	//
	char isfree;
	int x,y, lx,ly;
	int width,height;
	DWORD type;
	char caption[100];
	char qmsg[210];
	// Drawing priority, windows with lower priority value
	// will be drawn last on the list, and so forth
	// the specified window will be also shown at the bottom
	// of the "window pool".
	// You could also call this "priority" variable as
	// one kind of Z-coordinate for the window.
	int z; // aka priority
	// This variable is triggered to 1 when
	// the window contents or other parameters
	// that affect it's visual approach have
	// been changed.
	// 1 = refresh whole window (borders, caption, etc.)
	// 2 = refresh window's components
	char refresh;
	char refreshButtons;
	// Collision indicator:
	// Indicates that another object has overwritten
	// this window's area, and the window needs to be
	// redrawn in a while.
	int collision;
	// Size of the caption area(height, default=20 pixels)
	int captionHeight;
	// Window closes on click? Values: TRUE / FALSE
	char clickQuit;

	// Client area definition
	struct
	{
		int x,y;
		int width,height;
	}client;

	//
	struct
	{
		//
		BYTE *buf; int l_buf;
		int width,height;
	}clientarea;

	// All buttons this window has
	BUTTON button[10];
	int n_buttons;

	// PID of the owner of this window
	int pid;

	// Frame buffers
	FRAMEBUF framebuf[10];
	int n_framebuffers;
}WINDOW;

//
#include "graosprotocol.h"

#endif

