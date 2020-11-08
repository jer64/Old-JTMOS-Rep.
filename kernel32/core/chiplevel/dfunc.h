#ifndef __INCLUDED_DFUNC_H__
#define __INCLUDED_DFUNC_H__

#include "kernel32.h"

int dprintk(const char *fmt, ...);
int dsleep(int sec);

#endif
