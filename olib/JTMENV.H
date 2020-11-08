#ifndef __JTMENV_H__
#define __JTMENV_H__
#include <jtmconv.h>
unsigned	char jgetosdrive();
       unsigned long int jgetenv_lastfoundoffset,
                         jgetenv_lastfoundoffset2,
                         jgetenv_lastfoundoffset3,
                         jgetenv_lastfoundlength;
extern char *jgetenv(const char *gr,const char *vr);
int jsetenv(const char *gr,const char *vr,const char *rvr);

// JSETENV function
// ----------------
//
//
// Example:
//
//      jsetenv("mygroup","myname","Jari");
//
// Description:
//
//      This function sets a system environment variable from specified
//      system environment group, if the group does not exist,
//      group will be created.
//      Notice: This function uses JGETENV function and many other functions
//              provided within JTMOS distribution package.
//      My idea is to make JGETENV / JSETENV seperate from JTMCTRL library
//      and make it available from JTMCONV, which makes possible to use
//      these functions with other compilers.
//      But there might be some problems with the memory using,
//      atleast for 16bit compilers, because of the conventional memory
//      available might be insufficient in some operations.
//      This is not tested yet.
//
//      Note: If the '_rvar' is empty(NULL) then the variable will be
//            removed, just like in any other similiar
//            environment variable functions.
//
int jsetenv(const char *gr,const char *vr,const char *rvr)
{
 //
 char jtmfn_syscfg[255];

 //
 strcpy(jtmfn_syscfg,jtm_getsyscfgfname());
 return jsetenv1(jtmfn_syscfg,gr,vr,rvr);
}

int jsetenv1(const char *__jtmfn_syscfg,const char *_group,const char *_var,const char *_rvar)
{
 //
 unsigned long int ad,ad2,ad3,ad4;
 int i,i2,i3,i4;
 FILE *f,*f2;
 char str[255],str2[255];

 //
 f=fopen(__jtmfn_syscfg,"rb");
 if(f==NULL)return 1;
 i=f2wordseek(f,"begin",_group);

 // Jos kysytty„ ryhm„„ ei l”ytynyt niin se luodaan.
 // Uusi Muuttuja luodaan tietenkin automaattisesti
 // koska ryhm„„ ei ollut ennen t„t„ operaatiota olemassa.
 if(i)
 {
  //
  if(!_rvar[0])return 0;

  //
  fseek(f,0,SEEK_SET);
  fclose(f);
  // Offsetti nelj„ n„ytt„„ "X:\n\r" j„lkeiseen kohtaan.
  ad=4;
  // Huomaa kirjoitusj„rjestys.
  sprintf(str,"end.\n");
  jtm_addstrinfileoff(jtmfn_syscfg,ad,str);
  sprintf(str,"%s= %s\n",_var,_rvar);
  jtm_addstrinfileoff(jtmfn_syscfg,ad,str);
  sprintf(str,"begin %s\n",_group);
  jtm_addstrinfileoff(jtmfn_syscfg,ad,str);
  return 0;
 } // Jos ryhm„ l”yty, niin aloitetaan muuttujan etsint„.
 else
 {
  // Pane t„m„n hetkisen tiedoston lukukohta (lukukohta n„ytt„„ ryhm„n alkuun)
  // muistiin my”hemp„„ k„ytt”„ varten.
  ad=ftell(f);
  // Luo '=' p„„tteinen merkkijono.
  sprintf(str,"%s=",_var);
  // Sulje tiedosto, jotta sen voisi avata uudelleen. :)
  fclose(f);
  // Kutsu jgetenv funktiota mik„ sitten ehtii kyseisen muuttujan,
  // jos se on olemassa.
  strcpy(str2,jgetenv(_group,str));
  // Tarkista l”ytyyk” kyseist„ muuttujaa kyseisest„ ryhm„st„.
  if(!str2[0] && _rvar[0])
  {// Jos ei l”ytynyt, niin muuttuja luodaan kyseiseen ryhm„„n.
   //
   // Aukaistaan muuttuja j„rjestelm„n yll„pito tiedosto.
   f=fopen(jtmfn_syscfg,"rb");
   // Hypp„„ tiedostossa siihen kohtaan miss„ sijaitsee ryhm„n alku
   f2wordseek(f,"begin",_group);
   // Pane lukukohta muistiin.
   ad=ftell(f);
   // Sulje tiedosto.
   fclose(f);
   // Rakenna merkkijono ja lis„„ muuttuja.
   sprintf(str,"\n%s= %s",_var,_rvar);
   jtm_addstrinfileoff(jtmfn_syscfg,ad,str);
   // Teht„v„ suoritettu.
   return 0;
  }
  else
  {// Jos muuttuja on jo olemassa kyseisess„ ryhm„ss„
   // niin vain sen sis„lt”„ muokataan.

   // Poista entinen arvo ...

   // Onko korvaava muuttuja itseasiassa tyhj„?
   // Jos on niin muuttuja pyyhit„„n.
   if(!_rvar[0])
   {
    jtm_removebytesfromfile(jtmfn_syscfg,jgetenv_lastfoundoffset3,jgetenv_lastfoundlength+1);
   }
   else
   {
    jtm_removebytesfromfile(jtmfn_syscfg,jgetenv_lastfoundoffset2,jgetenv_lastfoundlength+1);
    // Lis„„ nykyinen arvo.
    sprintf(str," %s",_rvar);
    jtm_addstrinfileoff(jtmfn_syscfg,jgetenv_lastfoundoffset2,str);
   }
  }
 }
closenow:
 fclose(f);
 return 0;
}

