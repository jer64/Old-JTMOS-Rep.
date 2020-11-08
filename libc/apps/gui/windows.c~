//=======================================================
// WINDOWS SYSTEM CALL
// windows.c
//=======================================================
#include <stdio.h>
#include "graos.h"
#include "windows.h"

//
static FILE *dbg;

// Called in the end
void exitAllApplications(VMODE *v)
{
	int i;
	WINDOW *w;

	// Send QUIT message to all windows
	for(i=0; i<wdb.n_windows; i++)
	{
		// Get PTR
		w = GetWindowPTR(i);

		// Skip empty entries
		if(w->isfree)
			continue;

		// Post quit message
		PostQuitToWindowOwner(w);
	}

	// Wait 2 seconds
	sleep(2);

	// Terminate all windows
	for(i=0; i<wdb.n_windows; i++)
	{
		// Get PTR
		w = GetWindowPTR(i);

		// Skip empty entries
		if(w->isfree)
			continue;

		// Terminate PID
		KillThread(w->pid);
	}
}

// Open debug stream
void OpenDebugStream(const char *name)
{
	//
	dbg = fopen(name, "wb");
}

// Close debug stream
void CloseDebugStream(void)
{
	//
	fclose(dbg);
}

// Output debug message to the debug stream
void DebugMessage(const char *s)
{
	//
	fputs(s, dbg);
}

// Draws all windows, and so on..
int guiWindowSystemCall(VMODE *v)
{
	int i,i2,i3,i4;
	WINDOW *w;
	void *p1,*p2;

	//------------------------------------------------------
	// Build a list of windows to draw
	// and sort list according Z-coordinate
	// of all windows.

	// Rest list
	for(i=0; i<wdb.n_windows; i++)
		wdb.o[i] = NULL;

	// Get all windows which require a refresh
	for(i=0,wdb.n_o=0; i<wdb.n_windows; i++)
	{
		// Get window PTR
		w = &wdb.w[i];

		// Skip deallocated entries
		if(w->isfree)
			continue;

		// Requires a refresh?
		if(w->refresh || w->refreshButtons)
		{
			// Add to the list
			wdb.o[wdb.n_o] = w;
			wdb.n_o++;
		}
	}

	// Sort the list according Z-coordinates
	for(i2=0; i2<wdb.n_o; i2++)
	{
		// Go through all entries
		for(i=0; i<wdb.n_o; i++)
		{
			// Skip NULL entries
			if(wdb.o[i+1]==NULL || wdb.o[i]==NULL)
				continue;

			// Compare Z-coordinates
			if(wdb.o[i]->z > wdb.o[i+1]->z)
			{
				// Swap entries
				p1 = wdb.o[i+0];
				p2 = wdb.o[i+1];
				wdb.o[i+1] = p1;
				wdb.o[i+0] = p2;
			}
		}
	}

	//------------------------------------------------------
	// Trigger refresh on windows which might get below
	// lesser priority windows. This is a fix.
	//
	// Note: disable this handling if window refreshing
	// is too slow.
	//
/**	for(i=0; i<wdb.n_o; i++)
	{
		//
		EstimateWindowCollision(wdb.o[i]);
	}**/

	//-----------------------------------------------------
	// Draw all windows in correct order
	//
	for(i=0; i<wdb.n_o; i++)
	{
		// Get window PTR
		w = wdb.o[i];

		// Skip deallocated windows
		if(w->isfree)
			continue;

		// Process if it is not a NULL PTR
		if(w)
		{
			// Redraw if [refresh] is set
			if(w->refresh)
			{
				// Window draw
				windowDraw(v, w);

				// Zero refresh
				w->refresh=0;
			}
		}
	}

	//
	return 0;
}

//
int windowFollow(WINDOW *w, int x,int y)
{
	int ox,oy;

	// Get old coords
	ox=windowX(w); oy=windowY(w);
	// Follow
	ox=follow(ox, x); oy=follow(oy, y);
	// Set new coords
	moveWindowAt(w, ox,oy);
}

//
int windowX(WINDOW *w)
{
	//
	return w->x;
}

//
int windowY(WINDOW *w)
{
	//
	return w->y;
}

//
int moveWindowAt(WINDOW *w, int x,int y)
{
	//
	w->x = x;
	w->y = y;
	w->refresh = 1;
}



