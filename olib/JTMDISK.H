/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTMOS DISK library V1.00 (C)1997-98 by Jari Tuominen, all rights reserved.
 */

void jtm_dsmoveheadto(unsigned char *fname,unsigned track,unsigned sector,unsigned head,unsigned amount) // Moves head to specified position
{
 FILE *fp;
 long ad;
 char str[511];
 //
 for(ad=0; ad<512; ad++)str[ad]=0;
 for(ad=0; ad<strlen(fname); ad++)
 {
  str[ad]=fname[ad];
 }
 //
 str[0x100+0]=track&255;
 str[0x100+1]=track>>8;
 str[0x100+2]=sector&255;
 str[0x100+3]=sector>>8;
 str[0x100+4]=head&255;
 str[0x100+5]=head>>8;
 str[0x100+6]=amount&255;
 str[0x100+7]=amount>>8;
 //
 fp=fopen("ds.cfg","wb");
 fwrite(str, sizeof(char), 256*2, fp);
 fclose(fp);
}

void jtm_dswrite(char *fname,int track,int sector,int head,int amount) // Writes sector on disk A:
{
 char str[255];
 //
 sprintf(str,"%u,%u,%u <- %s\n",track,sector,head,fname);
 write(str);
 jtm_dsmoveheadto(fname,track,sector,head,amount);
 system("dswrite");
 //jdelete("ds.cfg");
}

void jtm_dsread(char *fname,int track,int sector,int head,int amount) // Writes sector on disk A:
{
 char str[255];
 //
 sprintf(str,"%u,%u,%u -> %s\n",track,sector,head,fname);
 write(str);
 jtm_dsmoveheadto(fname,track,sector,head,amount);
 system("dsread");
// jdelete("ds.cfg");
}

// 64KB file to disk (Only track number is specified)
// Usually used when writing KERNAL to diskette.
void jtm_dswritefile(char *fname,int track)
{
 int head=0;
 long ad=0,ad2=0;
 FILE *f,*f2;
 //
 do
 {
  // SPLIT FILE !
  f=fopen(fname,"rb");
  f2=fopen("temppi.tmp","wb");
  fseek(f,ad2,SEEK_SET);
  for(ad=0; ad<(512*18); ad++)
  {
   if(!feof(f))
   {
    fputc(fgetc(f),f2);
   }
   else
   {
    fputc(0x00,f2);
   }
  }
  fclose(f);
  //
  jtm_dswrite("temppi.tmp",track,1,head,512*18);
  //
  track++;
  head=1-head;
  ad2+=512*18;
  if(ad2>=0x10000)break;
 }while(ad==ad);
 jdelete("temppi.tmp");
}

