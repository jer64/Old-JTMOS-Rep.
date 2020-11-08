/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTM-OS -- Image progress library, last updated 10/97.    (For Borland C/C++ V5.0)
 *                                              (JTM-OS DOS graphics
 *                                               library, secondry priority.)
 *
 * -Features f.e. stanard MCGA256/16 color palette functions,
 *  and functions similiar to jxwde.h(Primary lib.).
 *
 * (C)1997 by Jari Tuominen, all rights reserved.
 * (C)Mango Productions Scandinavia.
 *
 *                JTM_GREYSCALE256
 *                JTM_BLUESCALE256
 *                JTM_SETVMODE          (Sets atleast VGA modes)
 *                JTM_WAITSD            (Works perfectely with MCGA256!)
 */
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
// For Borland C/C++
#ifdef __BORLANDC__
#pragma inline
#define WORD unsigned
#define BYTE char
#endif

#define _mcga_seg 0xa000
//
void jtm_setscreenstart(unsigned _location);
void jtm_fillbuffer(char *buffer,char _byte);
void jtm_waitsd();
void jtm_videoframe(char far *_pic);
void jtm_bluescale256(); // For MCGA 320x200/256(0x13) mode only !
void jtm_greyscale256(); // For MCGA 320x200/256(0x13) mode only !
void jtm_setvmode(unsigned char _vmode);
unsigned jtm_screenstart=0;
 /* Currect video mode */
 long jtm_pcximagelimit=64000; // 64KB

 /*
  * JICON image format(V26):
  *
  * string text     ManuFact       ( Stanard = 'JICON' )
  * string unsigned Version        ( Stanard version = 26 (V26) )
  * string unsigned codingtype     ( 0=UnCompressed(=STANDARD) )
  * string unsigned x-min    ==\      
  * string unsigned y-min      |   
  * string unsigned x-max      |   
  * string unsigned y-max      \==> For example: 0,0,320,200
  *
  *
  *
  */


int decode_pcx(char *_srcfname,char *_outfname,unsigned char _paletteadd)
		// Last parameter = Add palette (0=False, 1=True)
{

 unsigned long _ad,_ad2,_ad3,_ad4,_ad5;
 register unsigned char _m1,_m2;
 unsigned long _x,_y,_x2,_y2;
 int xlen;
 int ylen;
 FILE *_fp,*_fp2;
 //
 _fp= fopen(_srcfname,"rb");
 _fp2=fopen(_outfname,"wb");

 // Get image size
 fseek(_fp,4+4,SEEK_SET);
 xlen=(fgetc(_fp)|fgetc(_fp)<<8)+1;
 ylen=(fgetc(_fp)|fgetc(_fp)<<8)+1;

 // Seek to end of header, encoded image data begin.
 fseek(_fp,pcx_header,SEEK_SET);
 _x=0;
 while( !feof(_fp) )
 {
  if(_x>(xlen*ylen))break;
  _m1=fgetc(_fp);
  if((_m1&0xc0)==0xc0)
  {
   _m2=fgetc(_fp);
   //Duplicate character
   for(_x2=0; _x2<(_m1&0x3f); _x2++)
   {
    fputc(_m2,_fp2);
    _x++;
   }   
  }
  else
  {
   fputc(_m1,_fp2);
   _x++;
  }
 }

 if(_paletteadd==1) // Add palette if it's expected.
 {
  fseek(_fp2,-768,SEEK_CUR);
  fseek(_fp,-768,SEEK_END);
  for(_ad=0; _ad<768; _ad++)
  {
   _m1=fgetc(_fp);
   fputc(_m1,_fp2);
  }
 }
 fclose(_fp2);
 fclose(_fp);
 return 0;
}

