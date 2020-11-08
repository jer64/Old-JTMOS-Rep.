// ============================================
// guiBlit.c - GRAOS Windowing System
// (C) 2002-2003 by Jari Tuominen
// ============================================
#include <stdio.h>
#include <math.h>
#include "graos.h"
#include "guiApp.h"
#include "guiArrow.h"
#include "guidot.h"
#include "guiBlit.h"

// Mark
void guiMark(VMODE *v,
	int _x1, int _x2, int y1, int y2)
{
	int i,i2,i3,i4,ii,x1,x2,x,y;
	BYTE bits[16]={128,64,32,16,8,4,2,1,
		0,0,0,0, 0,0,0,0};

	//
	i = v->width/RADI;

	//
	x1=_x1/i;
	x2=_x2/i;

	//
	for(y=y1; y<(y2+1); y++)
	{
		//
		for(x=x1; x<(x2+1); x++)
		{
			//
			v->dtab[y] |= bits[x&15];
		}
	}
}

//
int guiReadArea(VMODE *v, int x1,int y1, int x2,int y2, BYTE *buf)
{
	int x,y,ad;

	//
	for(y=y1,ad=0; y<y2; y++)
	{
		for(x=x1; x<x2; x++,ad++)
		{
			buf[ad] = gui_dotGet(v, x,y);
		}
	}
}

// Specifically coded for arrows
int guiWriteArea1(VMODE *v, int x1,int y1, int x2,int y2, BYTE *buf)
{
	int x,y,ad;

	// Order update for the rectangle area
	guiMark(v,  x1,x2, y1,y2);

	//
	for(y=y1,ad=0; y<y2; y++)
	{
		//
		for(x=x1; x<x2; x++,ad++)
		{
			if(buf[ad]!=255)
			gui_dotPut(v, x,y, buf[ad]);
		}
	}
}

//
int guiWriteArea(VMODE *v, int x1,int y1, int x2,int y2, BYTE *buf)
{
	int x,y,ad,ad2;

	// Order update for the rectangle area
	guiMark(v,  x1,x2, y1,y2);

	//
	for(y=y1,ad=0; y<y2; y++)
	{
		//
		for(x=x1; x<x2; x++,ad++)
		{
			//
			gui_dotPut(v, x,y, buf[ad]);
		}
	}
}

//
int guiBlit(VMODE *v,
	int x1,int y1,int x2,int y2,
	BYTE *src,int swidth,int sheight)
{
	int x,y,ad,ad2,xx,yy;
	BYTE *s;

	// Order update for the rectangle area
	guiMark(v,  x1,x2, y1,y2);

	//
	for(y=y1; y<y2 && y<v->height; y++)
	{
		//
		ad = swidth*(y-y1);
		ad2 = v->mtab[y];

		//
		for(s=src+x1,x=x1,xx=0; x<x2 && x<v->width; x++,xx++)
		{
			//
			v->buf[ad2+x] = src[ad+xx];
		}
	}
}

