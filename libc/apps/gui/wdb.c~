// ----------------------------------------------------------------
//   		wdb.c - GRAOS Window Database
//   (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
// ----------------------------------------------------------------
#include <stdio.h>
#include <conio.h> // Color defs
#include "graos.h"
#include "wdb.h"

//--------------------------------------------------------------
// Create Window Database Structure
WDB wdb;

//
static winposx=0,winposy=0;

// Get window automatic position
void autoGetWinPos(VMODE *v,
	int *x,int *y, int width,int height)
{
	// Rewind back on boundary skips
	if( (winposx+width) >= v->width )
		winposx=0;
	if( (winposy+height) >= v->height )
		winposy=0;

	// Set auto position
	*x = winposx;
	*y = winposy;

	// Increase autoposition for next time
	winposx+=16; winposy+=16;
}	

//--------------------------------------------------------------
// Switch to next window on list
// (CTRL+TAB)
//
void switchToNextWindow(VMODE *v)
{
	WINDOW *w,*nw;

	// Get current window
	if( (w = v->selwin)==NULL )
	{
		// If no window selected, get any window from the WDB
		w = getAnyWindow(0);
	}

	// Get next window on the list
	nw = getNextWindow(w);

	//------------------------------------------
	// Select the window
	if(nw)
		SelectWindow(v, nw);
}

//--------------------------------------------------------------
// Return pointer to specified
// window pointed by window ID.
WINDOW *getWindowPTR(int wid)
{
	//
	return &wdb.w[wid];
}

// Searches for a window and returns PTR to it,
// begID defines the first window entry
// to begin the search at.
WINDOW *getAnyWindow(int begID)
{
	int id,i;
	WINDOW *ww;

	// Get ID
	id = begID;

	// Exceeding amount of windows?
	if(id >= wdb.n_windows)
	{
		// Rewind to 0
		id = 0;
	}

	// Search for first non-free window
	for(i=id; i<wdb.n_windows; i++)
	{
		// Get window PTR
		ww = &wdb.w[i];

		// This window is active?
		if(ww->isfree==FALSE)
			// Return the window PTR
			return ww;
	}

	//
	return NULL;
}

// Get next window, starting from the specified window
// Returns NULL if none found
WINDOW *getNextWindow(WINDOW *w)
{
	int nid;
	WINDOW *ww;

	// Handle NULL PTRs
	if(w==NULL)
		// Begin from zero
		nid = 0;
	else
		// Get next linear ID
		nid = w->id+1;

	// Search for the window
	if( (ww=getAnyWindow(nid))!=NULL )
		// Return handle
		return ww;
	else
		// Try searching from the beginning
		return getAnyWindow(0);
}

// Select a window, so it becomes on top
void SelectWindow(VMODE *v, WINDOW *w)
{
	// Set previously selwin (if any) to refresh status
	if(v->selwin)
		v->selwin->refresh = 1|2; // window + components

	// Move window on top of all windows
	putWindowOnTop(v, w);

	// Select new window
	v->selwin = w;

	// Set window to refresh
	w->refresh = 1|2; // window + components
}

// Returns window ID(wid)
int guiCreateWindow(VMODE *v,
		int x,int y,
			int width,int height,
			const char *caption,
			int type)
{
	int id,i,i2,i3,i4;
	WINDOW *w;
	static char str[256];

	//
	sprintf(str, "%s: creating new window:\n",
		__FUNCTION__); DebugMessage(str);
	sprintf(str, "%s: x=%d, y=%d, width=%d, height=%d, caption='%s'\n",
		__FUNCTION__,
		x,y,width,height,caption); DebugMessage(str);

	//
	if(caption==NULL)return 2;

	// Get new window ID
	id = emptyWDB(); if(id==-1)return 1;
	if( (w=GetWindowPTR(id))==NULL )return 3;
	//
	printf("Window ID = %d\n", id);

	// -------------------------------------------
	// Clear structure at the first place
	memset(w, 0, sizeof(WINDOW));

	// -------------------------------------------
	// Set window type
	w->type = type;

	// Define window ID
	w->id = id;

	// Define window geometric parameters
	if(x==-1 || y==-1)
	{
		//
		autoGetWinPos(v, &w->x,&w->y, width,height);
	}
	else
	{
		w->x = x;		w->y = y;
	}

	//
	w->width = width;	w->height = height;
	// Define last coordinates as "none" (-1, -1)
	w->lx=-1;
	w->ly=-1;
	// Define Z-coordinate
	wdb.top_z++; w->z = wdb.top_z;

	// -------------------------------------------
	// Define parameters according settings
	//

	// Set border width
	if(w->type&WINDOW_HAS_BORDER)
	{
		// Define border width
		w->borderWidth = DEF_BORDERWIDTH;
	}
	else
	{
		// Define empty border
		w->borderWidth = 0;
	}

	// Define client area according does
	// window have a caption or not.
	if( (w->type&WINDOW_HAS_CAPTION) )
	{
		// Define client area
		w->client.x = 0;
		w->client.y = 22-3;
		// II
		w->client.width = w->width-(w->client.x*2);
		w->client.height = w->height-(w->client.y*2);
	}
	else
	{
		// Define client area
		w->client.x = 0;
		w->client.y = 0;
		// II
		w->client.width = w->width-(w->client.x*2);
		w->client.height = w->height-(w->client.y*2);
	}

	// Copy caption string
	strcpy(w->caption, caption);

	// Request refresh for this window
	w->refresh = 1|2; // window + components

	// Create window close button if requested
	if(w->type&WINDOW_CLOSE_BUTTON)
	{
		// Close button creation
		guiCreateButton(w, 0,0,0,0,
			WINDOW_CLOSE_BUTTON | EXIT_ON_CLICK,
			"");
	}

	// Select new window as default
	SelectWindow(v, w);

	// Return window ID
	return id;
}

// Init window database
void wdbInit(void)
{
	int i,i2,i3,i4;

	//
	memset(&wdb, 0, sizeof(WDB));

	//
	for(i=0; i<N_MAX_WINDOWS; i++)
	{
		// Set window ID
		wdb.w[i].id = i;

		// Set window entry as free
		wdb.w[i].isfree = 1;
	}
}

// Create new window
int newWDB(void)
{
	//
	if( (wdb.n_windows+2)>=N_MAX_WINDOWS )return -1;
	wdb.n_windows++;
	return wdb.n_windows-1;
}

// Other values = New Window ID
int emptyWDB(void)
{
	int i,i2,i3,i4;

	//
	for(i=0; i<wdb.n_windows; i++)
	{
		if(wdb.w[i].isfree)
		{
def:
			wdb.w[i].isfree=0;
			return i;
		}
	}

	//
	i = newWDB();
	goto def;
}

// Get count of windows
int GetNWindows(void)
{
	//
	return wdb.n_windows;
}

//
void *GetWindowPTR(int id)
{
	//
	if(id<0 || id>wdb.n_windows) return NULL;
	return &wdb.w[id];
}

//
int DeleteWindow(int id)
{
	//
	if(id<0 || (id+2)>=wdb.n_windows) return 1;

	//
	wdb.w[id].isfree=1;

	//
	return 0;
}

//

