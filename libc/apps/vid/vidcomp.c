// vidcomp.c - JTMOS video compressor 1.2003
// (C) 2003 by Jari Tuominen
#include <stdio.h>
#include "vidpak.h"
#include "vidcomp.h"
#include "vidfunc.h"
#include "vidhdr.h"

//-------------------------------------------------------------
// Number of frames to search past
#define N_SEARCH 100
//-------------------------------------------------------------
// Video block size
int VID_BLOCK_SIZE=		16;
// Minimum noise detection level
int MIN_NOISE_DET_LEVEL=	32;
// 1 = Fair
// 2 = Good
// 3 = Very good
int QUALITY=3;
// How often keyframes? (larger = better quality)
int keyFrameInterval=		8;
// SIMILIARITY CHECK / ACCEPTANCE LEVEL
//
// 100 =		keep all drawn (produces bigger files)
// 16 =			ignore somethings (smaller files)
// 2 =			ignore alot (much smaller files)
int SIM_LEVEL=			2;
// Large ignore, more saving
// (0-7)
int IGNORE_BITS=		3;

//-------------------------------------------------------------
//
VC vc;
int keyFrameNow;

//-------------------------------------------------------------
// Detect frame stepping
int detectFrameWalking(const char *src)
{
	char str[256];
	int i,i2,beg,step;

	// Search for suitable file
	for(i=0,beg=-1; i<N_SEARCH; i++)
	{
		//
		sprintf(str, "%s%d.raw", src, i);
		if(fexist(str))
		{
			// Set beginning frame
			beg = i;
			break;
		}
	}

	// Check beginning frame #
	if(beg==-1)
	{
		// Error
		printf("Unable to find start frame.\n");
		return 1;
	}

	// Detect stepping
	i++;
	for(i2=0,step=-1; i2<100; i2++,i++)
	{
		//
		sprintf(str, "%s%d.raw", src, i);
		if(fexist(str))
		{
			// Set beginning frame
			step = i-beg;
			break;
		}
	}

	// Check stepping
	if(step==-1)
	{
		// Error
		printf("Unable to detect frame stepping (video must have atleast two frames).\n");
		return 2;
	}

	// Set settings
	vc.step = step;
	vc.beg = beg;

	// OK
	return 0;
}


// Output video header
int outputVideoHeader(FILE *f)
{
	VIDHDR hdr;

	// Init header
	memset(&hdr, 0, sizeof(VIDHDR));

	// Store identification string
	strcpy(hdr.ident, "JTMVIDEO-1");

	// Set resolution
	hdr.width = vc.width;
	hdr.height = vc.height;
	hdr.bpp = vc.bpp;

	// Write whole header to stream
	fwrite(&hdr, sizeof(VIDHDR),1, f);
}

// Determine block size according relative noise level
int determineBlockSize(int nl, int top)
{
	//
	if( nl>=(top/2) )
	{
		// High resolution
		return 1;
	}
	else
	if( nl>=(top/15) )
	{
		// Medium resolution
		return 2;
	}
	else
	if( nl>=(top/30) )
	{
		// Low resolution
		return 4;
	}
	else
	if( nl>=(top/60) )
	{
		// Low resolution
		return 8;
	}
	else
		// Very low resolution
		return 16;
}

// Get similiarity
int getBlockSim(BYTE *buf, int tx,int ty, int bs)
{
	int x,y,ad,ad2,i,i2,i3,i4,total,sim;

	// If key frame, just ignore this check
	if(keyFrameNow)
		return 0;

	//
	for(y=ty,total=0,sim=0; y<ty+bs; y++)
	{
		//
		ad = y*vc.width;

		//
		for(x=tx; x<tx+bs; x++,total++)
		{
			//
			if((vc.tmp[ad+x]>>IGNORE_BITS)==(vc.last[ad+x]>>IGNORE_BITS))
				sim++;
		}
	}

	// Too much similiarity? Let's skip it to save space!
	if(sim<(total/SIM_LEVEL))
		return 1;

	//
	return 0;
}

