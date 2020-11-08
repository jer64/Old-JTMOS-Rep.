// -----------------------------------------------------------
// guidot.c - GRAOS Window System
// (C) 2002 by Jari Tuominen
// -----------------------------------------------------------
#include <stdio.h>
#include "graos.h"
#include "guidot.h"

/******** BPP 1 MONOCHROME FUNCTIONS ******************/
static BYTE bits[]={128,64,32,16,8,4,2,1};

void DrawDot(VMODE *v, int x,int y, char state)
{
	//
        if(state)
                gui_dotPut(v, x,y, 1);
                else
                gui_dotPut(v, x,y, 0);
}

/********* 1-BIT DRAWING FUNCTIONS ************************/
inline int gui_dotGetBPP1(VMODE *v, int x,int y)
{
	int ad;

	if(x<0 || x>=v->width ||
	   y<0 || y>=v->height)
			return 1;

	//
	v->change++;

	//
	ad = (v->mtab[y] + x)>>3;
	return v->buf[ad] & bits[x&7];
}

//
inline int gui_dotSetBPP1(VMODE *v, int x,int y, int state)
{
	int ad;

	//
	if(x<0 || x>=v->width ||
	   y<0 || y>=v->height)
			return 1;

	//
	v->change++;

	//
	ad = (v->mtab[y] + x)>>3;
	if(state)
		v->buf[ad] |= bits[x&7];
		else
		v->buf[ad] &= (255-bits[x&7]);
	return 0;
}

/********* 8-BIT DRAWING FUNCTIONS ************************/
inline int gui_dotGetBPP8(VMODE *v, int x,int y)
{
	int ad;

	//
	if(x<0 || x>=v->width ||
	   y<0 || y>=v->height)
			return 0;

	//
	v->change++;

	//
	ad = (v->mtab[y] + x);
	return v->buf[ad];
}

// Dot put for 8-bit mode
inline int gui_dotPutBPP8(VMODE *v, int x,int y, int va)
{
	int ad;

	//
	if(x<0 || x>=v->width ||
	   y<0 || y>=v->height)
			return 1;

	//
	v->change++;

	//
	ad = (v->mtab[y] + x);
	v->buf[ad] = va;
	return 0;
}

/***************** WRAPPERS ***************************/
inline int gui_dotGet(VMODE *v, int x,int y)
{
	// Choose dotter routine upon BPP
	switch(v->bpp)
	{
		// -----------------------------------
		// MONOCHROME MODE
		case 1:
		if( gui_dotGetBPP1(v, x,y) )
			return 1;
			else
			return 0;

		// -----------------------------------
		// 8-BIT VGA
		case 8:
		return gui_dotGetBPP8(v, x,y);

		default:
		return 0;
	}
	return 0;
}

//
inline int gui_dotPut(VMODE *v, int x,int y, int va)
{
	// Choose dotter routine upon BPP
	switch(v->bpp)
	{
		// --- 8-BIT VGA MODE -------------------
		case 8:
		gui_dotPutBPP8(v, x,y, va);
		break;

		// --- MONOCHROME MODE -------------------
		case 1:
		if(va)
			gui_dotSetBPP1(v, x,y, 1);
			else
			gui_dotSetBPP1(v, x,y, 0);
		break;

		default:
		return 0;
	}
	return 0;
}

/*
int gui_dotSet(VMODE *v, int x,int y)
{
	// Choose dotter routine upon BPP
	switch(v->bpp)
	{
		case 1:
		gui_dotSetBPP1(v, x,y, 1);
		break;

		default:
		return 1;
	}
	return 0;
}

//
inline int gui_dotRemove(VMODE *v, int x,int y)
{
	// Choose dotter routine upon BPP
	switch(v->bpp)
	{
		case 1:
		gui_dotRemoveBPP1(v, x,y);
		break;

		default:
		return 1;
	}
	return 0;
}

*/
/******************************************************/

