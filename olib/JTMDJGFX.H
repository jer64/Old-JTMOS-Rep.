/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/** JTMOS series -- DJGPP port.            Another fine                 **
 **                                        JTMOS library for DJGPP!     **
 ** °°°°°° °°°°°°  °°°°°   °°°°°   °°°°°°                               **
 **     ±±   ±±   ±± ± ±± ±±   ±± ±±                                    **
 ** ²²  ²²   ²²   ²²   ²² ²²   ²²  ²²²²²    (Graphics  + G              **
 ** ÛÛ  ÛÛ   ÛÛ   ÛÛ   ÛÛ ÛÛ   ÛÛ      ÛÛ    User        F              **
 ** ÛÛ  ÛÛ   ÛÛ   ÛÛ   ÛÛ ÛÛ   ÛÛ ÛÛ   ÛÛ    Interface   X)             **
 **  ÛÛÛÛ    ÛÛ   ÛÛ   ÛÛ  ÛÛÛÛÛ   ÛÛÛÛÛ                                **
 **                                                                     **
 ** P O W E R  G R A P H I C S  L I B R A R Y  (Beta version)           **
 **                                                                     **
 ** -Features powerful Windowing system! (Underbuild)                   **
 ** -Support for multitasking(Underbuild)                               **
 ** -Supports all other JTMOS series libraries, naturally :)            **
 **                                                                     **
 **  (for DJGPP v2.x) -- Tested on S3VIRGE 3D(Model 325), vesa          **
 **                      1.2 & 2.0 & 3.0.                               **
 **                      Pentium-S running in 180Mhz.                   **
 **                                                                     **
 **                      Tested on S3VIRGE 3D(Model 375), vesa          **
 **                      1.2 & 2.0 & 3.0.                               **
 **                      Pentiun MMX running in 188Mhz(83Mhz BUS)       **
 **                                          .. 210Mhz(83Mhz BUS)       **
 **                                                                     **
 **     (C)1997-99 by Jari Tuominen, all rights reserved.               **
 **                                                                     **
 **   This library is updated very often, but some standard functions   **
 **   will be allways found.                                            **
 **                                                                     **

 Since long time ago, I ended my the Borland C/C++ "JxW" libraries developing,
 and I started to make new JTM series, which are in case
 quick conversions from older JxW ones, but since many routines have been
 replaced with new faster routines, in case many things have been really
 become much faster as I said, f.e. SVGA output, it is now about %50 faster,
 but it still doesn't feature linear frame buffer, but soon will, I promise.

 This library features(atleast):

 -> Quick image handling on VGA/VESA modes.
 -> All standard functions that you'll need when you're creating demo effects.

 Tip: Search for string "font drawing function" if you wish to
      edit code of the graphical font writter.

 Special thanks :

        To all those people who's code I have been looking for a little
        help in middle of this code work,
        so alot help 'given externally' :)
        (Everybody needs help at
         the beginning or even in veteran days ;) )

 */

/********************************************

  Vmodes below are the video modes which
  did work fine with my S3 virge clone,
  "Sparkle VGA CARD".
  All video modes like 16M/64K/32K ones are
  not defined same in other carts than S3,
  please include your own codes, if you want
  to access real color modes.

  Thanks to Oliver Fromme for nice utility
  called "vesatest", very fine nice little
  utility, which comes with QPV, I guess.

 ********************************************/

// jtmgl_arrowsystemon -> Turn ON the arrow system (Draws arrow in every
//                        jtmscr_updateVRAM() )
// jtmgl_arrowsystemoff-> Turn OFF the arrow system ....

/***********************************************/
// JTMOS graphic converting library
#ifndef __JTMDJGFX_H__
#define __JTMDJGFX_H__
/***********************************************/
#define __JTMDJGFX_H_INCLUDED TRUE
#include <jtmconv.h>
#include <jtmvga.h>
#include <jtmeff.h>

#define ValiStep 3

#define SmoothValue 2
#define kertoin 2

#define plusx1 0*kertoin
#define plusx2 1*kertoin
#define plusx3 2*kertoin
#define plusx4 3*kertoin
#define plusx5 4*kertoin

#define plusy1 0*320*kertoin
#define plusy2 1*320*kertoin
#define plusy3 2*320*kertoin
#define plusy4 3*320*kertoin
#define plusy5 4*320*kertoin

void jtmgl_drawscanline(unsigned line,unsigned char color);
void jtmscr_createnoisebackground(void);
void jtm_drawsbutton(void);
void jtmscr_piirratausta(void);
void jtmscr_requestsr(void);
void jlinew(int _x1,int _y1,int _x2,int _y2,unsigned char color,char width);

#include <jtmactiv.h>
#include <jtmsps.h> // SPS image de/compressing library

int jtmscr_set320200(void)
{
 jtmscr.fontcolor=0x3f;
 NewWindow.fontcolor=0x3f;
 NewWindow.fontbcolor=0;
 jtmscr.graphictimer=0;
 jtmscr.bpp=8;
 jtmscr.width=320;
 jtmscr.height=200;
 jtmscr.writexs=0; jtmscr.writeys=0;
 jtmscr.writexm=jtmscr.width-8; jtmscr.writeym=jtmscr.height-8;
 jtmscr.vbuffer=malloc(640*400);
 if(jtmgl_getfontsomewhere())
 {
  return 1;
 }
 return 0;
}

int jtmscr_setx(long bpp,long width,long height,
                long writexs,long writeys,long writexm,long writeym,
                long graphictimer,long fontcolor,long fontbcolor)
{
 jtmscr.fontcolor=fontcolor;
 NewWindow.fontcolor=fontcolor;
 NewWindow.fontbcolor=fontbcolor;
 jtmscr.graphictimer=graphictimer;
 jtmscr.bpp=bpp;
 jtmscr.width=width;
 jtmscr.height=height;
 jtmscr.writexs=writexs; jtmscr.writeys=writeys;
 jtmscr.writexm=writexm; jtmscr.writeym=writeym;
 if(bpp==8){jtmscr.vbuffer=malloc(width*height+1024*64);}else{jtmscr.vbuffer=malloc(width*height*4+1024*64);}
 if(jtmgl_getfontsomewhere())
 {
  return 1;
 }
 return 0;
}

void jtmgl_textcolor(unsigned long int textc)
{
 if(jtm_vmode==3)
 {
  textcolor(textc);
 }
 else
 {
  jtmscr.fontcolor=textc;
 }
}

void jtmgl_textbackground(unsigned long int textbc)
{
 jtmscr.textbackgroundcolor=textbc;
}

void jtm_setscreenstart(unsigned _loc)
{
 unsigned a1;
 //
 a1=0x0c; a1=a1|(_loc&0xff00);
 outportw(0x3d4,a1);
 //
 a1=0x0d; a1=a1|((_loc&0xff)<<8);
 outportw(0x3d4,a1);
}


void jtm_setemode(unsigned _mode)
{
 jtm_emode=_mode;
}

void jtm_loadpalette(char *filename,char *p)
{
 FILE *_fp;
 _fp=jtm_fopen(filename,"rb");
 fread(p,768,1,_fp);
 jtm_fclose(_fp);
}

void jtm_upscroll320(char *__videoframe,long PAIKKA)
{
 if(jtm_waitsdALLWAYS==1)jtm_waitsd();
 dosmemput(__videoframe+(PAIKKA*320), 0x10000, 0xb8000);
}

void jtm_readjascpalette(unsigned char *fname,unsigned char *pal256)
{
 FILE *f,*f2;
 unsigned long ad=0;
 char str[255];
 //
 f=jtm_fopen(fname,"rb");
 fscanf(f,"%s",&str);
 fscanf(f,"%s",&str);
 fscanf(f,"%s",&str);
 for(ad=0; ad<256; ad++)
 {
  fscanf(f,"%u",&pal256[ad*3+0]);
  fscanf(f,"%u",&pal256[ad*3+1]);
  fscanf(f,"%u",&pal256[ad*3+2]);
 }
 jtm_fclose(f);
}

void jtm_writejascpalette(unsigned char *fname,unsigned char *pal256)
{
 FILE *f,*f2;
 unsigned long ad0,ad=0;
 //
 f=jtm_fopen(fname,"wt");
 fprintf(f,"JASC-PAL\n"); // Indentify string
 fprintf(f,"0100\n"); // Version?
 fprintf(f,"256\n");  // Amount of colors?
 for(ad=0; ad<256; ad++)
 {
  fprintf(f,"%u %u %u\n",pal256[ad*3+0],pal256[ad*3+1],pal256[ad*3+2]);
 }
 jtm_fclose(f);
}

int jtm_setjascpalette(char *_fname)
{
 FILE *_fp;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 register unsigned _colori;
 char str[255];
 //
 _fp=jtm_fopen(_fname,"rt");
 if(_fp==NULL)exit(1);
 fscanf(_fp,"%s",&str);
 if(stricmp(str,"JASC-PAL"))
 {
  jtm_fclose(_fp);
  exit(3);
 }
 fscanf(_fp,"%u",&ad);
 fscanf(_fp,"%u",&ad2);
 for(_colori=0; _colori<0x100; _colori++)
 {
  fscanf(_fp,"%u",&ad);
  fscanf(_fp,"%u",&ad2);
  fscanf(_fp,"%u",&ad3);
  jtm_setpalette(_colori,ad,ad2,ad3);
 }
 jtm_fclose(_fp);
 return 0;
}

void jtm_palettereset(float clevel)
{
 unsigned uad,uad2; float fa,fb,fc;
 //
 fc=1/clevel;
 for(uad=0,fa=0; uad<0x100; uad++,fa=fa+fc)
 {
  uad2=fa;
  jtm_setpalette(uad,uad2>>2,uad2>>4,uad2>>4);
 }
}

void jtm_megapalette(float red11,float green11,float blue11)
{
 unsigned uad;
 float fa,fb,fc;
 for(uad=0,fa=0,fb=0,fc=0; uad<0x100; uad++,fa=fa+red11/4,fb=fb+green11/4,fc=fc+blue11/4)
 {
  jtm_setpalette(uad,fa,fb,fc);
 }
}

void jtm_negativeimage(unsigned char *image1,long size)
{
 register unsigned uad,uad2;
 //
 for(uad=0; uad<size; uad++)
 {
  uad2=image1[uad];
  uad2=0xff-uad2;
  image1[uad]=uad2;
 }
}

void jtm_smoothimage(char *_image,unsigned times)
{
 unsigned c1,c2,c3,c4,uad,uad2;
 //
 for(uad2=0; uad2<times; uad2++)
 {
  for(uad=0; uad<0xfffe; uad++)
  {
   c1=_image[uad];
   c2=_image[uad+1];
   c3=(c1+c2)/2;
   _image[uad]=c3;
  }
 }
}

void jtm_smoothimage32(char *_image,unsigned times,unsigned long int length)
{
 unsigned c1,c2,c3,c4,c5,c6,c7,c8;
 unsigned char ad=0,ad2=0;
 //
 for(ad2=0; ad2<times; ad2++)
 {
  for(ad=0; ad<length; ad+=4)
  {
   c1=_image[ad+0];
   c2=_image[ad+1];
   c3=_image[ad+2];
   c4=_image[ad+3];
   c5=_image[ad+4];
   c6=_image[ad+5];
   c7=_image[ad+6];
   c8=_image[ad+7];
   _image[ad+0x00]=(c1+c5)/2;
   _image[ad+0x01]=(c2+c6)/2;
   _image[ad+0x02]=(c3+c7)/2;
   _image[ad+0x03]=(c4+c8)/2;
  }
 }
}

void jtm_morphimage(char *image1,char *image2)
{
 register unsigned uad;
 //
 for(uad=0; uad<0xfffc; uad++)
 {
  if(image1[uad]!=image2[uad])
  {
   if(image1[uad]<image2[uad])
   {
    image1[uad]++;
   }
   else
   {
    image1[uad]--;
   }
  }
 }
}

int jtm_copytovram(char *_image24,unsigned banks)
{
 unsigned vbank;
 long _ad,_ad2,_ad3;
 //
 for(vbank=0,_ad2=0,_ad3=0; vbank<banks+1; vbank++,_ad3+=0x10000)
 {
  jtm_setvbank(vbank);
  jtm_vf(_image24+_ad3);
 }
 return 0;
}

void jtm_load24to32(char *_fname,char *_buffer) // Loads and converts 24bit truecolor image to
                                              // 32bit truecolor image
{
 unsigned long ad0,ad=0;
 unsigned char m1,m2,m3,m4;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 fp=jtm_fopen(_fname,"rb");
 ad=0;
 do
 {
  m1=fgetc(fp);
  m2=fgetc(fp);
  m3=fgetc(fp);
  m4=0; // 4th byte in 32bit 640x480 16M colors video mode is usually just set to zero,
        // memory wasting?, maybe, but the memory bit model is more fitable to use with
        // 32bit PMODE registers EAX, EDX, and so on, which are in case quite fast.
  _buffer[ad+0x00]=m3; // R
  _buffer[ad+0x01]=m2; // G  ==> Paint shop uses, order BGR, so I made the function
  _buffer[ad+0x02]=m1; // B      to use that rule.
  _buffer[ad+0x03]=m4;
  ad+=4;
 }while(!feof(fp));
 jtm_fclose(fp);
}

#ifndef __NOMODERNGRAPHICS__
/*
 *
 *
 * JxWindows graphic engine version 1.00 -- Tribute to the Bill Gates.
 * MODERN GRAPHICS LIBRARY -- Begins here !
 *                 (C)1997 by Jari Tuominen, all rights reserved.
 *
 */

void jlinew(int _x1,int _y1,int _x2,int _y2,unsigned char color,char width)
{
 int widx,widy;
 unsigned x,y;
 unsigned long ad=0,ad2=0;
 //
  x=_x1; y=_y1;
  do
  {
   ad=(y*jtmscr.width+x);
   for(widy=0; widy<width; widy++)
   {
    ad2=widy*jtmscr.width;
    for(widx=0; widx<width; widx++)
    {
     jtmscr.vbuffer[ad+ad2+widx]=color;
    }
   }
   if(y!=_y2)
   {
    if(y<_y2)y++;
    if(y>_y2)y--;
   }
   if(x!=_x2)
   {
    if(x<_x2)x++;
    if(x>_x2)x--;
   }
  }while(x!=_x2 || y!=_y2);
}

void jline(unsigned char *bm,int _x1,int _y1,int _x2,int _y2,unsigned char color)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2;
 //
 x=_x1; y=_y1;
 do
 {
  bm[y*640+x]=color;
  if(y!=_y2)
  {
   if(y<_y2)y++;
   if(y>_y2)y--;
  }
  if(x!=_x2)
  {
   if(x<_x2)x++;
   if(x>_x2)x--;
  }
 }while(x!=_x2 || y!=_y2);
 bm[y*640+x]=color;
}

void jdline(unsigned char *bm,int _x1,int _y1,int _x2,int _y2,unsigned char color)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2;
 long unsigned ad7;
 float ax,ay,fxs,fys,a,b,c,d;
 //
 ax=_x1; ay=_y1;
 a=_x1; b=_x2; c=a-b; fxs=c;
 a=_y1; b=_y2; c=a-b; fys=c;
 fxs=fxs/64; fys=fys/64; ad7=0;
 do
 {
  xx=ax; yy=ay;
  if( !(xx>-1 && yy>-1 && xx<320 && yy<200) )
  {
   bm[yy*320+xx]=color;
  }
  ax=ax-fxs; ay=ay-fys;
 }while(xx!=_x2 || yy!=_y2);
 if( !(xx>-1 && yy>-1 && xx<320 && yy<200) )
 {
  bm[yy*320+xx]=color;
 }
}

void jtm_fillareaw(int x1,int y1,int x2,int y2,char color)
{
 unsigned x,y,ad;
 //
 switch(jtmscr.bpp)
 {
  case 32:
  break;

  case 8:
  for(y=y1; y<y2; y++)
  {
   ad=y*jtmscr.width;
   for(x=x1; x<x2; x++)
   {
    jtmscr.vbuffer[ad+x]=color;
   }
  }
  break;
 }
}

void jtm_boxxi(int xx1,int yy1,int xx2,int yy2)
{
  //
  jline(jtmscr.vbuffer,xx1+0,yy1+0,xx1+0,yy2-0,15); // ^ YLŽ   > ALA
  jline(jtmscr.vbuffer,xx1+0,yy1+0,xx2-0,yy1+0,15); // ^ VASEN > OIKEA
  jline(jtmscr.vbuffer,xx2-0,yy2-0,xx1+0,yy2-0,15); // v OIKEA < VASEN
  jline(jtmscr.vbuffer,xx2-0,yy2-0,xx2-0,yy1+0,15); // v ALA   < YLŽ
  //
  jline(jtmscr.vbuffer,xx1+1,yy1+1,xx1+1,yy2-1,14); // ^ YLŽ   > ALA
  jline(jtmscr.vbuffer,xx1+1,yy1+1,xx2-1,yy1+1,14); // ^ VASEN > OIKEA
  jline(jtmscr.vbuffer,xx2-1,yy2-1,xx1+1,yy2-1,14); // v OIKEA < VASEN
  jline(jtmscr.vbuffer,xx2-1,yy2-1,xx2-1,yy1+1,14); // v ALA   < YLŽ
  //
  jline(jtmscr.vbuffer,xx1+2,yy1+2,xx1+2,yy2-2,7); // ^ YLŽ   > ALA
  jline(jtmscr.vbuffer,xx1+2,yy1+2,xx2-2,yy1+2,7); // ^ VASEN > OIKEA
  jline(jtmscr.vbuffer,xx2-2,yy2-2,xx1+2,yy2-2,7); // v OIKEA < VASEN
  jline(jtmscr.vbuffer,xx2-2,yy2-2,xx2-2,yy1+2,7); // v ALA   < YLŽ
  //
  jline(jtmscr.vbuffer,xx1+3,yy1+3,xx1+3,yy2-3,8); // ^ YLŽ   > ALA
  jline(jtmscr.vbuffer,xx1+3,yy1+3,xx2-3,yy1+3,8); // ^ VASEN > OIKEA
  jline(jtmscr.vbuffer,xx2-3,yy2-3,xx1+3,yy2-3,8); // v OIKEA < VASEN
  jline(jtmscr.vbuffer,xx2-3,yy2-3,xx2-3,yy1+3,8); // v ALA   < YLŽ
  //
  jline(jtmscr.vbuffer,xx1+4,yy1+4,xx1+4,yy2-4,0); // ^ YLŽ   > ALA
  jline(jtmscr.vbuffer,xx1+4,yy1+4,xx2-4,yy1+4,0); // ^ VASEN > OIKEA
  jline(jtmscr.vbuffer,xx2-4,yy2-4,xx1+4,yy2-4,0); // v OIKEA < VASEN
  jline(jtmscr.vbuffer,xx2-4,yy2-4,xx2-4,yy1+4,0); // v ALA   < YLŽ
}

unsigned jtm_dither256to16(unsigned x,unsigned y,unsigned color256)
{
 unsigned c1,c2,c3;             // Dither 256 to 16 color (BW)
 //
 c1=color256/16;
 c2=x & 1;
 c3=x % 1;
 return c1+c3;
}

void jtm_drawgraphbox(int xx1,int yy1,int xx2,int yy2, char type)
{
 float fa,fb,fc,fd,
       fx,fy;
 unsigned x,y;
 long ad,ad2,ad3;
 //
 if(type==1)
 {
  jline(jtmscr.vbuffer,xx1,yy1,xx1,yy2,7); // ^ YLŽ   > ALA
  jline(jtmscr.vbuffer,xx1,yy1,xx2,yy1,7); // ^ VASEN > OIKEA
  jline(jtmscr.vbuffer,xx2,yy2,xx1,yy2,7); // v OIKEA < VASEN
  jline(jtmscr.vbuffer,xx2,yy2,xx2,yy1,7); // v ALA   < YLŽ
 }
 if(type==2)
 {
  for(y=yy1,fa=0; y<yy2; y++,fa+=256/ ((float) (yy1-yy2)))
  {
   fy=y;
   ad=y*jtmscr.width;
   ad3=fa;
   for(x=xx1; x<xx2; x++)
   {
    jtmscr.vbuffer[ad+x]=jtm_dither256to16(x,y,ad3)+32;
   }
  }
  jtm_boxxi(xx1+1,yy1+1,xx2-1,yy2-1);
  jtm_boxxi(xx1+1,yy1+1,xx2-1,yy1+31);
  goto alldone;
 }
 if(type==3)
 {
 }
alldone:
 while(0!=0);
}

int jtm_drawgraphwindow(int x1,int y1,int x2,int y2,
                         int type,
                         char *headline,
                         int filled)
{
 jtm_drawgraphbox(x1,y1,x2,y2, 2);
 return 0;
}
#endif
/** END OF JxWindows DJGPP section **/

void jtm_viewframe(char *bf)
{
 char bf1[0xffff];
 long ad,ad2;
 //
 if(jtm_vmode==0x010f)
 {
  jtm_copytovram(bf,2);
 }
 else
 if(jtm_vmode==0x40)
 {
  for(ad=0,ad2=0; ad<0xffff; ad++,ad2+=4)
  {
   bf1[ad]=(bf[ad2+0]+bf[ad2+1]+bf[ad2+2])/3;
  }
  jtm_copytovram(bf1,1);
 }
}

