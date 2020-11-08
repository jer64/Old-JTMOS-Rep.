#ifndef __INCLUDED_GRAOS_H__
#define __INCLUDED_GRAOS_H__

#include <stdio.h>
#include "window.h" // WINDOW -structure
#include "config.h" // Configuration

// Max. window collisions to indicate at once
#define MAX_COLLISION		1024

//
#define RADI		8

//
#define dprintf printf

//
#ifdef DJGPP
#define HOST_OS "DOS (DJGPP)"
#endif

#ifdef WINDOWS
#define HOST_OS "Windows"
#endif

#ifdef LINUX
#define HOST_OS "Linux"
#endif

#ifdef UNIX
#define HOST_OS "UNIX (unknown)"
#endif

#ifdef JTMOS
#define HOST_OS "JTMOS"
#endif

//
typedef struct
{
	BYTE *buf;
	int l_buf,width,height,bpp;
}FONT;

// Video mode aka display structure
typedef struct
{
	// Null space
	long long nothing,nothing2;
	// Key pressed
	int keyPressed,alt,ctrl;
	// Data space
        int width,height,bpp,ulines;
        BYTE *buf,*tmp;
        int l_buf;
	FONT font[10];
	int change;
	int cycle;
	// Y-lines update values
	BYTE dtab[1280];
	// Multiplying table for width*y
	DWORD mtab[1280];
	// 640x480 background image(NULL if none)
	BYTE *background;
	int l_background;
	//
	BYTE taskbarRaw[(640*30)+10];
	BYTE cursorRaw[(32*32+10)];
	BYTE cursorBehind[(32*32+10)];
	char cursorBehindActive;
	//
	int lx,ly, buttons, x,y;

	// Window under cursor
	WINDOW *underwin;

	// Button under cursor
	BUTTON *underbut;

	// Currently selected window
	WINDOW *selwin;

	// Currently moving window
	WINDOW *movwin;

	// Currently moving a window: [TRUE or FALSE]
	int movingWindow;
	// Origo for cursor movement
	int origx,origy;
	// Old window position for moving
	int oldx,oldy;
}VMODE;

int main(int argc, const char **argv);

#include "guiApp.h"
#include "guiTaskbar.h"
#include "guiEnd.h"
#include "guiLoop.h"
#include "guiStart.h"
#include "guiUpdate.h"
#include "guidot.h"
#include "guifont.h"
#include "guifunc.h"
#include "guiBlit.h"
#include "guiRect.h"
#include "wdb.h"
#include "guiDesktopDraw.h"
#include "windowMoving.h"
#include "windowOrder.h"
#include "windowFunc.h"
#include "graosprotocol.h"

#endif

