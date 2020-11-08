#ifndef __INCLUDED_FDC_DEV_H__
#define __INCLUDED_FDC_DEV_H__

//
#include "basdef.h"
#include "kernel32.h"
#include "driver_api.h"
#include "driver_sys.h"
int floppy2_device_call(DEVICE_CALL_PARS);
int floppy2_register_device(void);

#endif
