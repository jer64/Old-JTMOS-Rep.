//-------------------------------------------------------------------------------
// Slip protocol driver 1.0
// (C) 2005 by Jari Tuominen (jari@vunet.org).
//-------------------------------------------------------------------------------
#include <stdio.h>
#include "slip.h"
#include "slipLogin.h"

//
int main(int argc, char **argv)
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
	printf("+----------------------------------+\n");
	printf("| SLIP protocol driver version 1.0 |\n");
	printf("+----------------------------------+\n");
	
	//
	slipReadConfiguration();

	// Attempt to login.
	// Start slip stack.
	rv = slipLogin("");

	//
	if(rv)
		// Login failed:
		return 1;

	// Start stack loop
	slipStack();

	//
	return 0;
}

