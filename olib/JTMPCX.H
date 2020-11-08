/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
int decode_pcx(char *_srcfname,char *_outfname,unsigned char _paletteadd)
                // Last parameter = Add palette (0=False, 1=True)
{
 /*
  **** Decode PCX image to JCO v26 image = Uncompressed PCX & fine format
  *
  * So the final image format will be :
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

 FILE *_fp,*_fp2;
 _fp= fopen(_srcfname,"rb");
 _fp2=fopen(_outfname,"wb");
 fseek(_fp,4+4,SEEK_SET);

 // Convert to ad
 ad3=fgetc(_fp); ad4=fgetc(_fp); ad=(ad4*256)|ad3;  //fprintf(_fp2,"%u ",ad);
 // Convert to ad2
 ad3=fgetc(_fp); ad4=fgetc(_fp); ad2=(ad4*256)|ad3; //fprintf(_fp2,"%u ",ad2);
                                                  
 fseek(_fp,pcx_header,SEEK_SET);
 x=0;
 while(!feof(_fp))
 {
  m1=fgetc(_fp);
  if((m1&0xc0)==0xc0)
  {
   m2=fgetc(_fp);
   //Duplicate charater
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
