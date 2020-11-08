#ifndef __INCLUDED_RTC_H__
#define __INCLUDED_RTC_H__

//
#include "kernel32.h"
#include "cmos.h" // TIME -structure

// Date/Time structure
typedef struct
{
        int second,minute,hour,
                day,month,year;
        int res[10];
}DATE;

// ??
typedef struct
{
	int offs;
	int a1,a2,a3,a4;
}RTCSTR;

// Function prototypes
int rtc_init(void);
void rtc_getdate(DATE *d);
void rtc_busyWait(void);
int rtc_getseconds(void);
int GetSeconds(void);

//
extern RTCSTR rtcstr;

// From Linux 1.0:
#ifndef BCD_TO_BIN
#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)
#endif

#ifndef BIN_TO_BCD
#define BIN_TO_BCD(val) ((val)=(((val)/10)<<4) + (val)%10)
#endif

#endif
