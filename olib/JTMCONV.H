/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
#ifndef __JTMCONV_H__
#define __JTMCONV_H__

#include <jtmglvar.h>

#include <jtmvga.h>
#ifndef CMDBYTE
#define CMDBYTE 0x54
#endif
#include <jtmc64.h>
/*
 *
 *  "JTMOS": Portable C functions library v1.00
 *  (C)1998-99 by Jari Tapio Tuominen, all rights reserved.
 *
 * This library is meant to work indepent, no other libraries are required.
 * This library is also used when converting JTMOS programs to work outside
 * DJGPP/DOS environment, which is very neccesery process when creating
 * compatible programs which'll be spreaded widely and should be very
 * portable.
 *
*/


#ifndef DJGPP
unsigned char c64pal[]={    0,0,0,
                            255,255,255,
                            184,0,20,
                            26,255,232,
                            148,0,152,
                            36,204,0,
                            4,16,176,
                            255,255,128,
                            152,103,1,
                            80,32,20,
                            255,64,107,
                            72,72,72,
                            112,112,112,
                            118,253,111,
                            84,112,232,
                            174,174,174
                          };
#endif

#ifdef JTMPORT2UNIX
setpalette(){} // To prove non functioning, but fine compiling result.
#endif

#ifndef DJGPP
#ifndef getch
#define getch getchar
#endif
#endif

void jtmgl_blurbuffer1(unsigned char *vbuffer,long width,long height);
void jtmgl_blurbuffer(unsigned char *vbuffer,long width,long height);
#include <jtmenv.h>

//
#include <jtmnoise.h>

//
//
jtmscr_putpixel(int x,int y,int color)
{
 //
 long ad,ad2,ad3,ad4;

 //
 if(x<jtmscr.width && y<jtmscr.height)
 {
  ad=(jtmscr.width*y)+x;
  jtmscr.vbuffer[ad]=color;
 }
}

void fputw(unsigned _wrd,FILE *_fp)
{
 fputc(_wrd&0xff,_fp);
 fputc((_wrd>>8)&0xff,_fp);
}

unsigned fgetw(FILE *_fp)
{
 unsigned _wrd,a1,a2;
 //
 if(feof(_fp))return 0;
 a1=fgetc(_fp);
 if(feof(_fp))return 0;
 a2=fgetc(_fp);
 if(feof(_fp))return 0;
 _wrd=a1;
 _wrd=_wrd|(a2<<8);
 return _wrd;
}

void fputd(long _dwrd,FILE *f)
{
 fputw( ((_dwrd)&0xffff),    f );
 fputw( ((_dwrd>>16)&0xffff),f );
}

long fgetd(FILE *f)
{
 return ( fgetw(f)+(fgetw(f)<<16) );
}

void jtm_hiresplot(unsigned char *frame,int x,int y,int c)
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

void jtm_hirescut(unsigned char *frame,int x,int y,int c)
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

int jtm_hiresread(unsigned char *frame,int x,int y)
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

void jtm_multiplot(unsigned char *frame,int _x,int _y,int _c)
{
 _x=_x&0xffe; // Graphic screen at 160x200+4c, or 2x1 pixels 320x200 display.
 switch(_c)
 {
  case 0:
  jtm_hirescut( frame,_x+0,_y,1);
  jtm_hirescut( frame,_x+1,_y,1);
  break;
  case 1:
  jtm_hirescut( frame,_x+0,_y,1);
  jtm_hiresplot(frame,_x+1,_y,1);
  break;
  case 2:
  jtm_hiresplot(frame,_x+0,_y,1);
  jtm_hirescut( frame,_x+1,_y,1);
  break;
  case 3:
  jtm_hiresplot(frame,_x,  _y,1);
  jtm_hiresplot(frame,_x+1,_y,1);
  break;
 }
}

void jtm_multi8plot(unsigned char *frame,int _x,int _y,int _c)
{
 long a1,a2,a3,a4;
 //
/* _c=_c&7;
 a1=(_c+0)/2;
 a2=(_c+1)/2;
 if( a1==3 )
 {
  a1=3;
  a2=3;
 }
 switch( (y&1) )
 {
  //
  case 0:
  if( (x&2)!=0 )
  {
   jtm_multiplot(frame,_x,_y,a1);
  }
  else
  {
   jtm_multiplot(frame,_x,_y,a2);
  }
  break;
  //
  case 1:
  if( (x&2)!=0 )
  {
   jtm_multiplot(frame,_x,_y,a2);
  }
  else
  {
   jtm_multiplot(frame,_x,_y,a1);
  }
  break;
 }*/

 _c=_c&7;
 a1=(_c+0)/2;
 a2=(_c+1)/2;
 if( a1==3 )
 {
  a1=3;
  a2=3;
 }
 switch( (((_x/2)+(_y/2))&1) )
 {
  //
  case 0:
  jtm_multiplot(frame,_x,_y,a1);
  break;
  //
  case 1:
  jtm_multiplot(frame,_x,_y,a2);
  break;
 }

}

void jtm_allocnewcolor(unsigned char colori,unsigned char index,long addy,unsigned char *a0400,unsigned char *ad800)
{
 long a1,a2,a3,a4,a5,a6,a7,a8;
 //
 a1=addy;
 //
 switch(index)
 {
  case 1:
  a0400[a1]=(a0400[a1]&0x0f)|(colori<<4);
  break;

  case 2:
  a0400[a1]=(a0400[a1]&0xf0)|(colori&0x0f);
  break;

  case 3:
  ad800[a1]=colori&0x0f;
  break;
 }
}

// JTM_CALRGBDIST function
// -----------------------
//
// This function calculates difference between two
// RGB colors, and returns the difference value.
//
//
//
long jtm_calrgbdist(int r1,int g1,int b1,
                    int r2,int g2,int b2)
{
 //
 long t1,t2,tt;

 //
 t1=r1+g1+b1;
 t2=r2+g2+b2;
 if(t1>t2)
 {
  tt=t1-t2;
 }
 else
 {
  tt=t2-t1;
 }
 return tt;
}

int jtm_multiplot16(unsigned char *frame,
                    unsigned char *a0400,
                    unsigned char *ad800,
                    unsigned char *atab,
                    int _x,int _y,int _c,int _bc)
                    // Requires pixel color and background color.
                    // Notice that background color is all the time
                    // same for one single image, atleast if not
                    // horizontal rasters are used.
{
 //
 long a1,a2,a3,a4,a5,a6,a7,a8,re,ad,ad2,ad3,ad4,d1,d2,d3,d4,d5,d6,d7,d8;

 //
 re=0;
 //
 _c=_c&0x0f;
 //
 a1=(_y/8)*40+(_x/8);
 //
repeatcolorcheck:
 if( _c==_bc) // Background color
 {
  jtm_multiplot(frame,_x,_y,0);
  goto blackcase;
 }
 if( (a0400[a1]>>4)==_c ) // Color 1  0xf?
 {
  jtm_multiplot(frame,_x,_y,1);
  goto blackcase;
 }
 if( (a0400[a1]&0x0f)==_c ) // Color 2       0x?f
 {
  jtm_multiplot(frame,_x,_y,2);
  goto blackcase;
 }
 if( (ad800[a1]&0x0f)==_c ) // Color 3       0x!?
 {
  jtm_multiplot(frame,_x,_y,3);  // f=used, ?=unused and !=Never used.
  goto blackcase;
 }
/*
 // IF FIXING NOT USED
 if(atab[a1]>=3)
 {
  re=1;
  // Color mistake.
  // Return value one means that no color places available
  goto blackcase; // No color available, this sucks }8-)
 }
*/
 // IF COLOR NOT FOUND THEN FIXING NEEDED!
 // -Get currently requested color
 // -Find nearest color from current color block or from background color
 // > Rule = Brightness
 if(atab[a1]>=3)
 {
  // Get the colors available currently (three colors in block, one background)
  ad=_bc;
  ad2=(a0400[a1]>>4);
  ad3=(a0400[a1]&0x0f);
  ad4=(ad800[a1]&0x0f);

  // Calculate distances(ad,c | ad2,c | ad3,c | ad4,c)
  d1=jtm_calrgbdist(c64pal[ad*3+0],c64pal[ad*3+0],c64pal[ad*3+0],
                    c64pal[_c*3+0],c64pal[_c*3+0],c64pal[_c*3+0]);
  d2=jtm_calrgbdist(c64pal[ad2*3+0],c64pal[ad2*3+0],c64pal[ad2*3+0],
                    c64pal[_c*3+0],c64pal[_c*3+0],c64pal[_c*3+0]);
  d3=jtm_calrgbdist(c64pal[ad3*3+0],c64pal[ad3*3+0],c64pal[ad3*3+0],
                    c64pal[_c*3+0],c64pal[_c*3+0],c64pal[_c*3+0]);
  d4=jtm_calrgbdist(c64pal[ad4*3+0],c64pal[ad4*3+0],c64pal[ad4*3+0],
                    c64pal[_c*3+0],c64pal[_c*3+0],c64pal[_c*3+0]);

  // Distance to something unbelieve
  d5=1000000;

  // Which one is the best choice ?
  if( d1<d5 )
  {
   d6=0;
   d5=d1;
  }
  if( d2<d5 )
  {
   d6=1;
   d5=d2;
  }
  if( d3<d5 )
  {
   d6=2;
   d5=d3;
  }
  if( d4<d5 )
  {
   d6=3;
   d5=d4;
  }

  // After all, now we got the BM plotting color(0-3)
  jtm_multiplot(frame,_x,_y,d6);
  goto blackcase;
 }

 //
 atab[a1]++;
 jtm_allocnewcolor(_c,atab[a1],a1,a0400,ad800);
 goto repeatcolorcheck;
 //
blackcase:
 return re;
}

