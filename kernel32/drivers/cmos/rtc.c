/**************************************************************
 * RTC - JTMOS Real Time Clock Functions - Part of CMOS driver.
 * (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
 */

//
#include "kernel32.h"
#include "rtc.h"

//
RTCSTR rtcstr;
static int last_known_seconds=0,tm=0,tm2=0;

// Wrapper which is easier to remember
int GetSeconds(void)
{
	//
	return rtc_getseconds();
}

//
int rtc_getseconds(void)
{
	static int flags,tick;
	static DATE d;

	// Decrease infinite loop preventer timer
	tm2--;
	//
	if( tm==0 || (GetTickCount()-tm)>=500 || tm2<=0 )
	{
		// Set values
		tm = GetTickCount();
		// Time-out to prevent infinite loops
		tm2 = 10000;

		// Receive new seconds count
		disable();
		flags=get_eflags();
		rtc_getdate(&d);
		tick = d.hour*60*60 + d.minute*60 + d.second;
		set_eflags(flags);
		last_known_seconds = tick;
		return tick;
	}
	else
	{
		// Not enough time has passed, we return the old seconds.
		return last_known_seconds;
	}
}

//
int rtc_readreg(int which)
{
	//
	outportb(0x70, which);
	return inportb(0x71);
}

//
void rtc_writereg(int which, int what)
{
	//
	outportb(0x70, which);
	outportb(0x71, what);
}

//
int rtc_getsecond(void)
{
	//
	return rtc_readreg(0);
}

//
int rtc_getminute(void)
{
	//
	return rtc_readreg(2);
}

//
int rtc_gethour(void)
{
	//
	return rtc_readreg(4);
}

//
int rtc_getday(void)
{
	//
	return rtc_readreg(7);
}

//
int rtc_getmonth(void)
{
	//
	return rtc_readreg(6);
}

//
int rtc_getyear(void)
{
	//
	return rtc_readreg(9);
}

//
void rtc_busyWait(void)
{
	//
	while(1)
	{
		if( !(rtc_readreg(0xA)&0x80) )break;
		if(Multitasking)	SwitchToThread();
	}
}

//
void rtc_getdate(DATE *d)
{
	static int flags;

	// Wait until time is available
	rtc_busyWait();

	//
	flags = get_eflags(); disable();

	// Receive
	d->second =	rtc_getsecond();
	d->minute =	rtc_getminute();
	d->hour =	rtc_gethour();
	d->day = 	rtc_getday();
	d->month =	rtc_getmonth();
	d->year =	rtc_getyear();
	// Convert
	BCD_TO_BIN(d->second);
	BCD_TO_BIN(d->minute);
	BCD_TO_BIN(d->hour);
	BCD_TO_BIN(d->day);
	BCD_TO_BIN(d->month);
	BCD_TO_BIN(d->year);
	//
	set_eflags(flags);
}


