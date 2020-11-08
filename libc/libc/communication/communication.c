//=================================================================
// LIBC communication
//=================================================================
#include <stdio.h>
#include "communication.h"

// Configure serial port at specified BPS rate
void SetSerialPortSpeed(int portNr, int bps)
{
	//
	scall(SYS_setserialportspeed, portNr,bps,0, 0,0,0);
}

// Waits until a specified string is received from serial device,
// tmOutSecs specifies a time-out period in seconds.
// Returns zero on success, non-zero no error.
int SerialWaitString(int portNr, const char *strWait, int tmOutSecs)
{
	int i,i2,i3,i4,t,ch;

	// Wait for string (e.g. "login:")
	for(t=GetSeconds(),i=0; (GetSeconds()-t)<tmOutSecs; )
	{
TryAgain:
		//
		ch = SerialGet(portNr);
		if(ch==-1)goto TryAgain;
		if(ch==strWait[i]) i++; else i=0;
		if( i>=strlen(strWait) ) goto notim1;

		//
		SwitchToThread();
        }

	//
	return 1;
notim1:
	//
	return 0;
}

// Input one character from serial communication device
// (returns -1 on none)
int SerialGet(int n_port)
{
	//
	return scall(SYS_serialget, n_port,0,0, 0,0,0);
}

// Output character at serial device
int SerialPut(int n_port, int ch)
{
	//
	return scall(SYS_serialput, n_port,ch,0, 0,0,0);
}

// Output string at serial device
int SerialPuts(int n_port, const char *s)
{
	int i,l;

	//
	l = strlen(s);
	for(i=0; i<l; i++)
		SerialPut(n_port, s[i]);
	return 0;
}

