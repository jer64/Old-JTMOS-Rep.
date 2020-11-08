/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*

  File functions, that are compatible with most of ANSI C compatible compilers
  Library is decicated to work independed, no other libraries required.
  Not only file functions included but also many string operations.

*/

#ifndef __JTMFILE_H__
#define __JTMFILE_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jtmglvar.h>
#include <jtmstdio.h>

#ifndef __jtmdj_h_included
 #ifndef jtm_getch
  #define jtm_getch getch
 #endif

 #ifndef jgetch
  #define jgetch getch
 #endif
#endif

void seektoeol(FILE *_fp);
int seektomark(unsigned char _mark,FILE *_fp);
int jfexist(const char *_fname);
int fexist(const char *_fname);
extern int jdelete(char *_fname);
int xcopy(char *_source,char *_output);
extern long fsizeof(char *_fname);
extern void DumpAscFile(char *filename,void SelectedOutputConsole());

long fsizeof(char *_fname)
{
 //
 long _return;
 FILE *f;

 //
 f=fopen(_fname,"rb");
 if(f==NULL)
 {
  _return=0;
  goto loppu;
 }
 fseek(f,0,SEEK_END);
 _return=ftell(f);
 fclose(f);
loppu:
 return _return;
}

int jtm_query_RetryAbort()
{
 write("(R)etry (A)bort?");
 return jtm_getch();
}

int jtm_query_RetrySkip()
{
 write("(R)etry (S)kip?");
 return jgetch();
}

int jtm_query_YesNo()
{
 write("(Y)es (N)o?");
 return jgetch();
}

/*
 * JTMOS: I-XCOPY, Internal X-copy v1.0, (C)1997 by Jari Tuominen.
 */
int xcopy(char *_source,char *_output)
{
 // Locals
 long ad,ad2;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8,re;
 FILE *fp,*fp2,*fp3,*fp4;
 char ast[255],ast2[255],ast3[255],ast4[255],
      str[255];

 //
 re=0;

 //
 sprintf(str,"%s -> %s\n\r",_source,_output);
 write(str);

 //
 if(!fexist(_source))
 {
  re=1;
  goto loppu;
 }

 //
 if(fexist(_output))
 {
  write("File exist, would you like to overwrite the file, ");
  m1=jtm_query_YesNo();
  if(m1!='y')
  {
   write(" Skipping ...\n");
   goto loppu;
  }
  write(" Overwriting ...\n");
 }

 //
 ad=fsizeof(_source);
 ad2=0;

 //
 fp=fopen(_source,"rb");
 fp2=fopen(_output,"wb");
 if(fp2==NULL)
 {
  write("Cannot create output file.\n");
  re=1;
  goto loppu;
 }
 do{
  if(ad2>=ad)break;
  //
  if(fp2==NULL)
  {
   sprintf(ast4,"I.XCOPY: File \"%s\" not created.",_output);
   fclose(fp); fclose(fp2);
   break;
  }
  fputc(fgetc(fp),fp2);
  ad2++;
 }while(0==0);
 fclose(fp);
 fclose(fp2);
 if( !fexist(_output) ) re=1;
loppu:
 return re;
}

void jtm_fgetstr(FILE *fp,char *str)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp2,*fp3,*fp4;
 char str2[255];
 //
/*
   Usage:       F.e. jgetenv uses this routine when it reads
                jtmos.cfg, JTMOS system configuration file.               

   Notice:      Before you start to use this function,
                it's good to notice how the function really works,
                here is a simple example, check this out:

                jtm_fgetstr(fp,str);

   special marks :

    hello world                 Invalid string..

    "hello world"               Very simple string..

    "hello world\n"             String with enter code (0x0d) in end of it

    "cd \"                      Invalid mark

    "cd \\"                     Correct!

 */