void jtm_view24bit(char *fn)
{
 char fr[0x3ffff];
 //
 jtm_loadbin(fn,fr);
 jtm_viewframe(fr);
}

void jtm_updatevmode()
{
 if(jtm_vmode>0xff)
 {
  jtm_setvesa1(jtm_vmode);
 }
 else
 if(jtm_vmode==0x40)
 {
  jtm_setvmode1(0x13);
  jtm_greyscale256();
 }
 else
 {
  jtm_setvmode(jtm_vmode);
 }
}

int jtm_viewbmp(char *fname) // Views 24bit & 8bit (in VGA)
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
 unsigned char header[0xff];
 unsigned char frame[0xffff];
 int x,y;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 jtm_setvmode(0x13);
 fp=jtm_fopen(fname,"rb");
 //if(fp==NULL)return 1;
 for(ad=0; ad<0x36; ad++)header[ad]=fgetc(fp);
 fseek(fp,0x36,SEEK_SET);
 if(header[0]==0x42 && header[1]==0x4d && header[2]==0x38 && header[3]==0xee)
 {
  jtm_greyscale256();
  for(y=0; y<200; y++)
  {
   for(x=0; x<320; x++)
   {
    frame[(199-y)*320+x]=(fgetc(fp)+fgetc(fp)+fgetc(fp))/3;
   }
  }
 }
 else
 if(header[0]==0x42 && header[1]==0x4d && header[2]==0x38 && header[3]==0xfe)
 {
  for(ad=0; ad<256; ad++){ jtm_setpalette(ad,fgetc(fp)/4,fgetc(fp)/4,fgetc(fp)/4); fgetc(fp); }
  fseek(fp,0x436,SEEK_SET);
  for(y=0; y<200; y++)
  {
   for(x=0; x<320; x++)
   {
    frame[(199-y)*320+x]=fgetc(fp);
   }
  }
 }
 jtm_fclose(fp);
 jtm_vf(frame);
 return 0;
}


/*void jtmgl_writebmp8(char *fname,char *bitmap) // Creates BMP 8bit (VGA256)
{
 FILE *f;
 unsigned x,y;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 //
 jtm_bmpheader8[0x12]=jtmscr.width&255;
 jtm_bmpheader8[0x13]=jtmscr.width/256;
 //
 jtm_bmpheader8[0x16]=jtmscr.height&255;
 jtm_bmpheader8[0x17]=jtmscr.height/256;
 //
 f=jtm_fopen(fname,"wb");
 for(ad=0; ad<0x36; ad++)
 {
  fputc(jtm_bmpheader8[ad],f);
 }
 for(ad=0; ad<256; ad++)
 {
  fputc(ad,f);
  fputc(ad,f);
  fputc(ad,f);
  fputc(0, f);
 }
 for(y=0; y<jtmscr.height; y++)
 {
  for(x=0; x<jtmscr.width; x++)
  {
   fputc(bitmap[jtmscr.width*((jtmscr.height-1)-y)+x],f);
  }
 }
 jtm_fclose(f);
} */

void jtm_clipscreen(char *fname) // Screen clipper (32bit RGBA/8bit inx.)
{
 FILE *f;
 long a1,a2,a3,a4,a5,a6,a7,a8;
 unsigned x,y;
 //
 switch(jtmscr.bpp)
 {
  case 8:
  jtmgl_writebmp8(fname,jtmscr.vbuffer);
  break;
  case 32:
  jtmgl_writebmp24(fname,jtmscr.vbuffer);
  break;
 }
}

/** JTMOS graphics environment 2 version 1.00 **/
void jtm_vplot(int _x,int _y,unsigned long int c)
{
 jtmscr.drawround++;
 if(jtmscr.vbufferBUSY==1 || jtmscr.vbuffer==NULL)
 {
  jtmscr.vbufferBUSY=0;
 }
 while(jtmscr.vbufferBUSY==2);
 jtmscr.srtab[_y]=0xff;
 jtmscr.vbufferBUSY=2;
 switch(jtmscr.bpp)
 {
  case 8:
  jtmscr.vbuffer[(_y*jtmscr.width+_x)]=c;
  break;
  case 32:
  if( c<256 )
  {
   jtmscr.vbuffer[((_y*jtmscr.width+_x)<<2)+0]=jtmansi.palette[(c<<2)+0]<<2;
   jtmscr.vbuffer[((_y*jtmscr.width+_x)<<2)+1]=jtmansi.palette[(c<<2)+1]<<2;
   jtmscr.vbuffer[((_y*jtmscr.width+_x)<<2)+2]=jtmansi.palette[(c<<2)+2]<<2;
  }
  else
  {
   jtmscr.vbuffer[((_y*jtmscr.width+_x)<<2)+2]=(c>>16)&255;
   jtmscr.vbuffer[((_y*jtmscr.width+_x)<<2)+1]=(c>>8)&255;
   jtmscr.vbuffer[((_y*jtmscr.width+_x)<<2)+0]=(c)&255;
  }
  break;
 }
 jtmscr.vbufferBUSY=0;
 jtmscr.drawround--;
}

void jtmgl_draw_vertical_line(int y1,int x1,int x2,int c)
{
 register long ad,ad2;
 register int x,y;
 //
 jtmscr.drawround++;
 switch(jtmscr.bpp)
 {
  case 8:
  ad=y1*jtmscr.width;
  for(x=x1; x<x2+1; x++)
  {
   jtmscr.vbuffer[ad+x]=c;
  }
  break;
  case 32:
  ad=y1*jtmscr.width;
  for(x=x1; x<x2+1; x++)
  {
   jtmscr.vbuffer[((ad+x)<<2)+0]=jtmansi.palette[(c<<2)+2];
   jtmscr.vbuffer[((ad+x)<<2)+1]=jtmansi.palette[(c<<2)+1];
   jtmscr.vbuffer[((ad+x)<<2)+2]=jtmansi.palette[(c<<2)+0];
  }
  break;
 }
 jtmscr.drawround--;
}

void jtmgl_draw_horizontal_line(int x1,int y1,int y2,int c)
{
 register long ad,ad2;
 register int x,y;
 //
 jtmscr.drawround++;
 switch(jtmscr.bpp)
 {
  case 8:
  ad=y1*jtmscr.width;
  for(y=y1; y<y2; y++)
  {
   jtmscr.vbuffer[ad+x1]=c;
   ad+=jtmscr.width;
  }
  break;
  case 32:
  ad=y1*jtmscr.width;
  for(y=y1; y<y2; y++)
  {
   jtmscr.vbuffer[(ad+x1)<<2]=jtmansi.palette[(c<<2)+0];
   jtmscr.vbuffer[(ad+x1)<<1]=jtmansi.palette[(c<<2)+1];
   jtmscr.vbuffer[(ad+x1)<<0]=jtmansi.palette[(c<<2)+2];
   ad+=jtmscr.width;
  }
  break;
 }
 jtmscr.drawround--;
}

void jtmgl_drectangle(int x1,int y1,int x2,int y2,int c)
{
 register long ad,ad2;
 register int x,y;
 //
 if( x1<0 )
 {
  x1=0;
 }
 if( y1<0 )
 {
  y1=0;
 }
 if(x2>=jtmscr.width)
 {
  x2=jtmscr.width;
 }
 if(y2>=jtmscr.height)
 {
  y2=jtmscr.height;
 }
 //
 jtmgl_draw_horizontal_line(x1,y1,y2,c);
 jtmgl_draw_horizontal_line(x2,y1,y2,c);
 jtmgl_draw_vertical_line(y1,x1,x2,c);
 jtmgl_draw_vertical_line(y2,x1,x2,c);
 //
}

void jtm_dgridbox(int x1,int y1,int x2,int y2,int c)
{
 // Locals
 register
 long ad,ad2,x,y;

 //
 switch(jtmscr.bpp)
 {
  case 8:
  for(y=y1; y<y2; y++)
  {
   ad=jtmscr.width*y;
   if(y>=0 && y<jtmscr.height)
   {
    for(x=x1; x<x2; x++)
    {
     ad2=(x+y)&1;
     if(ad2==0)
     {
      jtmscr.vbuffer[ad+x]=c;
     }
    }
   }
   jtmscr.srtab[y]=0xff;
  }
  break;

  case 32:
  for(y=y1; y<y2; y++)
  {
   ad=jtmscr.width*y;
   if(y>=0 && y<jtmscr.height)
   {
    for(x=x1; x<x2; x++)
    {
     ad2=(x+y)&1;
     if(ad2==0)
     {
      jtmscr.vbuffer[((ad+x)<<2)+0]=c;
      jtmscr.vbuffer[((ad+x)<<2)+1]=c;
      jtmscr.vbuffer[((ad+x)<<2)+2]=c;
     }
    }
   }
   jtmscr.srtab[y]=0xff;
  }
  break;
 }
}

void jtm_dtransparentgridbox(int x1,int y1,int x2,int y2,int c)
{
 // Locals
 register
 long ad,ad2,x,y;

 //
 switch(jtmscr.bpp)
 {
  case 8:
  for(y=y1; y<y2; y++)
  {
   ad=jtmscr.width*y;
   if(y>=0 && y<jtmscr.height)
   {
    for(x=x1; x<x2; x++)
    {
     ad2=(x+y)&1;
     if(ad2==0)
     {
      jtmscr.vbuffer[ad+x]=(c+jtmscr.vbuffer[ad+x])>>1;
     }
    }
   }
   jtmscr.srtab[y]=0xff;
  }
  break;

  case 32:
  for(y=y1; y<y2; y++)
  {
   ad=jtmscr.width*y;
   if(y>=0 && y<jtmscr.height)
   {
    for(x=x1; x<x2; x++)
    {
     ad2=(x+y)&1;
     if(ad2==0)
     {
      jtmscr.vbuffer[((ad+x)<<2)+0]=(c+jtmscr.vbuffer[((ad+x)<<2)+0])>>1;
      jtmscr.vbuffer[((ad+x)<<2)+1]=(c+jtmscr.vbuffer[((ad+x)<<2)+1])>>1;
      jtmscr.vbuffer[((ad+x)<<2)+2]=(c+jtmscr.vbuffer[((ad+x)<<2)+2])>>1;
     }
    }
   }
   jtmscr.srtab[y]=0xff;
  }
  break;
 }
}

void jtm_dbox(int x1,int y1,int x2,int y2,int c)
{
 register int x,y;
 register long a1,a2;
 //
 for(y=y1; y<y2+1; y++)
 {
  switch(jtmscr.bpp)
  {
   case 32:
   a1=jtmscr.width*y;
   if(y<jtmscr.height && y>-1)
   {
    for(x=x1; x<x2+1; x++)
    {
     if(x<jtmscr.width && x>-1)
     {
      jtmscr.vbuffer[((a1+x)<<2)+0]=jtmansi.palette[(c<<2)+2];
      jtmscr.vbuffer[((a1+x)<<2)+1]=jtmansi.palette[(c<<2)+1];
      jtmscr.vbuffer[((a1+x)<<2)+2]=jtmansi.palette[(c<<2)+0];
     }
    }
   }
   break;
   case 8:
   a1=jtmscr.width*y;
   if(y<jtmscr.height && y>-1)
   {
    for(x=x1; x<x2+1; x++)
    {
     if(x<jtmscr.width && x>-1)jtmscr.vbuffer[a1+x]=c;
    }
   }
   break;
  }
//   jtm_vplot(x,y,c);
 }
}

void jtmgl_dbox(int _x1,int _y1,int _x2,int _y2,int _c,int rsu)
{
 register int _x,_y;
 unsigned long _ad;
 //
 // 8 bpp mode
 jtmscr.drawround++;
 if(jtmscr.bpp==8)
 {
  for(_y=_y1; _y<_y2+1; _y++)
  {
   if(rsu!=0)jtmscr.srtab[_y]=0xff;
   _ad=_y*jtmscr.width;
   for(_x=_x1; _x<_x2+1; _x++)
   {
    jtmscr.vbuffer[_ad+_x]=_c;
   }
  }
 }
 // 24 bpp mode
 if(jtmscr.bpp==32)
 {
  for(_y=_y1; _y<_y2+1; _y++)
  {
   jtmscr.srtab[_y]=0xff;
   _ad=_y*jtmscr.width;
   for(_x=_x1; _x<_x2+1; _x++)
   {
    jtmscr.vbuffer[ ((_ad+_x)<<2)+0 ]=jtmansi.palette[(_c<<2)+2]<<2;
    jtmscr.vbuffer[ ((_ad+_x)<<2)+1 ]=jtmansi.palette[(_c<<2)+1]<<2;
    jtmscr.vbuffer[ ((_ad+_x)<<2)+2 ]=jtmansi.palette[(_c<<2)+0]<<2;
   }
  }
 }
 jtmscr.drawround--;
}

void jtmgl_dtransparent_box(int _x1,int _y1,int _x2,int _y2,int _c,int rsu)
{
 register int _x,_y;
 unsigned long _ad;
 //
 // 8 bpp mode
 switch(jtmscr.bpp)
 {
  case 8:
  jtmscr.drawround++;
  if(jtmscr.bpp==8)
  {
    for(_y=_y1; _y<_y2+1; _y++)
   {
    if(rsu!=0)jtmscr.srtab[_y]=0xff;
    _ad=_y*jtmscr.width;
    for(_x=_x1; _x<_x2+1; _x++)
    {
     jtmscr.vbuffer[_ad+_x]=(_c+jtmscr.vbuffer[_ad+_x])>>1;
    }
   }
  }
  break;
  case 32:
  for(_y=_y1; _y<_y2+1; _y++)
  {
   jtmscr.srtab[_y]=0xff;
   _ad=_y*jtmscr.width;
   for(_x=_x1; _x<_x2+1; _x++)
   {
    jtmscr.vbuffer[ ((_ad+_x)<<2)+0 ]=(jtmscr.vbuffer[ ((_ad+_x)<<2)+0 ]+(jtmansi.palette[(_c<<2)+2]<<2))>>1;
    jtmscr.vbuffer[ ((_ad+_x)<<2)+1 ]=(jtmscr.vbuffer[ ((_ad+_x)<<2)+0 ]+(jtmansi.palette[(_c<<2)+1]<<2))>>1;
    jtmscr.vbuffer[ ((_ad+_x)<<2)+2 ]=(jtmscr.vbuffer[ ((_ad+_x)<<2)+0 ]+(jtmansi.palette[(_c<<2)+0]<<2))>>1;
   }
  }
  break;
 }
 jtmscr.drawround--;
}

void jtmgl_drbox3d(int x1,int y1,int x2,int y2,int colsel)
{
 jtmscr.drawround++;
 // Maalaa alue yhdell„ v„rill„ (alustus)
 jtm_dbox(x1,y1,x2,y2,(0xbf/4)+colsel); //0xbf/4); jtmscr.backgroundcolor
 // Piirr„ kolmiulotteisuus (Windows tyylinen)
 // Sivumpi
 jtm_dbox(x2,y1,x2,y2,(0x7f/4)+colsel);
 // Alempi
 jtm_dbox(x1,y2,x2,y2,(0x7f/4)+colsel);

 //
 jtm_dbox(x1,y1,x2-1,y1,(0xff/4)+colsel);
 //
 jtm_dbox(x1,y1,x1,y2-1,(0xff/4)+colsel);
 jtmscr.drawround--;
}

void jtm_dbox3d(int x1,int y1,int x2,int y2)
{
 jtmscr.drawround++;
 // Maalaa alue yhdell„ v„rill„ (alustus)
 jtm_dbox(x1,y1,x2,y2,0xbf/4); //0xbf/4); jtmscr.backgroundcolor
 // Piirr„ kolmiulotteisuus (Windows tyylinen)
 // Sivumpi
 jtm_dbox(x2,y1,x2,y2,0x7f/4);
 // Alempi
 jtm_dbox(x1,y2,x2,y2,0x7f/4);

 //
 jtm_dbox(x1,y1,x2-1,y1,0xff/4);
 //
 jtm_dbox(x1,y1,x1,y2-1,0xff/4);
 jtmscr.drawround--;
}

void jtm_dbox3d3(int x1,int y1,int x2,int y2,int c3,int c1,int c2)
{
 jtmscr.drawround++;
 // Maalaa alue yhdell„ v„rill„ (alustus)
 jtm_dbox(x1,y1,x2,y2,c1); //0xbf/4); jtmscr.backgroundcolor
 // Piirr„ kolmiulotteisuus (Windows tyylinen)
 // Sivumpi
 jtm_dbox(x2,y1,x2,y2,c2);
 // Alempi
 jtm_dbox(x1,y2,x2,y2,c2);

 //
 jtm_dbox(x1,y1,x2-1,y1,c3);
 //
 jtm_dbox(x1,y1,x1,y2-1,c3);
 jtmscr.drawround--;
}

void jtmgl_dbox3d(int x1,int y1,int x2,int y2)
{
 jtmscr.drawround++;
 if( x1<0 )
 {
  x1=0;
 }
 if( y1<0 )
 {
  y1=0;
 }
 if(x2>=jtmscr.width)
 {
  x2=jtmscr.width;
 }
 if(y2>=jtmscr.height)
 {
  y2=jtmscr.height;
 }
 // Maalaa alue yhdell„ v„rill„ (alustus)
 jtmgl_dbox(x1,y1,x2,y2,0xbf/4,0); //0xbf/4);
 // Piirr„ kolmiulotteisuus (Hienohkon tyylinen)
 // Sivumpi
 jtmgl_dbox(x2,y1,x2,y2,0x7f/4,0);
 // Alempi
 jtmgl_dbox(x1,y2,x2,y2,0x7f/4,0);

 //
 jtmgl_dbox(x1,y1,x2-1,y1,0xff/4,0);
 //
 jtmgl_dbox(x1,y1,x1,y2-1,0xff/4,0);
 //
 jtmgl_tosrtab(y1,y2);
loppu:
 jtmscr.drawround--;
}

void jtm_gldcolorbox3d(int x1,int y1,int x2,int y2,int col)
{
 jtmscr.drawround++;
 // Maalaa alue yhdell„ v„rill„ (alustus)
 jtmgl_dbox(x1,y1,x2,y2,jtmscr.backgroundcolor,0); //0xbf/4);
 // Piirr„ kolmiulotteisuus (Windows tyylinen)
 // Sivumpi
 jtmgl_dbox(x2,y1,x2,y2,(0x7f/4)+col,0);
 // Alempi
 jtmgl_dbox(x1,y2,x2,y2,(0x7f/4)+col,0);

 //
 jtmgl_dbox(x1,y1,x2-2,y1,(0xff/4)+col,0);
 jtmgl_dbox(x1,y1,x1,y2-2,(0xff/4)+col,0);
 //
 jtmgl_tosrtab(y1,y2);
 jtmscr.drawround--;
}

void jtm_gldcolorbox3d2(int x1,int y1,int x2,int y2,int col,int amount)
{
 int slide;
 //
 jtmscr.drawround++;
 x2=x2-(amount-1);
 y2=y2-(amount-1);
 for(slide=0; slide<amount; slide++)
 {
  // Maalaa alue yhdell„ v„rill„ (alustus)
  jtmgl_dbox(x1,y1,x2+slide,y2+slide,jtmscr.backgroundcolor,0); //0xbf/4);
  // Piirr„ kolmiulotteisuus (Windows tyylinen)
  // Sivumpi
  jtmgl_dbox(x2,y1,x2+slide,y2+slide,(0x7f/4)+col,0);
  // Alempi
  jtmgl_dbox(x1,y2,x2+slide,y2+slide,(0x7f/4)+col,0);
  //
  jtmgl_dbox(x1,y1,x2+slide-2,y1+slide,(0xff/4)+col,0);
  //
  jtmgl_dbox(x1,y1,x1+slide,y2+slide-2,(0xff/4)+col,0);
 }
 jtmgl_tosrtab(y1,y2);
 jtmscr.drawround--;
}

void jtmgl_drawbutton(int _x1,int _y1,int col)
{
 jtmscr.drawround++;
 jtm_gldcolorbox3d2(_x1,_y1,_x1+16,_y1+16,col,3);
 jtmscr.drawround--;
}

/************************************************************************/
void jtm_accesswindow(long wid)
{
 unsigned x1,y1,x2,y2;
 //
 jtmscr.drawround++;
 x1=jwin.x1[wid]; y1=jwin.y1[wid]; x2=jwin.x2[wid]; y2=jwin.y2[wid];
 jtmscr.headlinebackgroundcolor=jwin.headline.bcolor[wid];
 jtmscr.writex=x1+8; jtmscr.writey=y1+6;
 jtmscr.fontcolor=jwin.headline.fcolor[wid];
 jtmscr.textbackgroundcolor=jwin.headline.bcolor[wid];
 jtmscr.backgroundcolor=jwin.bcolor[wid];
 // Siirr„ kohdistin ikkunan ty” alueelle
 jtmscr.defaultbackgroundcolor=jwin.bcolor[wid];
 jtmscr.fontcolor=jwin.fcolor[wid];
 jtmscr.backgroundcolor=jwin.bcolor[wid];
 jtmscr.writex=x1+8;  jtmscr.writey=y1+32;
 jtmscr.writexs=x1+8; jtmscr.writeys=y1+32;
 jtmscr.writexm=x2-8; jtmscr.writeym=y2-8;
 jtmscr.drawround--;
}

