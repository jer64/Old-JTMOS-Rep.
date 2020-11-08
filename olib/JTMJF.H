#ifndef __JTMJF_H__
#define __JTMJF_H__
//
int jdll_getamount()
{
 return jdllreg.amount;
}

//
char *jdll_getentry(int which)
{
 if(which>jdllreg.amount)return NULL;
 return jdllreg.fname[which];
}

//
struct DLL *jdll_load(const char *filename)
{
 //
 char str[255],str2[255];
 int i,i2;

 // Already existing DLL?
 for(i=0; i<jdllreg.amount; i++)
 {
  if(!strcmp(filename,jdllreg.fname[i]))
  {
   return jdllreg.funct[i];
  }
 }

 //
 jdllreg.fname[jdllreg.amount]=malloc(256);
 strcpy(jdllreg.fname[jdllreg.amount],filename);

 //
 jtm_translatefilename(filename,str);

 //
#ifdef DLL_INCLUDED
 jdllreg.funct[jdllreg.amount]=dll_load(str);
#endif

 //
// write("[DLL LOAD] "); write(filename); write("\n");
// write("[........] "); write(str); write("\n");

 //
 jdllreg.amount=jdllreg.amount+1;

 //
 sprintf(str2,"JDLL_LOAD: Amount = %u\n",jdllreg.amount);
 write(str2);

 //
 return jdllreg.funct[jdllreg.amount-1];
}


/*** BEGINNING OF JFILE FUNCTIONS ***/
// *****************************************************************
//
//  Quick file manipulating functions(Will be later modified alot).
//
// *****************************************************************
//
int jfeof(JFILE *file)
{
 return feof(file[0].file);
}

//
//
int jtm_findjefpos(JTMEXTFIL *_jtmextfil,const char *str)
{
 //
 long i,i2;

 //
 for(i=0,i2=0; i<_jtmextfil[0].drvamount; i++)
 {
  if(!strcmp(_jtmextfil[0].drvinf[i],str))
  {
   i2=1; break;
  }
 }
 if(i2)return i; else return -1;
}

// jfile:fopen
//
JFILE *jfopen(const char *filename, const char *mode)
{
 //
 char str[255],str2[255],str3[255],str4[255],
      ast[255],ast2[255],ast3[255],ast4[255];
 FILE *f;
 JFILE *jf;
 int i,i2,i3,i4;
 JFILE *(*func)(const char *__filename,const char *__mode);
 void (*funcinit)();

 // Handle drive
 i4=jtm_translatefilename(filename,str);
// sprintf(str2,"%u - %s\n",i4,str);
// write(str2); waitkey();
 switch( i4 )
 {
  // If it's same
  case 0:
  jf=malloc(sizeof(JFILE));
  //
  jf[0].file=fopen(str,mode);
  jf[0].compression=1; // No compression
  jf[0].type=1; // DOS handled
  return jf;

  // If it's changed
  case 1:
  jf=malloc(sizeof(JFILE));
  //
  jf[0].file=fopen(str,mode);
  jf[0].compression=1; // No compression
  jf[0].type=1; // DOS handled
  return jf;

  // If it's DLL based
  case 2:
  // "iojgejergioj" -> iojgejergioj
  jtm_cutnshorten(str,str2);
  // ^^Okay, so now we have got the DLL filename^^

  // Cut drive specifier string from filename like this:
  // foodriver:filename -> foodriver
  for(i=0; i<strlen(filename); i++)
  {
   if(filename[i]==':')break;
   ast[i]=filename[i];
  }
  ast[i]=0;

  // Check if DLL exists, roll over all DRIVES until match to
  // 'ast' drive found.
  i=jtm_findjefpos(&jtmextfil,str);
  //
  if(i!=-1) // ?? DLL
  {
   // DRIVE registed in offset I on jtmextfil registry.
   // Install DLL if doesn't exist, yet.
   if(!jtmextfil.drvdll[i])
   {
    jtmextfil.drvdll[i]=jdll_load(str2);
   }
   if(!jtmextfil.drvdll[i])
   {
    jtm_messagebuffer("Error while loading DLL.");
    goto terridll;
   }

   //
#ifdef DLL_INCLUDED
   funcinit=(void (*)())dll_lookup(jtmextfil.drvdll[i], "_initdll");
#endif
   if(funcinit)
   {
    funcinit();
   }
   else
   {
    jtm_messagebuffer("Error while DLL init.");
    goto terridll;
   }

   // Call DLL's dll_jfopen function.
#ifdef DLL_INCLUDED
   func=(JFILE *(*)(int,int))dll_lookup(jtmextfil.drvdll[i], "_dll_jfopen");
#endif
   if(func)
   {
    return func(filename,mode);
   }
   else
   {
terridll:
    jtm_messagebuffer("ERROR: UNABLE TO HANDLE JFOPEN CALL. - PAR :");
    jtm_messagebuffer(str2);
    return NULL;
   }
  }
  else
  {
   write("BOING ZHU THA .. Drive not found !\n");
   write("Impossible, this was never meant to happend.\n");
   write("Should I call this an error message? I do not know, sorry, :>\n");
   waitkey();
   return NULL;
  }
  return NULL;

  //
  default:
  return NULL;
 }
}

