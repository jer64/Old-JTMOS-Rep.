// -------------------------------------------------------
// guiUpdate.c - GRAOS Windowing System
// (C) 2002-2003 by Jari Tuominen
// -------------------------------------------------------
#include <stdio.h>
#include <jtmos/video.h>
#include "graos.h"
#include "guiUpdate.h"

// src ptr, dst offs, length
//
// Simulates 640x480 8-bit mode blit with 640x480 16c planed mode
void directBlit8(VMODE *v, int srcoffs, int dstoffs, int len)
{
	static int x,y,i,i2,i3,i4,pl;
	static BYTE *s,*o;
	static BYTE *plane[4];
	static BYTE *tmp;
	static BYTE bits[8]={128,64,32,16,8,4,2,1};
	static BYTE Xbits[8]={255-128,255-64,255-32,
			255-16,255-8,255-4,255-2,255-1};
	static BYTE *src;

	//
	tmp = v->tmp;
	src=v->buf+srcoffs;

	// Define planes
	for(i=0; i<4; i++) { plane[i] = v->tmp + i*len; }

	// Render each planes
	// 4 planes(bits per color) = 16 colors
	for(pl=0; pl<4; pl++)
	{
		//
		for(i=0,s=src; i<len; i++)
		{
			//
			if( s[i]>15 ) s[i]=15;
			if(  (s[i]&bits[7-pl])!=0  )
				plane[pl][i>>3] |= bits[i&7];
				else
				plane[pl][i>>3] &= Xbits[i&7];
		}
	}

	// Copy planes to VGA ram
	for(pl=0; pl<4; pl++)
	{
		// Select appropriate VGA plane
		vgaselectplane(pl);
		// Copy to VGA RAM
		drawframe(plane[pl], dstoffs>>3, len>>3);
	}
}

// 8 bpp
void guiUpdate8(VMODE *v)
{
	static int i,i2,i3,i4,x,y,ad,ii;
	static BYTE bits[8]={128,64,32,16,8,4,2,1,0,0,0,0,0,0,0,0};

	//
	for(i=0,v->ulines=0; i<v->height; i++)
		if(v->dtab[i])v->ulines++;

	//
	for(y=0,ad=0; y<v->height; y++,ad+=v->width)
	{
		//
		for(ii=0,i4=0; ii<(RADI+1); ii++,i4+=(v->width/RADI))
		{
			//
			//if(v->dtab[y]) // & bits[ii])
			if(v->dtab[y] & bits[ii])
			{
				//
				v->dtab[y] &= bits[ii]^0xFF;
				// src offs, dst offs, length
				directBlit8(v, ad+i4, ad+i4, v->width/RADI);
			}
		}
	}
}

// 1 bpp
void guiUpdate1(VMODE *v)
{
	int i,i2,x,y,ad;

	//
	for(y=0,ad=0; y<v->height; y++,ad+=(v->width>>3))
	{
		//
		if(v->dtab[y])
		{
			//
			v->dtab[y]=0;
			drawframe-(v->buf+ad, ad, v->width);
		}
	}
}

// Update visible screen
void guiUpdate(VMODE *v)
{
	waitretrace();
	if(v->change)
	{
		v->change = 0;
		switch(v->bpp)
		{
			// --: Monochrome
			case 1:
			guiUpdate1(v);
			break;
			// --: VGA 8-bit
			case 8:
			guiUpdate8(v);
			break;
			// --: UNKNOWN
			default:
			break;
		}
	}
}

