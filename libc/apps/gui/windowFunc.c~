// windowFunc.c
#include <stdio.h>
#include "graos.h"
#include "windowFunc.h"
#include "graosprotocol.h"
#include "framebuf.h"

// Handle button click events
void onButtonClick(VMODE *v, WINDOW *w, BUTTON *b)
{
	// If exit on click
	if( (b->type&EXIT_ON_CLICK) )
	{
		// Close the window
		guiCloseWindow(v, w);
	}
}

// Posts exit request to the owner of the window
void PostQuitToWindowOwner(WINDOW *w)
{
	GUICMD *cmd;
	static MSG m;

	// Get GUICMD PTR
	cmd = &m.buf;

	// Define "exit"
	cmd->id = GSID_EXIT;

	// Send GSID_EXIT to the application
	sendMessage(&m, w->pid);
}

// Unpress all buttons in specified window
void unpressAllButtons1(WINDOW *w)
{
	int i,i2;
	BUTTON *b;

	//
	for(i=0; i<w->n_buttons; i++)
	{
		// Get PTR
		b = &w->button[i];

		// Skip deallocated buttons
		if(b->isfree)
			continue;

		// unpress it
		if(b->pressed!=0)
		{
			//
			b->pressed = 0;
			b->refresh = 1;
			w->refreshButtons=1;
			w->refresh |= 2; // refresh components
		}
	}
}

// Unpress buttons in all windows
void unpressAllButtons(VMODE *v)
{
	int i,i2;
	WINDOW *w;

	//
	for(i=0; i<wdb.n_windows; i++)
	{
		// Get window PTR
		w = &wdb.w[i];

		// Skip deallocated windows
		if(w->isfree)
			continue;

		// 
		unpressAllButtons1(w);
	}
}

// Find an empty button entry
BUTTON *getNewButton(WINDOW *w)
{
	int i,i2;
	BUTTON *b;

	//
	for(i=0; i<w->n_buttons; i++)
	{
		//
		b = &w->button[i];

		//
		if(b->isfree)
		{
			//
			b->isfree=0;
			return b;
		}
	}

	// Reserve new button entry
	b = &w->button[w->n_buttons];
	w->n_buttons++;
	b->isfree = 0;
	return b;
}

// Create a button to a window
BUTTON *guiCreateButton(WINDOW *w,
	int x1,int y1,int x2,int y2,
	int type,
	const char *caption)
{
	BUTTON *b;

	// Get new button entry PTR
	if( (b=getNewButton(w))==NULL )
		return NULL;

	// Store caption
	strcpy(b->caption, caption);

	// Store type
	b->type = type;

	// Store coordinates
	b->x1 = x1;
	b->x2 = x2;
	b->y1 = y1;
	b->y2 = y2;

	// Return PTR
	return b;
}

// Close window
void guiCloseWindow(VMODE *v, WINDOW *w)
{
	//
	if(w==NULL || v==NULL)
		return;

	// Already closed?
	if(w->isfree)
		// Already closed, return back...
		// (prevents infinite loop)
		return;

	// Hide window
	HideDrawWindow(v, w);

	// Post "exit now" message to the application
	PostQuitToWindowOwner(w);

	// Free window
	w->isfree = TRUE;
}


// Hide window from screen by drawing
// (used by close window e.g.)
void HideDrawWindow(VMODE *v, WINDOW *w)
{
	// Erase window's area
	eraseArea(v,
		w->x-w->borderWidth, w->y-w->borderWidth,
		w->x+w->width+w->borderWidth+1, w->y+w->height+w->borderWidth+1);

	// Estimate which windows needs to be redrawn after the operation
	EstimateWindowCollision(w);
}

// Set window to close on click
void WindowClickQuit(int wid)
{
	//
	wdb.w[wid].clickQuit = TRUE;
}

// Set window quick message
void SetQuickMessage(int wid, const char *msg)
{
	//
	if(strlen(msg)<200)
		strcpy(wdb.w[wid].qmsg, msg);
}

// Create a new pop up window (used for quick system messages)
int PopUpWindow(VMODE *v, const char *caption, const char *msg)
{
	int wid,width,height;

	// Define size
	width=400;
	height=64;

	// Create the window
	wid = guiCreateWindow(v,
			(v->width>>1)-(width>>1), (v->height>>1)-(height>>1),
			width,height,
			caption,
			STANDARD_WINDOW);

	// Set quick message
	SetQuickMessage(wid, msg);

	// Set window so that it closes on click
	WindowClickQuit(wid);

	// Return window ID
	return wid;
}

//
int rectInsideRect(int x1,int y1,int x2,int y2,
		int xx1,int yy1,int xx2,int yy2)
{
	//
	if( pointInsideRect(xx1, yy1,
			x1,y1,x2,y2) )
		return 1;

	//
	if( pointInsideRect(xx1, yy2,
			x1,y1,x2,y2) )
		return 1;

	//
	if( pointInsideRect(xx2, yy1,
			x1,y1,x2,y2) )
		return 1;

	//
	if( pointInsideRect(xx2, yy2,
			x1,y1,x2,y2) )
		return 1;

	//
	return 0;
}

//
int pointInsideRect(int x,int y,
		int x1,int y1,int x2,int y2)
{
	//
	if(x>=x1 && y>=y1
		&&
	   x<x2 && y<y2)
	{
		//
		return 1;
	}
	else	return 0;
}

// Checks whether if there is a button under cursor,
// if so, it'll return handle to the button,
// if not it'll return NULL.
BUTTON *getButtonUnderCursor(VMODE *v)
{
	WINDOW *w;
	BUTTON *b;
	int i,i2;

	// Get the window
	w = getWindowUnderCursor(v);

	// Check which button
	for(i=0; i<w->n_buttons; i++)
	{
		// Get button
		b = &w->button[i];

		// Check cursor x,y to it's region
		if( pointInsideRect(v->x, v->y,
			b->x1+w->x, b->y1+w->y, b->x2+w->x, b->y2+w->y) )
		{
			// Found button under cursor
			return b;
		}
	}

	// No button found under cursor
	return NULL;
}

// Returns handle to the window under mouse cursor
WINDOW *getWindowUnderCursor(VMODE *v)
{
	//
	return getWindowUnderCursorEx(v, 0);
}

// Returns handle to a window that is under the mouse cursor,
// if none detected then it returns NULL.
WINDOW *getWindowUnderCursorEx(VMODE *v, int headline)
{
	int i,i2,level;
	WINDOW *w,*found;

	//
	for(i=0,level=-1,found=NULL; i<wdb.n_windows; i++)
	{
		// Get window
		w = &wdb.w[i];

		// Skip deallocated windows
		if(w->isfree)
			continue;

		// Check if cursor is within this window's area
		if( 
			(!headline && pointInsideRect(v->x, v->y,
			w->x, w->y, w->x+w->width, w->y+w->height))
			||
			(headline && pointInsideRect(v->x, v->y,
			w->x, w->y, w->x+w->width, w->y+16))
		  )
		{
			// If we have got a
			// greater level window then pick it first
			if(w->z > level)
			{
				// Cursor inside this window
				found = w;

				//
				level = w->z;
			}
		}
	}

	// None of windows inside the cursor
	return found;
}

//



