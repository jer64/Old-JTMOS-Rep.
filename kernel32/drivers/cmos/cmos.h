#ifndef __INCLUDED_CMOS_H__
#define __INCLUDED_CMOS_H__

//
#include "kernel32.h"
#include "driver_sys.h"
#include "cmos_dev.h"

// cmos.c's header
extern int cmos_read(int);
BYTE cmos_batterycheck(void);

#endif
