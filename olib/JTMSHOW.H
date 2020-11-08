/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
  DATA SHOW library 97, version 1.00 (C)1997 by Jari Tuominen,
                                        all right reserved.
 */

#define showtype_txt    0       // Normal text file
#define showtype_c      1       // C source
#define showtype_jtx    2       // JTM-OS advanced text file format

void show(char *_fname)
{
 textcolor(BLACK);
 fp=fopen(_fname,"rb"); y=0;
 textcolor(YELLOW);
 do
 {
again:
  if(feof(fp))break;
  m1=fgetc(fp);
  if(m1==0x0d)
  {
   textcolor(YELLOW);
   fgetc(fp);
   cprintf("\n\r");
   y++;
   if(y>11)
   {
    textcolor(BLACK);
    waitkey();
    textcolor(YELLOW);
    y=0;
   }
   goto again;
  }
  if(isprint(m1)==0)
  {
   textcolor(RED);
   m1='.';
  }
  jtm_colorsetbymark(m1);
  m1=tolower(m1);
  cprintf("%c",m1);
 }while(!feof(fp));
 fclose(fp);
 textcolor(BLACK);
 waitkey();
}

void ConvertToLowerCase(char *fname1,char *fname2)
{
 FILE *fp1,*fp2;
 fp1=fopen(fname1,"rb"); if(fp1==NULL)exit(1);
 fp2=fopen(fname2,"wb"); if(fp2==NULL)exit(2);
 do
 {
  fputc(toupper(fgetc(fp1)),fp2);
 }while(!feof(fp1));
 fclose(fp1);
 fclose(fp2);
}