int decode_pcxto(unsigned char *_srcfname,char far *_buffer,unsigned char _paletteadd)
                // Last parameter = Add palette (0=False, 1=True)
{
 /*
  * So the final image format will be :
  *
  *(Offset)
  * 00     char     ManuFact;      ( 10=Manufact/ZsoftPCX )
  * 01     char     Version;       ( 5 )
  * 02     char     codingtype;    ( 1=PCX run lenght encoding )
  * 03     char     bitsperpixel;  ( Plane(s): 1,2,4,8,24)
  *
  * 04     short x-min;  // ==\      
  * 06     short y-min;  //   |   (Usually just unsigned values)
  * 08     short x-max;  //   |   
  * 0a     short y-max;  //   \==> For example: 0,0,320,200
  * 0c     short HDPI;
  * 0e     short VDPI;
  * 10     char palette[3*16]; // 16 color palette (Kinda old feature..)
  * 40     char reserved;
  * 41     char planes; // (1=256c,4=16c)
  *
  * 42     short bytespline; // oltava parillinen !
  * 44     short paltyp;        (1=color, 2=bw)
  * 46     short hscr,vscr;  // Kuvankoko (vain P.Brush IV+..)
  * 48     char free[54];
  * 80     char encoded_image[]=......
  */

 unsigned long _ad,_ad2,_ad3,_ad4,_ad5;
 register unsigned char _m1,_m2;
 unsigned long _x,_y,_y2;
 register unsigned int _x2;
 int xlen;
 int ylen;
 FILE *_fp,*_fp2;
 //
 jtm_fillbuffer(_buffer,0x00);
 _fp= fopen(_srcfname,"rb");
 fseek(_fp,8,SEEK_SET);

 // Get image size
 fseek(_fp,4+4,SEEK_SET);
 xlen=(fgetc(_fp)|fgetc(_fp)<<8)+1;
 ylen=(fgetc(_fp)|fgetc(_fp)<<8)+1;

/* textmode(C80);
 cprintf("Image %s, %ux%u\n\r",_srcfname,xlen,ylen);
 exit(0);*/
                                                  
 fseek(_fp,pcx_header,SEEK_SET);
 _x=0;
 //
 while(_x<0xffff)
 {
  _m1=fgetc(_fp);
  if((_m1&0xc0)==0xc0)
  {
   _m2=fgetc(_fp);
   //Duplicate character
   for(_x2=0; _x2<(_m1&0x3f); _x2++)
   {
    _buffer[_x]=_m2;
    _x++;
   }   
  }
  else
  {
   _buffer[_x]=_m1;
   _x++;
  }
 }
/* if(_x<sizeof(_buffer))
 {
  while(_x<sizeof(_buffer))
  {
   _buffer[_x]=0;
   _x++;
  }
 }*/
// if(_paletteadd==1) // Add palette if it's expected.
// {
//  fseek(_fp,-768,SEEK_END);
//  for(_ad=0; _ad<768; _ad++)
//  {
//   _m1=fgetc(_fp);
//   _buffer[_x+_ad]=_m1;
//  }
// } 
 fclose(_fp);
 return 0;
}

/** Fantti-painter v1 image file format **/

 char *fbi_header;
 int   fbi_xpit,fbi_ypit;
 long  fbi_colours;
 float fbi_image_version,fbi_painter_version;
/** ++ Binary ... */

view_fbi(char *_fname)
{

 /** View FANTTI-PAINTER's image **

  Fantti format:

  3 bytes       "FAN" 
  word          image xpit    
  word          image ypit
  dword         Amount of colours used (2,16,256,16m)
  word          version of image file (f.e. 0x0100)
  word          version of the program used to create this image
  char *        Bitmap data

  **                             **/
 FILE *_fp;
 _fp=fopen(_fname,"rb");
 fscanf(_fp,"%s",&fbi_header);
 fscanf(_fp,"%d",&fbi_xpit);
 fscanf(_fp,"%d",&fbi_ypit);
 fscanf(_fp,"%l",&fbi_colours);
 fscanf(_fp,"%f",&fbi_image_version);
 fscanf(_fp,"%f",&fbi_painter_version);
 fclose(_fp);
 // Write here the rest of code:
 return 0;
}

void jtm_greyscale256() // For MCGA 320x200/256(0x13) mode only !
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 if(jtm_vmode==0x13) 
 {
  for(_color=0,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
  {
   red=fred;
   green=fgreen;
   blue=fblue;
   setpalette(_color,red,green,blue);
  }
 }
}