// JTM_DWINDOW2 function (Made obsolote by JTMGL_OPENWINDOW function)
// ---------------------
//
// Description:
//
// This function creates new'n'empty window.
// It is recommended not to use this function for creating own windows. It is
// more recommended to use functions like for example JTMGL_OPENWINDOW.
// JTMGL_OPENWINDOW function opens window and returns WindowID,
// so the function chooses free Window handle instead of the application
// should choose it self.
//
// DO NOT USE THIS FUNCTION IF YOU DO NOT KNOW WHAT IT DOES.
// Use JTMGL_OPENWINDOW instead!
//
// Example:
//
// jtm_dwindow2(32,32,jtmscr.width-32,jtmscr.height-32, mywindownumber);
//
//
//
//
//
//
//
void jtm_dwindow2(int x1,int y1,int x2,int y2,long wid)
{
 long ad,ad2;
 //
 jtmscr.drawround++;
 if( x1<0 )
 {
  x1=0;
 }
 if( y1<0 )
 {
  y1=0;
 }
 if(x2>=jtmscr.width)
 {
  x2=jtmscr.width-1;
 }
 if(y2>=(jtmscr.height-20)) // Start button included
 {
  y2=jtmscr.height-21;
 }
 //
 for(ad=0; ad<16; ad++)
 {
  ad2=ad;
  jtmgl_drectangle(x1+ad2,y1+ad2,x2-ad2,y2-ad2,ad2*4+0xc0);
 }
 jtmgl_dbox(x1+ad2,y1+ad2,x2-ad2,y2-ad2,ad2*4+0xc0,0);
 jtmgl_tosrtab(y1,y2);
 //
 jtmscr.drawround--;
}

void jtm_dwindow3(int x1,int y1,int x2,int y2,long wid)
{
 long ad,ad2;
 //
 jtmscr.drawround++;
 if( x1<0 )
 {
  x1=0;
 }
 if( y1<0 )
 {
  y1=0;
 }
 if(x2>=jtmscr.width)
 {
  x2=jtmscr.width-1;
 }
 if(y2>=(jtmscr.height-20)) // Start button included
 {
  y2=jtmscr.height-21;
 }
 //
 for(ad=0; ad<16; ad++)
 {
  ad2=ad;
  jtmgl_drectangle(x1+ad2,y1+ad2,x2-ad2,y2-ad2,(16-ad2)*4+0xc0);
 }
 jtmgl_dbox(x1+ad2,y1+ad2,x2-ad2,y2-ad2,(16-ad2)*4+0xc0,0);
 jtmgl_tosrtab(y1,y2);
 //
 jtmscr.drawround--;
}

void jtm_dwindow4(int x1,int y1,int x2,int y2,long wid)
{
 long ad,ad2;
 //
 jtmscr.drawround++;
 if( x1<0 )
 {
  x1=0;
 }
 if( y1<0 )
 {
  y1=0;
 }
 if(x2>=jtmscr.width)
 {
  x2=jtmscr.width-1;
 }
 if(y2>=(jtmscr.height-20)) // Start button included
 {
  y2=jtmscr.height-21;
 }
 //
 //

 jtm_dwindow2(x1,y1,x2,y2,wid);
 //
 jtmgl_dtransparent_box(x1+16,y1+16,x2-16,y1+16+16,0xf0,0x00);

 //
 //
 jtmgl_tosrtab(y1,y2);
 //
 jtmscr.drawround--;
}

//
//
//
//
//
//
void jtmgl_repaintwindow(long wid)
{
 jtm_accesswindow(wid);
 jtm_dwindow(jtmwreg.x1[wid],jtmwreg.y1[wid],jtmwreg.x2[wid],jtmwreg.y2[wid],
             jtmwreg.headline[wid],wid);
}

// JTM_DWINDOW function (JDF)
// --------------------
//
// Description:
//
//      This function draws window into visible screen.
//
//
//
//
void jtm_dwindow(int x1,int y1,int x2,int y2,char *headline,long wid)
{
 //
 jtmscr.drawround++;

 //
 jtmscr.textbackgroundcolor=0x1f;
 //jtmscr.headlinebackgroundcolor;

 //
 if( x1<0 )
 {
  x1=0;
 }
 if( y1<0 )
 {
  y1=0;
 }
 if(x2>=jtmscr.width)
 {
  x2=jtmscr.width-1;
 }
 if(y2>=(jtmscr.height-20)) // Start nappia ei saa ylipiirt„„.
 {
  y2=jtmscr.height-21;
 }

 // Piirr„ koko ikkuna alusta (Ikkunan alueen kokoinen 3D rectangle)
// jtmgl_dbox(x1,y1,x2,y2,0xff,0);
 jtmgl_dbox3d(x1,y1,x2,y2);

 // Piirr„ otsikko palkki (Aika tumma)
 jtmgl_dbox(x1+2,y1+2,x2-2,y1+16+2,jtmscr.headlinebackgroundcolor,0);
 jwin.x1[wid]=x1;
 jwin.y1[wid]=y1;
 jwin.x2[wid]=x2;
 jwin.y2[wid]=y2;

 //
 jwin.writexm[wid]=x2-8; jwin.writeym[wid]=y2-8;
 jtmscr.writexm=x2-8; jtmscr.writeym=y2-8;

 // Kirjoita otsikko (V„rit muutetaan v„liaikaisesti)
 jwin.headline.fcolor[wid]=0xff;
 jwin.headline.bcolor[wid]=jtmscr.headlinebackgroundcolor;
 jwin.headline.writexs[wid]=x1+8; jwin.headline.writeys[wid]=y1+6;
 jwin.headline.writexm[wid]=x2-8; jwin.headline.writeym[wid]=y2-8;
 jtmscr.fontcolor=jwin.headline.fcolor[wid];
 jtmscr.textbackgroundcolor=jwin.headline.bcolor[wid];
 jtmscr.writex=x1+8+16+4; jtmscr.writey=y1+6;
 write(headline);

 // Piirr„ aplikaation lippu
 copyarea_wbm(jtmscr.fn_systemimage_raw,
              jtm_icons[3*4+0],
              jtm_icons[3*4+1],
              jtm_icons[3*4+2],
              jtm_icons[3*4+3],
              x1+8,y1+6,
              0);
 // Piirr„ aplikaation sulkemis buttoni
 copyarea_wbm(jtmscr.fn_systemimage_raw,
              jtm_icons[11*4+0],
              jtm_icons[11*4+1],
              jtm_icons[11*4+2],
              jtm_icons[11*4+3],
              x2-12,y1+6,
              0);
 // Piirr„ aplikaation suurennus buttoni
 copyarea_wbm(jtmscr.fn_systemimage_raw,
              jtm_icons[13*4+0],
              jtm_icons[13*4+1],
              jtm_icons[13*4+2],
              jtm_icons[13*4+3],
              x2-12-12,y1+6,
              0);
 // Piirr„ aplikaation pienennys buttoni
 copyarea_wbm(jtmscr.fn_systemimage_raw,
              jtm_icons[49*4+0],
              jtm_icons[49*4+1],
              jtm_icons[49*4+2],
              jtm_icons[49*4+3],
              x2-12-12-12,y1+6,
              0);

 // Siirr„ kohdistin ikkunan ty” alueelle
 jwin.fcolor[wid]=NewWindow.fontcolor;  // 0x3f ?
 jwin.bcolor[wid]=NewWindow.fontbcolor; // jtmscr.defaultbackgroundcolor ?
 jwin.writexs[wid]=x1+8;   jwin.writeys[wid]=y1+32;
 jtmscr.fontcolor=jwin.fcolor[wid];
 jtmscr.backgroundcolor=jwin.bcolor[wid];
 jtmscr.textbackgroundcolor=0x1f;
 jtmscr.writex=x1+8; jtmscr.writey=y1+32;
 jtmscr.writexs=x1+8; jtmscr.writeys=y1+32;
 jtmscr.writexm=x2-8; jtmscr.writeym=y2-8;

 // Alusta(piirt„en) ikkunan ty”-alue
 jtmgl_dbox(jtmscr.writexs,jtmscr.writeys,
            jtmscr.writexm,jtmscr.writeym,
            jtmscr.textbackgroundcolor,0);


 //
 jtmgl_tosrtab(y1,y2);

 //
 jtmscr.drawround--;
}

void jtm_savewindowstodisk(unsigned char *fname)
{// Stores windows to disk from RAM
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 jtmscr.drawround++;
 fp=jtm_fopen(fname,"wb");
 fwrite(&jwin, sizeof(jwin), 1, fp);
 jtm_fclose(fp);
 jtmscr.drawround--;
}

void jtm_loadwindowstodisk(unsigned char *fname)
{// Restores windows from disk to RAM
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 jtmscr.drawround++;
 fp=jtm_fopen(fname,"wb");
 fread(&jwin, sizeof(jwin), 1, fp);
 jtm_fclose(fp);
 jtmscr.drawround--;
}

// JTMGL_OPENWINDOW function
//
int jtmgl_openwindow(char *headline)
{
 //
 return jtmgl_openwindow1(32,32,jtmscr.width-32,jtmscr.height-32,headline);
}

// JTMGL_OPENWINDOW1 function
//
int jtmgl_openwindow1(long x1,long y1,long x2,long y2,char *headline)
{
 // Draw Window
 jtm_dwindow(x1,y1,x2,y2,headline,jtmwreg.amount);

 // Register window in window registry.
 jtmwreg.x1[jtmwreg.amount]=x1;
 jtmwreg.y1[jtmwreg.amount]=y1;
 jtmwreg.x2[jtmwreg.amount]=x2;
 jtmwreg.y2[jtmwreg.amount]=y2;
 if(jtmwreg.headline[jtmwreg.amount]==NULL)
 {
  jtmwreg.headline[jtmwreg.amount]=malloc(256);
 }
 strcpy(jtmwreg.headline[jtmwreg.amount],headline);

 // Increase amount of windows registed.
 jtmwreg.amount++;
 return (jtmwreg.amount-1);
}

/*void jtm_vbufferupdate() // ?? 320x200 stuff, probaly useless. *Removed*
{
 if(jtmscr.vbufferDELAY>4096)
 {
  jtmscr.vbufferDELAY=0;
  if(jtmscr.vbufferBUSY==0)
  {
   jtm_vf(jtmscr.vbuffer);
  }
 }
} */

int jtmgl_arrowfirsttime=0xff;

void jtmgl_loadsprites(void)
{
 unsigned long _ad,_ad2;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 //
 // Allocate and load start button
 jtmscr.drawround++;
 if(jtmscr.sbutton.bm==NULL)jtmscr.sbutton.bm=malloc(80*16);

 if(fexist(jgetenv("shortcuts","startlogo=")))
 {
  jtm_loadbin(jgetenv("shortcuts","startlogo="),jtmscr.sbutton.bm);
 }
 else
 {
  for(ad=0; ad<(80*16); ad++)jtmscr.sbutton.bm[ad]=0;
 }
 //
 // Allocate and load mouse sprites
 if(jtmmouse.sprarrow==NULL)
 {
  if(jtmmouse.buf1==NULL)jtmmouse.buf1=malloc(32*32*5);
  if(jtmmouse.buf2==NULL)jtmmouse.buf2=malloc(32*32*5);
  jtmmouse.sprarrow= malloc(32*32*5);
  jtmmouse.sprdelay= malloc(32*32*5);
  jtmmouse.sprfinger=malloc(32*32*5);
  jtm_loadbin(jgetenv("shortcuts","arrow_raw="), jtmmouse.sprarrow);
  jtm_loadbin(jgetenv("shortcuts","arrowd_raw="),jtmmouse.sprdelay);
  jtm_loadbin(jgetenv("shortcuts","finger_raw="),jtmmouse.sprfinger);
//  jtm_loadbin("\\jtm\\pic\\arrow.raw", jtmmouse.sprarrow);
//  jtm_loadbin("\\jtm\\pic\\arrowd.raw",jtmmouse.sprdelay);

  for(_ad=0; _ad<32*32; _ad++)
  {
   _ad2=jtmmouse.sprarrow[_ad];
   if(_ad2==0x0f)_ad2=0xff;
   jtmmouse.sprarrow[_ad]=_ad2;
  }
  for(_ad=0; _ad<32*32; _ad++)
  {
   _ad2=jtmmouse.sprdelay[_ad];
   if(_ad2!=0x00)
   {
    _ad2=(_ad2/4)+0xc0;
   }
   jtmmouse.sprdelay[_ad]=_ad2;
  }
  //----//
 }
joojoo:
 jtmscr.drawround--;
}

void jtmgl_putarrow(int x,int y)
{
 jtmscr.drawround++;
 switch(jtmmouse.arrowmode)
 {
  // Normaali, eli nuoli-               Normal mode, just an arrow.
  case 0:
  jtmscr.drawexpection=1;
  jtmgl_putsprite(jtmmouse.sprarrow,x,y,0xff);
  break;
  // Tiimalasi          -               Sleep
  case 1:
  jtmscr.drawexpection=1;
  jtmgl_putsprite(jtmmouse.sprdelay,x,y,0xff);
  break;
  // Sormi              -               Finger
  case 2:
  jtmscr.drawexpection=1;
  jtmgl_putsprite(jtmmouse.sprfinger,x,y,0xff);
  break;
 }
 jtmscr.drawround--;
}

void jtmgl_drawarrow(void)
{
 //
 int dx,dy;

 //
 dx=jtmmouse.x; dy=jtmmouse.y;
 jtmscr.drawround++;
 jtmgl_putsprite(jtmmouse.buf1,jtmmouse.llx,jtmmouse.lly,0);
 jtmgl_getsprite(jtmmouse.buf1,dx,dy,0);
 jtmgl_putarrow(dx,dy);
 jtmmouse.llx=dx;
 jtmmouse.lly=dy;
 if(jtmscr.srsignallock==0)jtmscr.srsignal=0xff;
 jtmscr.drawround--;
}

void jtmgl_drawonarrow(void)
{
 //
 int dx,dy;

 //
 dx=jtmmouse.x;
 dy=jtmmouse.y;
 jtmgl_getsprite(jtmmouse.buf1,dx,dy,0);
 jtmgl_putarrow(dx,dy);
 jtmmouse.llx=dx;
 jtmmouse.lly=dy;
}

void jtmgl_drawoffarrow(void)
{
 jtmgl_putsprite(jtmmouse.buf1,jtmmouse.llx,jtmmouse.lly,0);
}

int jtmgl_arrowoff(void)
{
 if(jtmscr.glsystem)
 {
  jtmscr.drawround++;
  if(jtmmouse.arrowstate)
  {
   jtmmouse.arrowstate=0;
   if(jtmmouse.buf1!=NULL)
   {
    if(jtmgl_arrowfirsttime==0)jtmgl_putsprite(jtmmouse.buf1,jtmmouse.lx,jtmmouse.ly,0);
   }
  }
  jtmscr.drawround--;
  return 0;
 }
 else
 {
  return 1;
 }
}

int jtmgl_arrowon(void)
{
 if(jtmscr.glsystem)
 {
  jtmscr.drawround++;
  if(jtmmouse.cursorlock==0)
  {
   if(jtmmouse.arrowstate!=0xff)
   {
    jtmgl_arrowfirsttime=0xff;
    jtmmouse.arrowstate=0xff;
   }
  }
  jtmscr.drawround--;
  return 0;
 }
 else
 {
  return 1;
 }
}

void jtmgl_arrowmover(void)
{
 char _str[255];
 unsigned long _ad,_ad2;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 //
 jtmscr.drawround++;
// jtmmouse.x=mousexstat();
// jtmmouse.y=mouseystat();
 if(jtmgl_arrowfirsttime==0xff)
 {
  jtm_setmickey(8,16);
  jtmgl_arrowfirsttime=0;
  jtmmouse.lx=jtmmouse.x;
  jtmmouse.ly=jtmmouse.y;
  jtmgl_getsprite(jtmmouse.buf1,jtmmouse.x,jtmmouse.y,0);
  goto drawit;
 }
 if(jtmmouse.signal==0xff)
 {
  goto drawit;
 }
 if(jtmmouse.x==jtmmouse.lx &&
    jtmmouse.y==jtmmouse.ly)
 {
  goto ohoho;
 }
// sprintf(_str,"%u %u\n",jtmmouse.x,jtmmouse.y);
// write(_str);
drawit:
 if(jtmmouse.arrowstate!=0)jtmgl_drawarrow();
ohoho:
 if(jtmmouse.signal==0xff)jtmmouse.signal=0;
//stateoff:
 jtmscr.drawround--;
}

/*int jtm_loadbmp(char *fname,char *frame) // Loads 24bit & 8bit
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
 long width,height;
 FILE *fp,*fp2,*fp3,*fp4;

 //
 fp=jtm_fopen(fname,"rb");
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

 // c2 46
 // ee fe BIT 4 !
// if(header[0]==0x42 && header[1]==0x4d && header[2]==0x38 && header[3]==0xee)
// if(header[0]==0x42 && header[1]==0x4d && header[2]==0x38 && header[3]==0xfe)

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
//  for(ad=0; ad<256; ad++){ jtm_setpalette(ad,fgetc(fp)/4,fgetc(fp)/4,fgetc(fp)/4); fgetc(fp); }
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
  jtm_fclose(fp);
  return 1;
 }
 jtm_fclose(fp);
 jtmlpcxinfo.xlen=width;
 jtmlpcxinfo.ylen=height;
 return 0;
}*/

void jtm_ctvbuffer(char *src,long amount)
{
 long a1;
 //
 for(a1=0; a1<amount; a1++)
 {
  jtmscr.vbuffer[a1]=src[a1];
 }
}

void jtm_loadfont(char *fname)
{
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 /** It is expected that font is 320 pixels width,  **
  ** but height doesn't need to be.                 **/
 if(jtmscr.font==NULL)
 {
  jtmscr.font=malloc(320*400);
 }
 fp=jtm_fopen(fname,"rb"); /*if(fp==NULL)jtmerror(2,-1);*/
 ad=0;
 do
 {
  um1=fgetc(fp);
  jtmscr.font[ad]=um1; ad++; if(ad>(320*400-1))break;
 }while(!feof(fp));
 jtm_fclose(fp);
}

int jtm_detectfont(unsigned _mark)
{
 unsigned _af;
 char _color;
 char _done=0;
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 /* Detect font, if currect pixel in left upper corner isn't lightred,  *
  * then font format can't be correct                                   */
 if(jtmscr.font[0]!=LIGHTRED)
 {
  return -1; // Return: font=incorrect!
 }
 //
 _af=0; // Set currect mark counter = zero

 /****************************************************
  * Follow the lightred line down, move till lightred*
  * line ends                                        *
  ****************************************************/
  // Seek to Lightred line (In downercase direction >> )
 y=0; x=0;
 do
 {
  y++;
 }while(jtmscr.font[y*320]==LIGHTRED); y--; y1=y;
 /* After lightred line start moving to                                 *
  *                                                                     */
 do
 {
  // Seek to Yellow line (In vertical direction >> )
  y=y1;
  do
  {
   x++; if(jtmscr.font[x+(y*320)]==RED){_done=1; break;} // Confirm that line
                                                  // wasn't red for sure,
                                                  // if so, then done=true
  }while(jtmscr.font[x+(y*320)]!=14); x++; if(_done==1)break;
  x1=y; y1=y;

  // Find Green line  (>)
  do
  {
   x++;
  }while(jtmscr.font[x+(y*320)]!=GREEN); x++;

  // Seek to end of Green line in y-direction   (V)
  do
  {
   y++;
  }while(jtmscr.font[x+(y*320)]==GREEN); x2=x; y=y2;
//  x1=0; y1=0;
//  x2=8; y2=8;
  jtmscr.fontx1[_af]=x1;
  jtmscr.fonty1[_af]=y1;
  jtmscr.fontx2[_af]=x2;
  jtmscr.fonty2[_af]=y2;
  //
  _af++;
  x++;
  //
  m1=jgetchim(); if(m1==27)break;
  if(x>319)break;
  if(_af==_mark)break;
 }while(_done==0);
 return 0; // Font format correct, font table updated.
}