// MultiColorBitmap Plotter Routine with enchanced features(Multi d021's).
int jtm_multiplot16mb(unsigned char *frame,
                      unsigned char *a0400,
                      unsigned char *ad800,
                      unsigned char *atab,
                      int x,int y,int c,
                      unsigned char *mbctab)
{
 //
 long ad,ad2,ad3,ad4;

 //
 jtm_multiplot16mb(frame,a0400,ad800,atab,x,y,c,mbctab[y]);
}

// Create MBC for 320x200 IBMP/MBC.(Uses 320x200x16C 8bit source picture image)
int jtm_creatembc(unsigned char *frame,unsigned char *mbctab)
{
 //
 long ad,ad2,ad3,ad4,x,y;
 unsigned used[20];


 //
 for(y=0; y<200; y++)
 {
  //
  for(ad=0; ad<16; ad++)used[ad]=0;

  //
  ad=y*320;
  for(x=0,ad2=0; x<320; x++)
  {
   used[frame[ad+x]&15]++;
  }
  for(ad=0,ad2=0,ad3=0; ad<16; ad++)
  {
   if(used[ad]>ad2)
   {
    ad3=ad;
    ad2=used[ad];
   }
  }
  mbctab[y]=ad3;
 }
}

int jtm_multiread(unsigned char *frame,int _x,int _y)
{
 unsigned int c1,c2;
 _x=_x&0xffe; // Graphic screen at 160x200+4c, or 2x1 pixels 320x200 display.
 c1=jtm_hiresread(frame,_x+1,_y);
 c2=jtm_hiresread(frame,_x+0,_y);
 return (c2<<1|c1);
}

//320x200 8bit 4c ==> 160x200 4c
unsigned char *jtm_VGAVIC(unsigned char *bitmap256)
{
 unsigned char frame[0xffff];
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 //
 for(ad=0; ad<0xffff; ad++)frame[ad]=0;
 for(y=0; y<200; y++)
 {
  ad= y*320;
  for(x=0; x<320; x+=2)
  {
   jtm_multiplot(frame,x,y,bitmap256[ad+x]);
  }
 }
 return frame;
}

long jtm_loadcmp(unsigned char *fname,unsigned char *buf)
{
 long a1,a2,a3,a4,a5,a6,a7,a8;
 FILE *f;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 a1=0;
 f=fopen(fname,"rb");
 fgetw(f);
 do
 {
  m1=fgetc(f);
  if(m1==CMDBYTE)
  {
   m1=fgetc(f);
   a1+=m1;
  }
  else
  {
   buf[a1]=m1;
   a1++;
  }
 }while(!feof(f));
 fclose(f);
 return a1;
}

// Creates string filled with one single mark
void jtm_fillstring(char *stringi,char merkki,long amount)
{
 long a1,a2,a3,a4;
 //
 for(a1=0; a1<amount; a1++)
 {
  stringi[a1]=merkki;
 }
 stringi[amount]=0;
}

// Writes string inside string2 in place specified by _place,
// function stops writing when write pointer exceeds size of
// destination string.
// Notice: Function won't enter end of string mark, so the destination
//         string must be prepared to the operation.
void jtm_writestringinsidestring(char *string,char *string2,long _place)
{
 long a1,a2,a3,a4;
 //
 for(a1=_place,a2=0; a1<strlen(string2)+_place; a1++,a2++)
 {
  if(a1>=strlen(string))
  {
   break;
  }
  string[a1]=string2[a2];
 }
}

// Converts string to screen code format and stores it to string2.
// Notice: string2 must be atleast twice bigger than string,
//         because it's going to include color information inside also.
void jtm_createscreencodestring(char *string,char *string2,char cols)
{
 long a1,a2,a3,a4;
 //
 for(a1=0,a2=0; a1<strlen(string); a1++,a2+=2)
 {
  string2[a2+0]=string[a1];
  string2[a2+1]=cols;
 }
}

#ifdef DJGPP
// Writes string on screen(ANSI 80x.. mode).
// Notice: This function uses coordinates that starts from number one.
// Warning: Do not use illegal values(Unless you want to crash the system)
void jtm_writestringonscreen(char *string,long x,long y,char cols)
{
 unsigned char string2[512];
 //
 x--; y--;
 jtm_createscreencodestring(string,string2,cols);
 //
 dosmemput(string2, strlen(string)*2, 0xb8000+((y*80+x)*2));
}
#endif

void jtm_filteraudio(unsigned char *audio,unsigned repeat)
{
 register unsigned uad,uad2,uad3,uad4;
 long ad,ad2;
 //
 for(ad2=0; ad2<repeat; ad2++)
 {
  for(ad=0; ad<0xfffe; ad++)
  {
   uad= audio[ad+0x00];
   uad2=audio[ad+0x01];
   uad3=(uad+uad2)/2;
   audio[ad]=uad3;
  }
 }
}


// Alin merkkim„„r„ mik„ luokitellaan aktiiviseksi tiedoksi.
// Kaikki luvun alittavat merkkim„„r„t luokitellaan passiiviseksi.
// Aktiivinen tieto lis„t„„n animaatio tiedostoon.
// Passiivinen tietoa ei lis„t„ animaatio tiedostoon.
//

void BugFixBins(void)
{
 long ad,ad2;
 //
 for(ad=0; ad<icomp.a1; ad++)
 {
  if(icomp.bin1[ad]==CMDBYTE)
  {
   icomp.bin1[ad]=0x55;
  }
  if(icomp.bin2[ad]==CMDBYTE)
  {
   icomp.bin2[ad]=0x55;
  }
 }
}

int jtm_icomp(char *fnpic1,char *fnpic2,char *fnpic3,int lowrule)
{
 long ad,ad2,ad3,ad4;
 char str[255],str2[255];
 FILE *fp,*fp2;
 int Vaih; // Vaihtelu arvo (Verrataan LowRule muuttujaan)

 // IMAGE QUALITY : (Smaller value = better quality, Bigger value = better compression)
// LowRule=appconfig.data[0];

 //
 icomp.a1=fsizeof(fnpic1);

 //
 icomp.LowRule=lowrule;
 icomp.bin1=malloc(icomp.a1);
 icomp.bin2=malloc(icomp.a1);

 // File sizes of files specified in argv[1] and argv[2] must be SAME.
 jtm_loadbinc64(fnpic1,icomp.bin1);
 jtm_loadbinc64(fnpic2,icomp.bin2);
 BugFixBins();
 ad=0;
 strcpy(str,fnpic3);
 strcat(str,".cmp");
 printf("Writing %s ...\n",str);
 fp=fopen(str,"wb");
 fputw(0x2000,fp); // Write start address in front of
                   // the destination file ($2000)
 do
 {
  if( icomp.bin1[ad]==icomp.bin2[ad] && (icomp.a1-ad)>=3)
  {
   for(ad2=ad,ad3=0,Vaih=0; ad2<ad+250; ad2++,ad3++)
   {
    if(ad2>icomp.a1)
    {
     ad2=icomp.a1;
     break;
    }
    if(icomp.bin1[ad2]!=icomp.bin2[ad2])Vaih++;
    if( Vaih>=icomp.LowRule ) break;
   }
   if(ad3>=4)
   {
    fputc(CMDBYTE,fp);
    fputc(ad3,fp);
    ad+=ad3;
    goto cont;
   }
   else
   {
    goto else1;
   }
  }
  else
  {
else1:
   fputc(icomp.bin2[ad],fp);
  }
  ad++;
cont:
  asm("nop");
 }while(ad<icomp.a1);
 fclose(fp);
 printf("Done, good.\n");
 printf("%s %u bytes\n",fnpic1,fsizeof(fnpic1));
 printf("%s %u bytes\n",fnpic2,fsizeof(fnpic2));
 printf("%s %u bytes\n",str,    fsizeof(str));
 //
 free(icomp.bin1);
 free(icomp.bin2);
 return 0;
}

