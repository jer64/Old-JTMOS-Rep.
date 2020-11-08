//
// Processor Speed Determiner
// by Jari Tuominen(jari@vunet.org)
//
#include "basdef.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "ega.h"
#include "syssh.h"
#include "io.h"
#include "floppy.h"
#include "dma.h"
#include "cpu.h"
#include "ps2mouse.h"
#include "kernel32.h"
#include "speed.h"

//
DWORD determine_cpu_speed(void)
{
 //
 DWORD i,i2,i3,i4,
	a,b,c,d;

 //
/*
 printk("determining CPU speed ... ");

 //
 glvar.timer[6]=0;

 // Do some funny things
 for(a=10,b=1,i=1; ; i++)
 {
  //
  if(!i){i--; break;}

  //
  if(glvar.timer[6]>2000)break;

  //
  for(i2=0; i2<10; i2++)
  {
   c=a*b; b++;
   d=a+b+c; d--; d++;
  }

  //
 }

 //
 printk("CPU speed="); pd32(i); printk("(lower value=faster)\n");
*/

 //
 return i;
}