//
int jfclose(JFILE *file)
{
 //
 int reval;

 //
 switch(file[0].type)
 {
  //
  case 1:
  reval=fclose(file[0].file);
  free(file);
  break;
  //
  case 2:
  break;
 }
 return reval;
}

//
char *jfgets(char *buffer, int maxlength, JFILE *file)
{
 return fgets(buffer,maxlength,file[0].file);
}

int jfputs(const char *string, JFILE *file)
{
 return fputs(string,file[0].file);
}

//
int jfgetc(JFILE *file)
{
 return fgetc(file[0].file);
}

//
int jfputc(int character, JFILE *file)
{
 return fputc(character,file[0].file);
}

//
int jfseek(JFILE *file, long offset, int mode)
{
 return fseek(file[0].file,offset,mode);
}
/**** END OF JFILE FUNCTIONS ****/

int jfindfirst(const char *pathname, struct ffblk *ffblk, int attrib)
{
 //
 int i;
 char str[255],str2[255];

 //
 i=jtm_translatefilename(pathname,str);

 //
 switch(i)
 {
  /*** DLL CASING **/
  case 2:
  ffblk[0].ff_name[250]='d';
  ffblk[0].ff_name[251]='l';
  ffblk[0].ff_name[252]='l';
  write("JFINDFIRST/NEXT: DLL casing: Not yet supported.\n");
  return 1;

  /*** DOS CASING ***/
  default:
  ffblk[0].ff_name[250]='d';
  ffblk[0].ff_name[251]='o';
  ffblk[0].ff_name[252]='s';
  return findfirst(str,ffblk,attrib);
 }
}

int jfindnext(struct ffblk *ffblk)
{
 //
 int i;
 char str[255],str2[255];

 // DOS casing?
 if( ffblk[0].ff_name[250]=='d' &&
     ffblk[0].ff_name[251]=='o' &&
     ffblk[0].ff_name[252]=='s')return findnext(ffblk);

 // DLL casing?
 return 1;
}

/*** jfexist ***/
int jfexist(const char *fname)
{
 //
 char str[255];
 JFILE *_fp;
 int retu=0;

 //
 _fp=jfopen(fname,"rb");
 if(_fp==NULL)
 {
  retu=0;
 }
 else
 {
  jfclose(_fp);
  retu=1;
 }
 return retu;
}

// JFILE version
void jcatdoc(const char *fname,int _WAITKEY)
{
 //
 char m1,m2,m3,m4;
 int i,lines,key;
 JFILE *f;

 //
 lines=0;

 //
 f=jfopen(fname,"rb");

 //
 do
 {
  //
  key=jgetchim();
  if(key==27)break;

  //
  if(jfeof(f))break;
  m1=jfgetc(f); if(m1==0)break;
  cprintf("%c",m1);
  if(m1==13)lines++;
  if(lines>=20)
  {
   lines=0;
#ifdef JTMCTRL_INCLUDED
   cprintf("\n\r"); if(_WAITKEY)waitkey();
#endif
  }
 }while(0==0);
 if(lines>1)
 {
#ifdef JTMCTRL_INCLUDED
  cprintf("\n\r"); if(_WAITKEY)waitkey();
#endif
 }

 //
 jfclose(f);
 return;
}
#endif