int f3wordseek(FILE *f,char *word1,char *word2,char *word3)
{
 char	_str[255];
 while(!feof(f))
 {
  fscanf(f,"%s",&_str);
  if(!strcmp(_str,word1))
  {
	fscanf(f,"%s",&_str);
	if(!strcmp(_str,word2))
	{
	 fscanf(f,"%s",&_str);
	 if(!strcmp(_str,word3))
	 {
	  return	0;	  // 0 means no error(=STRING FOUND)
	 }
	}
  }
 }
 return 1;		 // 1 means error
}

int f2wordseek(FILE *f,char *word1,char *word2)
{
 char	_str[255];
 while(!feof(f))
 {
  fscanf(f,"%s",&_str);
  if(!strcmp(_str,word1))
  {
	fscanf(f,"%s",&_str);
	if(!strcmp(_str,word2))
	{
	 return 0;	 // 0 means no error
	}
  }
 }
 return 1;		 // 1 means error
}

int f1wordseek(FILE *f,char *word1)
{
 char	str[255];
 //
 while(!feof(f))
 {
  fscanf(f,"%s",&str);
  if(!strcmp(str,word1))
  {
	return 0;	// 0 means no error
  }
 }
 return 1;		 // 1 means error
}

/////////////////////////////////////////////////////////////////////////////
//JTM_STRETCHSTRINGLENGTH
//
//This function stretchs string length from old to new length.
//
void jtm_stretchstringlength(char *str2,int length)
{
 //
 int i;

 //
 if(strlen(str2)<length)
 {
  for(i=strlen(str2); i<length; i++)str2[i]=' ';
  str2[i]=0;
 }
 else
 {
  str2[length]=0;
 }
}

/////////////////////////////////////////////////////////////////////////////
//JTM_GETDRIVELETTERSFROMSTRING
//
//This function reads all drive letters from a string and
//puts these in another string pointed by str2, function(str,str2).
//
// Quick explain :
//
// str might be f.e. : "a: c: f:"
// after calling this function,
// str2 will contain following string : "acf".
//
void jtm_getdrivelettersfromstring(char *str,char *str2)
{
 //
 int i,i2;

 //
 for(i=0,i2=0; i<strlen(str); i++)
 {
  if(str[i]==':' && i>0)
  {
   // Edelt„v„ on asema kirjain.
   // Tehd„„n vertailu mik„ kutsuu funktiota mik„
   // m„„rittelee onko str[i-1] ASCII merkki aakkonen.
   if(isalpha(str[i-1])!=0)
   {
    str2[i2]=str[i-1];
    i2++;
   }
  }
 }
 str2[i2]=0;
}

// JTM_ADDSTRINFILEOFF function
// ----------------------------
//
// Description:
//
//      This function inserts(adds) string in specified place in
//      a specified file. File size of the file choosen for the operation
//      will be grown up if the operation completed successfully.
//
// Example:
//
// jtm_addstrinfileoff("myfile.txt",1000,"<Yeah, this text comes here>");
//
//
int jtm_addstrinfileoff(char *fname,unsigned long int offs,char *rstr)
{
 //
 FILE *f,*f2,*f3,*f4;
 unsigned long int ad,ad2,ad3,ad4,size1;
 unsigned char *fdata;

 // Lataa t„m„nhetkinen configuraatiotiedosto "v„li" muistiin.
 size1=fsizeof(fname);
 if(offs>size1)return 1; // >= ?
 fdata=malloc(size1);
 jtm_loadbin(fname,fdata);

 // Kirjoita sama configuraatio vanhan p„„lle siten ett„ uusi stringi
 // tulee osaksi configuraatio tiedostoa.
 f=fopen(fname,"wb");
 // Kirjoita osa #1
 for(ad=0; ad<offs; ad++)
 {
  fputc(fdata[ad],f);
 }

 // Kirjoita v„liin liitett„v„ merkkijono.
 fprintf(f,"%s",rstr);

 // Kirjoita osa #2
 for(ad=offs; ad<size1; ad++)
 {
  fputc(fdata[ad],f);
 }
 fclose(f);

 //
 free(fdata);
}

// JTM_REMOVEBYTESFROMFILE function
// ----------------------------
//
// Description:
//
//      This function removes specified amount of bytes from
//      specified position from a specified file.
//
// Example:
//
//      jtm_removebytesfromfile("myfile.txt",1000,28);
//
//
int jtm_removebytesfromfile(char *fname,unsigned long int offs,unsigned long int amount)
{
 //
 FILE *f,*f2,*f3,*f4;
 unsigned long int ad,ad2,ad3,ad4,size1;
 unsigned char *fdata;

 // Lataa t„m„nhetkinen configuraatiotiedosto "v„li" muistiin.
 size1=fsizeof(fname);
 if(offs>size1)return 1; // >= ?
 fdata=malloc(size1);
 jtm_loadbin(fname,fdata);

 // Kirjoita sama configuraatio vanhan p„„lle siten ett„ uusi stringi
 // tulee osaksi configuraatio tiedostoa.
 f=fopen(fname,"wb");
 // Kirjoita osa #1
 for(ad=0; ad<offs; ad++)
 {
  fputc(fdata[ad],f);
 }

 // Skippaa sen verran tavuja tiedostosta jotta
 // tapahtuisi m„„ritelty leikkaus prosessi. (Merkit katoavat)
 offs+=amount;

 // Kirjoita osa #2
 for(ad=offs; ad<size1; ad++)
 {
  fputc(fdata[ad],f);
 }
 fclose(f);

 //
 free(fdata);
}

int decode_pcx(char *_srcfname,char *_outfname,unsigned char _paletteadd)
		// Last parameter = Add palette (0=False, 1=True)
{

 unsigned long ad,ad2,ad3,ad4,ad5;
 register unsigned char m1,m2;
 unsigned long x,y,x2,y2;
 int xlen;
 int ylen;
 FILE *_fp,*_fp2;
 //
 _fp= fopen(_srcfname,"rb");
 if(_fp==NULL)
 {
  return(1);
 }
 _fp2=fopen(_outfname,"wb");
 if(_fp2==NULL)
 {
  fclose(_fp);
  return(1);
 }
 fseek(_fp,0,SEEK_END);
 if(ftell(_fp)==0)
 {
  fclose(_fp);
  fclose(_fp2);
  return 1;
 }
 fseek(_fp,0,SEEK_SET);

 // Get image size
 fseek(_fp,4+4,SEEK_SET);
 xlen=(fgetc(_fp)|(fgetc(_fp)<<8))+1;
 ylen=(fgetc(_fp)|(fgetc(_fp)<<8))+1;
 jtmlpcxinfo.lxlen=jtmlpcxinfo.xlen;
 jtmlpcxinfo.lylen=jtmlpcxinfo.ylen;
 jtmlpcxinfo.xlen=xlen;
 jtmlpcxinfo.ylen=ylen;

 // Seek to end of header, encoded image data begin.
 fseek(_fp,pcx_header,SEEK_SET);
 x=0;
 while( !feof(_fp) )
 {
  if(x>(xlen*ylen))break;
  m1=fgetc(_fp);
  if((m1&0xc0)==0xc0)
  {
   m2=fgetc(_fp);
   //Duplicate character
   for(x2=0; x2<(m1&0x3f); x2++)
   {
    fputc(m2,_fp2);
    x++;
   }   
  }
  else
  {
   fputc(m1,_fp2);
   x++;
  }
 }

 if(_paletteadd==1) // Add palette if it's expected.
 {
  fseek(_fp2,-768,SEEK_CUR);
  fseek(_fp,-768,SEEK_END);
  for(ad=0; ad<768; ad++)
  {
   m1=fgetc(_fp);
   fputc(m1,_fp2);
  }
 }
 fclose(_fp2);
 fclose(_fp);
 return 0;
}

void jtm_fillbuffer(char *_buf,unsigned char _byte)
{
 register long _uad;
 //
 for(_uad=0; _uad<0x10000; _uad++)_buf[_uad]=_byte;
}

int decode_pcxto(unsigned char *_srcfname,char *_buffer,unsigned char _paletteadd)
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

 unsigned long int ad,ad2,ad3,ad4,ad5;
 register unsigned char m1,m2;
 unsigned long int x,y,y2,MAX;
 register unsigned int x2;
 unsigned int xlen;
 unsigned int ylen;
 FILE *_fp,*_fp2;
 //