void writemark(unsigned _x,unsigned _y,unsigned char _mark)
{
 //
 int xb, yb;
 long a1,a2,a3,a4,a5,a6,a7,a8;
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;

 //
 x1=0; y1=0;
 if(_mark==32)
 {
  if(jtmscr.wfb)
  {
   for(yb=0, y=y1; y<y1+8; y++, yb++)
   {
    for(xb=0, x=x1; x<x1+8; x++, xb++) //x1+7??!!
    {
     jtm_vplot(_x+xb,_y+yb,jtmscr.textbackgroundcolor+_mark*8);
    }
   }
  }
  else
  {

  }
  goto skiptype;
 }
 //
 if(_mark==':')
 {
  _mark=0;
  y1=8;
  x1=8*0x00;
  goto final;
 }
 if(_mark==';')
 {
  _mark=0;
  y1=8;
  x1=8*0x01;
  goto final;
 }
 if(_mark=='')
 {
  _mark=0;
  y1=8;
  x1=8*0x02;
  goto final;
 }
 if(_mark=='/')
 {
  _mark=0;
  y1=8;
  x1=8*0x03;
  goto final;
 }
 if(_mark=='=')
 {
  _mark=0;
  y1=8;
  x1=8*0x04;
  goto final;
 }
 if(_mark=='\'')
 {
  _mark=0;
  y1=8;
  x1=8*0x05;
  goto final;
 }
 if(_mark=='(')
 {
  _mark=0;
  y1=8;
  x1=8*0x06;
  goto final;
 }
 if(_mark==')')
 {
  _mark=0;
  y1=8;
  x1=8*0x07;
  goto final;
 }
 if(_mark=='+')
 {
  _mark=0;
  y1=8;
  x1=8*0x08;
  goto final;
 }
 if(_mark=='-')
 {
  _mark=0;
  y1=8;
  x1=8*0x09;
  goto final;
 }
 if(_mark=='*')
 {
  _mark=0;
  y1=8;
  x1=8*0x0a;
  goto final;
 }
 if(_mark=='~')
 {
  _mark=0;
  y1=8;
  x1=8*0x0b;
  goto final;
 }
 if(_mark=='<')
 {
  _mark=0;
  y1=8;
  x1=8*0x0c;
  goto final;
 }
 if(_mark=='>')
 {
  _mark=0;
  y1=8;
  x1=8*0x0d;
  goto final;
 }
 if(_mark=='?')
 {
  _mark=0;
  y1=8;
  x1=8*0x0e;
  goto final;
 }
 if(_mark=='%')
 {
  _mark=0;
  y1=8;
  x1=8*0x0f;
  goto final;
 }
 if(_mark=='&')
 {
  _mark=0;
  y1=8;
  x1=8*0x10;
  goto final;
 }
 if(_mark=='\"')
 {
  _mark=0;
  y1=8;
  x1=8*0x11;
  goto final;
 }
 if(_mark=='[')
 {
  _mark=0;
  y1=8;
  x1=8*0x12;
  goto final;
 }
 if(_mark==']')
 {
  _mark=0;
  y1=8;
  x1=8*0x13;
  goto final;
 }
 if(_mark=='\\')
 {
  _mark=0;
  y1=8;
  x1=8*0x14;
  goto final;
 }
 if(_mark=='õ') // NewFont: JTM character(Looks like JF)
 {
  _mark=0;
  y1=8;
  x1=8*0x15;
  goto final;
 }
 if(_mark=='#')
 {
  _mark=0;
  y1=8;
  x1=8*0x16;
  goto final;
 }
 //
 if(_mark=='.')
 {
  _mark=('z'-'a')+11;
  goto final;
 }
 if(_mark==',')
 {
  _mark=('z'-'a')+12;
  goto final;
 }
 if(_mark=='!')
 {
  _mark=0;
  x1=39*8;
  y1=0;
  goto final;
 }
 if(_mark=='?')
 {
  x1=38*8;
  y1=0;
  goto final;
 }
 if(_mark<='9' && _mark>='0')
 {
  _mark=(_mark-'0')+(208/8);
  //_mark+=('z'-'a');
  goto final;
 }
 if(_mark<('z'+1) && _mark>('a'-1))
 {
  _mark-=0x61;
 }
 if(_mark<('Z'+1) && _mark>('A'-1))
 {
  _mark-=0x61+('A'-'a');
 }
final:
 // FONT DRAWING FUNCTION
 if(_mark!=0)
 {
  for(a1=0; a1<_mark; a1++)x1+=8;
 }
 for(yy=0, y=y1; y<y1+8; y++, yy++)
 {
  for(xx=0, x=x1; x<x1+8; x++, xx++) // !! : One mark = 8x8
  {
   if(jtmscr.font[x+(y*320)]==0)
   {
    jtm_vplot(_x+xx,_y+yy,jtmscr.fontcolor);
   }
   else
   {
    if(jtmscr.font[x+(y*320)]==2)
    {
     jtm_vplot(_x+xx,_y+yy,jtmscr.textbackgroundcolor);
    }
    else
    {
     if(jtmscr.wfb)jtm_vplot(_x+xx,_y+yy,jtmscr.textbackgroundcolor);
    }
   }
  }
 }
skiptype:
 while(0!=0);
}

void writemark1(unsigned _x,unsigned _y,unsigned _mark)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 //_mark-='a'; _mark+=10;
 //printf("\n\r%u %u %u %u\n\r",fontx1[_mark],fonty1[_mark],fontx2[_mark],fonty2[_mark]);
 if(jtm_detectfont(_mark)!=-1)
 {
  for(yy=0, y=y1; y<y2; y++, yy++)
  {
   for(xx=0, x=x1; x<x2; x++, xx++)
   {
    jtm_vplot(_x+xx,_y+yy,jtmscr.font[x+(y*320)]);
   }
  }
 }
}

void writenl(void)
{
   jtmscr.writex=jtmscr.writexs;
   jtmscr.writey+=8;
}

void jtmgl_enter(void)
{
 int x,y;
 //
 switch(jtmscr.bpp)
 {
   case 8:
   if(jtmscr.writey+16>jtmscr.writeym)
   {
    for(y=jtmscr.writeys; y<jtmscr.writeym; y++)
    {
     jtmscr.srtab[y]=0xff;
     for(x=jtmscr.writexs; x<jtmscr.writexm; x++)
     {
      jtmscr.vbuffer[jtmscr.width*y+x]=jtmscr.vbuffer[jtmscr.width*(y+8)+x];
     }
    }
    for(y=jtmscr.writeym-8; y<jtmscr.writeym; y++)
    {
     for(x=jtmscr.writexs; x<jtmscr.writexm; x++)
     {
      jtmscr.vbuffer[jtmscr.width*y+x]=jtmscr.textbackgroundcolor;
     }
    }
    jtmscr.writex=jtmscr.writexs;
    break;
   }
   jtmscr.writey+=8;
   jtmscr.writex=jtmscr.writexs;
   break;

   case 32:
   if(jtmscr.writey+16>jtmscr.writeym)
   {
    for(y=jtmscr.writeys; y<jtmscr.writeym; y++)
    {
     jtmscr.srtab[y]=0xff;
     for(x=jtmscr.writexs; x<jtmscr.writexm; x++)
     {
      jtmscr.vbuffer[(jtmscr.width*y+x)<<2]=jtmscr.vbuffer[(jtmscr.width*(y+8)+x)<<2];
     }
    }
    for(y=jtmscr.writeym-8; y<jtmscr.writeym; y++)
    {
     for(x=jtmscr.writexs; x<jtmscr.writexm; x++)
     {
      jtmscr.vbuffer[(jtmscr.width*y+x)<<2]=jtmscr.textbackgroundcolor; //*!*!
     }
    }
    jtmscr.writex=jtmscr.writexs;
    break;
   }
   jtmscr.writey+=8;
   jtmscr.writex=jtmscr.writexs;
   break;
 }
}

void jtmscr_printfUsingCPRINTF(char *texti)
{
 register long ad;
 //
 for(ad=0; ad<strlen(texti); ad++)
 {
  if(texti[ad]=='\n')
  {
   cprintf("\n\r");
   goto ohi;
  }
  cprintf("%c",texti[ad]);
ohi:
  if(ad!=ad)break;
 }
}

// Next two functions change mode of screen updating
// when using 'write' function.

//
//
write_dontupdate()
{
 jtmscr.noimmedwu=1;
}

//
//
write_allwaysupdate()
{
 jtmscr.noimmedwu=0;
}

// WRITE FUNCTION
//
int write(char *_text)
{
#ifndef NO_WRITE

 //
 unsigned _cm=0,_temp,x,y ;
 char str[1000],str2[255],str3[255],str4[255];
 char m1,m2,m3,m4;
 long ad,ad2,ad3,ad4;

 //
 if((systemtimer.counter1>4 && jtm_vmode!=7 && !jtmscr.busy) && !jtmscr.noimmedwu)
 {
  systemtimer.counter3=0; // Cursor counter !
  systemtimer.counter1=0;
  jtmscr_action();
 }

 //
 if(jtmio.stdout2file!=0)
 {
  sprintf(str,"%s",_text);
  if(jtmio.extfp!=NULL)fprintf(jtmio.extfp,"%s",str);
  goto loppu1;
 }

 //
 if(jtmio.logstdout!=0)
 {
  sprintf(str,"STDOUT_LOG: \"%s\"",_text);
  jtm_messagebuffer(str);
 }

 //
 if(jtmscr.write.busy)goto loppu1;
 jtmscr.write.busy=0xff;
 if(jtm_vmode==0x03)
 {
  jtmscr_printfUsingCPRINTF(_text);
  jtmscr.write.busy=0;
  return 0;
 }
 jtmgl_arrowoff();
 for(_cm=0; _cm<strlen(_text); _cm++)
 {
  if(_text[_cm]==-1)
  {
   goto done;
  }
  if(_text[_cm]=='\n' || _text[_cm]==0x0d)
  {
enter:
   jtmgl_enter();
   //
   if(jtm_vmode!=3)
   {
//    jtmscr.srsignal=0xff;
   }
   goto done;
  }
  if((strlen(_text)-_cm)>=4)
  {
   if(_text[_cm+0]=='Ï' &&
      _text[_cm+1]=='$')
   {
    if(_text[_cm+2]=='c')
    {
     str2[0]=_text[_cm+3];
     str2[1]=0;
     sscanf(str2,"%x",&ad);
     jtm_textcolor(ad);
     _cm+=3;
     goto done;
    }
    if(_text[_cm+2]=='b')
    {
     str2[0]=_text[_cm+3];
     str2[1]=0;
     sscanf(str2,"%x",&ad);
     jtm_textbackground(ad);
     _cm+=4;
     goto done;
    }
    //
   }
  }
  if(_text[_cm]=='\r')
  {
   jtmscr.writex=jtmscr.writexs;
   goto done;
  }
  if(_text[_cm]=='\b')
  {
   jtmscr.writex=jtmscr.writex-8;
   if(jtmscr.writex<jtmscr.writexs)
   {
    jtmscr.writex=jtmscr.writexm-8;
    jtmscr.writey=jtmscr.writey-8;
    if(jtmscr.writey<jtmscr.writeys)
    {
     jtmscr.writey=jtmscr.writeys;
    }
   }
   writemark(jtmscr.writex,jtmscr.writey,' ');
   goto done;
  }
  writemark(jtmscr.writex,jtmscr.writey,_text[_cm]);
  jtmscr.writex+=8;
  if(jtmscr.writex>jtmscr.writexm-8)
  {
   goto enter;
   writenl();
  }
  if(jtmscr.writex>jtmscr.width-8)
  {
   jtmscr.writex=0; jtmscr.writey+=8;
  }
done:
 while(0!=0);
 }
 jtmgl_arrowon();
 #ifdef ALLOW_EXTSIGNALS
// if(jtm_vmode!=0x03 && jtmtask.taskmode==0)jtmscr.srsignal=0xff;
 #else
 if(jtmscr.immediatly_update_on_function_write!=0)
 {
//  jtmscr.srsignal=0xff;
 }
 #endif
loppu:
 jtmscr.write.busy=0;
loppu1:
 while(0!=0);
#endif
}

//
void writec(char mark)
{
 //
 char str[255],str2[255];

 //
 sprintf(str,"%c",mark);
 write(str);
}

//  jtmscr_sendtowrite(jtmkeyb.ascbuf,"%u,",jtmkeyb.aplace);

void jtmscr_sendtowrite(char *ascbuf,char *typestr,unsigned long int aplace,unsigned long int alamount)
{
 // Locals
 char *str;
 long ad,ad2;

 // Allocate
 str=malloc(512);

 // Do it
 for(ad=0; ad<jtmkeyb.aplace+1; ad++)
 {
  if(ad>alamount)
  {
   write("\b...");
   break;
  }
  sprintf(str,typestr,ascbuf[ad]);
  write(str);
 }

 // Free
 free(str);
}

jtmgl_drawtxtcrsr(int mode)
{
 //
 long ad,ad2,ad3,ad4,x,y;

 //
 if(jtmscr.cursor.b==NULL)jtmscr.cursor.b=malloc(8*8);

 //
 for(y=0; y<8; y++)
 {
  ad=y*8;
  ad2=(y+jtmscr.writey)*jtmscr.width+jtmscr.writex;
  for(x=0; x<8; x++)
  {
   jtmscr.cursor.b[ad+x]=jtmscr.vbuffer[ad2+x];
  }
  jtmscr.srtab[y+jtmscr.writey]=0xff;
 }

 //
 if(mode)
 {
  switch(jtmscr.cursor.look)
  {
   case 1:
   jtm_dbox(jtmscr.writex,jtmscr.writey,jtmscr.writex+7,jtmscr.writey+7,0xff);
   break;
   case 2:
   jtmgl_drbox3d(jtmscr.writex,jtmscr.writey,jtmscr.writex+7,jtmscr.writey+7,0xc0);
   break;
   case 3:
   jtm_dgridbox(jtmscr.writex,jtmscr.writey,jtmscr.writex+7,jtmscr.writey+7,0xff);
   break;
   case 4:
   jtm_dtransparentgridbox(jtmscr.writex,jtmscr.writey,jtmscr.writex+7,jtmscr.writey+7,0xff);
   break;
   default:
   break;
  }
 }
}

jtmgl_wipetxtcrsr()
{
 //
 long ad,ad2,ad3,ad4,x,y;

 //
 for(y=0; y<8; y++)
 {
  ad=y*8;
  ad2=(y+jtmscr.writey)*jtmscr.width+jtmscr.writex;
  for(x=0; x<8; x++)
  {
   jtmscr.vbuffer[ad2+x]=jtmscr.cursor.b[ad+x];
  }
  jtmscr.srtab[y+jtmscr.writey]=0xff;
 }
}

// JTMGL_REMOVEGRAPHICTIMER function
//
jtmgl_removegraphictimer(int *lx,int *ly,
                       int *lxm,int *lym,
                       int *lxs,int *lys,
                       int *ltobuf)
{
  //
  long x,y,ad,ad2,ad3,ad4;

  //
  jtmscr.writex=lx[0];
  jtmscr.writey=ly[0];
  jtmscr.writexm=lxm[0];
  jtmscr.writeym=lym[0];
  jtmscr.writexs=lxs[0];
  jtmscr.writeys=lys[0];
  switch(jtmscr.bpp)
  {
   case 8:
   for(y=0; y<8*ltobuf[0]; y++)
   {
    ad=jtmscr.width*y;
    for(x=0; x<jtmscr.width; x++)
    {
     jtmscr.vbuffer[ad+x]=systemtimer.temp2[ad+x];
    }
    jtmscr.srtab[y]=0xff;
   }
   break;

   case 32:
   for(y=0; y<8*ltobuf[0]; y++)
   {
    ad=jtmscr.width*y;
    for(x=0; x<jtmscr.width; x++)
    {
     jtmscr.vbuffer[(ad+x<<2)+0]=systemtimer.temp2[(ad+x<<2)+0];
     jtmscr.vbuffer[(ad+x<<2)+1]=systemtimer.temp2[(ad+x<<2)+1];
     jtmscr.vbuffer[(ad+x<<2)+2]=systemtimer.temp2[(ad+x<<2)+2];
    }
    jtmscr.srtab[y]=0xff;
   }
   break;
  }
  if(systemtimer.temp2)free(systemtimer.temp);
}

// JTMGL_DRAWGRAPHICTIMER function
//
jtmgl_drawgraphictimer(int *lx,int *ly,
                       int *lxm,int *lym,
                       int *lxs,int *lys,
                       int *ltobuf)
{
  //
  long x,y,ad,ad2,ad3,ad4;
  char str[255];

  //
  lx[0]=jtmscr.writex;
  ly[0]=jtmscr.writey;
  lxm[0]=jtmscr.writexm;
  lym[0]=jtmscr.writeym;
  lxs[0]=jtmscr.writexs;
  lys[0]=jtmscr.writeys;

  //
  jtmscr.writexm=jtmscr.width;
  jtmscr.writeym=jtmscr.height;
  jtmscr.writexs=0;
  jtmscr.writeys=0;
  switch(jtmscr.bpp)
  {
   case 8:
   if(systemtimer.temp2==NULL)systemtimer.temp2=malloc(jtmscr.width*8*ltobuf[0]);
   for(y=0; y<8*ltobuf[0]; y++)
   {
    ad=jtmscr.width*y;
    for(x=0; x<jtmscr.width; x++)
    {
     systemtimer.temp2[ad+x]=jtmscr.vbuffer[ad+x];
    }
    jtmscr.srtab[y]=0xff;
   }
   break;

   case 32: // 32bit takes 4 times more than 8bit
   if(systemtimer.temp2==NULL)systemtimer.temp2=malloc(jtmscr.width*8*ltobuf[0]*4);
   for(y=0; y<8*ltobuf[0]; y++)
   {
    ad=jtmscr.width*y;
    for(x=0; x<jtmscr.width; x++)
    {
     systemtimer.temp2[(ad+x)<<0]=jtmscr.vbuffer[(ad+x<<2)+0];
     systemtimer.temp2[(ad+x)<<1]=jtmscr.vbuffer[(ad+x<<2)+1];
     systemtimer.temp2[(ad+x)<<2]=jtmscr.vbuffer[(ad+x<<2)+2];
    }
    jtmscr.srtab[y]=0xff;
   }
   break;
  }
  jtm_dgridbox(0,0,jtmscr.width,8*ltobuf[0],0);
  //
  jtmgl_gotoxy(0,0);
  sprintf(str,"counter8: 0x%1.8x\n",systemtimer.counter8);
  write(str);
  //
  sprintf(str,"ram: %u\n",_go32_dpmi_remaining_physical_memory());
  write(str);
  //
  sprintf(str,"swap: %u\n",_go32_dpmi_remaining_virtual_memory());
  write(str);
  //
  sprintf(str,"width=%u, height=%u, bpp=%u.\n",jtmscr.width,jtmscr.height,jtmscr.bpp);
  write(str);
  //
  sprintf(str,"mousex=%u, mousey=%u, mouseb=%x.\n",jtmmouse.x,jtmmouse.y,jtmmouse.buttons);
  write(str);
  //
 #ifdef JTMOSDEMOSYSTEM
  MIDASgetPlayStatus( playHandle,&status );
  sprintf(str,"(midas)position=%u,pattern=%u,row=%u.\n",status.position,
                                                        status.pattern,
                                                        status.row
                                                                        );
  write(str);
 #endif

  //
  sprintf(str,"akbuffer place: %u\n",jtmkeyb.aplace);
  write(str);

//  sprintf(str,"ka expression: %s",jtmkeyb.ascbuf);
//  write(str);
  //
  jtmscr_sendtowrite(jtmkeyb.ascbuf,"%u,",jtmkeyb.aplace,10);
  write("\n");
}







// JTMSCR_UPDATEVRAM function
//
int jtmscr_updateVRAM(void)
{
 //
 register int x,y;
 int ltobuf=8,dx,dy;
 register long ad,ad2;
 long lx,ly,lxm,lym,lxs,lys;
 char str[255],DRAWOFF;
 #ifdef JTMOSDEMOSYSTEM
 MIDASplayStatus status;
 #endif

 // Draw start button
 if(jtmscr.sbutton.state)jtm_drawsbutton();

 //
 jtmscr.busy++;
 DRAWOFF=0;

 //
 ad=16-(systemtimer.counter3&16);
 if(jtmscr.cursor.state)jtmgl_drawtxtcrsr(ad);

 if(jtmscr.graphictimer)
 {
  DRAWOFF=1;
  jtmgl_drawgraphictimer(lx,ly,lxm,lym,lxs,lys,ltobuf);
 }

 // If mouse arrow is ought to draw on screen.
 if(!jtmmouse.arrowstate)
 {
  // Draw arrow on top
  dx=jtmmouse.x;
  dy=jtmmouse.y;
  jtmgl_getsprite(jtmmouse.buf1,dx,dy,0);
  jtmgl_putarrow(dx,dy);
 }

 // Now refresh videoram with our buffer.
 jtmscr_updateVRAM1();

 // If mouse arrow is ought to draw on screen.
 if(!jtmmouse.arrowstate)
 {
  jtmgl_putsprite(jtmmouse.buf1,dx,dy,0);
 }

 //
 if(DRAWOFF)
 {
  jtmgl_removegraphictimer(lx,ly,lxm,lym,lxs,lys,ltobuf);
 }

 // If making movie
 #ifdef JTM_MAKINGMOVIE
 jtmgl_savecurrentscreentobmp2();
 #endif
 if(jtmscr.makingmovie)
 {
  jtmgl_savecurrentscreentobmp2();
 }

 //
 if(jtmscr.cursor.state)jtmgl_wipetxtcrsr();

 //
 if(jtmscr.sbutton.state)jtm_cutsbutton();

 //
 jtmscr.busy--;
}