// BLOCK ENCODING
// buf = PTR to the current frame buffer
// x,y = coordinate
// bs = block size
// sz = pixel stepping
int outputVideoBlock(BYTE *buf, int xs,int ys,
	int bs, int step, FILE *f2)
{
	int i,i2,i3,i4,x,y,ad,ad2,xx,yy,avg;
	long long big;

	// Block is almost same as the last frame's one?
	if( getBlockSim(buf, xs,ys, bs) )
	{
		// Let's skip it then to save space
		fputc(0xAF, f2);
		return 0;
	}

	// Indicate new block
	fputc(0xA0, f2);
	// Store stepping
	fputc(step, f2);
	// Amount of bytes in this block
	fputc((bs*bs/step)&255, f2);
	fputc((bs*bs/step>>8)&255, f2);

	//----------------------------------------------
	//
	for(y=ys; y<ys+bs; y+=step)
	{
		//
		ad = y*vc.width;

		//
		for(x=xs; x<xs+bs; x+=step)
		{
			//----------------------------------------------
			// Calculate avg. color
			if(step!=1)
			{
				// Pick all pixels
				for(yy=0,i=0,ad2=ad; yy<step; yy++,ad2+=vc.width)
				{
					//
					for(xx=0; xx<step; xx++,i++)
					{
						//
						vc.t[i] = buf[ad2+xx+x];
					}
				}

				// Calculate avg.
				for(i2=0,big=0; i2<i; i2++)
					big+=vc.t[i2];
				big/=i;

				// Store color
				avg = big;
			}
			else
				// Just pick color
				avg = buf[ad+x];

			// Output video data
			fputc(avg, f2);

			//
		}
	}

	//
	return 0;
}

// Determine a block's noise level
int getBlockNoiseLevel(BYTE *buf, int tx,int ty, int bs)
{	
	int x,y,i,i2,ad,ad2,nl,a1,a2,a3,a4;

	//
	for(y=ty,nl=0; y<ty+bs; y++)
	{
		//
		ad = vc.width*y;

		//
		for(x=tx; x<(tx+bs-4); x++)
		{
			// Calculate noise variation
			a1 = buf[ad+x]-buf[ad+x+4];
			if(a1<0) a1 = -a1;

			// Add to current noise level
			nl += a1;
		}
	}

	//
	return nl;
}

// Video frame encoder
int encodeFrame1(BYTE *buf, FILE *f2)
{
	static int i,i2,i3,i4,x,y,bs,ad,ad2,nl,avg,top,
		a1,a2,a3,a4,a5,a6,a7,a8,sz;

	//----------------------------------------------------
	// Set block size
	bs = VID_BLOCK_SIZE;

	//----------------------------------------------------
	// Search frame for top noise spot
	//
//	printf("Determining top noise level ...\n");
	for(y=0,top=0; y<vc.height; y+=bs)
	{
		//
		for(x=0; x<vc.width; x+=bs)
		{
			// Get noise level
			nl = getBlockNoiseLevel(buf, x,y, bs);

			// Exceeds current top noise level?
			if(nl>top)
				top = nl;
		}
	}

	// Check top noise level
	if(top<MIN_NOISE_DET_LEVEL)
	{
		// Noise level must be atleast X
		top = MIN_NOISE_DET_LEVEL;
	}

	//----------------------------------------------------
	// Encode frame
	//

	// Indicate new frame
	fputc(0xB0, f2);
	// Output block size
	fputc(bs, f2);

	//
//	printf("Encoding frame ...\n");
	for(y=0; y<vc.height; y+=bs)
	{
		//
		for(x=0; x<vc.width; x+=bs)
		{
			// Get noise level
			nl = getBlockNoiseLevel(buf, x,y, bs)*QUALITY;
			// Fix
			if(nl==0)
				nl = 1;

			// Get suggested block size
			sz = determineBlockSize(nl, top);

			// Is it valid?
			if( !(sz>=1 && sz<=bs) )
				sz = bs;

			// Output a single block of video
			outputVideoBlock(buf, x,y, bs, sz, f2);
		}
	} 

	//
	return 0;
}