// jtm_fillbuffer(_buffer,0x00);
 _fp= fopen(_srcfname,"rb");
 if(_fp==NULL)
 {
  return(1);
 }
 fseek(_fp,0,SEEK_END);
 if(ftell(_fp)==0)
 {
  fclose(_fp);
  return 1;
 }
 fseek(_fp,0,SEEK_SET);

 // Get image size
 fseek(_fp,8,SEEK_SET);
 xlen=(fgetc(_fp)|(fgetc(_fp)<<8))+1;
 ylen=(fgetc(_fp)|(fgetc(_fp)<<8))+1;
 MAX=xlen*ylen;
 jtmlpcxinfo.lxlen=jtmlpcxinfo.xlen;
 jtmlpcxinfo.lylen=jtmlpcxinfo.ylen;
 jtmlpcxinfo.xlen=xlen;
 jtmlpcxinfo.ylen=ylen;

 fseek(_fp,pcx_header,SEEK_SET);
 x=0;
 // * ** ************************ ** Start of decoding process ** *
 while(!feof(_fp))
 {
  m1=fgetc(_fp);
  if((m1&0xc0)==0xc0)
  {
   m2=fgetc(_fp);
   for(x2=0; x2<(m1&0x3f); x2++)
   {
    if(x>=MAX)break;
    _buffer[x]=m2;
    x++;
   }   
  }
  else
  {
   if(x>=MAX)break;
   _buffer[x]=m1;
   x++;
  }
 }
 // * ** ************************ ** End of decoding procress  ** *
 fclose(_fp);
 return 0;
}

int jtm_setpalettefrompcxto(char *_filename,char *palout)
{
 unsigned char m1,m2,m3;
 FILE *_fp;
 unsigned _c;
 //
 _fp=fopen(_filename,"rb");
 if(_fp==NULL)return(1);
 fseek(_fp,-768,SEEK_END); // Seek to end of PCX file
 for(_c=0; _c<256; _c++)
 {
  m1=fgetc(_fp)/4;
  m2=fgetc(_fp)/4;
  m3=fgetc(_fp)/4;
  palout[_c*3+0]=m1;
  palout[_c*3+1]=m2;
  palout[_c*3+2]=m3;
 }
 fclose(_fp);
 return 0;
}

#ifdef DJGPP

// stdio&conio version
void jtm_catdoc(char *fname)
{
 //
 char m1,m2,m3,m4;
 int i,lines;
 FILE *f;

 //
 lines=0;

 //
 f=fopen(fname,"rb");

 //
 do
 {
  if(feof(f))break;
  m1=fgetc(f); if(m1==0)break;
  cprintf("%c",m1);
  if(m1==13)lines++;
  if(lines>=20)
  {
   lines=0;
#ifdef JTMCTRL_INCLUDED
   cprintf("\n\r"); waitkey();
#endif
  }
 }while(0==0);
 if(lines>1)
 {
#ifdef JTMCTRL_INCLUDED
  cprintf("\n\r"); waitkey();
#endif
 }

 //
 fclose(f);
}
#endif

char *jgetspath(char *spath)
{
 //
 char str[255],str2[255];

 //
 sprintf(str,"%s=",spath);
 return jgetenv("path",str);
}

void jtm_viewdocpage(char *docname)
{
 //
 char str[255],str2[255],str3[255],str4[255];

 //
 sprintf(str,"%s/%s",jgetspath("help"),docname);
 jtm_messagebuffer("{Viewing document page:");
 jtm_messagebuffer(str);
 jtm_messagebuffer("}");
 if(fexist(str))
 {
#ifdef DJGPP
  jtm_catdoc(str);
#else // As default, if compiler is not DJGPP then it is UN*X/Linux.
  sprintf(str3,"cat %s |more",str); system(str3);
#endif
 }
 else
 {
  sprintf(str,"./%s",docname);
  if(fexist(str))
  {
#ifdef DJGPP
   jtm_catdoc(str);
#else
   sprintf(str3,"cat %s |more",str); system(str3);
#endif
  }
  else
  {
   sprintf(str,"VIEWDOCPAGE: Couldn't find document anywhere(%s).\n",docname);
   write(str);
  }
 }
}


int jtm_hexout(unsigned char *str)
{
   //
   int i;
   
   //
   for(i=0; i<strlen(str); i++)
     {
	if(i!=(strlen(str)-1))
	  {
	     printf("%1.3x,",str[i]);
	  }
	else
	  {
	     printf("%1.3x.",str[i]);
	  }
     }
   printf("\n");
}
 
void jtm_makejstr(char *str)
{
   //
   int i;
   
   //
   for(i=0; i<strlen(str); i++)
     {
	if(str[i]==13)
	  {
	     str[i]=0;
	     break;
	  }
	if(str[i]==10)
	  {
	     str[i]=0;
	     break;
	  }
     }
}

void jtm_replacemarksinstring(char *str,char mark,char bymark)
{
 //
 int i;

 //
 for(i=0; i<strlen(str); i++)
 {
  if(str[i]==mark)str[i]=bymark;
 }
}

// This little function retrieves the MSDOS three mark filetype describer
// from a filename to specified string. (Copies last three marks after '.')
// If no '.' mark found in filename it'll set 'str' length at 0,
// by copying chr0 at beginning of 'str'.
// "program.typ" -> "typ"
void jtm_getftypefromfname(char *fname,char *str)
{
 //
 char m1,m2,m3,m4;
 long ad,ad2,ad3,ad4,GOT;

 //
 for(ad=0,GOT=0; ad<strlen(fname); ad++)
 {
  if(fname[ad]=='.'){ad++; GOT=1; break;}
 }

 // Got the filetype?
 if(GOT)
 {
  for(ad2=ad,ad3=0; ad3<3; ad3++,ad2++)
  {
   str[ad3]=fname[ad2];
  }
  str[ad3]=0;
 }
 else
 {
  str[0]=0;
 }
}

// This function removes MSDOS stylish filetype from end of
// filename, it actually also does remove all the other marks after
// first '.' mark and sets end of string mark(chr:0) at the place.
// filename.typ -> filename
void jtm_removefiletypefromfilename(char *fname)
{
 //
 int i;

 //
 for(i=0; i<strlen(fname); i++)
 {
  if(fname[i]=='.')
  {
   fname[i]=0;
   break;
  }
 }
}

jtmscr_wipesrtab()
{
 //
 long y;

 //
 for(y=0; y<jtmscr.height; y++)
 {
  jtmscr.srtab[y]=(jtmscr.srtab[y]&3)>>1;
 }
}

void jtmscr_clearscreen(void)
{
 //
 register long ad,x,y;

 //
#ifdef __JTMDJGFX_H__
 jtmgl_homecursor();
#endif
 
 //
 switch(jtmscr.bpp)
 {
  case 8:
  for(y=0; y<jtmscr.height; y++)
  {
   ad=jtmscr.width*y;
   for(x=0; x<jtmscr.width; x++)
   {
    jtmscr.vbuffer[ad+x]=0;
   }
  }
  break;
  case 32:
  for(y=0; y<jtmscr.height; y++)
  {
   ad=jtmscr.width*y;
   for(x=0; x<jtmscr.width; x++)
   {
    jtmscr.vbuffer[((ad+x)<<2)+0]=0;
    jtmscr.vbuffer[((ad+x)<<2)+1]=0;
    jtmscr.vbuffer[((ad+x)<<2)+2]=0;
   }
  }
  break;
 }
 jtmscr_wipesrtab();
}

void jtm_greyscale256()
{
 float          fred=0,fgreen=0,fblue=0;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 for(_color=0,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=fred;
  green=fgreen;
  blue=fblue;
  jtm_setpalette(_color,red,green,blue);
 }
}

void jtm_bluescale256()
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 for(_color=0,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=0;
  green=0;
  blue=fblue;
  jtm_setpalette(_color,red,green,blue);
 }
}

void jtm_greenscale256()
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 for(_color=0,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=0;
  green=fblue;
  blue=0;
  jtm_setpalette(_color,red,green,blue);
 }
}

void jtm_redscale256()
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 for(_color=0,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=fblue;
  green=0;
  blue=0;
  jtm_setpalette(_color,red,green,blue);
 }
}

void jtm_lbluescale256()
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 for(_color=0,fred=0,fgreen=0,fblue=0; _color<128; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=0;
  green=0;
  blue=fblue*2;
  jtm_setpalette(_color,red,green,blue);
 }
 for(_color=128,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=fblue*2;
  green=fblue*2;
  blue=0x3f;
  jtm_setpalette(_color,red,green,blue);
 }
}

void jtm_lgreenscale256()
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 for(_color=0,fred=0,fgreen=0,fblue=0; _color<128; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=0;
  green=fblue*2;
  blue=0;
  jtm_setpalette(_color,red,green,blue);
 }
 for(_color=128,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=fblue*2;
  green=0x3f;
  blue=fblue*2;
  jtm_setpalette(_color,red,green,blue);
 }
}

void jtm_lredscale256()
{
 float          fred,fgreen,fblue;
 unsigned       char red,green,blue;
 unsigned       _color;
 //
 for(_color=0,fred=0,fgreen=0,fblue=0; _color<128; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=fblue*2;
  green=0;
  blue=0;
  jtm_setpalette(_color,red,green,blue);
 }
 for(_color=128,fred=0,fgreen=0,fblue=0; _color<256; _color++,fred+=.25,fgreen+=.25,fblue+=.25)
 {
  red=0x3f;
  green=fblue*2;
  blue=fblue*2;
  jtm_setpalette(_color,red,green,blue);
 }
}

