//=========================================================
// framebuf.c
// Frame buffer and EGA terminal emulation.
// (C) 2003 by Jari Tuominen
//=========================================================
#include <stdio.h>
#include <conio.h>
#include "graos.h"
#include "framebuf.h"

// Calculate 32-bit checksum
DWORD chksum(BYTE *buf, int len)
{
	int i,val;

	//
	for(i=0,val=0; i<len; i++)
		val+=buf[i]+1; // count zeroes too

	//
	return val;
}

// Get a new frame buffer
FRAMEBUF *newFrameBuf(WINDOW *w)
{
	int i;
	FRAMEBUF *fb;

	// Look for old unused framebuffer
	for(i=0; i<w->n_framebuffers; i++)
	{
		//
		fb = &w->framebuf[i];

		//
		if(fb->isfree)
		{
			//
			goto resv;
		}
	}

	// Get a new one
	fb = &w->framebuf[w->n_framebuffers];
	w->n_framebuffers++;

resv:
	// Reserve frame buffer
	fb->isfree = FALSE;

	// Return FB PTR
	return fb;
}

// Create a new frame buffer inside
// the window's client region.
int guiAddFrameBuffer(WINDOW *w,
        BYTE *buf, int x, int y,
        int width, int height, int bpp,
	int type,
	int terID)
{
	FRAMEBUF *fb;

	// Get new frame buffer
	fb = newFrameBuf(w);

	// Define location
	fb->x = x;
	fb->y = y;

	// Define width & height
	fb->width = width;
	fb->height = height;

	// Define bits per pixel
	fb->bpp = bpp;

	// Define buffer PTR
	fb->buf = buf;

	// Set type
	fb->type = type;

	// Set terminal ID
	fb->terID = terID;

	// Return OK
	return 0;
}

// Draw emulated EGA terminal.
void guiDrawEGATerminal(VMODE *v,
		WINDOW *w, FRAMEBUF *fb, BYTE *tmp)
{
	static char str[256],colmap[256];
	static int i,i2,ad,ad2,x,y,l,xx,yy,x2,y2,clx,cly,cursor,cx,cy;

	//-----------------------------------------------------
	// Get corner of client area
	xx = w->x + w->client.x + fb->x;
	yy = w->y + w->client.y + fb->y;

	// Get client area limit boundary
	x2 = w->x + w->client.x + w->client.width;
	y2 = w->y + w->client.y + w->client.height;

	// First sanity check
	if( (xx+8)>=x2 )
		return;
	if( (yy+8)>=y2 )
		return;

	//------------------------------------------------------------
	// Get cursor location
	// (this needs to be done before terminal drawing)
	cursor = GetCursorLocation(fb->terID);
	// Store last cursor position
	fb->lcx = fb->cx;
	fb->lcy = fb->cy;
	// Store current cursor position
	fb->cx = cursor&255;
	fb->cy = (cursor>>8)&255;
	// Calculate location of the cursor in graphical screen
	cx=fb->cx<<3; cy=fb->cy<<3;
	cx+=xx; cy+=yy;

	// Refresh the line where cursor is
	fb->rtab[fb->cy] = 1;
	// Also refresh the line where cursor last time was
	fb->rtab[fb->lcy % 50] = 1;

	//-----------------------------------------------------
	// Draw terminal, line by line
	//
	for(y=0,ad=0; y<fb->height; y++,ad+=fb->width<<1)
	{
		// Skip line if it does not contain updated content
		if(fb->rtab[y]==FALSE && (w->refresh&1)==0)
			continue;

		// Declare it done
		fb->rtab[y] = FALSE;

		// Break loop if we exceed boundary
	//	if( (yy+y*8+8)>=y2 )
	//		break;

		// Copy a single line to buffer
		l = fb->width;
		for(i=0; i<l; i++)
			str[i] = tmp[ad+(i<<1)];
		str[i]=0;
		// Copy colors
		for(i=0; i<l; i++)
			colmap[i] = tmp[ad+(i<<1)+1];

		// Trim string
		if( (strlen(str)*8)>=(w->width-w->client.x) )
		{
			str[(w->width>>3)-1] = 0;
		}

		// First wipe the area with black
		fillRect(v, xx,yy+y*8,x2,yy+y*8 +8, colmap[0]>>4);

		// Draw text line
		DrawTextColorful(v,
			&v->font[0],
			str, xx,yy+y*8,
			NON_TRANSPARENT | NON_THIN,
			colmap, 0x07);
	}

	// Cursor draw if it resides within the client region
	if( !((cx+8)>=x2) ) // && !((cy)>=y2) )
	{
		// Draw cursor
		fillRect(v, cx,cy,cx+8,cy+8, 10);
	}
}