//   seektomark('\"',fp);
   do
   {    
    m1=fgetc(fp);
    if(m1!=32)
    {
     if(m1!='\"')
     {
      fscanf(fp,"%s",&str);
      goto loppu;
     }
    }
   }while(m1!='\"');
   x=0;
   do
   {
    m1=fgetc(fp);
    if(m1=='\\')
    {
     m1=fgetc(fp);
     if(m1=='0')
     {
      m1=0x00;
      goto righton;
     }
     if(m1=='n') // Put cursor on new line
     {
      m1=0x0d;
      goto righton;
     }
     if(m1=='r') // Put cursor on left corner
     {
      m1=0x0e;
      goto righton;
     }
     fseek(fp,-2,SEEK_CUR);
     m1=fgetc(fp);
    }
    if(m1=='\"')break;
righton:
    str[x]=m1; x++;
   }while(m1!='\"');
   // Check if after mark '\"' is mark ';',
   // if that isn't true, add 0x0d(Enter) to end of string read before.
   m1=fgetc(fp);
   if(m1!=';')
   {
    str[x]=0x0d; x++;
    goto nextone11;
   }
   else
   {
//    fseek(fp,-1,SEEK_CUR);
   }
   seektoeol(fp);
nextone11:
   //Mark end of string
   str[x]=0;
loppu:
 while(0!=0);
}

void uni_getstr(FILE *fp)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp2,*fp3,*fp4;
 char str[255],str2[255];
 //
/*
   Usage:       F.e. jgetenv uses this routine when it reads
                jtmos.cfg, JTMOS system configuration file.               

   Notice:      Before you start to use this function,
                it's good to notice how the function really works,
                this is simple example, check this out:

   uni_getstr(fp);
   jtmglvar.h: char str[255];

   It reads string in JTMOS global variable str, system string #1

   special marks :

    hello world                 Invalid string..

    "hello world"               Very simple string..

    "hello world\n"             String with enter code (0x0d) in end of it

    "cd \"                      Invalid mark

    "cd \\"                     Correct!

 */
//   seektomark('\"',fp);
   do
   {    
    m1=fgetc(fp);
    if(m1!=32)
    {
     if(m1!='\"')
     {
      fscanf(fp,"%s",&str);
      goto loppu;
     }
    }
   }while(m1!='\"');
   x=0;
   do
   {
    m1=fgetc(fp);
    if(m1=='\\')
    {
     m1=fgetc(fp);
     if(m1=='0')
     {
      m1=0x00;
      goto righton;
     }
     if(m1=='n') // Put cursor on new line
     {
      m1=0x0d;
      goto righton;
     }
     if(m1=='r') // Put cursor on left corner
     {
      m1=0x0e;
      goto righton;
     }
     fseek(fp,-2,SEEK_CUR);
     m1=fgetc(fp);
    }
    if(m1=='\"')break;
righton:
    str[x]=m1; x++;
   }while(m1!='\"');
   // Check if after mark '\"' is mark ';',
   // if that isn't true, add 0x0d(Enter) to end of string read before.
   m1=fgetc(fp);
   if(m1!=';')
   {
    str[x]=0x0d; x++;
    goto nextone11;
   }
   seektoeol(fp);
nextone11:
   //Mark end of string
   str[x]=0;
loppu:
 while(0!=0);
}

void uni_getstri(FILE *_fp)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 char str[255];
 //
 seektomark('\"',_fp); x=0;
 do
 {
  m1=fgetc(_fp);
  if(m1=='\\')
  {
   m1=fgetc(_fp);
   if(m1=='0')
   {
    m1=0x00;
    goto righton;
   }
   if(m1=='n')
   {
    m1=0x0d;
    goto righton;
   }
  }
  if(m1=='\"')break;
righton:
  str[x]=m1; x++;
 }while(m1!='\"');
nextone11:
 //Mark end of string
 str[x]=0;
}











// Get string, way 1/2
void jtm_getstr(FILE *fp,char *str)
{
 //
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp2,*fp3,*fp4;
 char str2[255];

 //
   do
   {    
    m1=fgetc(fp);
    if(m1!=32)
    {
     if(m1!='\"')
     {
      fscanf(fp,"%s",&str);
      goto loppu;
     }
    }
   }while(m1!='\"');
   x=0;
   do
   {
rep:
    m1=fgetc(fp);
    if(m1=='\\')
    {
     m1=fgetc(fp);
     if(m1=='\"')
     {
      m1='\"';
      str[x]=m1; x++;
      goto rep;
     }
     if(m1=='0')
     {
      m1=0x00;
      goto righton;
     }
     if(m1=='n') // Put cursor on new line
     {
      m1=0x0d;
      goto righton;
     }
     if(m1=='r') // Put cursor on left corner
     {
      m1=0x0e;
      goto righton;
     }
     fseek(fp,-2,SEEK_CUR);
     m1=fgetc(fp);
    }
    if(m1=='\"')break;
righton:
    str[x]=m1; x++;
   }while(m1!='\"');
   // Check if after mark '\"' is mark ';',
   // if that isn't true, add 0x0d(Enter) to end of string read before.
   m1=fgetc(fp);
   if(m1!=';')
   {
    str[x]=0x0d; x++;
    goto nextone11;
   }
   seektoeol(fp);
nextone11:
   //Mark end of string
   str[x]=0;
loppu:
 while(0!=0);
}

