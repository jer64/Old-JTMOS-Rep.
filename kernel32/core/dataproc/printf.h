#ifndef __PRINTF_H__
#define __PRINTF_H__

//
#include "basdef.h"
#include "stdarg.h"
#include "dfunc.h"

// Function prototypes
int printf(const char *fmt, ...);
int vprintf(const char *fmt, va_list arg);
int dprintk(const char *fmt, ...);

// Variables
extern int printf_busy;

#endif

