/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
//
//      JTMOS SPS library
//
//

long jtmsps_compress(char *sname,char *oname,int cpower)
{
 //
 unsigned char *b1,*b2;
 FILE *f,*f2;
 long ad,ad2,ad3,ad4,siz,seglen=16,step=4;
 int i,i2,tdif,dif,dif2,dif3,dif4; // dif=difference ..

 //
 tdif=0;

 // Load binary data
 // write(sname[0]);
 if(strlen(sname)>=5)
 {
  if( sname[strlen(sname)-3]=='p' &&
      sname[strlen(sname)-2]=='c' &&
      sname[strlen(sname)-1]=='x')
  {
   siz=320*200;
   b1=malloc(siz);
   decode_pcxto(sname,b1,0);
   goto nobincase;
  }
 }
 siz=fsizeof(sname);
 b1=malloc(siz);
 jtm_loadbin(sname,b1);
nobincase:

 //

 //
 for(ad=0; ad<siz; ad++)
 {
  if( b1[ad]==0x54 )
  {
   b1[ad]=0x55;
  }
 }

 f2=jtm_fopen(oname,"wb");
 fputc(seglen,f2);
 // Laske vaihtelu arvo
 //
 for(ad=0,dif3=0; ad<siz; ad++)
 {
  if( b1[ad]!=b1[ad+1] )
  {
   if( b1[ad]<b1[ad+1] )
   {
    dif3=dif3+(b1[ad+1]-b1[ad]);
   }
   if( b1[ad]>b1[ad+1] )
   {
    dif3=dif3+(b1[ad]-b1[ad+1]);
   }
  }
 }

 // LETS COMPRESS ! :-)
 //
 for(ad=0; ad<siz; ad+=seglen)
 {
  if((ad+seglen)<siz)
  {
   i2=0;
   dif=0;
   for(ad2=ad; ad2<ad+(seglen-1); ad2++)
   {
    if( b1[ad2]!=b1[ad2+1] )
    {
     if( b1[ad2]<b1[ad2+1] )
     {
      i=(b1[ad2+1]-b1[ad2]);
     }
     if( b1[ad2]>b1[ad2+1] )
     {
      i=(b1[ad2]-b1[ad2+1]);
     }
     if(i>cpower)i2=1;
     dif=dif+i;
    }
    else
    {

    }
   }
   //
   tdif+=dif;
   //
   if( i2==0 ) // dif<128
   {
    // Specify a new resolution of saving data.
    fputc(0x54,f2);
    fputc(step,f2);
    // Save'n'convert from source to output(some information lost).
    for(ad2=ad+0; ad2<ad+seglen; ad2+=step)
    {
     fputc((b1[ad2]+b1[ad2+1]+b1[ad2+2]+b1[ad2+3])/4,f2);
    }
   }
   else
   {
    // Just dump the same information as in source image.
    for(ad2=ad; ad2<ad+seglen; ad2++)fputc(b1[ad2],f2);
   }
  }
  else
  {
   // Just dump the same information as in source image.
   for(ad2=ad; ad2<siz; ad2++)fputc(b1[ad2],f2);
  }
 }
 jtm_fclose(f2);
 return tdif;
}

// JTMSPS_DECOMPRESS function
// --------------------------
//
// The quality of image created is much under respond of the decompressor
// intelligence, how it can filter out the correct picture.
// Antialiasing and smooth helps alot.
//
// Decompression is made by from file to file decoding method.
//
jtmsps_decompress(char *sname,char *oname)
{
 //
 FILE *f,*f2;
 unsigned long ad,ad2,ad3,ad4,siz,siz2,step;
 int i,i2,seglen;
 unsigned char *b1,*b2,m1,m2,m3,m4;

 //
 siz=fsizeof(sname);
 f=jtm_fopen(sname,"rb");
 f2=jtm_fopen(oname,"wb");
 seglen=fgetc(f);
 do
 {
  m3=fgetc(f);
  if(m3==0x54)
  {
   step=fgetc(f);
   for(ad2=0; ad2<seglen; ad2+=step)
   {
    m1=fgetc(f);
    for(ad4=0; ad4<step; ad4++)fputc(m1,f2);
   }
  }
  else
  {
   m1=m3;
   fputc(m1,f2);
   for(ad2=1; ad2<step; ad2++)
   {
    fputc(fgetc(f),f2);
   }
  }
 }while(!feof(f));
 jtm_fclose(f2);
 fclose(f);
}