// Get string, way 2/2
void jtm_getstri(FILE *_fp,char *str)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;

 //
 seektomark('\"',_fp); x=0;
 do
 {
rep:
  m1=fgetc(_fp);
  if(m1=='\\')
  {
   m1=fgetc(_fp);
   if(m1=='\"')
   {
    m1='\"';
    str[x]=m1; x++;
    goto rep;
   }
   if(m1=='0')
   {
    m1=0x00;
    goto righton;
   }
   if(m1=='n')
   {
    m1=0x0d;
    goto righton;
   }
  }
  if(m1=='\"')break;
righton:
  str[x]=m1; x++;
 }while(m1!='\"');
nextone11:
 //Mark end of string
 str[x]=0;
}












// JTM_MESSAGEBUFFER function
// --------------------------
//
// This functions is used to report something out when program/system crashes,
// so it is aimed to debug processing, but is also usable with any
// other informating, like silent reporting(doesn't disturb user).
//
void jtm_messagebuffer(char *_message)
{
 /*
  *  C:/JTMOS.MSB       Message buffer, this is the file where all messages
  *                     will be stored, for example JTM_KILLPROCESSS;
  *                     uses this buffer, when it needs to tell user why
  *                     it had to kill the process, or why however called.
  *                     Buffer stays alive even if application/system crashes.
  */
 FILE *_fp;
 long ad,ad2,ad3,ad4;

 //
 _fp=fopen("c:/jtmos.msb","ab");
 fprintf(_fp,"msg%x: \"%s\" \n",systemtimer.counterx,
                                  _message);
 fclose(_fp);
}

int seektomark(unsigned char _mark,FILE *_fp)
{
 /*

   Returns -1 only if end of file reached before specified mark founded.


  */
 register unsigned char _m1;
 do
 {
  if(feof(_fp))
  {
   return -1;
  }
  _m1=fgetc(_fp);
 }while(_m1!=_mark);
 return 0;
}

int seektomarks(unsigned char *mrks,FILE *_fp)
{
 /*

   Returns -1 only if end of file reached before specified mark founded.


  */
 //
 register unsigned char _m1;
 long ad,ad2;

 //
 while(!0)
 {
  if(feof(_fp))
  {
   return -1;
  }
  _m1=fgetc(_fp);
  for(ad=0; ad<strlen(mrks); ad++)if(_m1==mrks[ad])break;
 }
 return 0;
}

// Seek to the end of file
void seektoeol(FILE *_fp)
{
 register unsigned char _m1;
 do
 {
  _m1=fgetc(_fp);
 }while(_m1!=0x0d);
}

// Look out for next mark, without moving buffer forward:
unsigned lnext(FILE *_fp) 
{
 register unsigned char _m1;
 _m1=fgetc(_fp); fseek(_fp,-1,SEEK_CUR);
 return _m1;
}

//Example: chevar("myvars.vba","name1=");
char *chevar(char *_fname,char *_var)
{
 char *_var1;
 FILE *_fp;
 _fp=fopen(_fname,"rb");
 do
 {
  fscanf(_fp,"%s",&_var1);
  if(_var1[strlen(_var1)-1]=='=')
  {
   if(!strcmp(_var1,_var))
   {
    fscanf(_fp,"%s",_var1);
    return _var1;
   }
  }
 }while(!feof(_fp));
 fclose(_fp);
 return NULL; // Return NULL if variable not found
}

void jtm_killspaces(char *stringi)
{
unl:
 if( stringi[strlen(stringi)-1]==' ' )
 {
  stringi[strlen(stringi)-1]=0;
  goto unl;
 }
}

