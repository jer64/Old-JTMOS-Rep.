#ifndef __PRINTF_H__
#define __PRINTF_H__

//
#include "basdef.h"
#include "stdarg.h"

//
extern int printf(const char *fmt, ...);
extern int vprintf(const char *fmt, va_list arg);

#endif

