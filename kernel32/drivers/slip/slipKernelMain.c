//-------------------------------------------------------------------------------
// Slip protocol driver 1.0 - kernel version main program.
// (C) 2005 by Jari Tuominen (jari@vunet.org).
//-------------------------------------------------------------------------------
#include <stdio.h>
#include "slip.h"
#include "slipLogin.h"
#include "slipKernelMain.h"

//
int SlipProcess(void)
{
	int rv;

	//
	if( getdevicenrbyname(SLIP_DEV_NAME) > 0 )
	{
		//
		printf("Slip device driver is already installed.\n");
		return 1;
	}

	//
	printf("SLIP protocol driver version 1.0\n");
	
	//
	slipReadConfiguration();

	// Set serial line speed.
	SetSerialPortSpeed(slip.port, slip.bps);

	// Attempt login.
	// Start slip stack.
	rv = slipLogin("");

	//
	if(rv)
		// Login failed:
		ExitThread(1);

	//
	slipOnline = TRUE;

	// Start stack loop
	slipStack();

	//
	ExitThread(0);
}
