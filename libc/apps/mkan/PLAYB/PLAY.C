//
//      PLAY program (V1.01)
//      --------------------
//
// Description:
//
//      This program plays animation files created with MKAN.
//      The animations files are designed to work on players
//      made for small platforms, like 8bit/16bit computers.
//      Animation format is following:
//
//      Resolution=160x100
//      Colors=4(16 dithered)
//      Compression: Frame comparing and akind of RLE.
//      Result: Usually every frame takes « of the original size
//              after compression.(Mainly up to the animation)
//
#define nograos
#define jtm_areqarg 2
#include <jtmdj.h>
int jmain(int argc,char *argv[])
{
 //
 FILE *f,*f2;
 long ad,ad2,ad3,ad4,ad5,ad6,ad7,ad8,len;
 long x,y,length;
 unsigned char *b,*b2,*a;
 char m1,m2,m3,m4;
 int AUTOPLAY=0,mkan_formatversion=-1;

 //
 a=malloc(1024*4096); // For the animation
 b=malloc(1024*64); // For the sfbmp output
 b2=malloc(1024*64); // ...

 //
 if(!fexist(argv[1]))
 {
  write("Couldn't find any matching file.\n");
 }
 else
 {
  //
  jtmgl_vgabootup(); jtm_greyscale256();

  //
  jtm_loadbin(argv[1],a);
  len=fsizeof(argv[1]);

repeat:
  //
  for(ad=0; ad<(1024*64); ad+=2)
  {
   b[ad+0]=a[2];
   b[ad+1]=0x00;
  }

  //
  if(b[ad+3]==0x53)
  {
   ad=3;
   mkan_formatversion=0; // As default version 0 means just
                         // an old version which
                         // didn't have version counting system yet,
                         // it might also mean possible error.
  }
  else
  {
   ad=4;
   mkan_formatversion=b[3];
  }

  //
  do
  {
   // Get atleast one frame
   //
   if(ad>=len)break;
//   if(ad>=0xffff)break;
   if(a[ad]!=0x53)break; // This actually means error !
   ad++;
   ad2=0;
   do
   {
    if(ad>=len)break;
//    if(ad>=0xffff)break;
    if(a[ad]==0x53)break;
    if(a[ad]==0x56) // "Yhdentavun monistus"
    {
     for(ad4=ad2; ad4<ad2+a[ad+1]*2; ad4+=2)b[ad4]=a[ad+2];
     ad2+=a[ad+1]*2;
     ad+=3;
    }
    else
    if(a[ad]==0x54)
    { // Warp --->
     ad2+=a[ad+1]*2;
     ad+=2;
    }
    else
    if(a[ad]==0x57 && mkan_formatversion!=0 )
    {// Uncompressed data ahead -->
     for(ad4=ad2,ad5=0; ad4<ad2+a[ad+1]*2; ad4+=2,ad5++)b[ad4]=a[ad+2+ad5];
     ad6=a[ad+1];
     ad2+=a[ad+1]*2;
     ad+=ad6+2;
    }
    else
    { // Absolute store. Version 0.00 or any other unknown format only.
//     jtm_setpalette(0,a[ad],0,0);
     b[ad2]=a[ad];
     ad++; ad2+=2;
    }
   }while(0==0);

   // SCBMP -> VGA
   for(y=0; y<200; y++)
   {
    ad2=y*320;
    for(x=0; x<320; x+=2)
    {
     jtmscr.vbuffer[ad2+x+0]=jtm_multiread(b,x,y)*(256/4);
     jtmscr.vbuffer[ad2+x+1]=jtm_multiread(b,x,y)*(256/4);
    }
   }
   jtm_waitsd(); jtm_vf(jtmscr.vbuffer);

   //
   if(AUTOPLAY){m1=jgetchim();}else{m1=jgetch();}
   if(m1=='a')AUTOPLAY=1-AUTOPLAY;
   if(m1==27)break;
   if(ad>=len)break;
  }while(0==0);

  //
 }
 if(m1==27)goto loppu;
 if(AUTOPLAY)goto repeat;
 m1=jgetch();
 if(m1==32)goto repeat;
loppu:
 return 0;
}
