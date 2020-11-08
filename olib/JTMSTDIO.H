/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 *
 *      JTMOS standard I/O library
 *
 *
 */


FILE *jtm_fopen(const char *filename, const char *mode)
{
 //
 if(jtmstdio.aohandles>4999)return NULL;

 //
 jtmstdio.handles[jtmstdio.aohandles]=fopen(filename,mode);

 //
 if(jtmstdio.handles[jtmstdio.aohandles]==NULL)return NULL;

 //
 jtmstdio.filesopen++;
 jtmstdio.aohandles++;

 //
 return jtmstdio.handles[jtmstdio.aohandles-1];
}

int jtm_fclose(FILE *file)
{
 //
 unsigned long int ad,ad2,ad3,ad4;
// if(!jtmstdio.filesopen)return -1;

 //
// cprintf("TRACE CALL: JTM_FCLOSE(%1.8x);\n\r",(unsigned long int)file);
// waitkey();

 //
 for(ad=0,ad2=0; ad<jtmstdio.aohandles; ad++)
 {
  if(jtmstdio.handles[ad]==file)
  {
   jtmstdio.handles[ad]=NULL; /// Remove information of an ex-handle.
   ad2++;
   break;
  }
 }
 if(!ad2)
 {
//  jtmscr_blinkbackground();

  // ADD YOUR ERROR HANDLER HERE !!
 }

 //
 jtmstdio.filesopen--;

 //
 fclose(file);
 return 0;
}


void jtm_showstdiodebugreport(void)
{
 //
 unsigned long int ad,ad2,ad3,ad4;
 char m1,m2,m3,m4,str[255];

 //
 sprintf(str,"JTMSTDIO debug report\n"); write(str);
 sprintf(str,"---------------------\n"); write(str);
 sprintf(str,"%u files open.\n",jtmstdio.filesopen); write(str),
 sprintf(str,"%u handles reserved.\n",jtmstdio.aohandles); write(str);
 sprintf(str,"Press 's' to show handles or anykey ...\n"); write(str);
#ifdef __jtmdj_h_included
 m1=jgetch();
#endif
 if(m1=='s')
 {
  sprintf(str,"Displaying list of all handles(This may take few seconds)\n"); write(str);
  for(ad=0,ad3=0; ad<jtmstdio.aohandles; ad++)
  {
   ad2=(unsigned long int)jtmstdio.handles[ad];
   sprintf(str,"#%u: offset 0x%1.8x \"%s\"\n",ad,ad2,"Untitled"); write(str);
   ad3++;
   if(ad3>=24)
   {
    ad3=0;
#ifdef WAITKEY_FUNCTION
    if(waitkey()==27)break;
#endif
   }
  }
 }
 else
 {
  write("Okay.\n");
 }
}
