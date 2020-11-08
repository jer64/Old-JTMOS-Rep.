// ===================================================
// Time/Date functions
// (C) 2002 by Jari Tuominen(jari@vunet.org
// ===================================================
#include "basdef.h"
#include "console.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "postman.h"
#include "filebuf.h"
#include "date.h"

//
int utime(const char *filename, struct utimbuf *buf)
{
	//
}

//
int utimes(char *filename, struct timeval *tvp)
{
	//
}

//
time_t mktime(struct tm * __tp)
{
	return 0;
}

//
char *asctime(const struct tm * __tp)
{
}

//
char *ctime(const time_t * __tp)
{
	return NULL;
}

//
size_t strftime(char * __s, size_t __smax,
			const char * __fmt, const struct tm *
			__tp)
{
	return 0;
}

//
char *strptime(char * __s, const char * __fmt,
	struct tm * __tm)
{
	return NULL;
}

//
void tzset(void)
{
}

//
struct tm* gmtime(const time_t *__tp)
{
	return NULL;
}

struct tm* localtime(const time_t * __tp)
{
	return NULL;
}

//
time_t time(time_t *t)
{
	*t = 0;
	return t;
}

//
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	return 0;
}

//
int settimeofday(const struct timeval *tv,
	const  struct
	timezone *tz)
{
	return 0;
}

//
int getdate(DATE *d)
{
	return scall(SYS_getdate, d,0,0, 0,0,0);
}

//


