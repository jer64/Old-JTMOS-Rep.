/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
//
//      JTMOS A/S communication bridge for the applications.
//      (C)1999 by Jari Tapio Tuominen, all rights reserved.
//
#ifndef JTMSYSTEM
#include <stdio.h>
#include <dpmi.h>
#include <jtmglvar.h>
#include <jtmfile.h>
#endif

// JTM_CHATSEND function
//
// This function sends chat command to JTMOS and
// waits for answer. Time out will occur if no answer will be received
// in specified time and function will return non-zero value.
int jtm_chatsend()
{
 //
 char *buf;
 long ad,ad2,ad3,ad4,re;

 //
 buf=malloc(0x40);

 //
 for(ad=0,re=1; ad<4; ad++)
 {
  buf[0]=0;
  dosmemput(buf,1,ACOF+1);
  for(ad2=0; ad2<(1024*50); ad2++)
  {
   dosmemget(ACOF+1,1,buf);
   if(buf[0])
   {
    re=0;
    goto re0;
   }
  }
 }
re0:
 free(buf);
 return re;
}

// JTM_CHECKHOST function
//
// The function returns zero if JTMOS exists.
int jtm_checkhost()
{
 //
 char *buf;
 long ad,ad2,ad3,ad4,re;

 //
 buf=malloc(10);

 //
 for(ad=0,re=1; ad<4; ad++)
 {
  buf[0]=0;
  dosmemput(buf,1,ACOF);
  for(ad2=0; ad2<(1024*50); ad2++)
  {
   dosmemget(ACOF,1,buf);
   if(buf[0])
   {
    re=0;
    goto re0;
   }
  }
 }
re0:
 free(buf);
 return re;
}

//
//
//

#ifndef JTMSYSTEM
int main(int argc,char *argv[])
{
 if(jtm_checkhost())
 {
  // If .. JTMOS doesn't exist :(
  printf("This program must be run under JTMOS.\n");
 }
 else
 {
  // If .. JTMOS does exist :)
  jmain(argc,argv);
 }
 return 0;
}
#endif

const char *___DSIGN___="[JTMOS APPLICATION SIGNATURE]";
