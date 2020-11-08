#ifndef __INCLUDED_SERIALMOUSEDEV_H__
#define __INCLUDED_SERIALMOUSEDEV_H__

#include "kernel32.h"

int SerialMouseInit(void);
int SerialMouse_register_device(void);
int SerialMouse_device_call(DEVICE_CALL_PARS);

#endif

