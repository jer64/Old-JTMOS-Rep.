//*******************************************
// guifunc.c
// (C) 2002-2003 by Jari Tuominen
//*******************************************
#include <stdio.h>
#include <jtmos/video.h>
#include <math.h>
#include <conio.h> // Colors
#include "graos.h"
#include "guifunc.h"

//
int follow(int what, int dest)
{
	//
	if(what==dest)
		return what;
		else
	if(what<dest)
		return what+1;
		else
	if(what>dest)
		return what-1;
}

//
int LoadBin(const char *fname, BYTE *buf, int limit)
{
	FILE *f;
	int i,d;

	//
	f=fopen(fname, "rb");
	if(!f) return 1;
	for(i=0; i<limit; i++) {
		d = fgetc(f);
		if(d==EOF)break;
		buf[i] = d;
	}
	fclose(f);
	return 0;
}

int waitkey(void)
{
	int key;
	printf("Press any key to continue ... ");
	key = getch();
	printf("\r                                    \r");
	return key;
}

// A fast clear screen function which clears with black color
void gui_FastClearScreen(VMODE *v)
{
	int i;

	//
	memset(v->buf, BLACK, v->l_buf);
	memset(v->dtab, 0xFF, v->height);
	v->change=1;
}

// Appropriate clear screen -function
void gui_ClearScreen(VMODE *v)
{
	int i;

	//
	eraseArea(v, 0,0,v->width,v->height);
}

//
void DrawTransparentMonoBitmap(VMODE *v,
        BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
        long target_x,    long target_y,
        long wanted_width,long wanted_height,
        long source_x,    long source_y)
{
	//
	DrawMonoBitmapEx(v, srcbuf, s_width,s_height, pal,
			target_x,target_y,
			wanted_width,wanted_height,
			source_x,source_y,
			1);
}

//
void DrawMonoBitmap(VMODE *v,
        BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
        long target_x,    long target_y,
        long wanted_width,long wanted_height,
        long source_x,    long source_y)
{
	//
	DrawMonoBitmapEx(v, srcbuf, s_width,s_height, pal,
			target_x,target_y,
			wanted_width,wanted_height,
			source_x,source_y,
			0);
}

//
void DrawMonoBitmapEx(VMODE *v,
	BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
	long target_x,    long target_y,
	long wanted_width,long wanted_height,
	long source_x,    long source_y,
	int tr)
{
	int x,y,ad,ad2,ad3,ad4;
	int xx,yy,xp,yp,rx,ry,i,i2,i3,i4;

	//
	for(y=target_y,yy=0; y<target_y+wanted_height; y++,yy++)
	{
		//
		if( (y>-1 && y<v->height) && (yy+source_y)>-1 && (yy+source_y)<s_height )
		{
			//
			guiMark(v,  target_x,target_x+wanted_width, y, y+1);

			//
			ad=(yy+source_y)*s_width;
			ad2=y*v->width;
			for(x=target_x,xx=0; x<target_x+wanted_width; x++,xx++)
			{
				//
				if(x<v->width && x>-1 && (xx+source_x)>-1 && (xx+source_x)<s_width )
				{
					if( srcbuf[ad+xx+source_x] )
						gui_dotPut(v, x,y, WHITE);
							else
						if(!tr)gui_dotPut(v, x,y, 0);
					// if
				}
				// if
			}
			// for
		}
		// if
	}
	// for

	//
	return;
}

// Draw color bitmap.
// Options for:
// - Transparency.
// - Color.
void DrawMonoBitmapColor(VMODE *v,
	BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
	long target_x,    long target_y,
	long wanted_width,long wanted_height,
	long source_x,    long source_y,
	int color,
	char drawbackground)
{
	int x,y,ad,ad2,ad3,ad4;
	int xx,yy,xp,yp,rx,ry,i,i2,i3,i4;

	//
	for(y=target_y,yy=0; y<target_y+wanted_height; y++,yy++)
	{
		//
		if( (y>-1 && y<v->height) )
			//&& (yy+source_y)>-1 && (yy+source_y)<s_height )
		{
			//
			guiMark(v,  target_x,target_x+wanted_width, y, y+1);

			//
			ad=(yy+source_y)*s_width;
			ad2=y*v->width;
			for(x=target_x,xx=0; x<target_x+wanted_width; x++,xx++)
			{
				//
				if(x<v->width && x>-1)
					//***&& (xx+source_x)>-1 && (xx+source_x)<s_width )
				{
					if( srcbuf[ad+xx+source_x] )
						gui_dotPut(v, x,y, color&15);
							else
						if(drawbackground)gui_dotPut(v, x,y, color>>4);
					// if
				}
				// if
			}
			// for
		}
		// if
	}
	// for

	//
	return;
}

//
void gui_DrawBitmap(VMODE *v,
			BYTE *src, int swidth, int sheight, BYTE *pal,
			int xp, int yp,
			int lenx,int leny,
			int Xread,int Yread)
{
	DrawMonoBitmap(v,	src, swidth,sheight, pal,
				xp,yp,
				lenx,leny,
				Xread,Yread);
}

//

