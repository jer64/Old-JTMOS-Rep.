/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 *      JTMOS GUI command processor
 *      ---------------------------
 *
 *      This library helps application parsing user commands
 *      to valid JTMOS GraOS function calls.
 *      It is very useful when you know how to use it.
 *
 *      (C)1998 by Jari Tapio Tuominen, all rights reserved.
 *
 */

#define JTMGUIC_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif
void jtmgl_guicommandinit(int *meters)
{
 //
 int i;
 unsigned char fn_cguicfg[255];

 //
 for(i=0; i<100; i++)meters[i]=0;

 //
/* if(jgetenv("shortcuts","jtmguic")!=NULL)
 {
  strcpy(fn_cguicfg,jgetenv("shortcuts","jtmguic_cfg"));
 }
 else
 {
  jsetenv("shortcuts","jtmguic_cfg","./jtmguic.cfg");
 }*/
 strcpy(fn_cguicfg,"jtmguic.cfg");

 //
 if(fexist(fn_cguicfg))
 {
  jtm_loadbin(fn_cguicfg,(char*)meters);
 }
}

void jtmgl_guicommandexit(int *meters)
{
 //
 int i;
 char fn_cguicfg[255];

 //
 /*
 if(jgetenv("shortcuts","jtmguic_cfg")!=NULL)
 {
  strcpy(fn_cguicfg,jgetenv("shortcuts","jtmguic_cfg"));
 }
 else
 {
  strcpy(fn_cguicfg,"jtmguic.cfg");
  jsetenv("shortcuts","jtmguic_cfg","./jtmguic.cfg");
 }
 */
 strcpy(fn_cguicfg,"jtmguic.cfg");
 jtm_savebin(fn_cguicfg,(char*)meters,100);
}

// JTMGL_GUICOMMAND function
// -------------------------
//
// This function provides simple user interface.
// It does small but useful things.
//
// The function does its work with the parameters your program will call it.
//
//
// Description of (unsigned char *)meters:
//
// (offset) (description)
// 0x0000-0x0010 Parameters
//
//      0-9 -> Parameters accessable via CTRL+'1'-'0' and +/-.
//
//
//
//
// 0x0010-0x0020 Reserved for gui commander
//
void jtmgl_guicommandprocess(char m1,int *meters,unsigned char *ktab)
{
 //
 int i,i2;

 //
 if(ktab[0x1d])
 {
  // Allow resetting meters.
  if(m1=='r')
  {
   for(i=0; i<100; i++)meters[i]=0;
  }

  // Allow selecting of paremeter number.
  // CTRL + '1' .. '0' -> Parameter 0-9
  //
  if(ktab[2])
  {
   meters[0x10]=1;
   return;
  }
  if(ktab[3])
  {
   meters[0x10]=2;
   return;
  }
  if(ktab[4])
  {
   meters[0x10]=3;
   return;
  }
  if(ktab[5])
  {
   meters[0x10]=4;
   return;
  }
  if(ktab[6])
  {
   meters[0x10]=5;
   return;
  }
  if(ktab[7])
  {
   meters[0x10]=6;
   return;
  }
  if(ktab[8])
  {
   meters[0x10]=7;
   return;
  }
  if(ktab[9])
  {
   meters[0x10]=8;
   return;
  }
  if(ktab[10])
  {
   meters[0x10]=9;
   return;
  }
  if(ktab[11])
  {
   meters[0x10]=0;
   return;
  }
 }

 //
 if(ktab[0x2a] || ktab[0x36])
 {
  // Wide variant of values.
  if(m1=='+')
  {
   meters[meters[0x10]]+=8;
   return;
  }
  if(m1=='-')
  {
   if(meters[meters[0x10]]>=8)meters[meters[0x10]]-=8;
   return;
  }
 }

 // Allow changing current value of the selected paremeter currently editing.
 switch(m1)
 {
  // Wide variant of values.
  case '+':
  meters[meters[0x10]]+=1;
  break;
  case '-':
  if(meters[meters[0x10]])meters[meters[0x10]]-=1;
  break;
  // True or false.
  case 'y':
  meters[meters[0x10]]=1;
  break;
  case 'n':
  meters[meters[0x10]]=0;
  break;
 }

 //
 if(jtmscr.bpp==8)
 {
  switch(m1)
  {
  }
 }

 if(meters[7])
 {
  meters[0]=jtmmouse.x;
  meters[1]=jtmmouse.y;
 }
 if(meters[6])
 {
  meters[2]=jtmmouse.x;
  meters[3]=jtmmouse.y;
 }

}

#ifdef __cplusplus
}
#endif

