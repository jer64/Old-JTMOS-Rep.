//==================================================
// windowDraw.c
// Functions that draw the core window on screen.
// (C) 2003 by Jari Tuominen
//==================================================
#include <stdio.h>
#include <conio.h> // Color defs
#include "graos.h"
#include "wdb.h" // Window Database
#include "windowDraw.h"
#include "framebuf.h"

// Draw a window
int windowDraw(VMODE *v, WINDOW *w)
{
	int x1,y1,x2,y2,color,
		i,i2,i3,i4;

	// Ignore freed windows
	if(w->isfree)
		return 1;

	//--------------------------------------------------------------------
	// WINDOW BORDER + CAPTION + ETC. (1)

	// If window refresh needed
	if( (w->refresh&1) )
	{
		// Restore old window area, if one
		// exists at the present moment.
		if(w->lx!=-1 && w->ly!=-1)
			eraseArea(v,
			w->lx-w->borderWidth, w->ly-w->borderWidth,
			w->lx+w->width+w->borderWidth+1, w->ly+w->height+w->borderWidth+1);
	
		// Define last x,y
		w->lx=w->x;
		w->ly=w->y;
	
		// Get rectangle area
		x1 = w->x;		y1 = w->y;
		x2 = x1+w->width; 	y2 = y1+w->height;
	
		// Fill window area
		if(w->type&WINDOW_AUTOCLEAR)
			// Automatically clear window area
			fillRect(v, x1,y1,x2,y2, 7);
	
		//----------------------------------------------------------
		// Fill headline area
	
		//
		if(w->type&WINDOW_HAS_CAPTION)
		{
			// Active or deactive window? (BLUE or GREY)
			if(w==v->selwin)
				color = BLUE;
				else
				color = 8;
			fillRect(v, x1,y1,x2,y1+2+16, color);
		}
	
		//----------------------------------------------------------
		// Write headline text
		//
		if(strlen(w->caption))
			DrawText(v, w->caption, x1+2+2,y1+2+4, TRANSPARENT, 0x0f);

		//----------------------------------------------------------
		// Draw border
		//
		if(w->type&WINDOW_HAS_BORDER)
		{
			//
			for(i=1; i<2; i++)
			{
				//
				drawRect(v, x1-i,y1-i,x2+i,y2+i, BLACK);
			}
		}
	
		// Draw quick message if one exists
		if(strlen(w->qmsg))
			DrawText(v, w->qmsg, x1+w->client.x, y1+w->client.y,
				TRANSPARENT, 0x0f);
	}

	//--------------------------------------------------------------------
	// COMPONENTS (2)
	if( w->refresh&2 )
	{
		// Draw all frame buffers
		drawWindowFrameBuffers(v, w);
	}

	// If window's button(s) refresh is needed
	if(w->refreshButtons || (w->refresh&2) )
	{
		// Draw all buttons that this window has
		drawWindowButtons(v, w);
	}

	//
	return 0;
}

// Draw all buttons that the window has
int drawWindowButtons(VMODE *v, WINDOW *w)
{
	int i,i2,x1,y1,x2,y2;
	BUTTON *b;

	// Get rectangle area
	x1 = w->x;		y1 = w->y;
	x2 = x1+w->width; 	y2 = y1+w->height;

	//
	for(i=0; i<w->n_buttons; i++)
	{
		// Get button
		b = &w->button[i];
		if(b->isfree)continue;

	/*	// Check for refresh requirement
		if(b->refresh==0 && !w->refresh)
			continue;
			else
			// Flag as refreshed
			b->refresh=0;*/

		// Draw button
		if( (b->type&WINDOW_CLOSE_BUTTON) )
		{
			// Define button's area
			b->x1 = (x2-14-2)-x1;
			b->y1 = (y1+2+2)-y1;
			b->x2 = (x2-2-2)-x1;
			b->y2 = (y1+14+2)-y1;
			b->x1+=2;
			b->x2+=2;
			b->y1-=1;
			b->y2-=1;
			// Draw button
			buttonDraw(v, b->x1+x1,b->y1+y1,b->x2+x1,b->y2+y1,
				b->pressed, b->caption);
		}
		else
		{
			// Custom button

			// Draw button
			buttonDraw(v, b->x1+x1,b->y1+y1,b->x2+x1,b->y2+y1,
				b->pressed, b->caption);
		}
	}

	//
	return 0;
}

// Draw a button
int buttonDraw(VMODE *v,
	int x1,int y1,int x2,int y2,
	int pressed,
	const char *caption)
{
	int color,
		i,i2,i3,i4,c1,c2,c3,x,y;

	//
	if(pressed==0)
	{
		c1 = 15;
		c2 = 7;
		c3 = 0;
	}
	else
	{
		c1 = 0;
		c2 = 7;
		c3 = 15;
	}

	// Draw button main color
	fillRect(v, x1,y1,x2,y2, c2);

	// Draw outlines
	fillRect(v, x1,y1,x2,y1, c1);
	fillRect(v, x1,y1,x1,y2, c1);
	fillRect(v, x2,y1,x2,y2, c3);
	fillRect(v, x1,y2,x2,y2, c3);

	// Draw button message
	if(strlen(caption))
	{
		//
		x = ((x2-x1)>>1) - (strlen(caption)>>1)*8;
		y = ((y2-y1)>>1)-4;

		//
		DrawText(v, caption, x1+x,y1+y,
			TRANSPARENT,
			BLACK);
	}
}