long jtm_mklong(unsigned char *buf)
{
 return (buf[0]+(buf[1]<<8)+(buf[2]<<16)+(buf[3]<<24));
}

unsigned int jtm_mkint(unsigned char *buf)
{
 return (buf[0]+(buf[1]<<8));
}

void jtmscr_maximize(void)
{
 //
 long ad,ad2,ad3,ad4;
 long x,y;
 float fx,fy,fx2,fy2,fa,fb,fc,fd,isoin=0,kertoin;

 switch(jtmscr.bpp)
 {
  case 8:
  //
  for(ad=0; ad<(320*200); ad++)
  {
   if(jtmscr.vbuffer[ad]>isoin)isoin=jtmscr.vbuffer[ad];
  }

  // cmv
  if(isoin==0)isoin++;
  fa=isoin; fa=255/fa; kertoin=fa;

  //
  for(ad=0; ad<(320*200); ad++)
  {
   fa=(unsigned char)jtmscr.vbuffer[ad];
   fa=fa*kertoin;
   ad2=fa;
   jtmscr.vbuffer[ad]=ad2;
  }
  break;
 }
}

/* JTMGL_STRETCHPUTIMAGE function (SPI)
 * ------------------------------------
 *
 * You have to call this function with following arguments :
 *
 * char *t ->   Pointer to the 8bit greyscale bitmap or what so ever
 * int width
 * int height
 * XS,YS-XM,YM consits an one single rectangle area that will be
 * used while stretch putting the image onto the screen.
 * int xs -----> Drawing coordinate on the screen, "START POINT".
 * int ys --/
 * int xm -----> Drawing coordinate on the screen, "END POINT".
 * int ym --/
 *
 * If you would like to write only NON black colors to the screen,
 * then just give the WIDTH and HEIGHT argument in NEGATIVE form,
 * function will think then you maybe would like to skip the NON black color
 * writing.
 *
 * Other:
 *
 *      Function doesn't return anything, so don't care.
 *
 */

jtmgl_stretchputimage(char *t, int width,int height,
                           int xs,int ys,int xm,int ym)
{
 //
 jtmgl_stretchputimage1(jtmscr.vbuffer,jtmscr.width,jtmscr.height,8,
                       t,width,height,xs,ys,xm,ym,8);
}

jtmgl_stretchputimage1(char *vbuffer,long jwidth,long jheight,long outbpp,
char *t, int width,int height, int xs,int ys,int xm,int ym,long srcbpp)
{
 //
 long x,y,xx,yy,tx,ty,ad,ad2,ad3,ad4,maxthru;
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
 asm("nop");
 //
}

jtmgl_directstretchputimage(char *t, int width,int height, int xs,int ys,int xm,int ym)
{
 //
 long x,y,xx,yy,tx,ty,ad,ad2,ad3,ad4;
 float fa,fb,fc,fd,fe,ff,fg,fh,fi,fx,fy,fxs,fys,fxm,fym,fxp,fyp;
 int NONBLACK;

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
  if(y<jtmscr.height)
  {
   ty=fy;
   if(ty<height && ty>-1)
   {
    ad=jtmscr.width*y;
    ad2=width*ty;
    for(x=xs,fx=0; x<xm; x++,fx=fx+fxp)
    {
     tx=fx;
     if(tx>=width)break;
     if(x<jtmscr.width && x>-1 && tx>-1 && ((ad2+tx)<ad4))
     {
      if(NONBLACK)
      {
       if(t[ad2+tx])jtmscr.vbuffer[ad+x]=t[ad2+tx];
      }
      else
      {
       jtmscr.vbuffer[ad+x]=t[ad2+tx];
      }
      //
     }
    }
   }
  }
  //
 }

 //
}

// JTM_FINDWHITEFROMCURRENTPALETTE function
// ----------------------------------------
//
// This function works only when using 8bit videomode.
//
int jtm_findwhitefromcurrentpalette()
{
 //
 long i,i2,i3,i4,ad,ad2,ad3,ad4;

 //
 switch(jtmscr.bpp)
 {
  case 8:
  for(i=0,i2=0,i3=0; i<256; i++)
  {
   ad=jtmansi.palette[(i<<2)+0];
   ad2=jtmansi.palette[(i<<2)+1];
   ad3=jtmansi.palette[(i<<2)+2];
   ad4=ad+ad2+ad3;
   if(ad4>i2){i2=ad4; i3=i;}
  }
  return i3;
  default:
  return -1;
  break;
 }
}

// JTM_FINDBLACKFROMCURRENTPALETTE function
// ----------------------------------------
//
// This function works only when using 8bit videomode.
//
int jtm_findblackfromcurrentpalette()
{
 //
 long i,i2,i3,i4,ad,ad2,ad3,ad4;

 //
 switch(jtmscr.bpp)
 {
  case 8:
  for(i=0,i2=0xffffff,i3=0; i<256; i++)
  {
   ad=jtmansi.palette[(i<<2)+0];
   ad2=jtmansi.palette[(i<<2)+1];
   ad3=jtmansi.palette[(i<<2)+2];
   ad4=ad+ad2+ad3;
   if(ad4<i2){i2=ad4; i3=i;}
  }
  return i3;
  default:
  return -1;
  break;
 }
}

// JTM_COUNTLINESFROMFILE function:
// Counts lines from a file.
long jtm_countlinesfromfile(char *fname)
{
 //
 FILE *f;
 long linez,ad,ad2,ad3,ad4;
 char m1,m2,m3,m4;

 //
 if(!fexist(fname))return -1;

 //
 linez=0;
 f=fopen(fname,"rb");
 do
 {
  if(feof(f))break;
  m1=fgetc(f);
  if(m1==13)linez++;
 }while(0==0);
 fclose(f);
 return linez;
}

long jtm_mod(long val,long modv)
{
 //
 long a,b,c;

 //
 if(modv)
 {
  a=(val/modv);
  b=a*modv;
  c=val-b;
 }
 else
 {
  c=0;
 }
 return c;
}

void jtmscr_blurcurrentscreen(void)
{
 //
 jtmgl_blurbuffer(jtmscr.vbuffer,320,200);
}

// Shortage fix..
void jtmgl_blurbuffer(unsigned char *vbuffer,long width,long height)
{
 jtmgl_blurbuffer1(vbuffer+320,width,height-1);
}

void jtmgl_blurbuffer1(unsigned char *vbuffer,long width,long height)
{
 //
 long ad,ad2,ad3,ad4,x,y,x2,y2;

 //--- CASE 1
 for(y=0; y<(height-1); y++)
 {
  ad=jtmscr.width*y;
  for(x=0; x<width-1; x++)
  {
   vbuffer[ad+x]=(vbuffer[ad+x]+vbuffer[ad+x+1])>>1;
  }
 }

 //
 for(y=0; y<(height-1); y++)
 {
  ad=jtmscr.width*y;
  for(x=0; x<width-1; x++)
  {
   vbuffer[ad+x]=(vbuffer[ad+x]+vbuffer[ad+x+320])>>1;
  }
 }

 //--- CASE 2
 //
 for(y=height-1; y!=1; y--)
 {
  ad=width*y;
  for(x=width-1; x>1; x--)
  {
   vbuffer[ad+x]=(vbuffer[ad+x]+vbuffer[ad+x-1])>>1;
  }
 }

 //
 for(y=height-1; y!=1; y--)
 {
  ad=width*y;
  for(x=width-1; x>1; x--)
  {
   vbuffer[ad+x]=(vbuffer[ad+x]+vbuffer[ad+x-320])>>1;
  }
 }
}

void jtmscr_makesnow(void)
{
 //
 long ad,ad2,ad3,ad4,x,y;

 //
 for(y=0; y<jtmscr.height; y++)
 {
  ad=jtmscr.width*y;
  for(x=0; x<jtmscr.width; x++)
  {
   jtmscr.vbuffer[ad+x]=rand()&0xff;
  }
 }
 jtmgl_tosrtab(0,200);
}

// 'tosrtab' function.
void jtmgl_tosrtab(int y1,int y2)
{
 //
 long ad;

 //
// jtmscr.drawround++;
 for(ad=y1; ad<y2; ad++)
 {
  jtmscr.srtab[ad]=0xff;
 }
// jtmscr.drawround--;
}

