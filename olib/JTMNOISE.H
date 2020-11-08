/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*** Noise killing special effects ***/
long jtmgl_countnoise(unsigned char *t,long width,long height,
                      long x1,long y1,long x2,long y2)
{
 //
 long ad,ad2,ad3,ad4,x,y,dif,total;

 //
 for(y=y1,total=0; y<y2; y++)
 {
  ad=y*width;
  for(x=x1; x<(x2-1); x++)
  {
   dif=abs(t[ad+x]-t[ad+x+1]);
   total=total+dif;
  }
 }
 return dif;
}

long jtmgl_countbignoise(unsigned char *t,long width,long height,
                      long x1,long y1,long x2,long y2)
{
 //
 long ad,ad2,ad3,ad4,x,y,dif,total;

 //
 for(y=y1,total=0; y<y2; y++)
 {
  ad=y*width;
  for(x=x1; x<(x2-1); x++)
  {
   dif=abs(t[ad+x]-t[ad+x+1]);
   if(dif>total)total=dif;
  }
 }
 return total;
}


//
jtmgl_mosaictile(unsigned char *t,
                 long width,long height,
                 unsigned char *t2,long x1,long y1,long x2,long y2)
{
 //

}

//
void jtmgl_killnoise(unsigned char *t,unsigned char *t2,long width,long height,long bsize)
{
 //
 long x,y,ad,ad2,ad3,ad4,i,i2,i3,i4;

 //
 for(y=0; y<height; y+=bsize)
 {
  ad=y*width;
  for(x=0; x<width; x+=bsize)
  {
   //
   i=jtmgl_countbignoise(t,width,height, x,y,x+bsize,y+bsize);

   //
   if(i>=(bsize*bsize/20))
   {
    jtmgl_copyimage1(t,width,height, x,y,x+bsize,y+bsize, x,y,
                     t2,width,height);
   }
   else
   if(i>=(bsize*bsize/30))
   {
    jtmgl_mosaictile1(t,width,height, x,y,x+bsize,y+bsize, x,y,
                      t2,width,height,
                      bsize/4);
   }
   else
   {
    jtmgl_mosaictile1(t,width,height, x,y,x+bsize,y+bsize, x,y,
                      t2,width,height,
                      bsize/2);
   }
  }
 }
}

