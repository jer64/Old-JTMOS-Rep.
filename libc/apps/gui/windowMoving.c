// windowMoving.c
#include <stdio.h>
#include "graos.h"
#include "windowMoving.h"

// Window moving functionallity
int windowMoving(VMODE *v)
{
	// CONTROL PART
	//--------------------------------------------


	//--------------------------------------------
	// Initially unpress all buttons
	unpressAllButtons(v);


	//--------------------------------------------
	// Get button under cursor
	v->underbut = getButtonUnderCursor(v);
	// Get window too (needed later)
	v->underwin = getWindowUnderCursorEx(v, AREA_WINDOW);

	// We have got a button under cursor?
	if(v->underbut)
	{
		//
		if(v->buttons&1)
		{
			// Press button
			if(v->underbut->pressed==0)
			{
				//
				v->underbut->pressed=1;
				v->underbut->refresh=1;
				//
				v->underwin->refreshButtons=1;
				v->underwin->refresh |= 2; // refresh components

				// Click click handler
				onButtonClick(v, v->underwin, v->underbut);
			}
		}
		else
		{
			// Unpress button
			if(v->underbut->pressed!=0)
			{
				//
				v->underbut->pressed=0;
				v->underbut->refresh=1;
				//
				v->underwin->refreshButtons=1;
				v->underwin->refresh |= 2; // refresh components
			}
		}

		//
		return 0;
	}




	//--------------------------------------------
	// Get window under cursor
	v->underwin = getWindowUnderCursorEx(v, AREA_WINDOW);

	// Window under cursor?
	if(v->underwin)
	{
		// Single click to close window?
		if(v->buttons&1 && v->underwin->clickQuit)
		{
			//
			guiCloseWindow(v, v->underwin);
			return 0;
		}
	}




	//--------------------------------------------
	// Get window headline under cursor
	v->underwin = getWindowUnderCursorEx(v, AREA_CAPTION);

	// If window under cursor
	if(v->underwin)
	{
		// Enable moving
		//---------------

		// If mouse button 1 hit:
		if(v->buttons&1 && !v->movingWindow)
		{
			// Make window selected

			// Got old selwin and it's not same as this?
			if(v->selwin && v->selwin!=v->underwin)
				// Set it to refresh
				v->selwin->refresh = 1|2; // window & components

			// Assign selwin
			v->selwin = v->underwin;


			// Start moving the window

			// Set moving on
			v->movingWindow=1;
			// Assign window
			v->movwin = v->underwin;
			// Define origo
			v->origx = v->x;
			v->origy = v->y;
			// Define old position
			v->oldx = v->underwin->x;
			v->oldy = v->underwin->y;

			// Put window on top
			putWindowOnTop(v, v->movwin);
		}

		// Release window from moving after the button is released
		if((v->buttons&1)==0)
		{
			// If we were moving a window
			if(v->movingWindow && v->movwin)
			{
				// Set movwin to refresh
				v->movwin->refresh = 1|2;

				// Undefine movwin
				v->movwin=NULL;
			}

			// Moving mode off
			v->movingWindow=0;

			// Redraw needed windows
			RedrawCollisionedWindows(v);
		}
	}

	// MOVER PART
	//--------------------------------------------

	// If we're now moving a window
	if(v->movingWindow)
	{
		// Move the window
		v->movwin->x = (v->x - v->origx) + v->oldx;
		v->movwin->y = (v->y - v->origy) + v->oldy;
		// Trigger refresh
		v->movwin->refresh=1|2;

		// Put other windows on redraw
		// which are collisioned with
		EstimateWindowCollision(v->movwin);
	}

	//
	return 0;
}

//


