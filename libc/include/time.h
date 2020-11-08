// ===============================================
// time.h - JTMOS LIBC HEADER FILE
//
// Note: Parts of the code is from GNU LIBC 1.x.x.
// Distributed under GPL.
// ===============================================

/*
 *	ANSI Standard: 4.12 DATE and TIME	<time.h>
 */

#ifndef	__INCLUDED_TIME_H__
#define __INCLUDED_TIME_H__

#include <_time.h>

#define __P(args) args

/* Get size_t and NULL from <stddef.h>.  */
#define	__need_size_t
#define	__need_NULL
#include <stddef.h>

/* Processor clock ticks per second.  */
#define	CLOCKS_PER_SEC	1	/* ??? */

//
#define	__clock_t_defined	1

#undef	__need_clock_t

#define	__time_t_defined	1
#undef	__need_time_t

#define	tzset()	__tzset()

/* Nonzero if YEAR is a leap year (every 4 years,
   except every 100th isn't, and every 400th is).  */
#define	__isleap(year)	\
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

#endif
