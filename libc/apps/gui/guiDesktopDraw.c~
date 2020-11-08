// -------------------------------------------------
// guiDesktopDraw.c - GRAOS Windowing System
// (C) 2002-2003 by Jari Tuominen
// -------------------------------------------------
#include <stdio.h>
#include "graos.h"
#include "guiDesktopDraw.h"

//
int guiDesktopDraw8(VMODE *v, int x1,int y1, int x2,int y2)
{
	//
	int x,y,xx,yy,ad;
	char pal[8]={
		0,8,7,14,15,
		14,7,8,0
		};

	//
	if(v->background || 1)
	{
		//
		guiBlit(v, x1,y1,x2,y2,
			v->background+x1+y1*v->width, 640,480);
              //  guiWriteArea(v,
             //           x1,y1,x2,y2,
             //           v->background+x1+y1*v->width);
	}
	else
	{
		//
		for(y=y1; y<(y2+1); y++)
		{
			//
			for(x=x1; x<(x2+1); x++)
			{
				//
			//	xx = x;
			//	yy = y;
			//	gui_dotPut(v, x,y,
			//		pal[((xx*yy)>>12) & 0x7] );
			}
		}
	}
}

//
int guiDesktopDraw(VMODE *v, int x1,int y1, int x2,int y2)
{
	//
	switch(v->bpp)
	{
		case 1:
		break;

		case 8:
		guiDesktopDraw8(v, x1,y1,x2,y2);
	}
	return 0;
}
