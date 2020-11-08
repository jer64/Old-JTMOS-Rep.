#ifndef __SPRINTF_H__
#define __SPRINTF_H__

//
#include "basdef.h"
#include "stdarg.h"

//
int sprintf(char *s,const char *fmt, ...);
int vsprintf(char *s,const char *fmt, va_list arg);
int _vsprintf(char *s,const char *fmt, va_list arg, int max_len);

#endif

