// mkanImage.c - Image processing functions
#include <stdio.h>
#include "mkan.h"

//
int jtmgl_blurbuffer(unsigned char *buf, int w,int h)
{
	//

}

//
void mkan_resizeframe(unsigned char *buf,int sx,int sy)
{
	int ad,ad2,ad3,ad4;
	unsigned char *buf2;

	//
	buf2=malloc(sx*sy+1024);

	//
	for(ad=0; ad<extparam[5]; ad++)jtmgl_blurbuffer(buf,sx,sy);

	//
	if(!extparam[4])
	{
		for(ad=0; ad<(320*200); ad++)buf2[ad]=0;
	}
	else
	{
		for(ad=0; ad<(320*200); ad++)buf2[ad]=extpointer[0][ad];
	}

	//
	jtmgl_stretchputimage1(buf2,320,200,8,
		buf,sx,sy,
		extparam[0],extparam[1],
		extparam[2],extparam[3],
		8);

	//
	for(ad=0; ad<(320*200); ad++)buf[ad]=buf2[ad];

	//
	free(buf2);
}

//
int mkan_decode_pcxto(unsigned char *_srcfname,char *_buffer,unsigned char _paletteadd)
{
	char *tmp;
	int x,y,i,i2,i3,i4;

	// TODO ??
	//
	decode_pcxto(_srcfname,_buffer,_paletteadd);

	// Image isn't 320x200? Resize it to 320x200.
	if(ixl!=320 || iyl!=200)
	{
		//
		tmp=calloc(1,320*200+1024);
		jtmgl_stretchputimage1(tmp,320,200,8,
			_buffer,ixl,iyl,
			0,0,320,200,
			8);
		//
		for(i=0; i<64000; i++)_buffer[i]=tmp[i];
		//
		free(tmp);

		//
		mkan_resizeframe(_buffer,320,200);
	}
	else
	{
		mkan_resizeframe(_buffer,ixl,iyl);
	}
}

//
int mkan_jtm_loadbmp(char *fname,char *frame) // Loads 24bit & 8bit
{
	//
	jtm_loadbmp(fname,frame);

	//
	mkan_resizeframe(frame,ixl,iyl);
}



