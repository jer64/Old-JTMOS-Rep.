//--------------------------------------------------------------------------------------------
// plasma_app.c - Plasma Demo Effect.
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include "plasma_app.h"

// Dir function (ls)
// -----------------
//
// This function outputs directory from specified block(linear) to stdout.
int plasma_app(int argc, char **argv)
{
	char pal[]={0,1,3,9,14,14,9,3,1}; // 8 bytes
	int x,y,i,ad;
	BYTE *sintab;
	float fa,fb;
	BYTE *colmap,color;

	//
	sintab=malloc(65536);
	for(i=0,fa=0; i<65536; i++,fa+=.0523)
	{
		//sintab[i] = 128+(127*sin(fa));
	}
	
	//
	for(i=0; getch1()!=27; i++)
	{
		//
		vga_waitretrace();
		//
		colmap = 0xB8000;
		for(y=0; y<25; y++)
		{
			ad = 80*2*y;
			for(x=0; x<80; x++)
			{
				//color = (sintab[(x+i)&65536]+sintab[(y+i)&65536])+i;
				color = ((x+i)&(y+i))+i;
				colmap[(ad+(x<<1))+1] = (pal[color & 7])<<4;
			}
		}
	}
	//
	return 0;
}