// dosmemput(__videoframe, 0x10000, 0xa0000);
// *** UPDATE VRAM -- VERY IMPORTANT AND MARKABLE FUNCTION ***
int jtmscr_updateVRAM1(void)
{
 register unsigned vbank;
 register long a1,a2,a3,a4,a5,a6,a7,a8;
 //
 vbank=0xff;
 a2=jtmscr.width*jtmscr.height;
 //

 switch(jtmscr.bpp)
 {
  // 8 bpp !
  case 8:
  for(a1=0,a8=0; a1<a2; a1+=jtmscr.width,a8++)
  {
   if(jtmscr.srtab[a8]!=0)
   {
    a4=a1&0xffff; // 16bittinen offsetti
    a3=a1>>16;    // Video bankki
    if(vbank!=a3)
    {
     jtm_setvbank(a3);
     vbank=a3;
    }
    if( !((a4+jtmscr.width)>0xffff) )
    {
     dosmemput(jtmscr.vbuffer+a1,jtmscr.width,0xa0000+a4);
    }
    else
    {
     a5=65536-a4;
     dosmemput(jtmscr.vbuffer+a1,a5,0xa0000+a4);
     vbank++;
     jtm_setvbank(vbank);
     dosmemput(jtmscr.vbuffer+a1+a5,(jtmscr.width-a5),0xa0000);
    }
   }
  }
  // 24 bpp !
  case 32:
  for(a1=0,a8=0; a1<a2<<2; a1+=(jtmscr.width<<2),a8++)
  {
   if(jtmscr.srtab[a8]!=0)
   {
    a4=a1&0xffff; // 16bittinen offsetti
    a3=a1>>16;    // Video bankki
     if(vbank!=a3)
    {
     jtm_setvbank(a3);
     vbank=a3;
    }
    if( !((a4+(jtmscr.width<<2) )>0xffff) )
    {
     dosmemput(jtmscr.vbuffer+a1,(jtmscr.width<<2),0xa0000+a4);
    }
    else
    {
     a5=65536-a4;
     dosmemput(jtmscr.vbuffer+a1,a5,0xa0000+a4);
     vbank++;
     jtm_setvbank(vbank);
     dosmemput(jtmscr.vbuffer+a1+a5,((jtmscr.width<<2)-a5),0xa0000);
    }
   }
  }
 }

 // Wipe srtable.
// if(jtmscr.wipesr)for(a1=0; a1<jtmscr.height; a1++)jtmscr.srtab[a1]=0;

 //
/* for(vbank=0,_ad2=0,_ad3=0; vbank<banks+1; vbank++,_ad3+=0x10000)
 {
  jtm_setvbank(vbank);
  jtm_vf(_image24+_ad3);
 }*/
 jtmscr_rollsrtab();
 return 0;
}

void jtmtask_updater(void)
{
 if(jtm_shutdownflag==0)
 {
  if(jtmupdater.action<200 && jtmscr.vbufferupdatemode==2)
  {
   jtmupdater.action++;
  }
  //
  if(jtmscr.vbufferupdatemode==2 && jtmscr.srsignal!=0)
  {
   if((jtmupdater.state==1 && jtmscr.drawround==0) || (jtmscr.drawexpection==1 && jtmupdater.state==1))
   {
    if(jtmupdater.action>jtmupdater.screenrefreshrate)
    {
     jtmscr.drawexpection=0;
     //
     jtmscr.srsignal=0;
     jtmupdater.action=0;
    }
   }
  }
  //
  if(jtmscr.vbufferupdatemode==1)
  {
   if((jtmupdater.state==1 && jtmscr.drawround==0) || (jtmscr.drawexpection==1 && jtmupdater.state==1))
   {
    if(jtmupdater.action>4)
    {
     jtmscr.drawexpection=0;
     //
     jtmscr.srsignal=0;
     jtmupdater.action=0;
//     jtmscr_updateVRAM();
//     jtm_copytovram(jtmscr.vbuffer,(jtmscr.width*jtmscr.height/0x10000));
    }
   }
  }
  //
 }
}

void jtm_updater_on(void)
{
 jtmupdater.action=0;
 jtmupdater.state=1;
 TM_AddProcess(jtmtask_updater);
 jtmupdater.action=0; // Roll counter to zero
 jtmscr.vbufferupdatemode=2;
                             // Modes are following:
                             //
                             // 00= No update
                             // 01= Immediatly update
                             // 02= Smart update
                             // 03= ?
 jtmscr_requestsr();
}

//
//
void jtmgl_savecurrentscreentobmp(void)
{
 //
 char nimi[255];

 //
 sprintf(nimi,"%sclip%1.4u.bmp",jtmscr.screencapturepath,jtmscr.clipnumber);
 jtm_clipscreen(nimi);
 jtmscr.clipnumber++;
}

//
//
void jtmgl_savecurrentscreentobmp2(void)
{
 //
 char nimi[255];

 //
 sprintf(nimi,"%sclip%u.bmp",jtmscr.screencapturepath,jtmscr.clipnumber);
 jtm_clipscreen(nimi);
 jtmscr.clipnumber++;
}


/////////////////////////////////////////////////////////////////////////////
//JTMPROCESS_SCREENCLIPPER
//------------------------
//
// This process allows screen clipping into sereval file formats.
//
// Like : RAW(8),BMP(8,24).
//
//
void jtmprocess_screenclipper(void)
{
 unsigned char nimi[256];
 //
 if(jtmclipper.timer<64)
 {
  jtmclipper.timer++;
 }
 // rCTRL+lSHIFT+F12 -> Capture screen as BMP file. (Path=x:\JTM\CAPTURES\)
 if(
    jtmkeyb.ktab[0x1d]==0xff &&
    jtmkeyb.ktab[0x58]==0xff &&
    jtmkeyb.ktab[0x36]==0xff && jtmclipper.timer>62
   )
 {
  jtmclipper.timer=0;
  jtmgl_savecurrentscreentobmp();
 }
 // rCTRL+lALT+F12   -> Capture screen as RAW 8bit greyscale image.
 if(
    jtmkeyb.ktab[0x1d]==0xff &&
    jtmkeyb.ktab[0x38]==0xff &&
    jtmkeyb.ktab[0x58]==0xff && jtmclipper.timer>62
   )
 {
  jtmclipper.timer=0;
  sprintf(nimi,"%sclip%1.4u.raw",jtmscr.screencapturepath,jtmscr.clipnumber);
  jtm_savebin(nimi,jtmscr.vbuffer,jtmscr.width*jtmscr.height);
  jtmscr.clipnumber++;
 }
}
//          jtm_phyavailable/(1024*1024),
//          jtm_vmmavailable/(1024*1024)

void jtmtask_smeter(void)
{
/* jtmgl_dbox(jtmscr.width-(16*6),jtmscr.height-16,
            jtmscr.width,       jtmscr.height,
            0x50);*/
}

void jtmgl_fullscreen(void)
{
 long a1;
 //
 jtmgl_arrowoff();
 jtmscr.sbutton.state=0;
 jtmscr_requestsr();
}

void jtmprocess_buttondrawer(void)
{
 if(jtmscr.sbutton.state==0)goto noton;
 if( jtmmouse.y>(jtmscr.height-16) && jtmmouse.x<74 && jtmmouse.button1!=0)
 {
  jtmscr.sbutton.mode=1;
 }
 else
 if( jtmmouse.y>(jtmscr.height-16) && jtmmouse.x<74)
 {
  jtmscr.sbutton.mode=0xff;
 }
 else
 {
  jtmscr.sbutton.mode=0;
 }
 //
 if(jtmscr.bsignal==0xff)
 {
  jtmscr.bsignal=0;
//  jtm_drawsbutton();
  if(jtmscr.srsignallock==0)jtmscr.srsignal=0xff;
 }
noton:
 while(0!=0);
}

void jtmprocess_blinker(void)
{
 jtmscr.blinker.delay2++;
 if((jtmscr.blinker.delay2&7)==0)
 {
  jtmscr.blinker.cont++;
  jtmscr.bsignal=0xff;
 }
 //
 if(jtmscr.blinker.state!=0)
 {
  jtmscr.blinker.delay++;
  if(jtmscr.blinker.delay>jtmscr.blinker.step)
  {
   jtmscr.blinker.cont2++;
   jtmscr.blinker.delay=0;
   jtmscr.blinker.mode=0xff-jtmscr.blinker.mode;
  }
 }
}

void jtmgl_blinkeroff(void)
{
 jtmscr.blinker.state=0;
}

void jtmgl_blinkeron(void)
{
 jtmscr.blinker.cont=0;
 jtmscr.blinker.cont2=0;
 jtmscr.blinker.mode=0;
 jtmscr.blinker.step=128;
 jtmscr.blinker.delay=0;
 jtmscr.blinker.state=0xff;
 TM_AddProcess(jtmprocess_blinker);
}

void jtmgl_arrowsystemon(void)
{
 jtmgl_loadsprites();
 jtmscr.arrowsystempid=TM_AddProcess(jtmgl_arrowmover);
}

void jtmgl_arrowsystemoff(void)
{
 jtmmouse.arrowstate=1;
 TM_RemoveProcess(jtmscr.arrowsystempid);
}

void jtmprocess_keybmouse(void)
{
 //
 long speed,ad,ad2;

 if( jtmkeyb.ktab[76] )
 {
  //
  jtmmouse.button1=0xff;
 }
 else
 {
  jtmmouse.button1=0;
 }

 //
 if( jtmkeyb.ktab[0x53] )
 {
  //
  jtmmouse.button2=0xff;
 }
 else
 {
  jtmmouse.button2=0;
 }

 //
 if(jtmmouse.mousekeys==0xff)
 {
  //
  if( jtmkeyb.ktab[56] )
  {
   //
   if(jtmkeyb.ktab[JTMKNUM_CTRL])
   {
    speed=1;
   }
   else
   {
    speed=8;
   }

   //
   for(ad=0; ad<speed; ad++)
   {
    if( jtmkeyb.ktab[72]==0xff )
    {
     //
     jtmmouse.viewy--;
     jtmmouse.signal=0xff;
    }
    if( jtmkeyb.ktab[80]==0xff )
    {
     //
     jtmmouse.viewy++;
     jtmmouse.signal=0xff;
    }
    if( jtmkeyb.ktab[75]==0xff )
    {
     //
     jtmmouse.viewx--;
     jtmmouse.signal=0xff;
    }
    if( jtmkeyb.ktab[77]==0xff )
    {
     //
     jtmmouse.viewx++;
     jtmmouse.signal=0xff;
    }
   }
  }
  else
  {
   //
   if(jtmkeyb.ktab[JTMKNUM_CTRL])
   {
    speed=1;
   }
   else
   {
    speed=8;
   }

   //
   for(ad=0; ad<speed; ad++)
   {
    if( jtmkeyb.ktab[72]==0xff )
    {
     //
     jtmmouse.y--;
     jtmmouse.signal=0xff;
    }
    if( jtmkeyb.ktab[80]==0xff )
    {
     //
     jtmmouse.y++;
     jtmmouse.signal=0xff;
    }
    if( jtmkeyb.ktab[75]==0xff )
    {
     //
     jtmmouse.x--;
     jtmmouse.signal=0xff;
    }
    if( jtmkeyb.ktab[77]==0xff )
    {
     //
     jtmmouse.x++;
     jtmmouse.signal=0xff;
    }
   }
  }
 }
}

void jtmscr_selectbg(char *fname)
{
 //
 unsigned char *b;
 long x,y,ad,ad2,ad3,ad4;

 //
 b=malloc(fsizeof(fname)+1024);
 jtm_loadbin(fname,b);
 for(ad=0; ad<(320*200); ad++)
 {
  b[ad]=b[ad]>>2;
 }
// jtmgl_stretchputimage(b,320,200, 0,0,jtmscr.width,jtmscr.height);
 free(b);
}

void jtmscr_setasbackground32(char *fname,int xle,int yle)
{
 int x,y;
 long ad,ad2;
 FILE *f;
 //
 f=jtm_fopen(fname,"rb");
 switch(jtmscr.bpp)
 {
  case 32:
  for(y=0; y<yle; y++)
  {
   ad=jtmscr.width*y;
   for(x=0; x<xle; x++)
   {
    jtmscr.vbuffer[((ad+x)<<2)+2]=fgetc(f);
    jtmscr.vbuffer[((ad+x)<<2)+1]=fgetc(f);
    jtmscr.vbuffer[((ad+x)<<2)+0]=fgetc(f);
   }
  }
  break;

  case 8:
  for(y=0; y<yle; y++)
  {
   ad=jtmscr.width*y;
   for(x=0; x<xle; x++)
   {
    jtmscr.vbuffer[ad+x]=fgetc(f)>>2;
    fgetc(f);
    fgetc(f);
   }
  }
  break;
 }
 jtm_fclose(f);
}

int jtmgl_getfontsomewhere(void)
{
 if( jtmscr.fn_systemfont_raw!=NULL && fexist(jtmscr.fn_systemfont_raw))
 {
  jtm_loadfont(jtmscr.fn_systemfont_raw);
 }
 else
 if(fexist(jtmscr.defaultff))
 {
  jtm_loadfont(jtmscr.defaultff);
 }
 else
 if(fexist(jtmscr.defaultff))
 {
  jtm_loadfont(jtmscr.defaultff);
 }
 else
 {
  jtm_messagebuffer("jtm_glsystemon: System font not found or invalid font defined in C:\\JTMOS.CFG.\n");
  return 1;
 }
 return 0;
}

void jtmscr_screenclipper_on(void)
{
 jtmclipper.timer=0;
 TM_AddProcess(jtmprocess_screenclipper);
}

void jtm_keybmouseon(void)
{
 TM_AddProcess(jtmprocess_keybmouse);
}

void jtm_keybmouseoff(void)
{
 //TM_AddProcess(jtmprocess_keybmouse);
}

void jtmscr_setdefcols(void)
{
 //
 jtmscr.defaultbackgroundcolor=0x1f/4;
 jtmscr.backgroundcolor=0x1f/4;
 jtmscr.headlinebackgroundcolor=0x4f/4; //0x4f/4;
}

void jtm_glsystemon() // Turn ON graphics environment revision 2
{
 int WorkAreaSize=28; // In 8 pixel units.
 char m1,m2,m3,m4;
 struct dfree d;
 char lastd='z'+1;
 unsigned long int freetab[255];
 long a1,a2,a3,a4,a5,a6,a7,a8,ad,ad2,ad3,ad4;
 unsigned x,y,xx,yy;
 char str[2550],str2[2550],str3[2550],str4[2550];
 JTIME aika;
 //
 textmode(C80);
 cprintf("Now initializing JTMOS v%1.4f GraOS, please stand by...\n\r",jtm_stationversion);
 //
 jtmscr.drawround=0;
 //
 for(a1=0,a2=0; a1<128; a1++,a2+=2)
 {
  jtmscr.sin1[a1]=a2;
 }
 for(a1=128,a2=255; a1<256; a1++,a2-=2)
 {
  jtmscr.sin1[a1]=a2;
 }
 jtmmouse_delaymode();
 jtmscr.forcedelaysr=0;

 // Font setup !
 jtmscr.fontcolor=0xff;
 jtmscr.ma=0;
 jtmscr.writex=8; jtmscr.writey=64+32; jtmscr.writexs=8; jtmscr.writeys=64+32;
 jtmscr.writexm=jtmscr.writexs+8*40;
 jtmscr.writeym=jtmscr.writeys+8*25;
 //
 jtmscr.sizeofvbuffer=2000*1800; //jtmscr_maxvbuffersize;
                                 // <== Decrease screen buffer size if you
                                 //     think it is too large to fit in memory.
                                 //     it's set to 1600x1200 size because
                                 //     1600x1200 is maximum screen resolution
                                 //     to display with current home
                                 //     monitor/gfx cart technology.
 // Videomode setup !
 switch(jtmscr.bpp)
 {
  case 8:
  jtmscr.vbuffer=malloc(jtmscr.sizeofvbuffer);
  break;
  case 32:
  jtmscr.vbuffer=malloc(jtmscr.sizeofvbuffer*4);
  /**** 32bit mode SPRITE functions are NOT compatible ****/
  jtmgl_arrowoff();
  jtmmouse.cursorlock=0xff;
  break;
 }

 // Clip system setup
 jtmscr.clipnumber=0;
 //
 if(jtmscr.vbuffer==NULL)
 {
  jtm_setvmode(C80);
  printf("Not enough memory to allocate video buffer.\n");
  printf("16M-32M RAM required.\n");
  waitkey();
  jtmscr.width=0; jtmscr.height=0;
  goto nosvga;
 }
 else
 {
  switch(jtmscr.bpp)
  {
   case 8:
   for(a1=0; a1<jtmscr.sizeofvbuffer; a1++)jtmscr.vbuffer[a1]=0;
   break;
   case 32:
   for(a1=0; a1<jtmscr.sizeofvbuffer*4; a1++)jtmscr.vbuffer[a1]=0;
   break;
  }
 }
 // 0x13   320x200   256c *
 // 0x0100 640x400   256c
 // 0x0101 640x480   256c <- #1
 // 0x0103 800x600   256c <- #2
 // 0x0105 1024x768  256c
 // 0x0107 1280x1024 256c
 //
 // 0x0141 400x300   256c ??
 // 0x0151 512x384   256c ??
 // 0x0120 1600x1200 256c ?? (Caution: Very slow ! (Indeed :)) ))
 //
 // ?? = S3 own standard, not globally used, maybe dangerous on other cards.
 //
//// #ifdef graos
 jtm_vmode=jtmscr.defaultgfxmode;
 #ifdef ___VMODE16001200
 jtm_vmode=0x120;
 #endif
 #ifdef ___VMODE512384
 jtm_vmode=0x151;
 #endif
 #ifdef ___VMODE320240
 jtm_vmode=0x131;
 #endif
 #ifdef ___VMODE320200
 jtm_vmode=0x13;
 #endif
 #ifdef ___VMODE400300
 jtm_vmode=0x0141;
 #endif
 #ifdef ___VMODE640400
 jtm_vmode=0x0100;
 #endif
 #ifdef ___VMODE640480
 jtm_vmode=0x0101;
 #endif
 #ifdef ___VMODE800600
 jtm_vmode=0x0103;
 #endif
 jtm_setvesa(jtm_vmode);
 jtm_multip256();
 switch(jtm_vmode)
 {
  // 24(32) bpp graphic modes
  case 0x0154:
  jtmscr.width=512;
  jtmscr.height=384;
  break;
  case 0x010f:
  jtmscr.width=320;
  jtmscr.height=200;
  break;
  case 0x0144:
  jtmscr.width=400;
  jtmscr.height=300;
  break;
  case 0x0112:
  jtmscr.width=640;
  jtmscr.height=480;
  break;
  case 0x0115:
  jtmscr.width=800;
  jtmscr.height=600;
  break;
  case 0x0117:
  jtmscr.width=1024;
  jtmscr.height=768;
  break;

  // 8 bpp graphic modes
  case 0x0151:
  jtmscr.width=512;
  jtmscr.height=384;
  break;
  case 0x0131:
  jtmscr.width=320;
  jtmscr.height=240;
  break;
  case 0x0141:
  jtmscr.width=400;
  jtmscr.height=300;
  break;
  case 0x0013:
  jtmscr.width=320;
  jtmscr.height=200;
  break;
  case 0x0100:
  jtmscr.width=640;
  jtmscr.height=400;
  break;
  case 0x0101:
  jtmscr.width=640;
  jtmscr.height=480;
  break;
  case 0x0103:
  jtmscr.width=800;
  jtmscr.height=600;
  break;
  case 0x0105:
  jtmscr.width=1024;
  jtmscr.height=768;
  break;
  case 0x0107:
  jtmscr.width=1280;
  jtmscr.height=1024;
  break;
  case 0x0120:
  jtmscr.width=1600;
  jtmscr.height=1200;
  break;
 }

 //jtmscr_setasbackground32("d:\\jtm\\pic\\suomi.raw",640,480);
 jtmscr_setasbackground32("d:\\jtm\\pic\\rgb.raw",640,480);

 // jtmscr_createnoisebackground();
/* #else
  jtm_vmode=0x03; // EGA text mode
  jtmscr.width=80;
  jtmscr.height=25;
 #endif*/
 //
 // Reset VRAM poke table (Which is used by GLdrawing functions and VRAMupdater)
 for(y=0; y<jtmscr.height; y++)jtmscr.srtab[y]=0xff;
 //
 //
nosvga:
 jtmscr_setdefcols();
 //
 jtmgl_getfontsomewhere();
 //
 //
 //
 // DRAWING ALLOWED AFTER THIS LINE!! NOT BEFORE !!
 //
 //
 //
#ifndef NO_DINFO
 // Draw disk free meter at downer left corner on the desktop.
 //
 for(ad2=('c'-'a')+1; ad2<(lastd-'a')+1; ad2++)
 {
  getdfree(ad2,&d);
  freetab[ad2]=d.df_avail*d.df_bsec*d.df_sclus;
 }
 //
 for(ad=('c'-'a')+1,x=0,m1='c'; ad<(lastd-'a')+1; ad++,m1++)
 {
  if(freetab[ad]!=0)
  {
   if(ad==0 || ad==1)
   {
    copyarea_wbm(jtmscr.fn_systemimage_raw,
                 jtm_icons[34*4+0],
                 jtm_icons[34*4+1],
                 jtm_icons[34*4+2],
                 jtm_icons[34*4+3],
                 x,jtmscr.height-16-64,
                 1);
   }
   else
   {
    copyarea_wbm(jtmscr.fn_systemimage_raw,
                 jtm_icons[37*4+0],
                 jtm_icons[37*4+1],
                 jtm_icons[37*4+2],
                 jtm_icons[37*4+3],
                 x,jtmscr.height-16-64,
                 1);
   }
   jtmscr.backgroundcolor=0x30;
   jtmgl_gotoxy(x+12,jtmscr.height-16-32);
   sprintf(str,"%c:",m1);
   write(str);
   x+=48;
  }
 }
#endif


 jtmgl_setupdrawingcolors();


#ifdef jtm_aname
 strcpy(str2,jtm_aname);
#else
 strcpy(str2,"no name");
#endif
 sprintf(str,"graos windowing system %1.2f : %s",jtm_stationversion,str2);
// jtm_piirratausta();
 jtmgl_blinkeron();
 TM_AddProcess(jtmtask_smeter);
 jtm_keybmouseon();
#ifndef NO_SWINS
 #ifndef FULLSCREEN
  // ** Open system windows **
  // -System status window
 // jtm_dwindow(0*8,35*8,80*8,(8+35)*8,"status window",system_window);
  #ifdef allownotices
  write(jtmscr.systemnotice);
  #endif
  // -Quick work board
  #ifdef EMULATETEXT
   #ifdef jtm_aname
    sprintf(str,"EGA textmode board - %s",jtm_aname);
   #else
    sprintf(str,"EGA textmode board - no name");
   #endif
  #else
   #ifdef jtm_aname
    sprintf(str,"Quick work board - %s",jtm_aname);
   #else
    sprintf(str,"Quick work board - %s","Untitled1");
   #endif
  #endif
 // jtm_dwindow((jtmscr.width/2)-WorkAreaSize*8,(jtmscr.height/2)-WorkAreaSize*8,(jtmscr.width/2)+WorkAreaSize*8,(jtmscr.height/2)+WorkAreaSize*8,str,user_window);
 jtm_dwindow(0,0,90*8,32*8,
             str,user_window);
 #endif
#endif
#ifdef FULLSCREEN
 jtmscr.writexs=0;
 jtmscr.writeys=0;
 jtmscr.writex=0;
 jtmscr.writey=0;
 jtmscr.writexm=jtmscr.width;
 jtmscr.writeym=jtmscr.height;
#endif




 /*** END OF SCREEN INITIALIZING PROCESS ***/

 /*** START OF VIDEO SCREEN SYSTEM DEVICE DRIVERS INSTALLATION PROCESS ***/
 //
 jtmscr.glsystem=1;
 jtmscr_screenclipper_on();
 //
 #ifndef NO_SBUTTON
  #ifdef graos
  jtmscr.sbutton.state=0xff;
  #else
  jtmscr.sbutton.state=0;
  #endif
 #else
 jtmscr.sbutton.state=0;
 #endif
 TM_AddProcess(jtmprocess_buttondrawer);
 jtmgl_arrowsystemon();
// jtmmouse_setxy(jtmscr.width/2,jtmscr.height/2);
 #ifndef NO_UPDATER
  #ifdef graos
  jtm_updater_on();
  #endif
 #endif
 /**DEBUG**
 textmode(C80);
 cprintf("Toimii t„h„n saakka.\n\r");
 cprintf("*Warp*\n\r");
 goto nomore;
 /**DEBUG**/
nomore:
 jtmmouse_normalmode();
 jtmscr_requestsr();
 jtmmouse.x=jtmscr.width/2;
 jtmmouse.y=jtmscr.height/2;
 if(jtmscr.bwmode!=0)
 {

 }
 jtmscr.defaultbackgroundcolor=0xbf/4;
 jtmscr.backgroundcolor=0xbf/4;
 jtmscr.headlinebackgroundcolor=0x4f/4;
}

