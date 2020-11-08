/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
// An ability to execute program. "Quick execute"(Dirty code.. *<:D).
int QEXEC(char *apfn)
{
 //
 void (*arta)(int __ARGC,char *__APFN); // Application Run Time Address
 struct DLL *dll;
 char str[255],str2[255],str3[255],str4[255],str5[255],str6[255];
 long ad,ad2,ad3,ad4,argc;
 char *argv[200]; // 200 arguments max. .. Satisfied? :>

 // Count argument count (So that the application don't have to do it)
 argc=jtm_countargc(apfn);

 //
 sscanf(apfn,"%s",&str2);
 strcpy(str5,str2);

 // Filename with path?
 for(ad=0; ad<strlen(str2); ad++)
 {
  if(str2[ad]=='/' || str2[ad]==':')goto zapit;
 }

 // Quick search !
 for(ad=1; ad<10000; ad++)
 {
  sprintf(str3,"pos%u=",ad);
  strcpy(str4,jgetenv("cmdsearchlist",str3));
  if(!str4[0])break;
  sprintf(str,"%s/%s",str4,str2);
  if(jfexist(str))
  {
   strcpy(str2,str);
   goto yoo;
  }
 }
zapit:
 return 1;

 //
yoo:
 dll=dll_load(str2);
 if(!dll)
 {
  write("Error: File is not executable.\n");
  return 2;
 }
 arta=dll_lookup(dll,"_japmain");
 if(arta)
 {
  arta(argc,apfn);
 }
 else
 {
  write("Error: File not suitable for execution.\n");
 }
 dll_unload(dll);
 return 0;
}

