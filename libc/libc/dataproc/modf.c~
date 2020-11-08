// Copyright (C) 1991 Free Software Foundation, Inc.
// This file is part of the GNU C Library.
//
// NAME
//	modf  - extract signed integral
//	and fractional values from
//	floating-point number
//
// SYNOPSIS
//        #include <math.h>
//
//        double modf(double x, double *iptr);
#include <stdio.h>
#include <math.h>

#undef	modf

/* Break VALUE into integral and fractional parts.  */
double modf(double value, double *iptr)
{
  register double ipart = floor(value);
  *iptr = ipart;
  return value - ipart;
}
