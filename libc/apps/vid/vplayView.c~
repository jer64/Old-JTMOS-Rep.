// vplayView.c
#include <stdio.h>
#include "vplay.h"
#include "vplayView.h"

// 1 = fast playback as default (70 fps)
// 2 = pretty fast playback
// 3 = normal (~12 fps)
int SLOWDOWN=3;

//
VIDHDR hdr;

// Decode a single block of frame
int decodeBlock(FILE *f, BYTE *buf, int tx,int ty, int bs)
{
	int x,y,i,i2,i3,i4,step,ad,ad2,bytes,col,xx,yy,last,col1;

	//--------------------------------------------------
	// Get block ID
	i = fgetc(f);
	if( (i&0xF0)!=0xA0 )
	{
		// Incorrect block
		return 1;
	}

	// Skip block?
	if(i==0xAF)
	{
		// Let's skip this block
		return 0;
	}

	// Get step (pixel size)
	step = fgetc(f);

	// Get amount of bytes in this block
	bytes = fgetc(f) + (fgetc(f)<<8);

	//--------------------------------------------------
	// Draw block
	for(y=ty; y<ty+bs; y+=step)
	{
		//
		ad = hdr.width*y;

		//
		for(x=tx; x<tx+bs; x+=step)
		{
			//
			col = fgetc(f);

                        //-----------------------------------
                        // Store as solid pixel
                        for(yy=0,ad2=ad; yy<step; yy++,ad2+=hdr.width)
                        {
                                //
                                for(xx=0; xx<step; xx++)
                                {
                                        //
                                        buf[ad2+xx+x] = col;
                                }
                        }

			//
			bytes--; if(bytes==0) return 0;
		}
	}

/*	//--------------------------------------------------
	// Attempt to antialiasize the rectangle area
	if(step>1)
	{
		//
		for(y=ty; y<(ty+bs-(step>>1)); y++)
		{
			//
			ad = hdr.width*y;
			ad2 = hdr.width*(y+(step>>1));
	
			//
			for(x=tx; x<(tx+bs-(step>>1)); x++)
			{
				//
				buf[ad+x] = (buf[ad+x] + buf[ad+x+(step>>1)] +
					buf[ad2+x] + buf[ad2+x+(step>>1)] 
					)>>2;
			}
		}
	}*/

	//
	return 0;
}

// Code a single frame
int decodeFrame(FILE *f, BYTE *buf, int w,int h,int bpp)
{
	int x,y,i,i2,i3,i4,step,bs;

	// Get frame beginning code
	i = fgetc(f);
	if(i!=0xB0)
		// End frame
		return 1;

	// Get block size
	bs = fgetc(f);

	//
	for(y=0; y<h; y+=bs)
	{
		//
		for(x=0; x<w; x+=bs)
		{
			//
			if( decodeBlock(f, buf, x,y, bs) )
			{
				// Incorrect block
				return 2;
			}
		}
	}

	//
	return 0;
}

// Video player start
int startPlayer(const char *src)
{
	int frame,i,i2,i3,i4,reason,place,total;
	BYTE *buf;
	FILE *f;

	//
	reason=0;

	//
	f=fopen(src, "rb");
	if(f==NULL)
		return 1;

	// Read header
	fread(&hdr, sizeof(VIDHDR),1, f);

	// Get data offset
	place = ftell(f);

	// Allocate frame buffer
	buf = malloc(320*200);

	//
	openGraphics();

	// While ...
	for(total=0; ;)
	{
		// Seek to data offset
		fseek(f, place, SEEK_SET);

		// Go through all frames
		for(frame=0; ; frame++,total++)
		{
			//
			if( (reason=decodeFrame(f, buf, 320,200,8)) )
				// end of frames
				break;
	
			// View frame
			if(!fastMode)
			{
				for(i=0; i<SLOWDOWN; i++)
					waitsd();
			}
			else
			{
				waitsd();
			}
			vf(buf);
	
			//
			if(bioskey(1)!=0)
				break;
		}

		//
		if(bioskey(1)!=0)
			if(getch()==27)	break;
	}

	//
	closeGraphics();

	//
	printf("%d frames played (exit reason=%d).\n",
		total, reason);

	// Return OK
	return 0;
}


