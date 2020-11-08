/* Copyright (C) 1993  Hongjiu Lu
This file is part of the Linux C Library.

The Linux C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Linux C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details. */

#include <errno.h>
#include "mathl.h"
#include <fpu_control.h>
//#include <fp.h>
    
long double powl (long double x, long double y)  	
{
  int negative;
  __volatile__ unsigned short cw, saved_cw;	/* 80387 control word */

  if (y == 0.0L) return 1.0L;

  if (x == 0.0L) return (y < 0.0L) ? __infnanl(EDOM) : 0.0L;

  if (y == 1.0L) return x;

  if (x < 0.0L) {
    long long tmp;

    tmp = (long long) y; 

    /* Even or odd */
    negative = tmp & 1;

    /* That is only valid if |y| < 2^64. */
    if (y != (double) tmp) {
       return __infnanl( EDOM);
    }

    x = -x;
    } else {
	negative = 0;
    }

 /*
  * Inline assembler implements the following algorithm:
  *
  * 1 - x' = y *log2(x)
  * 2 - find x1, x2 where x' = x1 + x2 and |x2| <= 0.5
  * 3 - x = 2^x2 scaled by x1
  */

  __asm__ __volatile__ ("fnstcw %0" : "=m" (cw) : );
  saved_cw = cw;

  cw &= 0xf3ff;	/* force round to nearest */
  cw |= 0x003f; /* turn off exceptions (see ANSI/ISO 9989 section 7.5.1) */

  __asm__ __volatile__ ("fldcw %0" : : "m" (cw));

  __asm__ __volatile__ ("fyl2x;fstl %2;frndint;fstl %%st(2);"
			"fsubrp;f2xm1;fld1;faddp;fscale"
    			  : "=t" (x) : "0" (x), "u" (y));

  /* Restore the control word */
  __asm__ __volatile__ ("fldcw %0" : : "m" (saved_cw));

  /* Check for results that can not be represented (kludgy): */

  if (isinfl((long double)x) || isnanl((long double)x))
    errno = ERANGE;

  return (negative) ? -x : x;
}
