#ifndef __JTMLIBC_FLOATCONV_H__
#define __JTMLIBC_FLOATCONV_H__

#include <stdio.h>

//
#define unsigned32	unsigned long int
#define _G_uint32_t	unsigned long int
#define _G_int32_t	long int

//
#define CONST
#define KR_headers
/* In this case, we assume IEEE floats. */
#define FLT_ROUNDS 1
#define FLT_RADIX 2
#define DBL_MANT_DIG 53
#define DBL_DIG 15
#define DBL_MAX_10_EXP 308
#define DBL_MAX_EXP 1024

double _IO_strtod(CONST char *s00, char **se);

#endif