// JTMGL_MERGEPICTURE1 function
//
// Example :
//
//      jtmgl_mergepicture1(2,
//                          jtmscr.vbuffer,jtmscr.width,jtmscr.height,
//                          texture,320,200,
//                          0,0);
//
void jtmgl_mergepicture1(int type,
                         unsigned char *vbuffer,long width,long height,
                         unsigned char *tex,long xpit,long ypit,
                         int wx,int wy)
{
 //
 long x,y,ad,ad2;

 //
 for(y=0; y<height; y++)
 {
  ad=jtmscr.width*(y+wy);
  if((y+wy)<ypit)
  {
   for(x=0; x<width; x++)
   {
    if((x+wx)<xpit)
    {
     switch(type)
     {
      case 0: // Copy and paste
      vbuffer[ad+x+wx]=tex[y*xpit+x];
      break;
      case 1: // Leave both images visible (make combination)
      vbuffer[ad+x+wx]=(tex[y*xpit+x]+vbuffer[ad+x+wx])>>1;
      break;
      case 2: // Plot if NON black
      if(tex[y*xpit+x])vbuffer[ad+x+wx]=tex[y*xpit+x];
      break;
     }
    }
   }
  }
 }
}

jtm_waitwhile(long amount)
{
 systemtimer.counter2=0;
 while(systemtimer.counter2<amount);
}

jtm_counterdemo()
{
 //
 char m1,str[255];

 //
 do
 {
  //
  sprintf(str,"systemtimer.counter2=%u\r",systemtimer.counter2);
  write(str);

  //
#ifdef __JTMINTER_H__
  m1=jgetchim();
  if(m1==27)break;
#else
  break;
#endif
 }while(!0);
}


int jtm_getbmpxlen(char *fname)
{
 //
 FILE *f;
 long cac;
 
 //
 f=fopen(fname,"rb");
 fseek(f,0x12,SEEK_SET); cac=fgetw(f);
 fclose(f);
 return cac;
}

int jtm_getbmpylen(char *fname)
{
 //
 FILE *f;
 long cac;
 
 //
 f=fopen(fname,"rb");
 fseek(f,0x16,SEEK_SET); cac=fgetw(f);
 fclose(f);
 return cac;
}

int jtm_loadbmp(char *fname,char *frame) // Loads 24bit & 8bit
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
  fclose(fp);
  return 1;
 }
 fclose(fp);
 jtmlpcxinfo.xlen=width;
 jtmlpcxinfo.ylen=height;
 return 0;
}

void jtm_multip256(void)
{
 long a1;
 //
 for(a1=0; a1<64; a1++)jtm_setpalette(a1+0x00,  0,  0,   a1);
 for(a1=0; a1<64; a1++)jtm_setpalette(a1+0x40,  0,   a1,  0);
 for(a1=0; a1<64; a1++)jtm_setpalette(a1+0x80,  a1,  0,   0);
 for(a1=0; a1<64; a1++)jtm_setpalette(a1+0xc0,  a1,  a1,  a1);

// for(a1=0; a1<64; a1++)jtm_setpalette(a1+0x00,  a1,  a1/3,   a1);
// for(a1=0; a1<64; a1++)jtm_setpalette(a1+0x40,  0,   a1,  a1/3);
// for(a1=0; a1<64; a1++)jtm_setpalette(a1+0x80,  a1,  a1/3,   0);
// for(a1=0; a1<64; a1++)jtm_setpalette(a1+0xc0,  a1,  a1,  a1);
}

// SURROUND effect function - Much like antialiasing !
void jtmscr_suref(void)
{
 //
 jtm_suref1(jtmscr.vbuffer,jtmscr.width,jtmscr.height,jtmscr.bpp);
}

// Surround Effect
void jtm_suref1(unsigned char *vbuffer,long width,long height,long bpp)
{
 //
 unsigned long ad2,ad3,ad4,ad5,ad6,ad7,ad8;
 register unsigned long ad,x,y;
 char m1,m2,m3,m4;
 char *tmp;

 // 8 bpp ..
 if(bpp==8)
 {
  //
  ad2=width*height;
  tmp=malloc(ad2+1024);
  for(ad=0; ad<ad2; ad++)tmp[ad]=0;

  //
  for(y=0; y<height; y++)
  {
   ad=width*y;
   for(x=0; x<width; x++)
   {
    if( vbuffer[ad+x] && (x && y) && (x!=width && y!=height) )
    {
     if(!vbuffer[ad+x+(1*width+0)])tmp[ad+x+(1*width+0)]=vbuffer[ad+x]>>1;
     if(!vbuffer[ad+x+(0*width+1)])tmp[ad+x+(0*width+1)]=vbuffer[ad+x]>>1;
     if(!vbuffer[ad+x+((-1*width)+0)])tmp[ad+x+((-1*width)+0)]=vbuffer[ad+x]>>1;
     if(!vbuffer[ad+x+((0*width)+-1)])tmp[ad+x+((0*width)+-1)]=vbuffer[ad+x]>>1;
    }
   }
  }

  //
/*  for(y=height-1; y>-1; y--)
  {
   ad=width*y;
   for(x=0; x<width; x++)
   {
    if( vbuffer[ad+x] && (x && y) && (x!=width && y!=height) )
    {
     if(vbuffer[ad+x+(1*width+0)])tmp[ad+x+(1*width+0)]=vbuffer[ad+x]>>1;
     if(vbuffer[ad+x+(0*width+1)])tmp[ad+x+(0*width+1)]=vbuffer[ad+x]>>1;
     if(vbuffer[ad+x+((-1*width)+0)])tmp[ad+x+((-1*width)+0)]=vbuffer[ad+x]>>1;
     if(vbuffer[ad+x+((0*width)+-1)])tmp[ad+x+((0*width)+-1)]=vbuffer[ad+x]>>1;
    }
   }
  }*/

  //
  ad2=width*height;
  for(ad=0; ad<ad2; ad++)if(tmp[ad])jtmscr.vbuffer[ad]=tmp[ad];

  //
  free(tmp);
 }

 //

 // 24 bpp ..
}

void jtmscr_clearsrarea(void)
{
 //
 register long ad,ad2,x,y;

 //
 for(y=0; y<jtmscr.height; y++)
 {
  if(jtmscr.srtab[y])
  {
   switch(jtmscr.bpp)
   {
    case 8:
    ad=y*jtmscr.width;
    for(x=0; x<jtmscr.width; x++)jtmscr.vbuffer[ad+x]=0;
    break;
   }
  }
 }
}

void jtmscr_rollsrtab(void)
{
 //
 long ad,ad2;

 //
 for(ad=0; ad<jtmscr.height; ad++)jtmscr.srtab[ad]=(jtmscr.srtab[ad]&3)>>1;
}


long jtm_getcpuspeedinfo(int testnum)
{
 //
 long ad,ad2,ad3,ad4,ad5,ad6,ad7,ad8;
 float fa,fb,fc,fd;

 switch(testnum)
 {
  //
  case 0: // FPU+CPU test (80386+x87)
  systemtimer.counter1=0;

  //
  ad=0;
  do
  {
   fc=120+ad;
   for(fa=0; fa<10; fa++)
   {
    fb=(fc*fa/10+6363)*fa;
   }
   ad++;
   if(systemtimer.counter1>=20)break;
  }while(0==0);

  //
  case 1: // CPU test (80386 ++)
  systemtimer.counter1=0;

  //
  ad=0; ad4=0;
  do
  {
   ad2=120*ad;
   ad4+=2;
   for(ad3=0; ad3<100; ad3++)
   {
    ad4=(ad2*ad3/10+6363)*ad3;
   }
   ad++;
   if(systemtimer.counter1>=20)break;
  }while(0==0);
 }

 //
 return ad;
}

void jtm_calstrebuf(unsigned char *buf,long sval,long eval,long len)
{
 //
 long ad,ad2,ad3,ad4;
 float fa,fb,fc,fd;

 //
 if(sval>eval)
 {
  ad4=sval-eval;
  fa=ad4; fb=len; fc=-(fa/fb);
 }
 else
 {
  ad4=eval-sval;
  fa=ad4; fb=len; fc=fa/fb;
 }


 //
 for(ad=0,fa=sval; ad<len; ad++,fa+=fc)
 {
  ad2=fa;
  buf[ad]=ad2;
 }
}

int jtm_stringtostrings(char *str,char *strs[])
{
 //
 long ad,ad2,ad3,ad4,ad5,ad6,ad7,ad8;

 //
 ad=0; // str[ad]
 ad2=0; // strs[ad2][ad3]
 ad4=0; // argc
 while(0==0)
 {
  ad3=0;
  while(0==0)
  {
   if(!str[ad])
   {
    strs[ad2][ad3]=0;
    goto over;
   }
   if(str[ad]!=' ')break;
   ad++;
  }
  ad4++;
  while(0==0)
  {
   strs[ad2][ad3]=str[ad];
   ad++; ad3++;
   if(!str[ad])break;
   if(str[ad]==' ')break;
  }
  if(str[ad])ad++;
  strs[ad2][ad3]=0;
  ad2++;
 }
over:
 return ad2; // argc
}

