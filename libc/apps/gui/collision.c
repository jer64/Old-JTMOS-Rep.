// collision.c
#include <stdio.h>
#include "graos.h"
#include "collision.h"

// Redraw all collisioned windows
int RedrawCollisionedWindows(VMODE *v)
{
	int i,i2;
	WINDOW *w;

	// Go through all windows
	for(i=0; i<wdb.n_windows; i++)
	{
		// Get window
		w = &wdb.w[i];

		// Got collision?
		if(w->collision)
		{
			// Order redraw

			// Zero collision counter
			w->collision = 0;

			// Set refresh
			w->refresh = 1|2; // window + components
		}
	}
}

//
int EstimateWindowCollision(WINDOW *ww)
{
	int i,i2;
	WINDOW *w;

	//
	for(i=0; i<wdb.n_windows; i++)
	{
		// Get window
		w = &wdb.w[i];

		// Skip our window
		if(w==ww)
			continue;

		// Check if cursor is within this window's area
		if( rectInsideRect(
			ww->x, ww->y, ww->x+ww->width, ww->y+ww->height,
			w->x, w->y, w->x+w->width, w->y+w->height) )
		{
			// Set window on collision
			if(w->collision<MAX_COLLISION)
				w->collision++;
		}
	}

	//
	return 0;
}