// buf = where to read to
// f = where to read from
// srclen = number of bytes to read
// bypp = bytes per pixel
void ReadAndConvert(unsigned char *buf,
	int srclen, int bypp, FILE *f)
{
	int i,i2,t,d,si;

	//
	for(i=0,si=0; i<srclen; i+=bypp)
	{
		for(i2=0,t=0; i2<bypp; i2++)
		{
			if( (d = fgetc(f))==EOF )
				return;
			t += d;
		}
		t /= bypp;
		buf[si++] = t;
	}
}

// Encode a single video frame and
// output compressed video frame to
// the output stream.
int encodeFrame(const char *src, FILE *f2)
{
	FILE *f;
	BYTE *buf;
	int l;

	// Get frame buffer PTR
	buf = vc.tmp;

	// Wipe
//	memset(buf, 0, vc.width*vc.height);

	// Load frame
	f=fopen(src, "rb");
	if(f==NULL)
		return 1;
	fseek(f,0,SEEK_END);
	l = ftell(f);
	fseek(f,0,SEEK_SET);
	if(l==(320*240*3))
		ReadAndConvert(buf, 320*200*3, 3, f);
	else
	if(l==(320*256*3))
		ReadAndConvert(buf, 320*200*3, 3, f);
	else
	if(l==(320*200*3))
		ReadAndConvert(buf, 320*200*3, 3, f);
	else
	if(l==(320*200))
		fread(buf, vc.width*vc.height,1, f);
	else
	{
		fclose(f);
		printf("Unknown raw file size.\n");
		abort();
	}
	fclose(f);

	// Begin encoding
	encodeFrame1(buf, f2);

	// Return OK
	return 0;
}

// Compress video
int compress1(const char *src, const char *dst)
{
	int i,frame,keycounter;
	FILE *f,*f2;
	char str[256];

	// Create destination file
	f2 = fopen(dst, "wb");
	if(f2==NULL)
	{
		// File creation error
		printf("Unable create destination file(%s).\n",
			dst);
		return 1;
	}

	// Output header
	outputVideoHeader(f2);

	// Clear last frame
	memset(vc.last, 0, vc.width*vc.height);

	// Frame loop
	for(frame=vc.beg,i=0,keycounter=0; ; frame+=vc.step,i++)
	{
		// Check source frame file
		sprintf(str, "%s%d.raw", src, i);
		if(!fexist(str))
			break;

		// Report
		printf("Encoding frame %d (%dK) ...\n",
			frame, ftell(f2)/1024);

		// Encode a single frame
		encodeFrame(str, f2);

		// Produce last frame
		memcpy(vc.last, vc.tmp, 64000);

		// Estimate key frame
		if( (keycounter++)>=keyFrameInterval )
		{
			keycounter=0;
			keyFrameNow = 1;
		}
		else
			keyFrameNow = 0;
	}

	// Mark end
	fputc(0xFF, f2);

	// Close output stream
	fclose(f2);

	// Report
	printf("%d frames compressed.\n",
		i);
}

// Initialize compressor
int initCompressor(void)
{
	// Set output format parameters
	vc.width = 320;
	vc.height = 200;
	vc.bpp = 8;

	// Allocate temporary frame buffer
	vc.tmp = malloc(vc.width*vc.height*(vc.bpp/8));

	// Allocate "last frame" buffer
	vc.last = malloc(vc.width*vc.height*(vc.bpp/8));

	// Allocate misc. temporary storage
	vc.t = malloc(1024*64);

	// Return OK
	return 0;
}

// Detect video frames & compress video 
int compress(const char *src, const char *dst)
{
	//
	initCompressor();

	//
	if( detectFrameWalking(src) )
	{
		//
		printf("Unable find any suitable video frame files.\n");
		printf("Searched for %s0.raw - %s100.raw.\n",
			src, src);
		return 1;
	}

	//
	compress1(src, dst);
}



