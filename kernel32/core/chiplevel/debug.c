// Kernel debug functions.
// See also dfunc.c.
#include "kernel32.h"

//-----------------------------------------------------------------------------------------------
// Debug Output CHaracter
//
void doch(int ch)
{
	//
	if(serial_debug_line!=-1)
	{
		// Confirm valid rate
		if(GetSerialLineSpeed(serial_debug_line)!=115200)
			// Set up correct line speed
			SetSerialPortSpeed(serial_debug_line, 115200);

		// Send whole string
		serial_directput(serial_debug_line, ch);
	}
}

//-----------------------------------------------------------------------------------------------
// Send report message to the serial debug line
// (debug level 2)
//
int ReportMessage(const char *fmt, ...)
{
	va_list arg;
	int result;
	int i,l;
	char tmp[1024];

	//
	if(!debug2)
		// No reporting enabled.
		return 1;
	

	// Produce string
	va_start(arg, fmt);
	result=svprintf(tmp, fmt,arg);
	va_end(arg);

	// Print string.
	printk(tmp);

	// Write to system log also.
	WriteSystemLog(tmp);

	//
	if(serial_debug_line!=-1)
	{
		// Get string length
		l = strlen(tmp);
		// Send whole string
		for(i=0; i<l; i++)
			doch(tmp[i]);
	}

	//
	return 0;
}

//


