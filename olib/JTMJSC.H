/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
//
// ∞∞∞∞ ∞∞∞∞∞ ∞∞∞∞∞ ∞∞∞∞ ∞∞∞∞ ∞∞∞∞
//    ∞   ∞   ∞ ∞ ∞    ∞ ∞    ∞  ∞
//    ∞   ∞   ∞ ∞ ∞    ∞ ∞    ∞
//    ±   ±   ±   ±    ± ±±±± ±        "J script support library"
// ≤  ≤   ≤   ≤   ≤ ≤  ≤    ≤ ≤
// €  €   €   €   € €  €    € €  €     (C)1998 by Jari Tapio Tuominen,
// €€€€   €   €   € €€€€ €€€€ €€€€     all rights reserved.
//
#define JTMJSC_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

void jtmjsc_fgetvariable(FILE *f,char *ostr)
{
 char str[512],str2[512],str3[512],str4[512];
 long ad,ad2,ad3,ad4;
 //
 fscanf(f,"%s",&str);

 // ** JTMOS system environment variable ? **
 if( (str[strlen(str)-1]=='%') && ifstrcontainsmark(str,'.')==0 )
 {
  // str2 will contain group
  // str3 will contain member
  for(ad=0,ad2=0; ad2<strlen(str); ad++,ad2++)
  {
   if(str[ad2]=='.' )break;
   str2[ad]=str[ad2];
  } str2[ad]=0;

  ad3=ad2;
  for(ad=0,ad2=ad3+1; ad2<strlen(str); ad++,ad2++)
  {
   if(str[ad2]=='%' )break;
   str3[ad]=str[ad2];
  } str3[ad]=0;

  // "member=" not "member".
  strcat( str3,"=" );

  // Get JTMOS system environment variable!
  strcpy( ostr,jgetenv( str2,str3 ) );
 }

}

// jtmjsc_parser
// -------------
//
// If this function returns number 1, it means that the end of the program
// arrived(legally).
//
// If this function returns number 2, it means that "Syntax error" occured,
// and that command requested was unknown.
//
int jtmjsc_parser(FILE *f)
{
 // Locals
 char *str,*str2;
 int ReVal;

 // Allocate
 str=malloc(512);
 str2=malloc(512);

 // Init locals
 ReVal=2;

 //
 fscanf(f,"%s",str);
// sprintf(str2,"JTMOS Scripting Host: Now parsing %s.\n",str);
// jtm_addnotice(str2);

 if( ifstrcontainsmark(str,'}')==0 )  // Legal end of current function?
 {
  ReVal=1;
  goto loppu;
 }

 // Don't handle label definations
 if( str[strlen(str)-1]==':' )
 {
  ReVal=0;
  goto loppu;
 }

 //
 // Following script commands requires JTMSND! (Not available under Linux)
 //
#ifdef JTMSND_INCLUDED
 if(!strcmp(str,"PlayAudio"))
 {
  jtmjsc_fgetvariable(f,str);
  sprintf(str2,"JTMOS Scripting Host: Attempting to play audiofile \"%s\".\n",str);
  jtm_messagebuffer(str2);
  jtmsnd_startplay( str,WPLAY_NORMAL );
//  while( jtmsnd.sbsys.playmode!=WPLAY_STOP );
//  jtmsnd_stopplay();
  ReVal=0;
  goto loppu;
 }
#endif

 //
 // Following commands are fully independent.
 //

 // C kind of REM command ?
 if( !strcmp(str,"//") )
 {
  seektoeol(f);
  ReVal=0;
  goto loppu;
 }


 //
 //
 //




 //
 // Following commands use GraOS
 //

 // C kind of REM command ?
 if( !strcmp(str,"write") )
 {
  jtm_fgetstr(f,str2);
  write(str2);
  ReVal=0;
  goto loppu;
 }


 // Syntax error? -> Parser didn't recognize meaning of the command
 if(ReVal==2)
 {
  sprintf(str2,"JTMOS Scriting Host: Syntax error \"%s\".\n",str);
  jtm_addnotice(str2);
 }

loppu:
 // Free
 free(str);
 free(str2);
 return ReVal;
}

int jtmjsc_findfunction(FILE *f,char *strfunction)
{
 char str[512];
 //
 sprintf(str,"%s()",strfunction);
 f1wordseek(f,str);
 fscanf(f,"%s",&str);

/* if( ifstrcontainsmark(str,"{")==0 )
 {
  return 0;
 }
 else
 {
  return 1;
 }*/
 return 0;
}

/*
 * jtmjsc_executesript
 * -------------------
 *
 * Returns :
 *
 *  0 = Script successfully executed
 *  1 = File not found
 *  2 = Main function doesn't exist
 *  3 = Syntax error
 *  4 = Unexpected end of file
 *
 *
 */
int jtmjsc_executescript(char *fn_script)
{
 // Some locals
 int I1;
 FILE *f;
 unsigned char *str,*str2;
 int ReVal;

 // Allocate
 str=malloc(256);
 str2=malloc(256);

 //
 sprintf(str,"JTMOS Scripting Host event: Now attempting to execute script %s.",fn_script);
 jtm_messagebuffer(str);


 //
 ReVal=0;

 //
 f=fopen(fn_script,"rb");

 if(f==NULL) return 1;

 // main function must exist, if not there is no possibilities to execute
 // the program.
 if( jtmjsc_findfunction(f,"main")!=0 )
 {
  ReVal=2;
  goto legalend;
 }

 //
 do
 {
  if(feof(f))
  {
   ReVal=4;
   goto legalend;
  }
  I1=jtmjsc_parser(f);
  switch( I1 )
  {
   //
   case 1: // Legal end of file?
   ReVal=0;
   goto legalend;
   break;

   //
   case 2: // Syntax error?
   jtm_addnotice("JTMOS Scripting Host: Syntax error.\n");
   ReVal=3;
   goto legalend;
   break;

   //
   default:
   break;
  }
 }while(0==0);

legalend:
 // close
 fclose(f);

 // Free
 free(str);
 free(str2);

 return ReVal;
}

#ifdef __cplusplus
}
#endif
