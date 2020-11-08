#ifndef __INCLUDED_CMOS_DEV_H__
#define __INCLUDED_CMOS_DEV_H__

#include "kernel32.h"

int cmos_register_device(void);
int cmos_device_call(DEVICE_CALL_PARS);

#endif