void jtm_playaff(char *aff)
{
 char *bu;
 long pix,piy;
 long a1,a2,a3,a4,a5,a6,a7,a8;
 unsigned x,y,xx,yy;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 if(jtmscr.vbuffer!=NULL)free(jtmscr.vbuffer);
 jtmscr.vbuffer=malloc(320*200);
 bu=malloc(fsizeof(aff));
 a3=fsizeof(aff);
 printf("Loading animation.\n\r");
 fp=jtm_fopen(aff,"rb");
 pix=fgetc(fp);
 piy=fgetc(fp);
 printf("Pixel size = %u,%u\n\r",pix,piy);
 a1=0;
 do
 {
  bu[a1]=fgetc(fp); a1++;
 }while(!feof(fp));
 jtm_fclose(fp);
 a2=0;
 printf("Press any key to play animation.\n\r"); getch();
 //
 jtm_setvmode(0x13); jtm_greyscale256();
 //
 do
 {
  a2=0; a4=0;
  do
  {
   for(y=0; y<200; y+=piy)
   {
    a5=y*320;
    for(x=0; x<320; x+=pix,a4++)
    {
     m4=bu[a4+0];
     m5=bu[a4+(320/piy)];
     m6=m4;
     a7=abs(m5-m4)/piy;
     for(yy=0,a6=a5; yy<piy; yy++,a6+=320)
     {
      //
      m1=bu[a4+0];
      m2=bu[a4+1];
      m3=m1;
      a8=abs(m2-m1)/pix;
      for(xx=0; xx<pix; xx++)
      {
       jtmscr.vbuffer[a6+(xx+x)]=(m3+m6)/2;
       if(m3!=m2)
       {
        if(m3<m2)
        {
         if(m3+a8<0x100)m3+=a8;
        }
        else
        {
         if(m3-a8>-1)m3-=a8;
        }
       }
       //
      }
      if(m6!=m5)
      {
       if(m6<m5)
       {
        if((m6+a7)<0x100)m6+=a7;
       }
       else
       {
        if((m6-a7)>-1)m6-=a7;
       }
      }
     }
    }
   }
   jtm_waitsd(); jtm_vf(jtmscr.vbuffer);
   a2+=((320/pix)*(200/piy));
   if( a2>(a3- ((320/pix)*(200/piy)) ) )break;
   m1=jgetchim();
  }while(m1!=27);
  m1=getch();
 }while(m1!=27);
 textmode(C80);
}

void jtm_playaff2(char *aff) // No antialiasing
{
 char *bu;
 long pix,piy;
 int x,y,xx,yy;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 if(jtmscr.vbuffer!=NULL)free(jtmscr.vbuffer);
 jtmscr.vbuffer=malloc(320*200);
 bu=malloc(fsizeof(aff));
 ad3=fsizeof(aff);
 printf("Loading animation.\n\r");
 fp=jtm_fopen(aff,"rb");
 pix=fgetc(fp);
 piy=fgetc(fp);
 printf("Pixel size = %u,%u\n\r",pix,piy);
 ad=0;
 do
 {
  bu[ad]=fgetc(fp); ad++;
 }while(!feof(fp));
 jtm_fclose(fp);
 ad2=0;
 printf("Press any key to play animation.\n\r"); getch();
 //
 jtm_setvmode(0x13); jtm_greyscale256();
 //
 do
 {
  ad2=0; ad4=0;
  do
  {
   for(y=0; y<200; y+=piy)
   {
    ad5=y*320;
    for(x=0; x<320; x+=pix,ad4++)
    {
     jtmscr.vbuffer[ad6+(xx+x)]=bu[ad4];
    }
   }
   for(y=0; y<200-piy; y++)
   {
    ad=y*320;
    for(xx=0; xx<pix; xx++)
    {
     for(x=0; x<320-pix; x++)
     {
      jtmscr.vbuffer[y*320+x]=(jtmscr.vbuffer[ad+xx+x]+jtmscr.vbuffer[ad+x])>>1;
     }
    }
   }
   jtm_waitsd(); jtm_vf(jtmscr.vbuffer);
   ad2+=((320/pix)*(200/piy));
   if( ad2>(ad3- ((320/pix)*(200/piy)) ) )break;
   m1=jgetchim();
  }while(m1!=27);
  m1=getch();
 }while(m1!=27);
 textmode(C80);
}

// COPYAREA_WBM function
//
// Copy area from Workstation system image library
int copyarea_wbm(char *_fname,unsigned x1,unsigned y1,
                  unsigned x2,unsigned y2,
                  unsigned _pastex,unsigned _pastey,unsigned char _mode)
{

/*
 //
 FILE *fp;
 char *sysimage;
 unsigned long x,y,xx,yy,segm;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8,re;

 //
 re=0;

 //
 x2++; y2++;
 switch(jtmscr.bpp)
 {
  case 8:
  fp=fopen(_fname,"rb"); if(fp==NULL){jtm_messagebuffer("copyarea_wbm: error occured while accessing image.\n"); re=1; goto loppu;}
  for(yy=_pastey, y=y1; y<y2; y++, yy++)
  {
   fseek(fp,((y*640)+x1)-ftell(fp),SEEK_CUR);
   for(xx=_pastex, x=x1; x<x2; x++, xx++)
   {
    m1=fgetc(fp);
    if(_mode==1)
    {
     if(m1==0)goto skip;
    }
    if(yy<jtmscr.height && xx<jtmscr.width)jtmscr.vbuffer[yy*jtmscr.width+xx]=m1;
skip:
   }
   jtmscr.srtab[yy]=0xff;
  }
  fclose(fp);
  break;
  case 32:
  fp=fopen(_fname,"rb"); if(fp==NULL){jtm_messagebuffer("copyarea_wbm: error occured while accessing image.\n"); goto loppu;}
  for(yy=_pastey, y=y1; y<y2; y++, yy++)
  {
   fseek(fp,((y*640)+x1)-ftell(fp),SEEK_CUR);
   for(xx=_pastex, x=x1; x<x2; x++, xx++)
   {
    m1=fgetc(fp);
    if(_mode==1)
    {
     if(m1==0)goto skip2;
    }
    jtmscr.vbuffer[((yy*jtmscr.width+xx)<<2)+0]=jtmansi.palette[(m1<<2)+2]<<2;
    jtmscr.vbuffer[((yy*jtmscr.width+xx)<<2)+1]=jtmansi.palette[(m1<<2)+1]<<2;
    jtmscr.vbuffer[((yy*jtmscr.width+xx)<<2)+2]=jtmansi.palette[(m1<<2)+0]<<2;
    //jtm_vplot(xx,yy,m1);
skip2:
 while(0!=0);
   }
   jtmscr.srtab[yy]=0xff;
  }
  fclose(fp);
  break;
 }
loppu:
 return re;*/
}

void jtm_makepix(unsigned char *frame,unsigned char *frame1,unsigned pix)
{
 unsigned x,y,xx,yy;
 //
 for(y=0; y<200; y+=pix)
 {
  for(x=0; x<320; x+=pix)
  {
   for(xx=0; xx<8; xx++)
   {
    for(yy=0; yy<8; yy++)
    {
     frame1[(y+yy)*320+xx+x]=frame[y*320+x];
    }
   }
  }
 }
}

int jtm_countcolors(unsigned char *src,long length)
{
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 unsigned char colors[0xffff];
 unsigned char colsfound=0;
 for(ad=0; ad<0xffff; ad++)colors[ad]=0;
 for(ad=0,colsfound=0; ad<length; ad++)
 {
  for(ad2=0; ad2<colsfound; ad2++)
  {
   if(src[ad]==colors[ad2])
   {
    goto aldef;
   }
  }
  colors[colsfound]=src[ad]; colsfound++;
aldef:
 while(0!=0);
 }
 return colsfound;
}

int jtm_countmostusedcolor(unsigned char *src,long length)
{
 unsigned char colors[0xffff];
 long usedc[256];
 unsigned char colsfound=0;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 //
 for(ad=0; ad<256; ad++)usedc[ad]=0;
 for(ad=0; ad<0xffff; ad++)colors[ad]=0;
 for(ad=0,colsfound=0; ad<length; ad++)
 {
  usedc[src[ad]]++;
/*  for(ad2=0; ad2<colsfound; ad2++)
  {
   if(src[ad]==colors[ad2])
   {
    goto aldef;
   }
  }
  colors[colsfound]=src[ad]; colsfound++;
aldef:*/
 }
 // Result: AD2=Most used color
 //         AD3=Power value
 for(ad=0,ad2=0,ad3=0; ad<256; ad++)
 {
  if( usedc[ad]>ad3 )
  {
   ad3=usedc[ad];
   ad2=ad;
  }
 }
 return ad2;
}

void jtm_plot85(unsigned char *bm,int _x,int _y,int _c)
{
 unsigned char bits[]={128,64,32,16,8,4,2,1};
 long a1,a2,a3,a4;
 //
 a1=(_x/2)+(((_y/2)*40));
 a2=1-(_x&1); a2+=(1-(_y&1))*2; a2=a2*2;
 a4=_c&3;
 bm[a1]=bm[a1]|(a4<<a2);
}

void jtm_drawsprite(unsigned char *obm,unsigned char *bm,unsigned int dx,unsigned int dy)
{
 long a1,a2,a3,a4,a5,a6,a7,a8;
 unsigned x,y;
 //
 for(y=0; y<32; y++)
 {
  for(x=0; x<32; x++)
  {
   if((dx+x)<319)
   {
    if((dy+y)<199)
    {
     a1=(y+dy)*320+(dx+x);
     obm[a1]=bm[y*32+x];
    }
   }
  }
 }
}

// JTMGL_GETSPRITE function
// ------------------------
//
//
void jtmgl_getsprite(unsigned char *bm,unsigned int dx,unsigned int dy,int mode)
{
 int _x,_y;
 long _ad,_ad2,a1,a2,a3,a4,a5,a6,a7,a8;
 unsigned char m1,m2;
 //
 switch(jtmscr.bpp)
 {
  // 8 bpp version
  case 8:
  for(_y=0; _y<32; _y++)
  {
   a1=_y*32;
   a2=(_y+dy)*jtmscr.width;
   for(_x=0; _x<32; _x++)
   {
    if((dx+_x)<(jtmscr.width-1))
    {
     if((dy+_y)<jtmscr.height)
     {
      _ad=a2+(dx+_x);
      m1=jtmscr.vbuffer[_ad];
      bm[a1+_x]=m1;
     }
    }
   }
  }
  break;
  // 32 bpp version
  case 32:
  for(_y=0; _y<32; _y++)
  {
   a1=_y*32;
   a2=(_y+dy)*jtmscr.width;
   for(_x=0; _x<32; _x++)
   {
    if((dx+_x)<(jtmscr.width-1))
    {
     if((dy+_y)<jtmscr.height)
     {
      _ad=a2+(dx+_x);
      m1=jtmscr.vbuffer[(_ad<<2)+0];
      bm[((a1+_x)<<2)+0]=m1;
      m1=jtmscr.vbuffer[(_ad<<2)+1];
      bm[((a1+_x)<<2)+1]=m1;
      m1=jtmscr.vbuffer[(_ad<<2)+2];
      bm[((a1+_x)<<2)+2]=m1;
     }
    }
   }
  }
  break;
 }
 //
}

// JTMGL_PUTSPRITE function
// ------------------------
//
//
void jtmgl_putsprite(unsigned char *bm,int dx,int dy,int mode)
{
 int _x,_y;
 long _ad,_ad2,a1,a2,a3,a4,a5,a6,a7,a8;
 //
 switch(jtmscr.bpp)
 {
  case 8:
  for(_y=0; _y<32; _y++)
  {
   a1=_y*32;
   a2=(_y+dy)*jtmscr.width;
   if( (dy+_y)>-1 )
   {
    for(_x=0; _x<32; _x++)
    {
     if((dx+_x)<(jtmscr.width) && (dx+_x)>-1 )
     {
      if((dy+_y)<jtmscr.height)
      {
       _ad=a2+(dx+_x);
       _ad2=bm[a1+_x];
       if(_ad2!=0 || mode==0)
       {
        jtmscr.vbuffer[_ad]=_ad2;
       }
      }
     }
    }
    jtmscr.srtab[_y+dy]=0xff;
   }
   //
  }
  break;
  case 32:
  for(_y=0; _y<32; _y++)
  {
   a1=_y*32;
   a2=(_y+dy)*jtmscr.width;
   if( (dy+_y)>-1 )
   {
    for(_x=0; _x<32; _x++)
    {
     if((dx+_x)<(jtmscr.width) && (dx+_x)>-1 )
     {
      if((dy+_y)<jtmscr.height)
      {
       _ad=(a2+(dx+_x))<<2;
       _ad2=bm[a1+_x];
       if(_ad2!=0 || mode==0)
       {
        jtmscr.vbuffer[_ad+0]=jtmansi.palette[(_ad2<<2)+2]<<2;
        jtmscr.vbuffer[_ad+1]=jtmansi.palette[(_ad2<<2)+1]<<2;
        jtmscr.vbuffer[_ad+2]=jtmansi.palette[(_ad2<<2)+0]<<2;
       }
      }
     }
    }
    jtmscr.srtab[_y+dy]=0xff;
   }
   //
  }
  break;
 }
}

void jtm_vgatowindow(unsigned char *bm)
{
 long a1,a2,a3,a4;
 unsigned x,y,xx,yy;
 //
 for(y=0; y<200; y++)
 {
  a1=(jtmscr.writeys+y)*jtmscr.width;
  a2=y*320;
  jtmscr.srtab[jtmscr.writeys+y]=0xff;
  if((y+jtmscr.writeys)>jtmscr.writeym)break;
  for(x=0; x<320; x++)
  {
   jtmscr.vbuffer[a1+x+jtmscr.writexs]=(bm[a2+x])+0x40;
  }
 }
}

void jtm_vgatowindow1(unsigned char *bm)
{
 long a1,a2,a3,a4;
 unsigned x,y;
 //
 for(y=0; y<200; y++)
 {
  a1=(jtmscr.writeys+y)*jtmscr.width;
  a2=y*320;
  jtmscr.srtab[jtmscr.writeys+y]=0xff;
  if((y+jtmscr.writeys)>jtmscr.writeym)break;
  for(x=0; x<320; x++)
  {
   jtmscr.vbuffer[a1+x+jtmscr.writexs]=bm[a2+x];
  }
 }
}

void jtm_bwvgatowindow(unsigned char *bm)
{
 //
 long a1,a2,a3,a4;
 register unsigned x,y;

 //
 jtm_greyscale256();

 //
 for(y=0,a3=0; y<200; y++,a3++)
 {
  if(a3==8){ write("\n"); a3=0; }
  a1=(jtmscr.writey+a3)*jtmscr.width;
  a2=y*320;
  jtmscr.srtab[jtmscr.writey+a3]=0xff;
  if((a3+jtmscr.writey)>jtmscr.writeym)break;
  for(x=0; x<320; x++)
  {
   jtmscr.vbuffer[a1+x+jtmscr.writex]=(bm[a2+x]>>2)+0x40;
  }
 }

 //
 write("\n");
}

void jtm_bwvgatowindow_DIRECT(unsigned char *bm)
{
 long a1,a2,a3,a4;
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 //
 for(y=0; y<200; y++)
 {
  a1=(jtmscr.writeys+y)*jtmscr.width;
  a2=y*320;
  jtmscr.srtab[jtmscr.writeys+y]=0xff;
  if((y+jtmscr.writey)>jtmscr.writeym)break;
  for(x=0; x<320; x++)
  {
   jtmscr.vbuffer[a1+x+jtmscr.writexs]=bm[a2+x];
  }
 }
}

void jtmscr_waitsr(void)
{
 if(jtmtask.state!=0)
 {
  while(jtmscr.srsignal==0xff);
 }
// if(jtmscr.forcedelaysr)
}

void jtmscr_requestsr(void) // Request screen redraw !
{
 if(jtmupdater.state==0)
 {
//  jtmscr_updateVRAM();
 }
 else
 {
  #ifndef NO_SBUTTONUPDATEVIASR
//  jtm_drawsbutton();
  #endif
  jtmscr.bsignal=0xff;
  jtmmouse.signal=0xff;
  if(jtmupdater.state!=0 && jtmtask.state!=0)
  {
   jtmscr_waitsr();
   jtm_waitsd();
   jtmscr.srsignal=0xff;
  }
  #ifdef ALLWAYS_WAIT_UNTIL_SR_COMPLETE
  while(jtmscr.srsignal==0xff);
  #endif
 }
}

// Graphics Library's Draw Line Function (The code is be somewhat confusing).
void jtm_gldrawline(int _x1,int _y1,int _x2,int _y2,unsigned char color)
{
 jtm_gldrawline1(jtmscr.vbuffer,jtmscr.width,jtmscr.height,
                 _x1,_y1,_x2,_y2,color);
}
void jtmgl_drawline(int _x1,int _y1,int _x2,int _y2,unsigned char color)
{
 jtm_gldrawline1(jtmscr.vbuffer,jtmscr.width,jtmscr.height,
                 _x1,_y1,_x2,_y2,color);
 if(_y1<_y2)
 {
  jtmgl_tosrtab(_y1,_y2+1);
 }
 else
 {
  jtmgl_tosrtab(_y2,_y1+1);
 }
}

