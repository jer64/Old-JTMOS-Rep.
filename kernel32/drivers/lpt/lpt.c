// ===============================================================
// LPT port driver
// Thanks to PC Parallel Port Mini-FAQ.
// (C) 2002 by Jari Tuominen(jari@vunet.org)
// SLIGHTLY outdated code, not maintaned since 2002.
// TODO: Fix alot of ugly looking code lines.
//
// LPT device will not be registered if no port is detected.
// Check with 'lsdev' command.
// ===============================================================

//
#define LPT_TIMEOUT_VALUE 100
//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
#include "zero.h"
//
#include "lpt.h"

// Specify 8 possible LPT port's structures.
LPTPORT lptport[8];

//
int lpt_base = 0x378; // Set to base I/O address

//
// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int lpt_device_call(DEVICE_CALL_PARS)
{
 //
 DWORD *size;

 //
 switch(n_call)
 {
  //
  case DEVICE_CMD_INIT:
  break;
  //
  case DEVICE_CMD_SHUTDOWN:
  break;
  //
  case DEVICE_CMD_READBLOCK:
  break;
  //
  case DEVICE_CMD_WRITEBLOCK:
  break;
  //
  case DEVICE_CMD_GETSIZE:
  size=po1;
  size[0]=1;
  break;
  // return 0, it's not a block device
  case DEVICE_CMD_GETBLOCKSIZE:
  size=po1;
  size[0]=0;
  break;
  //
  case DEVICE_CMD_SEEK:
  break;
  //
  case DEVICE_CMD_GETCH:
  size=po1;
  size[0]=0;
  break;
  //
  case DEVICE_CMD_PUTCH:
  size=po1;
  size[0]=0;
  break;
  //
  case DEVICE_CMD_TELL:
  size=po1;
  size[0]=0;
  break;
  //
  default:
  break;
 }

 //
 return 0;
}

// Registers device
int lpt_register_device(void)
{
 // Check that the device isn't already registered
 if( driver_getdevicebyname("lpt") )
 {
  print("cannot register device lpt: device already registered");
  return 1;
 }

 // Register the device
 driver_register_new_device("lpt",lpt_device_call,DEVICE_TYPE_CHAR);

 //
 return 0;
}

// Returns non-zero if the port is working.
//
int lpt_sniff_port(int which)
{
 //
 int i,i2;

 // Give 10,000 changes
 outportb(which,0x00);
 for(i=0,i2=0; i<10000; i++)
 {
  outportb(which,0x55);
  if(inportb(which)==0x55)return 1;
 }

 //
 return 0;
}

// Will sniff for ports at:
// LPT1: 0x378
// LPT2: 0x278
// LPT3: 0x3bc
// Return value:	non-zero if any LPT port is detected,
// 			zero if no LPT ports detected.
int lpt_hwinit(void)
{
 //
 int found;

 //
 found=0;

 // As default none of the ports are there.
 lptport[0].isWorking=0;
 lptport[1].isWorking=0;
 lptport[2].isWorking=0;
 lptport[3].isWorking=0;

 //
 if(lpt_sniff_port(0x378))
 {
  //
  if(!found)lpt_base=0x378;
  lptport[0].ad=0x378; lptport[0].isWorking=1; found++;
  //
  printk("Found LPT1 printer port at 0x378.\n");
 }

 //
 if(lpt_sniff_port(0x278))
 {
  //
  if(!found)lpt_base=0x278;
  lptport[1].ad=0x278; lptport[1].isWorking=1; found++;
  //
  printk("Found LPT2 printer port at 0x278.\n");
 }

 //
 if(lpt_sniff_port(0x3bc))
 {
  //
  if(!found)lpt_base=0x3bc;
  lptport[2].ad=0x3bc; lptport[2].isWorking=1; found++;
  //
  printk("Found LPT3 printer port at 0x3bc.\n");
 }

 // Return the amount of LPT ports detected.
 return found;
}

//
int lpt_init(void)
{
 // If found => Register.
 if( lpt_hwinit() )
 {
	//
	printk("lpt.c - lpt port driver by Jari Tuominen\n");
	//
	lpt_register_device();
 }

 //
 return 0;
}


// Return input value as five-bit number.  If input has changed since
// this function was last called, verify that the input is stable.
unsigned int lpt_input_value(void)
{
 //
 static unsigned char last_value = 0xFF;
 auto unsigned char new1, new2;

 //
 new1 = inportb(lpt_base + 1) & 0xF8;
 if (new1 != last_value)
 {
  while (1)
  {
   new2 = inportb(lpt_base + 1) & 0xF8;
   if (new2 == new1) // Wait for stable value
    break;
   new1 = new2;
   SwitchToThread();
  }
  last_value = new1;
 }
 return (last_value ^ 0x80) >> 3;
}

// Receive an 8-bit byte value, returns -1 if no data available yet
signed int lpt_receive_byte(void)
{
 unsigned int portvalue, bytevalue;
 portvalue = lpt_input_value(); // Read input
 if ((portvalue & 0x10) == 0)
   return -1; // Await high flag
 outportb(lpt_base, 0x10); // Assert reverse flag
 bytevalue = portvalue & 0x0F; // Keep low nibble
 do {
	portvalue = lpt_input_value();
	SwitchToThread();
 }while ((portvalue & 0x10) != 0); // Await low flag
 outportb(lpt_base, 0); // Deassert reverse flag
 bytevalue |= (portvalue << 4); // High nibble
 return bytevalue & 0xFF;
}

// Transmit an 8-bit byte value, won't return until value is sent
// New: Added time-out value.
void lpt_transmit_byte(unsigned int val)
{
 //
 long i;

 //
 val &= 0xFF;
 outportb(lpt_base, (val & 0x0F) | 0x10); // Set nibble flag
 // Await returned flag high
 for (i=GetTickCount(); (lpt_input_value() & 0x10) == 0 ; )
 {
  if( GetTickCount()>(LPT_TIMEOUT_VALUE+i) )break;
  SwitchToThread();
 }
 outportb(lpt_base, val >> 4); // Clear nibble flag
 // Await returned flag low
 for (i=GetTickCount(); (lpt_input_value() & 0x10) != 0; )
 {
  if( GetTickCount()>(LPT_TIMEOUT_VALUE+i) )break;
  SwitchToThread();
 }
 return;
}

//
int lpt_get(void)
{
 //
 return lpt_receive_byte();
// return inportb(lpt_base);
}

//
int lpt_put(int c)
{
 //
 int status;

 //
 outportb(lpt_base,c);
 outportb(lpt_base+2,0x0d); // set strobe high
 outportb(lpt_base+2,0x0d); // do it again to kill some time
 outportb(lpt_base+2,0x0c); // set strobe low
 inportb(lpt_base+1); // pre-charge the line if +busy is Floating
 status=(inportb(lpt_base+1)&0xf8)^0x48;
 return status;
}

//