void jtm_writebmp24(char *fname,char *bitmap,int resx,int resy) // Creates BMP 24bit (RGB)
{
 FILE *f;
 unsigned x,y;
 unsigned ad,ad2,ad3,ad4;
 //
 jtm_bmpheader24[0x12]=resx&255;
 jtm_bmpheader24[0x13]=resx/256;
 //
 jtm_bmpheader24[0x16]=resy&255;
 jtm_bmpheader24[0x17]=resy/256;
 //
 f=fopen(fname,"wb");
 for(ad=0; ad<0x36; ad++)
 {
  fputc(jtm_bmpheader24[ad],f);
 }
 for(y=0; y<resy; y++)
 {
  for(x=0; x<resx; x++)
  {
   fputc(bitmap[4*(199-y)*320+x],f);
  }
 }
 fclose(f);
}


// Cynicism ?-[= =]

void jtm_writebmp8(char *fname,char *bitmap,int resx,int resy) // Creates BMP 8bit (VGA256)
{
 FILE *f;
 unsigned x,y;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 //
 f=fopen(fname,"wb");
 for(ad=0; ad<0x36; ad++)
 {
  fputc(jtm_bmpheader8[ad],f);
 }
 jtm_bmpheader8[0x12]=resx&255;
 jtm_bmpheader8[0x13]=resx/256;
 //
 jtm_bmpheader8[0x16]=resy&255;
 jtm_bmpheader8[0x17]=resy/256;
 //
 for(ad=0; ad<256; ad++)
 {
  fputc(ad,f);
  fputc(ad,f);
  fputc(ad,f);
  fputc(0, f);
 }
 for(y=0; y<resy; y++)
 {
  for(x=0; x<resx; x++)
  {
   fputc(bitmap[resx*((resy-1)-y)+x],f);
  }
 }
 fclose(f);
}

void jtmgl_writebmp8(char *fname,char *bitmap) // Creates BMP 8bit (VGA256)
{
 FILE *f;
 unsigned x,y;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 //
 f=fopen(fname,"wb");
 for(ad=0; ad<0x36; ad++)
 {
  fputc(jtm_bmpheader8[ad],f);
 }
 jtm_bmpheader8[0x12]=jtmscr.width&255;
 jtm_bmpheader8[0x13]=jtmscr.width/256;
 //
 jtm_bmpheader8[0x16]=jtmscr.height&255;
 jtm_bmpheader8[0x17]=jtmscr.height/256;
 //
 for(ad=0; ad<256; ad++)
 {
  fputc(jtmansi.palette[(ad<<2)+2]<<2 ,f);
  fputc(jtmansi.palette[(ad<<2)+1]<<2 ,f);
  fputc(jtmansi.palette[(ad<<2)+0]<<2 ,f);
  fputc(0, f);
 }
 for(y=0; y<jtmscr.height; y++)
 {
  for(x=0; x<jtmscr.width; x++)
  {
   fputc(bitmap[jtmscr.width*((jtmscr.height-1)-y)+x],f);
  }
 }
 fclose(f);
}

// Does samething as function jtmgl_writebmp8 expect it doubles image size.
void jtmgl_writebmp8_dbl(char *fname,char *bitmap) // Creates BMP 8bit (VGA256)
{
 FILE *f;
 unsigned x,y;
 unsigned long ad0,ad=0,ad2=0,ad3,ad4,ad5,ad6,ad7,ad8,ad9,ad10,ad11,ad12,ad13,ad14,ad15,xs,ys,xm=360,ym=480;
 //
 f=fopen(fname,"wb");
 for(ad=0; ad<0x36; ad++)
 {
  fputc(jtm_bmpheader8[ad],f);
 }
 jtm_bmpheader8[0x12]=(jtmscr.width*2)&255;
 jtm_bmpheader8[0x13]=(jtmscr.width*2)/256;
 //
 jtm_bmpheader8[0x16]=(jtmscr.height*2)&255;
 jtm_bmpheader8[0x17]=(jtmscr.height*2)/256;
 //
 for(ad=0; ad<256; ad++)
 {
  fputc(jtmansi.palette[(ad<<2)+0]<<2 ,f);
  fputc(jtmansi.palette[(ad<<2)+1]<<2 ,f);
  fputc(jtmansi.palette[(ad<<2)+2]<<2 ,f);
  fputc(0, f);
 }
 for(y=0; y<jtmscr.height*2; y++)
 {
  for(x=0; x<jtmscr.width*2; x++)
  {
   fputc(bitmap[jtmscr.width*((jtmscr.height-1)-(y/2))+(x/2)],f);
  }
 }
 fclose(f);
}

void jtmgl_writebmp24(char *fname,char *bitmap) // Creates BMP 24bit (RGB)
{
 FILE *f;
 unsigned long x,y;
 unsigned long ad,ad2,ad3,ad4;
 //
 jtm_bmpheader24[0x12]=jtmscr.width&255;
 jtm_bmpheader24[0x13]=jtmscr.width/256;
 //
 jtm_bmpheader24[0x16]=jtmscr.height&255;
 jtm_bmpheader24[0x17]=jtmscr.height/256;
 //
 f=fopen(fname,"wb");
 for(ad=0; ad<0x36; ad++)
 {
  fputc(jtm_bmpheader24[ad],f);
 }
 for(y=0; y<jtmscr.height; y++)
 {
  for(x=0; x<jtmscr.width; x++)
  {
   fputc(bitmap[4*(((jtmscr.height-1)-y)*jtmscr.width+x)+0],f);
   fputc(bitmap[4*(((jtmscr.height-1)-y)*jtmscr.width+x)+1],f);
   fputc(bitmap[4*(((jtmscr.height-1)-y)*jtmscr.width+x)+2],f);
//   fputc(0,f);
  }
 }
 fclose(f);
}

// Note: Only 8bit bitmap supported.
jtm_force2color(unsigned char *bmp,long xlen,long ylen)
{
 //
 long ad,ad2,ad3,ad4,x,y;

 //
 for(y=0; y<ylen; y++)
 {
  ad=xlen*y;
  for(x=0; x<xlen; x++)
  {
   if(bmp[ad+x])bmp[ad+x]=0xff; else bmp[ad+x]=0x0;
  }
 }
}

// jtm_cutstring function
//
// This function creates new string from specified location of
// another string to the end of the string.
// Note: The cutting offset is specified by a single mark.
//
// program!hello -> hello
// where are the#beatiful weather, isn't it? -> 't it?
jtm_cutstring(char mark,char *str,char *str2)
{
 //
 long ad,ad2,ad3,ad4;
 char m1,m2,m3,m4;

 //
 for(ad=0,ad2=0,ad3=0; ad<strlen(str); ad++)
 {
  if( !str[ad] ) break;
  if( str[ad]==mark )
  {
   ad2=1; ad++;
  }
  if(ad2)
  {
   str2[ad3]=str[ad];
   ad3++;
  }
 }
 str2[ad3]=0;
}

jtm_printifmf(char *str,char mark)
{
 //
 long ad,ad2;

 //
 for(ad=0,ad2=0; ad<strlen(str); ad++)
 {
  if(str[ad]=='\\')
  {
   if(str[ad+1]=='n')write("\n");
   if(str[ad+1]=='r')write("\r");
   if(str[ad+1]=='c')
   #ifdef __JTMDJGFX_H__
   jtm_clrscr();
   #endif
   if(str[ad+1]=='h')
   #ifdef __JTMDJGFX_H__
   jtm_homecursor();
   #endif
   ad++;
   goto sk;
  }
  if(str[ad]==mark){ ad2=1-ad2; goto sk; }
  if(ad2)
  {
#ifdef __JTMDJGFX_H__
  writec(str[ad]);
#endif
  }
sk:
 }
}

// Note: This function translates special marks to special codes C like.
char *jtm_getfieldtostr(char *ast,char mark)
{ 
 //
 char str[255];
 long ad,ad2,ad3,ad4;

 //
 for(ad=0,ad2=0,ad3=0; ad<strlen(ast); ad++)
 {
  if(ast[ad]==mark){ ad2=1-ad2; goto sk; }
  if(ad2)
  {
   if(ast[ad]=='\\')
   {
    ad++;
    if(!ast[ad])break;
    if(ast[ad]=='\\')ad4='\\'; else
    if(ast[ad]=='n')ad4='\n'; else
    if(ast[ad]=='r')ad4='\r'; else ad4=' ';
    str[ad3]=ad4; ad3++;
    goto sk;
   }
   str[ad3]=ast[ad]; ad3++;
  }
sk:
 }
 str[ad3]=0;
}

// Note: Function returns amount of members found. If return value = -1
//       then error occured.
//       If pointer 'list' points to NULL then no stores will be made.
// Note also that the list generated should be freed with
// the jgetenvfreegroupmemberlist function !
// The function should return 0 if nothing found.
long jgetenvgroupmemberlist(const char *group,char *list[])
{
 //
 long ad,ad2,ad3,ad4,amount;
 FILE *f,*f2;
 char str[255],str2[255];
 char m1,m2;

 //
 f=fopen(jtmfn_syscfg,"rb");
 if(f2wordseek(f,"begin",group))return -1;
 ad=0;
 while(!0)
 {
  if(feof(f))break;
  fscanf(f,"%s",&str);
  if(!strcmp(str,"end."))break;
  if(str[strlen(str)-1]=='=')
  {
   str[strlen(str)-1]=0;
   list[ad]=malloc(256);
   strcpy(list[ad],str);
   ad++;
  }
 }
 fclose(f);

 //
 amount=ad;

 //
 return amount;
}