//
void jtm_gldrawline1(unsigned char *vbuffer,long width,long height,
                    int _x1,int _y1,int _x2,int _y2,unsigned char color)
{
 //
 int x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4,tem[10],matka,aika;
 long a1,a2,a3,a4,a5,a6,a7,a8;
 float ax,ay,fxs,fys,a,b,c,d,fa,fb,fc,fd;

 //
/* if((_x1<0 || _y1<0) || (_x1>=width || _y1>=height))
 {
  tem[0]=_x1;
  tem[1]=_y1;
  tem[2]=_x2;
  tem[3]=_y2;

  _x2=tem[0];
  _y2=tem[1];
  _x1=tem[2];
  _y1=tem[3];
 }*/

 //
 if(_x1>=width || _x1<0)return;
 if(_y1>=width || _y1<0)return;
 if(_x2>=width || _x2<0)return;
 if(_y2>=width || _y2<0)return;

 //
 ax=_x1; ay=_y1;
 a=_x1; b=_x2; c=a-b; fxs=c;
 a=_y1; b=_y2; c=a-b; fys=c;
 fa=fxs; if(fa<0)fa=-fa;
 fb=fys; if(fb<0)fb=-fb;
 if( ((fa+fb)/2)>31 )
 {
  if( ((fa+fb)/2)>63 )
  {
   if( ((fa+fb)/2)>127 )
   {
    if( ((fa+fb)/2)>255 )
    {
     fc=512;
    }
    else
    {
     fc=256;
    }
   }
   else
   {
    fc=128;
   }
  }
  else
  {
   fc=64;
  }
 }
 else
 {
  fc=32;
 }
 fxs=fxs/fc; fys=fys/fc; a7=0;
 do
 {
  //
  xx=ax; yy=ay;
  if(xx<0 || yy<0 || xx>=width || yy>=height)
  {
   break;
  }
  else
  {
   vbuffer[yy*width+xx]=color;
  }
  ax=ax-fxs; ay=ay-fys;
 }while(xx!=_x2 || yy!=_y2);
 if(xx<0 || yy<0 || xx>=width || yy>=height)
 {
 }
 else
 {
  vbuffer[yy*width+xx]=color;
 }
}

void jtm_piirratausta(void)
{
 unsigned char *texkukka;
 unsigned _x,_y;
 long a1,a2,a3,a4;
 //
 texkukka=malloc(256*256+1024);
 for(a1=0; a1<(256*256); a1++)
 {
  texkukka[a1]=0;
 }
 #ifndef NO_BACKGROUNDPICTURE
 if(fexist("\\jtm\\pic\\texkukka.raw"))
 {
  jtm_loadbin("\\jtm\\pic\\texkukka.raw",texkukka);
  for(_y=0; _y<jtmscr.height; _y++)
  {
   a1=(_y&255)*256;
   a2=_y*jtmscr.width;
   for(_x=0; _x<jtmscr.width; _x++)
   {
    jtmscr.vbuffer[a2+_x]=(texkukka[a1+(_x&255)]/4)+0x80;
   }
  }
  // **
 }
 #endif
joo:
 while(0!=0);
}

void jtmscr_selectwholescreenarea(JTMSCR jscr)
{
 jscr.writexs=0;
 jscr.writeys=0;
 jscr.writexm=1000;
 jscr.writeym=1000;
}

void jtmscr_setwritecolor(JTMSCR jscr,int tcolor,int tbcolor)
{
 jscr.fontcolor=tcolor;
 jscr.backgroundcolor=tbcolor;
}

void jtm_cutsbutton(void)
{
 //
 long ad,ad2,ad3,ad4,x,y;

 //
 if(jtm_vmode==3)return;

 //
 if(jtmscr.sbutton.tmp)
 {
  for(y=0; y<16; y++)
  {
   ad=jtmscr.width*((jtmscr.height-16)+y);
   ad2=76*y;
   for(x=0; x<76; x++)
   {
    jtmscr.vbuffer[ad+x]=jtmscr.sbutton.tmp[ad2+x];
   }
  }
 }
}

void jtm_drawsbutton(void) // Draws "Start button" and start desk area.
{
 JSTRING str,str2,str3,str4;
 JTMSCR jscr;
 JTIME aika;
 unsigned _x,_y,_x2,_y2;
 long a1,a2,a3,a4,a5,a6,a7,a8,drawwi=0;
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4,ad5,ad6,ad7;

 //


 //
 if(jtm_vmode==3)return;

 //
 if(!jtmscr.sbutton.tmp)
 {
  jtmscr.sbutton.tmp=malloc(74*16+1024);
 }
 for(y=0; y<16; y++)
 {
  ad=jtmscr.width*((jtmscr.height-16)+y);
  ad2=76*y;
  for(x=0; x<76; x++)
  {
   jtmscr.sbutton.tmp[ad2+x]=jtmscr.vbuffer[ad+x];
  }
 }

 //
 if(jtmmouse.y>(jtmscr.height-48))
 {
  drawwi=0xff;
 }
 else
 {
  drawwi=0;
 }
 if(jtmmouse.button2!=0 && jtmkeyb.ktab[0x2a]==0xff)
 {
  if(jtmmouse.y>-1 && jtmmouse.y<jtmscr.height)
  {
   a1=((jtmscr.height-1)-jtmmouse.y)*(jtmscr.width/4);
   if(a1<65536)
   {
    jtmscr.sbegin=a1;
   }
  }
 }
 //
 jtm_setscreenstart(jtmscr.sbegin);
 //
 if(jtmscr.sbutton.mode==0)
 {
  a8=0;
 }
 if(jtmscr.sbutton.mode==0xff)
 {
  a8=jtmscr.blinker.mode>>3;
 }
 if(jtmscr.sbutton.mode==1)
 {
  a8=64;
 }
 //
 for(_y=jtmscr.height-16,ad2=0; _y<jtmscr.height; _y++,ad2++)
 {
  a1=(jtmscr.sin1[(ad2+(jtmscr.blinker.cont>>3)<<1)&255]>>2)+0xc0;
  ad=_y*jtmscr.width;
  for(_x=0; _x<jtmscr.width; _x++)
  {
//   jtmgl_drawscanline(_y,a1);
  }
 }
 for(_y=0,a1=0; _y<16; _y++)
 {
  ad=(jtmscr.height-(15-_y)-1)*jtmscr.width;
  switch(jtmscr.bpp)
  {
   case 8:
   for(_x=0; _x<74; _x++,a1++)
   {
    ad2=jtmscr.sbutton.bm[a1]/4;
    ad2=(ad2+a8)/2;
    jtmscr.vbuffer[ad+_x]=ad2+0x40;
   }
   break;
   case 32:
   for(_x=0; _x<74; _x++,a1++)
   {
    ad2=jtmscr.sbutton.bm[a1]/4;
    ad2=(ad2+a8)/2;
    jtmscr.vbuffer[((ad+_x)<<2)+0]=jtmansi.palette[((ad2+0x40)<<2)+2]<<2;
    jtmscr.vbuffer[((ad+_x)<<2)+1]=jtmansi.palette[((ad2+0x40)<<2)+1]<<2;
    jtmscr.vbuffer[((ad+_x)<<2)+2]=jtmansi.palette[((ad2+0x40)<<2)+0]<<2;
   }
   break;
  }
 }
 //
 for(y=0; y<16; y++)
 {
  jtmscr.srtab[jtmscr.height-y-1]=0xff;
 }
/* if(jtmscr.write.busy==0)
 {
  jtm_readtime(aika);
  memcpy(&jscr,&jtmscr,sizeof(jtmscr));
  jtmscr.fontcolor=0xff;
  jtmscr.backgroundcolor=0x00;
  jtmscr.writexs=0;
  jtmscr.writeys=0;
  jtmscr.writexm=jtmscr.width;
  jtmscr.writeym=jtmscr.height;
  jtmgl_gotoxy(jtmscr.width-(10*8),jtmscr.height-12);
  sprintf(str.s,"%1.2u:%1.2u:%1.2u",sizeof(jtmscr),sizeof(jtmscr),sizeof(jtmscr));
  //aika.hours,aika.minutes,aika.seconds);
  write(str.s);
  memcpy(&jtmscr,&jscr,sizeof(jtmscr));
 }*/
 //
loppu:
 while(0!=0);
}

void jtmmouse_delaymode(void)
{
 jtmmouse.arrowmode=1;
 jtmmouse.signal=0xff;
}

void jtmmouse_normalmode(void)
{
 jtmmouse.arrowmode=0;
 jtmmouse.signal=0xff;
}

void jtmmouse_fingermode(void)
{
 jtmmouse.arrowmode=2;
 jtmmouse.signal=0xff;
}

void jtmgl_gotoxy(int gx,int gy)
{
 jtmscr.writex=gx;
 jtmscr.writey=gy;
}

void writeputc(char merkki)
{
 char stringi[255];
 //
 sprintf(stringi,"%c",merkki);
 write(stringi);
}

void jtm_typefile(char *filename,void SelectedOutputConsole())
{
 FILE *f,*f2;
 unsigned __temp;
 //
 __temp=jtmscr.immediatly_update_on_function_write;
 jtmscr.immediatly_update_on_function_write=0;
 //
 f=jtm_fopen(filename,"rb");
 do
 {
  cprintf("%c",fgetc(f));
  //SelectedOutputConsole(fgetc(f));
 }while(!feof(f));
 jtm_fclose(f);
 jtmscr.immediatly_update_on_function_write=__temp;
}

void jtmscr_createnoisebackground(void)
{
 int x,y;
 long ad,ad2;
 char m1,m2;
 //
 jtmgl_arrowoff();
 switch(jtmscr.bpp)
 {
  case 8:
  for(y=0; y<jtmscr.height; y++)
  {
   ad=y*jtmscr.width;
   for(x=0; x<jtmscr.width; x++)
   {
    jtmscr.vbuffer[ad+x]=(rand()&7)+0x50;
   }
  }
  break;
  case 32:
  for(y=0; y<jtmscr.height; y++)
  {
   ad=y*jtmscr.width;
   for(x=0; x<jtmscr.width; x++)
   {
    m1=rand()&63;
    jtmscr.vbuffer[((ad+x)<<2)+0]=m1;
    jtmscr.vbuffer[((ad+x)<<2)+1]=m1;
    jtmscr.vbuffer[((ad+x)<<2)+2]=m1;
   }
  }
  break;
 }
 for(y=0; y<jtmscr.height; y++)
 {
  jtmscr.srtab[y]=0xff;
 }
 jtmgl_arrowon();
}

void jtmscr_openerrorwindow(JSTRING str)
{
 jtm_dwindow(jtmscr.width/2-150,jtmscr.height/2-56,
             jtmscr.width/2+150,jtmscr.height/2+56,
             "error occured",errormsgbox_window);
 write(str.s);
 jtmmouse_waitforclick();
}

void jtmscr_openinputwindow(JSTRING str,JSTRING str2)
{
 jtmscr.cursor.state=1;
 jtm_dwindow(jtmscr.width/2-200,jtmscr.height/2-32,
             jtmscr.width/2+200,jtmscr.height/2+32,
             str.s,errormsgbox_window);
 strcpy(str2.s,jinputstr());
}

void jtmgl_drawscanline(unsigned line,unsigned char color)
{
 register int x,y;
 long ad,ad2;
 //
 ad=jtmscr.width*line;
 switch(jtmscr.bpp)
 {
  case 8:
  for(x=0; x<jtmscr.width; x++)jtmscr.vbuffer[x+ad]=color;
  break;
  case 32:
  for(x=0; x<jtmscr.width; x++)jtmscr.vbuffer[(x+ad)<<2]=color;
  break;
 }
}

// RGB to 256c convert result is really bad, not recommended to use.
// But it anyway works even.
inline void jtmscr_putrgb(unsigned char *buffer,unsigned long int address,unsigned char red,unsigned char green,unsigned char blue)
{
 switch(jtmscr.bpp)
 {
  case 8:
  address=address>>2;
  if(red>green && red>blue)
  {
   buffer[address]=(red>>2)+0x40;
  }
  else
  if(blue>green && blue>red)
  {
   buffer[address]=(blue>>2)+0x00;
  }
  else
  if(green>red && green>blue)
  {
   buffer[address]=(green>>2)+0x80;
  }
  else
  buffer[address]=(blue>>2)+0xc0;
  break;
  case 32:
  buffer[address+0]=red;
  buffer[address+1]=green;
  buffer[address+2]=blue;
  break;
 }
}

// 256 color display only !
// Use values between 1-255 only.
void jtmscr_setupbrightness(float value)
{
 long ad,ad2,ad3,ad4,r,g,b;
 float fa,fb,fc,fd;
 //
 value=(value/48)+0.001;
 //
 switch(jtmscr.bpp)
 {
  case 32: // 32bit casing is quite anonymous
  break;
  case 8:
  for(ad=0; ad<256; ad++)
  {
   fa=jtmansi.palette[(ad<<2)+0];
   fb=jtmansi.palette[(ad<<2)+1];
   fc=jtmansi.palette[(ad<<2)+2];
   fa=fa/value;
   fb=fb/value;
   fc=fc/value;
   r=fa; g=fb; b=fc;
   if(r>255)r=255;
   if(g>255)g=255;
   if(b>255)b=255;
   jtm_setpalette1(ad,r>>2,g>>2,b>>2);
  }
  break;
 }
}

void SmoothIt(void)
{
 register long ad,ad2,x,y;
 register
 int aplusx1=plusx1,aplusy1=plusy1,
     aplusx2=plusx2,aplusy2=plusy2,
     aplusx3=plusx3,aplusy3=plusy3,
     aplusx4=plusx4,aplusy4=plusy4,
     aplusx5=plusx5,aplusy5=plusy5;

 // INTERPOLATION
 for(ad2=0; ad2<SmoothValue; ad2++)
 {
  for(ad=0; ad<(320*0xbc); ad+=ValiStep)
  {
   jtmscr.vbuffer[ad+aplusx1]=(jtmscr.vbuffer[ad+aplusy1]+jtmscr.vbuffer[ad+aplusy2]) >>1;
   jtmscr.vbuffer[ad+aplusx2]=(jtmscr.vbuffer[ad+aplusy2]+jtmscr.vbuffer[ad+aplusy3]) >>1;
   jtmscr.vbuffer[ad+aplusx3]=(jtmscr.vbuffer[ad+aplusy3]+jtmscr.vbuffer[ad+aplusy4]) >>1;
   jtmscr.vbuffer[ad+aplusx4]=(jtmscr.vbuffer[ad+aplusy4]+jtmscr.vbuffer[ad+aplusy5]) >>1;
  }
  for(y=0,ad=0; y<200; y++,ad+=320)
  {
   for(x=0; x<4; x++)
   {
    jtmscr.vbuffer[ad+x]=0;
   }
  }
  //
  for(y=0,ad=0; y<200; y++,ad+=320)
  {
   for(x=316; x<320; x++)
   {
    jtmscr.vbuffer[ad+x]=0;
   }
  }
 }
}

void jtmscr_manualsetmcga320200b(void)
{
 //
 jtmscr_set320200();
 jtm_setvmode(0x13);
 waitkey();
 jtmscr_clearscreen();
 jtm_multip256();
}

void jtmscr_manualsetmcga(void)
{
 jtmscr_manualsetmcga320200();
}

void jtmscr_manualsethmcga(void)
{
 jtmscr_manualsetmcga320240();
}

void jtmscr_manualsetmed(void)
{
 jtmscr_manualsetvga512384();
}

void jtmscr_manualsethigh(void)
{
 jtmscr_manualsetvga640480();
}

void jtmscr_mousecheese(void)
{
/* jtmmouse.arrowstate=0x0;
 jtmmouse.x=0;
 jtmmouse.y=0;
 jtmmouse.arrowmode=0;
 jtmgl_loadsprites();*/
}

int jtmscr_manualsetmcga320200(void)
{
 //
 jtmscr.graphictimer=0;
 jtmscr.bpp=8;
 jtmscr.width=320;
 jtmscr.height=200;

 //
 jtmscr.writexs=0; jtmscr.writeys=0;
 jtmscr.writexm=jtmscr.width; jtmscr.writeym=jtmscr.height;
 jtmscr.writex=0; jtmscr.writey=0;
 jtmscr.fontcolor=0x3f;
 NewWindow.fontcolor=0x3f;
 NewWindow.fontbcolor=0;

 //
 jtmscr.vbuffer=malloc(640*400);
 jtm_setvmode(0x13);
 jtmscr_clearscreen();

 //
 if(jtmgl_getfontsomewhere())
 {
  return 1;
 }
 return 0;
}

int jtmscr_manualset32bpp320200(void)
{
 //
 jtmscr.graphictimer=0;
 jtmscr.bpp=32;
 jtmscr.width=320;
 jtmscr.height=200;

 //
 jtmscr.writexs=0; jtmscr.writeys=0;
 jtmscr.writexm=jtmscr.width; jtmscr.writeym=jtmscr.height;
 jtmscr.writex=0; jtmscr.writey=0;
 jtmscr.fontcolor=0x3f;
 NewWindow.fontcolor=0x3f;
 NewWindow.fontbcolor=0;

 //
 jtmscr.vbuffer=malloc(640*400*4);
 jtm_setvesa(0x10f);
 jtmscr_clearscreen();

 //
 if(jtmgl_getfontsomewhere())
 {
  return 1;
 }
 return 0;
}

void jtmscr_manualsetmcga320240(void)
{
 //
 jtmscr.graphictimer=0;
 jtmscr.bpp=8;
 jtmscr.width=320;
 jtmscr.height=240;
 jtmscr.vbuffer=malloc(640*400);

 //
 jtmscr.writexs=0; jtmscr.writeys=0;
 jtmscr.writexm=jtmscr.width; jtmscr.writeym=jtmscr.height;
 jtmscr.writex=0; jtmscr.writey=0;
 jtmscr.fontcolor=0x3f;
 NewWindow.fontcolor=0x3f;
 NewWindow.fontbcolor=0;

 //
 jtm_setvesa(0x131);
 jtmscr_clearscreen();

 //
 if(jtmgl_getfontsomewhere())
 {
  return 1;
 }
 return 0;
}

void jtmscr_manualsetvga512384(void)
{
 //
 jtmscr.graphictimer=0;
 jtmscr.bpp=8;
 jtmscr.width=512;
 jtmscr.height=384;
 jtmscr.vbuffer=malloc(800*600);
 jtm_setvesa(0x151);
 jtmscr_clearscreen();

 //
 jtmscr.writexs=0; jtmscr.writeys=0;
 jtmscr.writexm=jtmscr.width; jtmscr.writeym=jtmscr.height;
 jtmscr.writex=0; jtmscr.writey=0;
 jtmscr.fontcolor=0x3f;
 NewWindow.fontcolor=0x3f;
 NewWindow.fontbcolor=0;

 //
 if(jtmgl_getfontsomewhere())
 {
  return 1;
 }
 return 0;
}

int jtmscr_manualsetvga640480(void)
{
 //
// jtmscr_mousecheese();

 //
 jtmscr.graphictimer=0;
 jtmscr.bpp=8;
 jtmscr.width=640;
 jtmscr.height=480;
 jtmscr.vbuffer=malloc(1600*1200);
 jtm_setvesa(0x101);
 jtmscr_clearscreen();

 //
 jtmscr.writexs=0; jtmscr.writeys=0;
 jtmscr.writexm=jtmscr.width; jtmscr.writeym=jtmscr.height;
 jtmscr.writex=0; jtmscr.writey=0;
 jtmscr.fontcolor=0x3f;
 NewWindow.fontcolor=0x3f;
 NewWindow.fontbcolor=0;

 //
 if(jtmgl_getfontsomewhere())
 {
  return 1;
 }
 return 0;
}


void jtmscr_assignwholescreentosr(void)
{
 long ad;
 //
 for(ad=0; ad<jtmscr.height; ad++)jtmscr.srtab[ad]=0xff;
}

// void jtmscr_blackscreen(void);
//
// Notice:
//
// The function updates(Copies the data to VRAM) automatically,
// so there is no need to call handler/or wait for handler.
// Handler example: jtmscr_requestsr();
//
void jtmscr_blackscreen(void)
{
 register long ad,x,y;
 //
 switch(jtmscr.bpp)
 {
  // 8bit mode casing
  case 8:
  for(ad=0; ad<(jtmscr.width*jtmscr.height); ad++)jtmscr.vbuffer[ad]=0;
  break;

  // 24bit mode casing
  case 24:
  for(ad=0; ad<(jtmscr.width*jtmscr.height)*4; ad++)jtmscr.vbuffer[ad]=0;
  break;
 }
 for(y=0; y<jtmscr.height; y++)jtmscr.srtab[y]=0xff;
 jtmscr_updateVRAM(); // <-- Updates videoram automatically, doesn't wait
                      //     for handler to handle session.
}

