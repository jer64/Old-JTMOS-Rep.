// Thanks to GNU ppl.

#ifndef __INCLUDED_DATE_H__
#define __INCLUDED_DATE_H__

#include "time_t.h"

#ifndef _CLOCK_T
#define _CLOCK_T
typedef long clock_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
#ifndef size_t
typedef unsigned int size_t;
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL	0
#else
#define NULL	((void *) 0)
#endif
#endif

#define CLOCKS_PER_SEC	100
#define CLK_TCK		100	/* That must be the same as HZ ???? */

#define	__isleap(year)	\
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

// ================================================
// Date/Time structure
typedef struct
{
        int second,minute,hour,
                day,month,year;
        int res[10];
}DATE;

struct timespec {
	long	tv_sec;		/* seconds */
	long	tv_nsec;	/* nanoseconds */
};

struct timeval {
	int	tv_sec;		/* seconds */
	int	tv_usec;	/* microseconds */
};

struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;	/* type of dst correction */
};
// ================================================


// **** PROTOTYPES ********************
int getdate(DATE *d);
time_t time(time_t *t);
/*int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(const struct timeval *tv,
        const struct timezone *tz);*/
int getdate(DATE *d);

#define __P(args) args

//
//
struct tm* localtime(const time_t * __tp);
//
struct tm* gmtime(const time_t *__tp);
//
void tzset(void);
//
char *strptime(char * __s,
const char * __fmt, struct tm * __tm);
//
size_t strftime(char * __s, size_t __smax,
const char * __fmt, const struct tm *
__tp);
//
char *ctime(const time_t * __tp);


/*********************************************************/
/* Used by other time functions.  */
struct tm
{
  int tm_sec;			/* Seconds.	[0-61] (2 leap seconds) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/
  long int tm_gmtoff;		/* Seconds west of UTC.  */
  __const char *tm_zone;	/* Timezone abbreviation.  */
};

//
struct utimbuf {
    time_t actime;  /* access time */
    time_t modtime; /* modification time */
};


//
int utime(const char *filename, struct utimbuf *buf);
int utimes(char *filename, struct timeval *tvp);

#endif