void DumpAscFile(char *filename,void SelectedOutputConsole())
{
 FILE *f,*f2;
 f=fopen(filename,"rb");
 do
 {
#ifdef DJGPP
  cprintf("%c",fgetc(f));
#else // Well, lets try to trick it work under UN*X too :-)
  printf("%c",fgetc(f));
#endif
  //SelectedOutputConsole(fgetc(f));
 }while(!feof(f));
 fclose(f);
}

void jtm_putc(char merkki)
{
 printf("%c",merkki);
}

int jmkdir(char *dirn)
{
 return mkdir(dirn);
}

int jtm_loadbin64(char *_fname,char	*_tab)
{
 FILE	*_fp;
 long	_ad=0,re=0;
 //
 _fp=fopen(_fname,"rb");
 if(_fp==NULL)
 {
  re=1;
  goto nofile;
 }
 do
 {
  _tab[_ad]=fgetc(_fp)/4;
  _ad++;
 }while(!feof(_fp));
 fclose(_fp);
nofile:
 return re;
}

int jtm_loadbin(char	*fname,char	*_tab)
{
 FILE	*fp;
 long	_ad=0,re;
 //
 re=0;
 fp=fopen(fname,"rb");
 if(fp==NULL)
 {
  re=-1;
  goto nofile;
 }
 do
 {
  _tab[_ad]=fgetc(fp);
  _ad++;
 }while(!feof(fp));
 fclose(fp);
nofile:
 return re;
}

int jtm_loadbinao(char	*fname,char	*_tab,unsigned long length)
{
 FILE	*fp;
 long	_ad=0,re;
 //
 re=0;
 fp=fopen(fname,"rb");
 if(fp==NULL)
 {
  re=-1;
  goto nofile;
 }
 do
 {
  if(_ad>=length)break;
  _tab[_ad]=fgetc(fp);
  _ad++;
 }while(!feof(fp));
 fclose(fp);
nofile:
 return re;
}

unsigned	jtm_loadbinc64(char *_fname,char	*_tab)
{
 FILE	*_fp;
 long	_ad=0,re=0;
 long	a1,a2,a3,a4;
 //
// goto avoidit;
 _fp=fopen(_fname,"rb");
 a1=fgetc(_fp); a2=fgetc(_fp);
 a3=(a2<<8)|a1;
 if(_fp==NULL)
 {
  re=1;
  goto nofile;
 }
 do
 {
  _tab[_ad]=fgetc(_fp);
  _ad++;
 }while(!feof(_fp));
 fclose(_fp);
nofile:
avoidit:
 return re;
}

int jtm_savebin(char	*_fname,unsigned char *_tab,long	length)
{
 FILE	*_fp;
 long	_ad=0,re=0;
 //
 _ad=0;
 _fp=fopen(_fname,"wb");
 if(_fp==NULL)
 {
  re=1;
  goto nofile;
 }
 do
 {
  fputc(_tab[_ad],_fp);
  _ad++;
 }while(_ad<length);
 fclose(_fp);
nofile:
 return re;
}

int jtm_savebinc64(char	*_fname,unsigned char *_tab,long	length,unsigned address)
{
 FILE	*_fp;
 long	_ad=0,re=0;
 //
 re=0;
 _ad=0;
 _fp=fopen(_fname,"wb");
 if(_fp==NULL)
 {
  re=1;
  goto nofile;
 }
 fputc(address&0xff,_fp);
 fputc(address>>8,_fp);
 do
 {
  fputc(_tab[_ad],_fp);
  _ad++;
 }while(_ad<length);
 fclose(_fp);
nofile:
 return re;
}

int jtm_loadbinary(char	*_fname,char *_tab)
{
 FILE	*_fp;
 long	_ad=0;
 //
 _fp=fopen(_fname,"rb");
 do
 {
  _tab[_ad]=fgetc(_fp);
  _ad++;
 }while(!feof(_fp));
 fclose(_fp);
 return 0;
}