// JTM_INITEXTFIL - Initializes Extended Filing System
// (Adds some drives to the file system)
jtm_initextfil(JTMEXTFIL *_jtmextfil)
{
 //
 char str[255],str2[255],str3[255],str4[255];
 long ad,ad2,ad3,ad4;

 //
 _jtmextfil[0].state=1;
 _jtmextfil[0].drvamount=jgetenvgroupmemberlist("drives",_jtmextfil[0].drvlst);

 // Load Device drives / Get mirrorred path targets
 for(ad=0; ad<_jtmextfil[0].drvamount; ad++)
 {
  sprintf(str,"%s=",_jtmextfil[0].drvlst[ad]);
  strcpy(str2,jgetenv("drives",str));
#ifdef DEBUG_INITEXTFIL
  write(str); write(str2); write("\n"); waitkey();
#endif
  switch(str2[0])
  {
   // A device driver based drive ?
   case '\"':
   // -Ahh, not yet done <:)
   _jtmextfil[0].drvtyp[ad]=2;
   _jtmextfil[0].drvinf[ad]=malloc(256);
   strcpy(_jtmextfil[0].drvinf[ad],str2);
   break;

   // A mirror path ?
   default:
   _jtmextfil[0].drvtyp[ad]=1;
   _jtmextfil[0].drvinf[ad]=malloc(256);
   strcpy(_jtmextfil[0].drvinf[ad],str2);
   break;
  }
 }
}

//
int jtm_shutdownextfil(JTMEXTFIL *_jtmextfil)
{
 //
 long ad,ad2,ad3,ad4;

 //
 if(_jtmextfil[0].state)
 {
  for(ad=0; ad<_jtmextfil[0].drvamount; ad++)
  {
   switch(_jtmextfil[0].drvtyp[ad])
   {
    // Drive is a Path Mirror based
    case 1:
    free(_jtmextfil[0].drvlst[ad]); // Free Name
    free(_jtmextfil[0].drvinf[ad]); // Free Mirror Path string
    break;
    // Drive is a Driver based
    case 2:
    free(_jtmextfil[0].drvlst[ad]); // Free Name
    free(_jtmextfil[0].drvdll[ad]); // Free DLL
    break;
    //
    default:
    break;
   }
  }
  // Mission completed ! d:)
  _jtmextfil[0].state=0;
  return 0;
 }
 else
 {
  return -1;
 }
}
// Translatefilename function,
// Return values are following :
// 0: If old filename=new filename (No change)
// 1: Drive points to a path converted
// 2: Drive points to a DLL
int jtm_translatefilename(char *ofname,char *nfname) // OLD -> NEW
{
 //
 long ad,ad2,ad3,ad4;
 char str[255],str2[255],str3[255],str4[255];

 //
 for(ad=0,ad2=0; ad<strlen(ofname); ad++)
 {
  if(ofname[ad]==':')
  {
   if(ad>=2)ad2=1;
   break;
  }
 }

 // If no drive letter.
 if(!ad2)
 {
casing:
  strcpy(nfname,ofname);
  return 0;
 }
 else
 {
  for(ad=0,ad2=0; ad<strlen(ofname); ad++)
  {
   if(ofname[ad]==':')break;
   str[ad2]=ofname[ad];
   ad2++;
  }
  str[ad2]=0;
  ad3=0;
  ad++;
  while(!0)
  {
   if(!ofname[ad])break;
   str4[ad3]=ofname[ad];
   ad3++; ad++;
  }
  str4[ad3]=0;
  // YES!! STR4 has now the rest of filename ->    xxxx:filename
  //                                               vvvv ^^^str4^

  //
  for(ad=0; ad<jtmextfil.drvamount; ad++)
  {
   if(!strcmp(str,jtmextfil.drvlst[ad]))
   {
    // DLL case
    if(jtmextfil.drvinf[ad][0]=='\"')
    {
     strcpy(nfname,jtmextfil.drvinf[ad]);
     return 2;
    }

    // Get new path
    strcpy(str2,jtmextfil.drvinf[ad]);
    strcat(str2,str4);
    strcpy(nfname,str2);
    return 1;
   }
  }
  goto casing;
 }
}

int jrm(char *_fname)
{
 return jdelete(_fname);
}

int jdelete(char *_fname)
{
 char str[255];

 jtm_translatefilename(_fname,str);
 return remove(str);
}


void jchdir(char *path)
{
 char str[255];

 jtm_translatefilename(path,str);
 return chdir(str);
}
/** END OF JTMJF_H **/
