// ============================================================
// dfunc.c
// Debugging Functions Collection
// (C) 2002-2004 by Jari Tuominen.
// ============================================================
// Required includes
#include "kernel32.h" // preferences for kernel32(init)
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "sb.h"
#include "syssh.h"
#include "driver_sys.h"
#include "zero.h"
#include "null.h"
#include "printf.h"
#include "stdarg.h"
#include "dfunc.h"

// Debug sleep
int dsleep(int sec)
{
	//
	dprintk("%s: duration in seconds=%d\n",
		__FUNCTION__,
		sec);
	if(jtmos_debug)sleep(sec);
}

// Debug printk
int dprintk(const char *fmt, ...)
{
	va_list arg;
	char tmp[1024];

	// ONLY WHEN DEBUG MESSAGES ARE ENABLED:
	if(jtmos_debug)
	{
		//
		va_start(arg, fmt);
		vprintf(fmt, arg);
		va_end(arg);

		// Produce string.
		va_start(arg, fmt);
		svprintf(tmp, fmt,arg);
		va_end(arg);
		// Write to system log.
		WriteSystemLog(tmp);

		//
		if(serial_debug_line != -1)
		{
			// Send to serial line
			ReportMessage(tmp);
		}

		//
		return 0;
	}

	//
	return 0;
}