void jtm_bluescale256() // For MCGA 320x200/256(0x13) mode only !
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 if(jtm_vmode==0x13) 
 {
  for(_color=0,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
  {
   red=0;
   green=0;
   blue=fblue;
   setpalette(_color,red,green,blue);
  }
 }
}

void jtm_greyscale16() // Recommended for all 4 plane modes
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned _color;
 //
 if(jtm_vmode!=0x13) 
 {
  for(_color=0,fred=0,fgreen=0,fblue=0; _color<16; _color++,fred+=(1/16),fgreen+=(1/16),fblue+=(1/16))
  {
   red=fred;
   green=fgreen;
   blue=fblue;
   setpalette(_color,red,green,blue);
  }
 }
}

void jtm_waitsd()
{
 asm{
  mov dx,0x3da
 }
loop1:
 asm{
  in al,dx
  test al,8
  jz loop1
 }
/*loop2:
 asm
 {
  in al,dx
  test al,8
  jnz loop2
 }*/
}

void jtm_setvmode(unsigned char _vmode)
{
 if(jtm_vmode==_vmode)goto alreadyset;
 jtm_vmode=_vmode;
 asm{
  mov al,byte ptr _vmode
  mov ah,0
  int 0x10
 }
alreadyset:
}

void jtm_videoframe(char far *_pic)
{
 register unsigned int uad;
 register unsigned char m1,m2,m3,m4;
 //
 jtm_waitsd();
 for(uad=0; uad<0xfffc; uad+=4)
 {
  m1=_pic[uad+0x00];
  m2=_pic[uad+0x01];
  m3=_pic[uad+0x02];
  m4=_pic[uad+0x03];
  asm{
   push es
   mov ax,0xa000
   mov es,ax
   mov di,word ptr uad
   mov al,byte ptr m1
   mov ah,byte ptr m2
   mov bl,byte ptr m3
   mov bh,byte ptr m4
   mov word ptr [es:di+0x00],ax
   mov word ptr [es:di+0x02],bx
   pop es
  }
 }
}

void jtm_setscreenstart(unsigned _location)
{
 asm{
  PUSH AX
  PUSH DX
  MOV  BX,_location
  MOV  DX,3D4H
  MOV  AL,0CH
  MOV  AH,BH
  OUT  DX,AX
  INC  AL
  MOV  AH,BL
  OUT  DX,AX
  POP  DX
  POP  AX
 }
}

void jtm_vgaoutput(unsigned char _reg,unsigned char _data)
{
 asm{
  mov al,byte ptr _reg
  mov dx,0x3d4
  out dx,al
  inc dx
  mov al,byte ptr _data
  out dx,al
 }
}

void jtm_setmaxscanlines(unsigned char _maxs)
{
 jtm_vgaoutput(0x9,_maxs);
}

void jtm_fillbuffer(char *buffer,char _byte)
{
 long _ad;
 for(_ad=0; _ad<sizeof(buffer); _ad++)buffer[_ad]=_byte;
}


void jtm_palettereset(float clevel)
{
 unsigned uad,uad2,uad3,uad4; float fa,fb,fc,fd;
 fc=1/clevel;
 for(uad=0,fa=0; uad<0xff; uad++,fa=fa+fc)
 {
  uad2=fa;
  setpalette(uad,uad2>>2,uad2>>4,uad2>>4);
 }
}

void jtm_negativeimage(char far *image1)
{
 register unsigned uad,uad2;
 for(uad=0; uad<0xffff; uad++)
 {
  uad2=image1[uad];
  asm{
   mov ax,word ptr uad2
   xor ax,0xff
   mov word ptr uad2,ax
  }
  image1[uad]=uad2;
 }
}

void jtm_smoothimage(char far *_image,unsigned times)
{
 unsigned c1,c2,c3,c4,uad,uad2;
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

void jtm_morphimage(char far *image1,char far *image2)
{
 register unsigned uad,uad2,uad3,uad4;
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