//
// It frees group member list. It requires two arguments.
// 1. Pointer to the list and 2. Amount of members
//
void jgetenvfreegroupmemberlist(char *list[],long amount)
{
 long ad;

 for(ad=0; ad<amount; ad++)free(list[ad]);
}

//
//
long jtm_countargc(const char *str)
{
 //
 long ad,ad2,ad3,ad4;

 // ad3=space value x:> 0=Space, 1=Other mark.
 ad3=0;

 //
 for(ad=0,ad2=0,ad3=0; ad<strlen(str); ad++)
 {
  if(str[ad]==' '){ad3=0;}
  if(str[ad]!=' '){if(!ad3)ad2++; ad3=1;}
 }

 //
 return ad2;
}

// COPYIMAGE
jtmgl_copyimage(char *t,long xlen,long ylen,
                long xs,long ys,long xm,long ym,
                long xput,long yput)
{
 //
 jtmgl_copyimage1(t,xlen,ylen, xs,ys,xm,ym,xput,yput,
                  jtmscr.vbuffer,jtmscr.width,jtmscr.height);
}

// COPYIMAGE1
jtmgl_copyimage1(char *t,long xlen,long ylen,
                 long xs,long ys,long xm,long ym,
                 long xput,long yput,
                 unsigned char *vbuffer,long width,long height)
{
 //
 long x,y,ad,ad2;

 //
 for(y=0; y<(ym-ys); y++)
 {
  if( (y+yput)>=height ) break;
  ad=(y+ys)*xlen+xs;
  for(x=0; x<(xm-xs); x++)
  {
   if((x+xput)<width)
   {
    vbuffer[(y+yput)*width+(x+xput)]=t[ad+x];
   }
  }
 }
}

// MOSAICTILE1
jtmgl_mosaictile1(char *t,long xlen,long ylen,
                  long xs,long ys,long xm,long ym,
                  long xput,long yput,
                  unsigned char *vbuffer,long width,long height,
                  long bsize)
{
 //
 long x,y,ad,ad2,i,i2,i3,i4,xx,yy,ix,iy;

 //
 for(y=0,iy=0; y<(ym-ys); y++)
 {
  //
  if(!iy)
  {
   yy=y;
  }
  iy++;
  if(iy==bsize)iy=0;

  //
  if( (y+yput)>=height ) break;
  ad=(y+ys)*xlen+xs;
  ad2=(yy+ys)*xlen+xs;
  for(x=0,ix=0; x<(xm-xs); x++)
  {
   //
   if(!ix)
   {
    xx=x;
   }
   ix++;
   if(ix==bsize)ix=0;

   //
   if((x+xput)<width)
   {
    vbuffer[(y+yput)*width+(x+xput)]=t[ad2+xx];
   }
  }
 }
}

// TRANSPARENT COPYIMAGE
jtmgl_tpcopyimage(const unsigned char *t,const long xlen,const long ylen,
                const long xs,const long ys,const long xm,const long ym,
                const long xput,const long yput)
{
 //
 unsigned long x,y,ad,ad2;
 register unsigned long i,i2,i3,i4;

 //
 for(y=0; y<(ym-ys); y++)
 {
  if( (y+yput)>=jtmscr.height ) break;
  ad=(y+ys)*xlen+xs;
  jtmscr.srtab[y]=1;
  for(x=0; x<(xm-xs); x++)
  {
   if((x+xput)<jtmscr.width)
   {
    i=jtmscr.vbuffer[(y+yput)*jtmscr.width+(x+xput)];
    i2=t[ad+x];
    i3=(i+i2)>>1;
    jtmscr.vbuffer[(y+yput)*jtmscr.width+(x+xput)]=i3;
   }
  }
 }
}

// COPYIMAGE WITH MASK COLOR (The mask color won't be outputted on screen.
jtmgl_copyimagewithmaskcolor(char *t,long xlen,long ylen,
                long xs,long ys,long xm,long ym,
                long xput,long yput,unsigned int maskcolor)
{
 //
 long x,y,ad,ad2;

 //
 for(y=0; y<(ym-ys); y++)
 {
  if( (y+yput)>=jtmscr.height ) break;
  ad=(y+ys)*xlen+xs;
  jtmscr.srtab[y]=1;
  for(x=0; x<(xm-xs); x++)
  {
   if((x+xput)<jtmscr.width)
   {
    if(t[ad+x]!=maskcolor)
    {
     jtmscr.vbuffer[(y+yput)*jtmscr.width+(x+xput)]=t[ad+x];
    }
   }
  }
 }
}

// Notice: It handles strings. (Requires: CTYPE.H)
jtm_convert2lowercase(char *str,char *str2)
{
 long i;

 for(i=0; i<strlen(str); i++)
 {
  str2[i]=tolower(str[i]);
 }
 str2[i]=0;
}

// Notice: It handles strings.
jtm_convert2uppercase(char *str,char *str2)
{
 long i;

 for(i=0; i<strlen(str); i++)
 {
  str2[i]=toupper(str[i]);
 }
 str2[i]=0;
}


void jtm_getbuffer(int (*streamingfunction)(void),char *str,char stopmark,long maxlen)
{
 //
 long i;

 //
 for(i=0; i<maxlen-1; i++)
 {
  str[i]=streamingfunction();
  if(str[i]==stopmark)break;
 }
 str[i]=0;
}

/******** SLOW POLYGON FILLER ROUTINE *******/
// CAUTION: IT IS VERY SLOW AND UNREADY :>
// Note: It uses the 'line structure' called as LINESTR, which
// specifies two points A and B, which are specified by the variables
// (coordinates) x1,y1,x2,y2.
// ls.x1
// ls.y1
// ls.x2
/* ls.y2
 * Drawing starts so that the both 'active line' heads points to
 * POINT-A, active line contains two points, 'START' and 'END'
 * POINT-START starts to morph in POINT-B's direction,
 * POINT-END starts to morph in POINT-C's direction,
 * line will be draw in every while until POINT-START will reach POINT-B
 * and POINT-END will reach POINT-C so that the entire polygon gets drawn.
 */
drawpoly2(POLG *ls,long color)
{
 //
 //
 const int ADDT=1;
 long x,y,
      xs,ys,xd,yd,
      xs2,ys2,xd2,yd2,
      key,meter,x1,y1,x2,y2;

 //
 xs=ls[0].x2-ls[0].x1;
 ys=ls[0].y2-ls[0].y1;
 xs2=ls[0].x3-ls[0].x1;
 ys2=ls[0].y3-ls[0].y1;

 // both
 x1=ls[0].x1; y1=ls[0].y1;
 x2=ls[0].x1; y2=ls[0].y1;
 // both
 xd=x1<<8; yd=y1<<8;
 xd2=x2<<8; yd2=y2<<8;

 //
 for(meter=0; ; meter++)
 {
  //
  xd=xd+xs; yd=yd+ys;
  xd2=xd2+xs2; yd2=yd2+ys2;

  //
  x1=xd>>8; y1=yd>>8;
  x2=xd2>>8; y2=yd2>>8;

  //
  line2(x1,y1,x2,y2,color);

  //
  if(x1==ls[0].x2 && y1==ls[0].y2)break;
 }

}

// THIS IS THE SUPER FAST LINE DRAWING ROUTINE,
// OPTIMIZED FOR 256x256 DOT MATRIX, SO DO NOT ROLL OVER THE LIMIT
// OR YOU WILL CRASH THE MACHINE.
//#define DEBUG_LINE2
line2(long x1,long y1,long x2,long y2,long color)
{
 //
 long x,y,xs,ys,xd,yd,key,meter;

 //
 xs=x2-x1;
 ys=y2-y1;

 //
 x=x1; y=y1;
 xd=x<<8; yd=y<<8;

 //
 for(meter=0; ; meter++)
 {
  //
  xd=xd+xs;
  yd=yd+ys;

  //
  x=xd>>8; y=yd>>8;

#ifdef DEBUG_LINE2
  //
  cprintf("%d,%d\n\r",x,y);
  if( (key=jgetchim())==27 ) break;
#endif

#ifndef DEBUG_LINE2
  //
  jtmscr.vbuffer[y*jtmscr.width+x]=color;
#endif

  //
  if(x==x2 && y==y2)break;
 }

#ifdef DEBUG_LINE2
 //
 waitkey();
#endif
}


//*** END OF THE LIBRARY, DO NOT WRITE LINES AFTER THIS ENDIF
//*** IF YOU DO, PREPARE FOR THE ACTION WHICH OCCURS OF IT.
#endif
