/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTMJTX.H -- Library taken from old JXW.H programmed for
 *                                    Borland C/C++ v5.0
 * (C)1997 by Jari Tuominen, all rights reserved.
 *
 * JTM-OS colourful ansi text file format [.JTX]
 * ---------------------------------------------
 * JTX format is almost %100 same format as dos
 * [.TXT] file format, expect you have extended
 * fuctions in use, the list of function goes :
 *
 * [@<@>]                 Types mark "@"
 * [@<c 15>]              Changes textcolor to white
 * [@<C 01>]              Changes background color to blue
 * [@<.>]                 Stop and start waiting key press
 * [@<e command.com>]     Execute dos command
 * [@<t 03>]              Changes textmode (03=Textmode, 19=MCGA 256, 18=VGA 640x480 16c)
 * [@<s 03>]              Order computer to speak user with it's ugly voice
 * [@<\>]                 Clear whole screen
 * [@</>]                 Clear area inside the window
 * [@<!girls>]            Mark topic, used with topic search to find
 *                        topics, however, it is very useful, time saving,
 *                        but please remember that this function will be
 *                        normaly ignored, when you're about to show textfile
 *                        with fuction "SHOWJTX".
 */
void seekeofl_fp()
{
 char m5;
 //
 FILE *fp;
 do{
 m5=fgetc(fp);
 }while(m5!=13 && m5!=0);
 if(m5==13)fgetc(fp);
}

int showjtx(char *_fname)
{
 char str[255],str2[255],m1,m2,m3,m4;
 long ad,ad2;
 FILE *fp,*fp2;
 //
 if( (fp=fopen(_fname,"rb"))==NULL)
 {
  fclose(fp);
  return 1; //File doesn't exist, just return 1(=File not found).
 }
 do
 {
_nextone:
  if(feof(fp))break;
  m1=fgetc(fp);
  if(m1=='@')
  {
   /*
    *   Function('@'): Search for extended fuction option
    *
    *   Example: @<c15 c1>         
    */

   /*
    * Search for mark '<', which means command string begin
    */
   m2=fgetc(fp);
   if(m2!='<')exit(3);

_nextonec:
   if(feof(fp))exit(4);

   m2=fgetc(fp);
   if(m2==' ')
   {
    /*
     * Bypass space's, so they won't make any additional syntax errors
     */
    goto _nextonec;
   }
   if(m2=='!')
   {
    /*
     * Bypass shortcuts, so they won't make any additional syntax errors
     */
    goto _nextonec;
   }
   if(m2=='"')
   {//Center line, read "Hello" stylish string
    int _x=0;
    do{
     if(feof(fp))exit(7);
     m2=fgetc(fp);
     str[_x]=m2;
     _x++;
    }while(m2!='"');
    if(_x==1)exit(8);
    _x--; str[_x]=0;
    gotoxy(40-(strlen(str)/2),wherey());
    cprintf("%s\r\n",str);
    goto _nextonec;
   }
   if(m2=='|')
   {
    cprintf("\r\n");
    goto _nextonec;
   }
   if(m2=='>')
   {
    seekeofl_fp();
    goto _nextone;
   }
   if(m2=='\\')
   {
    textmode(C80);
    clrscr();
    goto _nextonec;
   }
   if(m2=='/')
   {
    clrscr();
    goto _nextonec;
   }
   if(m2=='@')
   {
    cprintf("@");
    goto _nextonec;
   }    
   if(m2=='e')
   {//Execute command:
    fscanf(fp,"%s",&str);
    system(str);
    goto _nextonec;
   }
   if(m2=='.')
   {
    //Wait key press ...
    waitkey();
    goto _nextonec;
   }
   if(m2=='c')
   {
    fscanf(fp,"%u",&ad);
    textcolor(ad);
    goto _nextonec;
   }
   if(m2=='C')
   {
    fscanf(fp,"%u",&ad);
    textbackground(ad);
    goto _nextonec;
   }
   if(m2=='t')
   {
    fscanf(fp,"%u",&ad);
    textmode(ad);
    goto _nextonec;
   }
   if(m2=='s')
   {
    strcpy(str,"speaker.exe ");
    fscanf(fp,"%s",&str2);
    strcat(str,str2);
    system(str);
    goto _nextonec;
   }
   exit(5);
  }
  cprintf("%c",m1);
 }while(!feof(fp));
 fclose(fp);
 return 0;
}
