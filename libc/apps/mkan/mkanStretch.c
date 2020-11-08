#include <stdio.h>
#include "mkanStretch.h"

//
void jtmgl_stretchputimage1(char *vbuffer,long jwidth,long jheight,long outbpp,
	char *t, int width,int height, int xs,int ys,int xm,int ym,long srcbpp)
{
	int x,y,xx,yy,tx,ty,ad,ad2,ad3,ad4,maxthru;
	float fa,fb,fc,fd,fe,ff,fg,fh,fi,fx,fy,fxs,fys,fxm,fym,fxp,fyp;
	int NONBLACK;

	// Actually we currently support 8BPP mode only :]
	if(outbpp!=8 || srcbpp!=8)goto loppu;

	//
	if(jwidth<0 || jheight<0)
	{
		goto loppu;
	}

	//
	if(width<0 && height<0)
	{
		width=-width;
		height=-height;
		NONBLACK=1;
	}
	else
	{
		NONBLACK=0;
	}

	//
	fa=xs; fb=xm; fd=width; fc=fd/(fb-fa); fxp=fc;

	//
	fa=ys; fb=ym; fd=height; fc=fd/(fb-fa); fyp=fc;

	//
	ad4=width*height;

	//
	for(y=ys,fy=0; y<ym; y++,fy=fy+fyp)
	{
		if(y<jheight)
		{
			ty=fy;
			if(ty<height && ty>-1)
			{
				ad=jwidth*y;
				ad2=width*ty;
				for(x=xs,fx=0; x<xm; x++,fx=fx+fxp)
				{
					tx=fx;
					if(tx>=width)break;
					if(y<jheight && x<jwidth && y>-1 && x>-1 && tx>-1 && ((ad2+tx)<ad4) )
					{
						if(NONBLACK)
						{
							if(t[ad2+tx])vbuffer[ad+x]=t[ad2+tx];
						}
						else
						{
							vbuffer[ad+x]=t[ad2+tx];
						}
						//
					}
				}
			}
		}
		//
	}
loppu:
	//
}




