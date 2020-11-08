//
#include <stdio.h>
#include "mkan.h"
#include "mkanBMP.h"

//
int jtm_loadbmp(const char *fname,const char *frame)
{
	// * Windows bitmap format *
	//
	// (24bit)                      (8bit)
	// 0000: 42 4d 38 ee            0000: 42 4d 38 fe
	// 0012: (word) x-width         ...
	// 0016: (word) y-heigth        ...
	// 0036: *bitmap*               0036: *palette - 1024 bytes (R G B -)*
	//                              0436: *bitmap*
	//
	unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
	unsigned char header[0xff];
	unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
	int width,height;
	FILE *fp,*fp2,*fp3,*fp4;

	//
	fp=fopen(fname,"rb");
	if(fp==NULL)
	{
		return 2;
	}
	for(ad=0; ad<0x36; ad++)header[ad]=fgetc(fp);

	//
	width=header[0x12]|(header[0x13]<<8);
	height=header[0x16]|(header[0x17]<<8);

	//
	fseek(fp,0x36,SEEK_SET);

	//
	if(!(header[0]==0x42 && header[1]==0x4d))return 3;

	//
	if(header[0x1c]==24)
	{
		for(y=0; y<height; y++)
		{
			for(x=0; x<width; x++)
			{
				 frame[(199-y)*width+x]=(fgetc(fp)+fgetc(fp)+fgetc(fp))/3;
			}
		}
	}

	//
	if(header[0x1c]==8)
	{
		fseek(fp,0x436,SEEK_SET);
		for(y=0; y<height; y++)
		{
			for(x=0; x<width; x++)
			{
				frame[((height-1)-y)*width+x]=fgetc(fp);
			}
		}
	}
	else
	{
		fclose(fp);
		return 1;
	}

	//
	fclose(fp);
	ixl=width;
	iyl=height;
	return 0;
}


