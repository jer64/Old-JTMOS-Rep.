// ******************************************
// guifunc.c - font renderation functions.
// (C) 2002-2003 by Jari Tuominen
// ******************************************
#include <stdio.h>
#include <jtmos/video.h>
#include <math.h>
#include <conio.h> // Colors
#include "guifont.h"
#define FONTCOLOR RED

// Load 512x16 font with 32x2 fonts
int LoadFont(const char *fname, FONT *fon, int invert)
{
	FILE *f;
	int i,d,l,lines;

	// Open font file
	f=fopen(fname, "rb");
	if(f==NULL)
		// File not found!
		return 1;


	// Read font
	lines = 2;
	l = 512*8 * lines;

	// Allocate font bitmap buffer
	fon->buf = malloc(l);

	//
	for(i=0; i<l; i++)
	{
		// Get data
		d = fgetc(f); if(d==EOF)break;

		// Filter
		if(d==15)
			d=FONTCOLOR;
		else
			d=0;

		// Invert
		if(invert)
		{
			if(d==15)
				d = 0;
			else
				d = FONTCOLOR;
		}

		// Store data
		fon->buf[i]=d;
	}

	//
	fon->l_buf = i;
	fon->width =	512;
	fon->height =	8;
	fon->bpp =	8;
	fclose(f);
	return 0;
}

// Draw text
void DrawText(VMODE *v, const char *s, int tx,int ty,
		int t,
		int color)
{
	//
	DrawTextEx(v, &v->font[0], s,tx,ty, t, color);
}

// Draw text (2)
void DrawTextEx(VMODE *v, FONT *f, const char *s, int tx,int ty,
			int transparency,
			int color)
{
	//
	_DrawText(v, s,tx,ty,
		f->buf, f->width, f->height,
		transparency,
		NULL,
		color);
}

// Draw text, with possibility to define a seperate
// forgeground and background color for each of
// the letters.
// Used by EGA terminal emulation.
void DrawTextColorful(VMODE *v,
		FONT *f,	
		const char *s, int tx,int ty,
                int transparency,
		char *colmap,
		int color)
{
	//
	_DrawText(v,
		s,tx,ty,
		f->buf, f->width, f->height,
		transparency,
		colmap,
		color);
}

// Draw text (3)
// Render graphic text
void _DrawText(VMODE *v,
		const char *s,long x,long y,
		const char *font,long font_width,long font_height,
		int transparency,
		char *colmap, int color)
{
	// Font configuration
	static char *contab= "abcdefghijklmnopqrstuvwxyz0123456789_ ()-+.,%/#':;$[]<>=@*\\_^¨½§!?|~צהו!?|~צהו";
	static char *contab2="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ ()-+.,%/#':;$[]<>=@*\\_^¨½§!?|~צהו!?|~צהו";
	static char *thinchars="iI,.()[]:;' <>!|";
	//
	int i,i2,i3,i4,xp,yp,line,xx,xplus,yplus,l,checkThins,col;
	int c,c2,c3,c4,ch;

	// Thin chars option
	checkThins = transparency & 2 ^ 2;

	//
	xp=x; yp=y;

	// Spring text!
	for(i=0,xplus=0,line=0,xx=0; i<strlen(s); i++,xx++)
	{
		// Read mark
		c = s[i]; ch=c;

		// End of string?
		if(!c)break;

		//
		if(c=='\r')continue;

		// New line?
		if(c=='\n')
		{
			xplus=0;
			xx=-1;
			line++;
			continue;
		}

		//
		l = strlen(contab);
		for(i2=0,i3=0; i2<l; i2++)
		{
			//
			if( contab[i2]==c || contab2[i2]==c )
			{
				//
				c=i2;
				i3=1;
				break;
			}
		}

		// No matching? => Just empty.
		if(!i3)continue;

		// Scroll over thinchars(4 pixels width)
		// Regular is 8 pixels.
		if(checkThins)
		{
			l = strlen(thinchars);
			for(i2=0,i3=8; i2<l; i2++)
			{
				// End of thinchars?
				if(!thinchars[i2])break;
				// Is it a thinchar?
				if(ch==thinchars[i2]){i3=4; break;}
			}
		}
		else	i3=8; // 8 pixels wide letter

		//
		if(colmap!=NULL)
			col = colmap[i];
		else
			col = color;

		// Draw single letter.
		if(!transparency)
		{
			DrawMonoBitmapColor(v, (BYTE*)font,font_width,font_height,NULL,
					         xp+xplus,yp+(line*font_height), 
						// write coordinates
						font_height,font_height,
						(c%64)*8, (c/64)*8,
						col,
						FALSE);
		}
		else
		{
			DrawMonoBitmapColor(v, (BYTE*)font,font_width,font_height,NULL,
					         xp+xplus,yp+(line*font_height), 
						// write coordinates
						font_height,font_height,
						(c%64)*8, (c/64)*8,
						col,
						TRUE);
		}

		//
		xplus += i3;
	}
}