// Automatically refresh a single window on terminal updates
int automaticallyRefreshTerminalWindow(VMODE *v, WINDOW *w)
{
	int i,chk,x,y,ad,ad2;
	FRAMEBUF *fb;

	//
	for(i=0; i<w->n_framebuffers; i++)
	{
		// Get framebuf PTR
		fb = &w->framebuf[i];

		// Skip deallocated entries
		if(fb->isfree)
			continue;

		// Only terminal emulation FBs
		if(fb->type!=TYPE_TERMINAL_EMULATION)
			continue;

		// Get updated terminal data
		CopyTerminal(fb->terID, v->tmp);

		// We need to redraw the content entirely?
		if(w->refresh)
		{
			// Set all lines to being refreshed
			for(y=0; y<fb->height; y++)
				fb->rtab[y] = TRUE;
		}
		else
		// Check all lines for updated data
		for(y=0; y<fb->height; y++)
		{
			// No refresh for this line as default
			fb->rtab[y] = FALSE;

			//
			chk = chksum(v->tmp+y*fb->width*2, fb->width*2);
			if(chk!=fb->chksum[y])
			{
				// Checksum differs:
				// Update checksum
				fb->chksum[y] = chk;

				// Trigger line refresh
				fb->rtab[y] = TRUE;

				// Trigger window refresh
				// (2 = components only)
				w->refresh |= 2;
			}
		}
	}
}

//
int automaticallyRefreshTerminalWindows(VMODE *v)
{
	int i;
	WINDOW *w;

	// Go through all windows
	for(i=0; i<wdb.n_windows; i++)
	{
		// Get window PTR
		w = &wdb.w[i];

		// Skip deallocated windows
		if(w->isfree)
			continue;

		// Got frame buffer(s)?
		if(w->n_framebuffers)
			automaticallyRefreshTerminalWindow(v, w);
	}
}

// Draw a single frame buffer
int drawFB(VMODE *v, WINDOW *w, FRAMEBUF *fb)
{
	static int i,i2,x,y,xx,yy,ad,ad2,x1,y1,x2,y2;

	//
	switch(fb->type)
	{
		//------------------------------------------
		case TYPE_BITMAP_SCREEN:

		// Copy updated buffer
		globalcopy(w->pid,fb->buf,
			GetCurrentThread(),v->tmp,
			fb->width*fb->height*2);

		// Define rectangle
		x1 = fb->x+w->client.x+w->x;
		y1 = fb->y+w->client.y+w->y;
		x2 = fb->x+w->client.x+fb->width+w->x;
		y2 = fb->y+w->client.y+fb->height+w->y;
	
		// Draw frame buffer
		guiWriteArea(v,
			x1,y1,x2,y2,
			v->tmp);
		break;

		//------------------------------------------
		case TYPE_TERMINAL_EMULATION:

		// Copy updated buffer
		CopyTerminal(fb->terID, v->tmp);
	//	fb->chksum = chksum(v->tmp, fb->width*fb->height*2);

		// Draw frame buffer
		guiDrawEGATerminal(v, w, fb, v->tmp);
		break;

		// Unknown frame buffer
		default:
		break;
	}
}

// Draw all frame buffers of the window
int drawWindowFrameBuffers(VMODE *v, WINDOW *w)
{
	int i,i2;
	FRAMEBUF *fb;

	// Draw all frame buffers
	for(i=0; i<w->n_framebuffers; i++)
	{
		// Get PTR
		fb = &w->framebuf[i];

		// Skip free entries
		if(fb->isfree)
			continue;

		// Draw FB
		drawFB(v, w, fb);
	}
}

