//
#include <stdio.h>
#include "mkan.h"
#include "mkanPCX.h"

//
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
 ixl=xlen;
 iyl=ylen;

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



