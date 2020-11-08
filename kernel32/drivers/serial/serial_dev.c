// ===========================================================
// RS232 - SERIAL PORT DRIVER FOR COM1, COM2, COM3, COM4, ETC.
//
// (C) 2002-2008 by Jari Tuominen(jari@vunet.org)
// ===========================================================
//
#define SERIAL_TIMEOUT_VALUE 100
//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
//
#include "serial.h"
#include "serial_dev.h"

//
// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int serial_device_call(DEVICE_CALL_PARS)
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
int serial_register_device(void)
{
 // Check that the device isn't already registered
 if( driver_getdevicebyname("serial") )
 {
  print("cannot register device serial: device already registered");
  return 1;
 }

 // Register the device
 driver_register_new_device("serial",serial_device_call,DEVICE_TYPE_CHAR);

 //
 return 0;
}

