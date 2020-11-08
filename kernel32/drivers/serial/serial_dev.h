#ifndef __INCLUDED_SERIAL_DEV_H__
#define __INCLUDED_SERIAL_DEV_H__

//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
#include "zero.h"

//
int serial_device_call(DEVICE_CALL_PARS);
int serial_register_device(void);

#endif
