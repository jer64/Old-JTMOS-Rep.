#ifndef __SPRINTF_H__
#define __SPRINTF_H__

//
#include "basdef.h"
#include "stdarg.h"

//
int sscanf(char *s,const char *fmt, ...);
int svscanf(char *s,const char *fmt, va_list arg);

#endif

