// ---------------------------------------------------------
// guiRect.c - GRAOS Windowing System
// (C) 2002-2003 by Jari Tuominen
// ---------------------------------------------------------
#include <stdio.h>
#include "graos.h"

// Erases specified area
int eraseArea(VMODE *v, int x1,int y1, int x2,int y2)
{
	//
	switch(v->bpp)
	{
		//
		case 1:
		return fillRect(v, x1,y1,x2,y2, 0);

		//
		case 8:
		return guiDesktopDraw(v, x1,y1,x2,y2);
	}
}

// Fill a rectangle
int fillRect(VMODE *v, int x1,int y1, int x2,int y2,  int vari)
{
	int x,y,i,i2,i3,i4;

	// Mark update area
	guiMark(v,  x1,x2+1, y1,y2+1);

	//
	for(y=y1; y<(y2+1); y++)
	{
		//
		for(x=x1; x<(x2+1); x++)
		{
			gui_dotPut(v, x,y, vari);
		}
	}
}

// Draw a rectangle
int drawRect(VMODE *v, int x1,int y1, int x2,int y2,  int vari)
{
	int x,y,i,i2,i3,i4;

	//
	guiMark(v,  x1,x1+1, y1,y2);

	// 1
	for(y=y1; y<(y2+1); y++)
	{
		//
		gui_dotPut(v, x1,y, vari);
	}

	// 2
	for(y=y1; y<(y2+1); y++)
	{
		gui_dotPut(v, x2,y, vari);
	}

	// 3
	for(x=x1; x<(x2+1); x++)
	{
		gui_dotPut(v, x,y1, vari);
	}

	// 4
	for(x=x1; x<(x2+1); x++)
	{
		gui_dotPut(v, x,y2, vari);
	}
}