// Example: jtmscr_drawline(jtmscr.vbuffer, 0,0,160,160, 255, 0.25 );
void jtmscr_drawline(unsigned char *bm,int _x1,int _y1,int _x2,int _y2,unsigned char color,float fReso)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2;
 long unsigned ad7;
 float ax,ay,fxs,fys,a,b,c,d;
 //
 ax=_x1; ay=_y1;
 a=_x1; b=_x2; c=a-b; fxs=c;
 a=_y1; b=_y2; c=a-b; fys=c;
 fxs=fxs/fReso; fys=fys/fReso; ad7=0;
 do
 {
  xx=ax; yy=ay;
  if( !(xx>-1 && yy>-1 && xx<320 && yy<200) )
  {
   bm[yy*320+xx]=color;
  }
  ax=ax-fxs; ay=ay-fys;
 }while(xx!=_x2 || yy!=_y2);
 if( !(xx>-1 && yy>-1 && xx<320 && yy<200) )
 {
  bm[yy*320+xx]=color;
 }
}

// Fills whole screen with specified object filling style,
// this function is usually used to fill vectors.
//
void jtmscr_fillobject(void)
{
 register
 long x,y,ad,ad2,
      x1,y1,x2,y2;
 char m1,m2;
 //
 for(y=0; y<jtmscr.height; y++)
 {
  ad=y*jtmscr.width;
  for(x=0; x<jtmscr.width; x++)
  {
   if( jtmscr.vbuffer[ad+x]!=0 ) goto colorfound;
  }
  goto colornotfound;
colorfound:
  x1=x;
  for(x=319; x>-1; x--)
  {
   if( jtmscr.vbuffer[ad+x]!=0 ) goto colorfound2;
  }
  goto colornotfound;
colorfound2:
  x2=x;
  for(x=x1,m1=0; x<x2; x++)
  {
   if( jtmscr.vbuffer[ad+x]!=0 )m1=jtmscr.vbuffer[ad+x];
   jtmscr.vbuffer[ad+x]=m1;
  }
  // If color not found then skip to next line
colornotfound:
  asm("nop");
 }
}

/*
 * jtmscr_b800tojtmscrvbuffer
 *
 * Normal ANSI screen located at 0xb8000(VGA videomode 3).
 * will be converted to currently visible graphic window.
 *
 */
void jtmscr_b800tojtmscrvbuffer(void)
{
 long x,y,ad,ad2;
 unsigned char save_screen[(80*50*2)];
 unsigned char *str,m1,m2,m3,m4;
 // Alloc
 str=malloc(256);
 //

// dosgetput(save_screen, 80*2*25, 0xb8000);
 dosmemget(0xb8000, 80*25*2, &save_screen);
 //
 for(y=0; y<25; y++)
 {
  ad=y*80;
  jtmgl_gotoxy(jtmscr.writexs,(y*8)+jtmscr.writeys);
  for(x=0; x<80; x++)
  {
   m1=save_screen[((ad+x)<<1)+0];
   m2=save_screen[((ad+x)<<1)+1];
   if(0==0) //if( (isdigit(m1)!=0) || (isalpha(m1)!=0) )
   {
    switch(jtmscr.bpp)
    {
     case 8:
     jtmgl_textcolor(
                     ((JTM_EGAPALETTE[((m2&15)*3)+0])+
                     (JTM_EGAPALETTE[((m2&15)*3)+1])+
                     (JTM_EGAPALETTE[((m2&15)*3)+2]))/3/4+0x40
                    );
     jtmgl_textbackground(
                          ((JTM_EGAPALETTE[((m2>>4)*3)+0]<<16)+
                          (JTM_EGAPALETTE[((m2>>4)*3)+1]<<8)  +
                          (JTM_EGAPALETTE[((m2>>4)*3)+2]))/3/4+0x40
                         );
     break;
     case 32:
     jtmgl_textcolor(
                     (JTM_EGAPALETTE[((m2&15)*3)+0]<< 16 )  |
                     (JTM_EGAPALETTE[((m2&15)*3)+1]<< 8  )  |
                     (JTM_EGAPALETTE[((m2&15)*3)+2])
                    );
     jtmgl_textbackground(
                          (JTM_EGAPALETTE[((m2>>4)*3)+0]<< 16 )  |
                          (JTM_EGAPALETTE[((m2>>4)*3)+1]<< 8  )  |
                          (JTM_EGAPALETTE[((m2>>4)*3)+2])
                         );
     break;
    }
    sprintf(str,"%c",m1);
    write(str);
   }
   else
   {
    write("?");
   }
  }
 }
 // Free
 free(str);
}

int jtm_dumpfiletowrite(char *fname)
{
 FILE *f;
 char str[100],str2[100];
 //
 f=jtm_fopen(fname,"rb");
 if(f==NULL)return 1;
 //
 sprintf(str,"Decimal dump of file \"%s\":\n",fname);
 write(str);
 //
 do
 {
  if(feof(f))break;
  fscanf(f,"%s",&str);
  sprintf(str2,"%s>",str);
  write(str2);
 }while(0==0);
 jtm_fclose(f);
 write("\n*End of dump*\n");
 return 0;
}

void jtmscr_opendump(void)
{
 jtm_dwindow(0,0,90*8,32*8,
             "dump window",user_window);
}

int jtm_wherex()
{
 //
 if(jtm_vmode!=3)
 {
  return (jtmscr.writex-jtmscr.writexs)/8;
 }
 else
 {
  return wherex()-1;
 }
}

int jtm_wherey()
{
 //
 if(jtm_vmode!=3)
 {
  return (jtmscr.writey-jtmscr.writeys)/8;
 }
 else
 {
  return wherey()-1;
 }
}

void jtm_gotoxy(int x,int y)
{
 if(jtm_vmode!=3)
 {
  jtmscr.writex=jtmscr.writexs+x*8;
  jtmscr.writey=jtmscr.writeys+y*8;
 }
 else
 {
  gotoxy(x+1,y+1);
 }
}

void jtm_manualmousewakeup(void)
{
 jtmgl_arrowsystemon();
 jtmgl_arrowon();
 jtm_keybmouseon();
 jtmmouse.x=0; jtmmouse.y=0;
}

void jtmgl_vgabootup(void)
{
 //

 //
 checkos();
 jtmgl_vgabootup1();
 jtm_keybmouseon();
}

void jtmgl_vgabootup1(void)
{
 //

 //
 jtmscr_manualsetmcga320200b();
 jtmgl_getfontsomewhere();
 jtmgl_setupdrawingcolors();
 jtmscr_screenclipper_on();
}

void jtmgl_setupdrawingcolors(void)
{
 //---- Setup drawing colors ------------------------
 if(jtmscr.bpp==8)
 {
  NewWindow.fontcolor=0xff;
 }
 else
 {
  NewWindow.fontcolor=0xffa010;
 }
 //
 //
 jtmscr.defaultbackgroundcolor=0x1f/4;
 jtmscr.backgroundcolor=0x1f/4;
 jtmscr.headlinebackgroundcolor=0x00; //0x4f/4;

 jtmscr.textbackgroundcolor=0x4f/4;
 NewWindow.fontbcolor=0x4f/4;
 jtmscr.fontcolor=0xff;
 //--------------------------------------------------
}

void jtmscr_clrwin()
{
 //
 jtmgl_dbox(jtmscr.writexs,jtmscr.writeys,jtmscr.writexm,jtmscr.writeym,jtmscr.fontbcolor,0);
}


void jtm_clrscr(void)
{
 if(jtm_vmode==3)
 {
  clrscr();
 }
 else
 {
  if(jtmscr.glsystem)
  {
   jtmscr_clrwin();
   jtm_gotoxy(0,0);
  }
 }
}
void jtmscr_clrscr(void){jtm_clrscr();}

void jtmgl_manualbootup(void)
{
// jtmgl_vgabootup();
// jtm_manualmousewakeup();
}

int jtm_setpalettefrompcx(char *_filename)
{
 unsigned char m1,m2,m3;
 FILE *_fp;
 unsigned _c;
 //
 _fp=jtm_fopen(_filename,"rb");
 if(_fp==NULL)return(1);
 fseek(_fp,-768,SEEK_END); // Seek to end of PCX file
 for(_c=0; _c<256; _c++)
 {
  m1=fgetc(_fp)/4;
  m2=fgetc(_fp)/4;
  m3=fgetc(_fp)/4;
  jtm_setpalette(_c,m1,m2,m3);
 }
 jtm_fclose(_fp);
 return 0;
}

int jtm_setpalettefrombin(char *_filename)
{
 FILE *_fp;
 unsigned char m1,m2,m3;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 unsigned _c;
 _fp=jtm_fopen(_filename,"rb");
 if(_fp==NULL)return(1);
 for(_c=0; _c<0x100; _c++)
 {
  m1=fgetc(_fp);
  m2=fgetc(_fp);
  m3=fgetc(_fp);
  jtm_setpalette(_c,m1,m1,m1);
 }
 jtm_fclose(_fp);
 return 0;
}

int jtmgl_waitforaction(void)
{
 //
 char m1,m2,m3,m4;

 //
 do
 {
  jtmscr_updateVRAM();
  m1=jgetchim();
  if(m1==27)break;
 }while(0==0);
}

void jtmgl_setspecifiedpalette(int type__)
{
 switch(type__)
 {
   // Dark
   case 0:
   jtm_greyscale256();
   break;
   case 1:
   jtm_redscale256();
   break;
   case 2:
   jtm_greenscale256();
   break;
   case 3:
   jtm_bluescale256();
   break;
   // Light
   case 4:
   jtm_greyscale256();
   break;
   case 5:
   jtm_lredscale256();
   break;
   case 6:
   jtm_lgreenscale256();
   break;
   case 7:
   jtm_lbluescale256();
   break;
 }
}

jtmscr_action()
{
   jtmscr_updateVRAM();
   jtmscr_wipesrtab();
}

jtmgl_orderpalette()
{
 //
#ifdef LOWRES
 jtmscr_manualsetmcga();
#else
 jtmscr_manualsethigh();
#endif
 switch(jtmscr.psetup)
 {
  case 0: // Multi palettte (4 scales)
  jtm_multip256();
  break;
  case 1: // Greyscale (256c GS)
  jtm_greyscale256();
  break;
  case 2: //       Redscale
  jtm_redscale256();
  break;
  case 3: // Light Redscale
  jtm_lredscale256();
  break;
  case 4: //       Greenscale
  jtm_greenscale256();
  break;
  case 5: // Light Greenscale
  jtm_lgreenscale256();
  break;
  case 6: //       Bluescale
  jtm_greyscale256();
  break;
  case 7: // Light Bluescale
  jtm_greyscale256();
  break;
  default:
  break;
 }
}

jtmgl_opengas()
{
 //
 jtmscr.gas.state=1;

 // Tilaa paletti
 jtmgl_orderpalette();

 //
 jtmscr.glsystem=1;

 //
 jtmgl_arrowsystemon();
 jtm_keybmouseon();

 //
#ifndef dontslctbg
// jtmscr_selectbg("c:/jtm/bin/image/gs.raw");
#endif

 //
 jtmscr.cursor.state=1;
}

jtmgl_closegas()
{
 //
 jtmscr.gas.shutdown=1;
 jtm_keybmouseoff();
 jtmgl_arrowsystemoff();
 jtmscr.gas.state=0;

 //
 jtmvga_settextpage(1);
 jtmvga_setactivedisplaypage(0);
 jtm_window(1,1,80,25);
 jtm_textcolor(7); jtm_textbackground(0);
 jtm_clrscr();
 jtm_setvmode(3);
}

jtm_homecursor()
{
 jtmgl_homecursor();
}

jtmgl_homecursor()
{
 jtmscr.writex=jtmscr.writexs;
 jtmscr.writey=jtmscr.writeys;
}

jtm_textbackground(int c)
{
 if(jtm_vmode==3)
 {
  textbackground(c);
  return;
 }
 else
 {
  jtmscr.textbackgroundcolor=c;
 }
 return;
}

jtm_textcolor(int color)
{
 jtmgl_textcolor(color);
}

void jtmgl_window(int x1,int y1,int x2,int y2)
{
 jtmscr.writexs=x1; jtmscr.writeys=y1;
 jtmscr.writexm=x2; jtmscr.writeym=y2;
}

void jtm_window(int x1,int y1,int x2,int y2)
{
 if(jtm_vmode==3)
 {
  window(x1+1,y1+1,x2+1,y2+1);
 }
 else
 {
  jtmgl_window(x1*8,y1*8,x2*8,y2*8);
 }
}

jtmvga_restorefont()
{
 __dpmi_regs regs;
 regs.x.ax=0x1129;
 regs.x.bx=0x0000;
 __dpmi_int(0x10,&regs);
}

jtmvga_settextpage(int page)
{
 __dpmi_regs regs;
 regs.x.ax=0x0200;
 regs.x.bx=(page<<8)&0xff00;
 regs.x.dx=0x0000;
 __dpmi_int(0x10,&regs);
}

jtmvga_setactivedisplaypage(int page)
{
 __dpmi_regs regs;
 regs.x.ax=0x0500|(page&255);
 __dpmi_int(0x10,&regs);
}

void jtmgl_allspritesoff(void)
{
 jtmgl_arrowsystemoff();
 jtm_setcursortype(_NOCURSOR);
}

void jtmgl_centerarrow(void)
{
 jtmmouse.x=jtmscr.width>>1;
 jtmmouse.y=jtmscr.height>>1;
}

/**** JTMDJGFX DIALOG FUNCTION ****/
// This function is a copy of JTMANSI function jtm_dialog,
// well, as this is the newer version and the graphical version,
// this is also abit improved.
//
jtmgl_dialog_scrollup(int *Cactive,int *entriesperscreen,int *totalentries,int *scrollposition)
{
    //
    if(Cactive[0])
    {
     Cactive[0]--;
    }
    else
    {
     if(scrollposition[0])
     {
      scrollposition[0]--;
     }
    }
}

//
jtmgl_dialog_scrolldown(int *Cactive,int *entriesperscreen,int *totalentries,int *scrollposition)
{
    //
    if(!Cactive[0] || (Cactive[0]<(entriesperscreen[0]-1) && Cactive[0]<(totalentries[0]-1)))
    {
     Cactive[0]++;
    }
    else
    {
     if( !(Cactive[0]+scrollposition[0]>=(entriesperscreen[0]-1)) )
     {
      if( Cactive[0]<(entriesperscreen[0]-1) && Cactive[0]<(totalentries[0]-1) )
      {
       Cactive[0]++;
      }
     }
     else
     {
      if( scrollposition[0]<(totalentries[0]-entriesperscreen[0]) )
      {
       scrollposition[0]++;
      }
     }
    }
}


//
// The main function of the graphical dialog function.
//
long jtmgl_dialog(const char *texts[],const long active,const char *headline)
{
 //
 long x,y,i,i2,i3,i4,ReturnValue;
 int key,Cactive,scrollposition,totalentries,entriesperscreen,tmp,tmp2;
 char str[255],str2[255],str3[255],str4[255];

 //
 jtmgl_allspritesoff();

 //
 write_dontupdate();

 //
 Cactive=active;
 scrollposition=0;
 entriesperscreen=21;

 //
 tmp=jtmkeyb.specialkeyconvert2asc;
 tmp2=jtmscr.wfb;
 jtmscr.wfb=1;
 jtmkeyb.specialkeyconvert2asc=1;

 //
 for(i=0,i2=0; ; i++)
 {
  if(!strcmp(texts[i],"*"))
  {
   totalentries=i;
   break;
  }
 }

 //
 jtmscr.writexs=8; jtmscr.writeys=8;
 jtmscr.writexm=320; jtmscr.writeym=200;

 //
 if(totalentries<entriesperscreen)
 {
  y=totalentries*8+8*4;
 }
 else
 {
  y=entriesperscreen*8+8*4;
 }

 // Clear entire screen area
 jtm_dbox(0,0,jtmscr.width-1,jtmscr.height-1, BLACK);

 // Fill dialog area
 jtm_dbox(0,0,319,y-1, BLUE);
 jtm_dbox(1,1,318,y-2, LIGHTBLUE);

 // Headline box
 jtm_dbox(4,4,319-4,(3*8)-5,0);
 jtm_dbox(5,5,319-5,(3*8)-6,8);

 // Write headline text
 jtm_textcolor(YELLOW); jtm_textbackground(8);
 strcpy(str,headline);
 jtm_stretchstringlength(str,38);
 jtm_gotoxy(0,0);
 write(str);

 //
 jtmscr.writexs=8; jtmscr.writeys=8+8*2;
 jtm_gotoxy(jtmscr.writexs,jtmscr.writeys);

 // Fill entries area border
 jtm_dbox(jtmscr.writexs-1,jtmscr.writeys-1,319-7,y-8*1, BLACK);

 //
 while(!0)
 {
  //
  jtm_gotoxy(0,0);

  //
  for(y=0; y<entriesperscreen; y++)
  {
   if((y+scrollposition)<totalentries)
   {
    if(y)write("\n");
    strcpy(str,texts[y+scrollposition]);
    jtm_stretchstringlength(str,38);
    if((y+scrollposition)==(Cactive+scrollposition))
    {
     jtm_textbackground(CYAN);
     if( (systemtimer.counterx&16) )
     {
      jtm_textcolor(BLUE);
     }
     else
     {
      jtm_textcolor(15);
     }
    }
    else
    {
     jtm_textcolor(CYAN);
     jtm_textbackground(1);
    }
    write(str);
   }
  }

  // Refresh display
  jtmgl_tosrtab(0,jtmscr.height);
  jtm_waitsd();
  jtmscr_updateVRAM1();

  //
  key=jgetchim();

  //
  if(key=='q')goto crup;
  if(key=='a')goto crdown;

  // Special key?
  if(key==-2)
  {
   // Get special key!
   jtm_getbuffer(jgetchim,str,'#',250);

   //
   if(!strcmp(str,"[crup]"))
   {
crup:
    jtmgl_dialog_scrollup(&Cactive,&entriesperscreen,&totalentries,&scrollposition);
   }

   //
   if(!strcmp(str,"[crdown]"))
   {
crdown:
    //
    jtmgl_dialog_scrolldown(&Cactive,&entriesperscreen,&totalentries,&scrollposition);
   }

   //
   if(!strcmp(str,"[pgup]"))
   {
    for(i=0; i<entriesperscreen; i++)
    {
     jtmgl_dialog_scrollup(&Cactive,&entriesperscreen,&totalentries,&scrollposition);
    }
   }

   //
   if(!strcmp(str,"[pgdown]"))
   {
    //
    for(i=0; i<entriesperscreen; i++)
    {
     jtmgl_dialog_scrolldown(&Cactive,&entriesperscreen,&totalentries,&scrollposition);
    }
   }

   //
//   write(str);
  }
  else
  {
//   write("                              ");
  }

  //
  if(key==27)
  {
   ReturnValue=-1;
   break;
  }
  //
  if(key==13)
  {
   ReturnValue=Cactive+scrollposition;
   break;
  }
 }

 // After dialog function has completed its action it will
 // try to restore graphic environment as it was before.

 //
 write_allwaysupdate();

 //
 jtmscr.writexs=0; jtmscr.writeys=0;
 jtmscr.writexm=320; jtmscr.writeym=200;

 //
 jtmkeyb.specialkeyconvert2asc=tmp;
 jtmscr.wfb=tmp2;

 //
 jtm_gotoxy(0,0);
 jtm_dbox(jtmscr.writexs,jtmscr.writeys,jtmscr.writexm,jtmscr.writeym,0);
 jtm_textcolor(7); jtm_textbackground(BLACK);

 //
 return ReturnValue;
}
/*** END OF JTMDJGFX DIALOG FUNCTION ***/

//
jtm_randomstuff()
{
 //
 long *tmp,i;

 //
 tmp=calloc(1,64000);

 //
 for(i=0; i<16000; i++)tmp[i]=rand()&0xffffffff;

 //
 jtm_vf(tmp);

 //
 cfree(tmp);
}
// JTM_WAVEDRAW16 function
// -----------------------
//
// This function displays audio data in graphical form.
// Warning: Graphical display must be opended first or function may not work.
//
jtm_wavedraw16(int *s,int *s2,long pit)
{
 //
 char m1,m2,m3,m4;
 int i,i2,i3,i4,c1,c2;
 unsigned long ad,ad2,ad3,ad4,x,y,ker=1;

 //
// c1=jtm_findblackfromcurrentpalette();
// c2=jtm_findwhitefromcurrentpalette();

 do
 {
  //
  jtmscr_clearscreen();

  //
  for(x=0; x<jtmscr.width; x++)
  {
   ad=x<<ker;
   if(ad<pit)
   {
    ad2=(s[ad]>>9)+1;
    for(y=0; y<(ad2&255); y++)jtmscr_putpixel(x,y,0xff);
   }
  }

  //
  jtm_waitsd();
  jtmgl_tosrtab(0,jtmscr.height); jtmscr_updateVRAM();

  //
  m1=jgetchim();
  if(m1==27)break;
  if(m1=='1')
  {
   ker++;
  }
  if(m1=='2')
  {
   ker--;
  }
 }while(0==0);
}

void jtm_vfr(char *__videoframe)
{
 if(jtmscr.glsystem==0)
 {
  jtm_vf(__videoframe);
 }
 else
 {
  if(jtmscr.srsignallock==0)jtmscr.srsignal=0xff;
  jtm_bwvgatowindow(__videoframe);
 }
}
#endif
/** END OF JTMDJGFX.H **/
