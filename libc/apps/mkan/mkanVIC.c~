//
#include <stdio.h>
#include "mkan.h"
#include "mkanVIC.h"

//
void hiresplot(unsigned char *frame,int x,int y,int c)
{
 unsigned char bits[]={128,64,32,16,8,4,2,1};
 long a1,a2,a3;
 //
 if(c!=0)
 {
  a2=(y>>3)*320;
  a3=(y & 7)+a2+(x & 0xff8);
  frame[a3]=frame[a3]|bits[x&7];
 }
}

void hirescut(unsigned char *frame,int x,int y,int c)
{
 unsigned char bits[]={128,64,32,16,8,4,2,1};
 long a1,a2,a3;
 //
 if(c!=0)
 {
  a2=(y>>3)*320;
  a3=(y & 7)+a2+(x & 0xff8);
  frame[a3]=frame[a3]&(255-bits[x&7]);
 }
}

//
int multiread(unsigned char *frame,int x,int y)
{
	int re1,re2;

	//
	re1 = hiresread(x+0, y);
	re2 = hiresread(x+1, y);
	if(re1)re1=2;
	if(re2)re2=1;
	return re1|re2;
}

int hiresread(unsigned char *frame,int x,int y)
{
 unsigned char bits[]={128,64,32,16,8,4,2,1};
 long ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 ad2=(y>>3)*320;
 ad3=(y & 7)+ad2+(x & 0xff8);
 m1=frame[ad3]&bits[x&7];
 if(m1!=0)m1=1;
 return m1;
}

void multiplot(unsigned char *frame,int _x,int _y,int _c)
{
 _x=_x&0xffe; // Graphic screen at 160x200+4c, or 2x1 pixels 320x200 display.
 switch(_c)
 {
  case 0:
  hirescut( frame,_x+0,_y,1);
  hirescut( frame,_x+1,_y,1);
  break;
  case 1:
  hirescut( frame,_x+0,_y,1);
  hiresplot(frame,_x+1,_y,1);
  break;
  case 2:
  hiresplot(frame,_x+0,_y,1);
  hirescut( frame,_x+1,_y,1);
  break;
  case 3:
  hiresplot(frame,_x,  _y,1);
  hiresplot(frame,_x+1,_y,1);
  break;
 }
}