#ifdef DJGPP
/*

JTM_SETFILEATTR(filename,attribute);       -- DJGPP only!

Description:

The function changes attribute of the file.
This function will be replaced with JTMOS's own function,
but it is now temporarily working under DOS.H(DOS functions).

Example :

        jtm_setfileattr("c:\\jtmos.cfg",_A_NORMAL|_A_RDONLY|_A_SYSTEM);




`_A_NORMAL (0x00)'
     Normal file (no read/write restrictions)

`_A_RDONLY (0x01)'
     Read only file

`_A_HIDDEN (0x02)'
     Hidden file

`_A_SYSTEM (0x04)'
     System file

`_A_VOLID (0x08)'
     Volume ID file

`_A_SUBDIR (0x10)'
     Subdirectory

`_A_ARCH (0x20)'
     Archive file
*/
unsigned int jtm_setfileattr(const char *filename,unsigned int attr)
{
 _dos_setfileattr(filename,attr);
}
#endif

#ifdef DJGPP
#include <dir.h>
long jtm_dirtofile(unsigned char *ryhma,unsigned char *fname)
{
 //
 long amount;
 int done;
 struct ffblk fdir;
 FILE *f,*f2;

 //
 amount=0;
 done=findfirst(ryhma,&fdir,0x0);
 f=fopen(fname,"wb");
 while(!done)
 {
  fprintf(f,"%s\n",fdir.ff_name);
  amount++;
  done=findnext(&fdir);
 }
 fprintf(f,"**\n");
 fclose(f);
 return amount;
}

long jtm_dirtobuf(unsigned char *ryhma,unsigned char *buf)
{
 //
 long amount;
 int done;
 struct ffblk fdir;
 FILE *f,*f2;
 char str[255],str2[255];

 //
 amount=0;
 done=findfirst(ryhma,&fdir,FA_ARCH|FA_RDONLY);
 while(!done)
 {
  (char*)buf[amount]=malloc(strlen(fdir.ff_name)+2);
  strcpy((char*)buf[amount],fdir.ff_name);
  amount++;
  done=findnext(&fdir);
 }
 return amount;
}

void jtm_reldirbuf(char *buf,long amount)
{
 //
 long ad,ad2;

 //
 for(ad=0; ad<amount; ad++)
 {
  free(buf[ad]);
 }
}

/*
 * Before using function, allways remember to put f.e. "c:" at the beginning
 */

unsigned	char jgetosdrive()
{
 FILE	*_fp;
 unsigned char	_driveletter;
 //
 _fp=fopen(jtmos_cfg,"rb");
 _driveletter=fgetc(_fp);
 fclose(_fp);
 return _driveletter;
}
#endif

int jtm_chdir(char *path)
{
 return chdir(path);
}

long fchksum32(char *fname)
{
 //
 char str[255];
 unsigned long ad;
 FILE *fp;

 //
 ad=0;
 fp=fopen(fname,"rb");
 if(fp==NULL)return -1;
 do
 {
  ad=fgetc(fp)+ad;
 }while(!feof(fp));
 fclose(fp);
 return ad;
}


// JTM_CHECKDLL function
// Action: The function makes a check in DLL system registry.
// If it founds an active and registed DLL named same as specified
// DLL, it'll return non zero(Just like strcmp f.e.).
// The DLL must be located at exactly same location(path),
// or otherways it'll not recognize it exactly same,
// even if it would match in byte compare.
jtm_checkdll(char *fname)
{
 //
 long ad,ad2,ad3,ad4;

 //

}

// Cut'n'shorten. Removes first and last mark from the string so that
// that the string length in destination string is two marks less than
// in the source string.
jtm_cutnshorten(const char *str,char *str2)
{
 //
 long i,i2;

 //
 for(i=0,i2=0; i<strlen(str); i++)
 {
  if(i && i<(strlen(str)-2))
  {
   str2[i2]=str[i];
   i2++;
  }
 }
 str2[i2]=0;
}







/*** File handling routines, works under Linux ***/
int fexist(const char *_fname)
{
 //
 FILE *_fp;
 int retu=0;
 char str[255];

 //
 _fp=fopen(_fname,"rb");
 if(_fp==NULL)
 {
  retu=0;
 }
 else
 {
  fclose(_fp);
  retu=1;
//  sprintf(str,"fde:%s.",_fname);
//  jtm_messagebuffer(str);
 }
 return retu;
}
#endif