//
//
int jtm_getsyscfgfname(char *fn_sysenv)
{
 //
 strcpy(fn_sysenv,jtmfn_syscfg);
 if( !fexist(fn_sysenv) )
 {
  if( fexist("jtmos.cfg") )
  {
   strcpy(fn_sysenv,"jtmos.cfg");
  }
  else
  {
   return 1;
  }
 }
 return 0;
}

// JGETENV function
// ----------------
//
// Example :
//
//      strcpy(myname,jgetenv("mygroup","myname"));
//
//      myname <- char* jgetenv(..)
//
//
// Description :
//
// Gets JTMOS environment variable from c:\\jtmos.cfg.
// Notice: Function returns NULL terminated string if group/variable
//         doesn't exist.
//
char *jgetenv(const char *gr,const char *vr)
{
 //
 char fn_sysenv[255];

 //
 if(jtm_getsyscfgfname(fn_sysenv))return NULL;
 return jgetenv1(fn_sysenv,gr,vr);
}

char *jgetenv1(char *fn_sysenv,const char *_group,const char *_var)
{
 // Locals
 int sysin;
 long ad,ad2,ad3,ad4;
 FILE	*fp;
 unsigned char	_vari[255],str[255],ast[255],ast2[255],ast3[255],ast4[255],m1,m2,m3,m4;
 int groupfound,variablefound;

 // Init
 groupfound=0; variablefound=0;

#ifdef __DJGPP__
 sysin=jgetosdrive(); // MSDOS only!!!
#endif

 //
#ifdef DEBUG_JGETENV
 jtm_messagebuffer("* jgetenv1 call, debug :");
 sprintf(ast4,"fn_sysenv='%s', _group='%s', _var='%s',jtmfn_syscfg='%s'.",
 fn_sysenv,_group,_var,jtmfn_syscfg);
 jtm_messagebuffer(ast4);
#endif

 //
 fp=fopen(fn_sysenv,"rb");
 if(fp==NULL)return NULL;
 do
 {
nextgroup:
  fscanf(fp,"%s",&str);
  if(!strcmp(str,"begin"))
  {
	fscanf(fp,"%s",&str);
	if(!strcmp(str,_group))
	{
	 groupfound=1;
	 goto	searchprocess2;
	}
	if(feof(fp))goto	terror;
	goto nextgroup;
  }
 }while(!feof(fp));
terror:
 if(groupfound==0)
 {
  sprintf(ast4,"?Cannot find specified group(%s)",_group);
 }
 else
 {
  sprintf(ast4,"?Cannot find specified variable(%s)",_var);
 }
 jtm_messagebuffer(ast4);
 _vari[0]=0;
 goto	loppu;
searchprocess2:
 do
 {
  fscanf(fp,"%s",&_vari);
  if(!strcmp(_vari,"end.") || !strcmp(_vari,"end") || feof(fp))
  {
   _vari[0]=0;
   goto loppu;
  }
 }while(strcmp(_vari,_var));
 jgetenv_lastfoundoffset2=ftell(fp);

 // Etsi rivin alku ! (Tai tiedoston loppu..)
 ad=ftell(fp);
 while(!0)
 {
  if(!ftell(fp))break;
  m1=fgetc(fp); if(m1==0x0a || m1==0x0d)break;
  fseek(fp,-2,SEEK_CUR);
 }
 jgetenv_lastfoundoffset3=ftell(fp)+1;

 // Return position after information needed is received.
 fseek(fp,ad,SEEK_SET);

 //
 while((m1=fgetc(fp))==' '); fseek(fp,-1,SEEK_CUR);

 if(m1=='\"')
 {
  jtm_getstr(fp,_vari);
  goto loppu;
 }
 if(jgetenvth==64)
 {
  uni_getstr(fp);
  strcpy(_vari,str);
 }
 if(jgetenvth==0)
 {
  fscanf(fp,"%s",&_vari);
 }
 if(jgetenvth==1)
 {
  fscanf(fp,"%s",&_vari);
  fscanf(fp,"%s",&_vari);
 }
 if(jgetenvth==2)
 {
  fscanf(fp,"%s",&_vari);
  fscanf(fp,"%s",&_vari);
  fscanf(fp,"%s",&_vari);
 }
 jgetenv_lastfoundoffset=ftell(fp);
 jgetenv_lastfoundlength=strlen(_vari);
 fclose(fp);
#ifdef __DJGPP__
 if(_vari[0]=='%')_vari[0]=sysin;
#endif
loppu:
 return _vari;
} 
#endif
