/*
 * HEXDUMP
 *
 * A function to dump out memory in hexadecimal format to the screen.
 */
#include "basdef.h" // DWORD,WORD,BYTE,FALSE,TRUE,Etc..
#include "syssh.h"
#include "keyb.h"
#include "ega.h"
#include "string.h"
#include "foo.h"
#include "sysglvar.h"
#include "ramdisk.h"
#include "vga.h"
#include "kernel32.h"
#include "sysmem.h"
#include "cmos.h"
#include "floppy.h"
#include "int.h"
#include "hexdump.h"

// The hexdump function
void hexdump(BYTE *s,long length)
{
 //
 long i,i2,i3,i4,i5,i6,per,addspace;
 
 // 20 numbers per a line
 per=20;
 // spaces after every 8th number
 addspace=8;
 
 //
 for(i=0; i<length; i+=per)
 {
  // PRINT ADDRESS
  p32(s+i); print(": ");
  // DUMP DATA (Line at once)
  for(i2=i,i6=0; i2<i+per; i2++,i6++)
  {
   p8(s[i2]); print(" ");
   if( (i6&(addspace-1))==(addspace-1) ) printk(" ");
  }
  i3++;
  // New line
  print("\n");
  
  //
  if( !(i3&31) )
  {
   print("> [ Hit any key or [ESC] to break ]\r");
   if(getch()==27)
   {
    print("                                      \r");
    break;
   }
   print("                                      \r");
  }

  //
 }

 //
}









