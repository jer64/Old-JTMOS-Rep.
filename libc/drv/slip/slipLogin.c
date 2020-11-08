/*===========================================================
	    SLIP DIAL UP ROUTINES - AUTOLOGIN FUNCTIONS
		SLIP LOGIN PROGRAM FOR JTMOS
   (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
  ===========================================================*/
#include <stdio.h>
#include "slipLogin.h"
#include "slip.h" // slipOnline

//
SLIP slip;

// Slip autologin
int slipLogin(const char *opt)
{
	int manual;
	static char loginStr[50], passwdStr[50];

	// ===============================================
	// PREPARE SERIAL COM PORT FOR CONNECTION
	// DEFAULTING TO [E.G. 38,400] BPS - SERVER
	// MUST RESPOND TO THIS BPS RATE EXACTLY,
	// OTHERWISE THE CONNECTION WILL FAIL.
	SetSerialPortSpeed(slip.port, slip.bps);

	//
	printf("SLIP autologin version 1.0\n");
	printf("Using serial port #%d at %d bps.\n",
		slip.port, slip.bps);

	// Manual requested ?
	if( strcmp(opt,"") )
		manual=TRUE;
	else
		manual=FALSE;

	// ============ Start SLIP login ======================
loginstart:
	// Send some new lines
	SerialPut(slip.port, '\n');
	SerialPut(slip.port, '\n');
	SerialPut(slip.port, '\n');
	SerialPut(slip.port, '\n');
	sleep(1);
	printf("Login pass: ");
	// Query for login prompt
	if( SerialWaitString(slip.port, "login:", 1) )	goto endi;
	printf("[login prompt detected] ");
	// Enter login
	if(manual)
	{
		printf("(Enter login:) ");
		gets(loginStr);
		strcat(loginStr,"\n");
	}
	else	sprintf(loginStr, "%s\n", user);
	printf("[sending login string] ");
	SerialPuts(slip.port, loginStr);
	//
	printf("OK\n");

	// Query for password prompt
	printf("Password pass: ");
	if( SerialWaitString(slip.port, "Password:", 4) )	goto endi;
	printf("[password prompt detected] ");
	// Enter password
	if(manual)
	{
		printf("(Enter password:) ");
		gets(passwdStr);
		strcat(passwdStr,"\n");
	}
	else	sprintf(passwdStr, "%s\n", passwd);
	printf("[sending password] ");
	SerialPuts(slip.port, passwdStr);
	//
	printf("OK\n");

	//
	printf("Waiting for PASS #1\n");
	//
	if( SerialWaitString(slip.port, "Your IP address", 4) )
	{
		printf("SLIP login failed, do you want to try again manually?\n");
		if( getch()!='y' )goto endi2;
		manual=1;
		printf("*** Starting manual SLIP login procedure ***\n");
		goto loginstart;
	}

	//
	printf("Waiting for PASS #2\n");
	//
	if( SerialWaitString(slip.port, "Starting Slip", 4) )
	{
		printf("DIP STANDARD STRING 'Starting Slip' not found, not");
		printf(" a DIP standard SLIP server?\n");
		goto endi;
	}
	else
	{
		// If DIP found:
	}

	// Empty serial buffer
	while(slip_recv_char()!=-1)
		SwitchToThread();

	// Now we are online ...
	slipOnline=1;

	//
	printf("Login completed.\n");
	printf("SLIP connection has been established.\n");

	// Return OK
	return 0;

	//-------------------------------------------------------------
endi:
	printf("SLIP login failed.\n");
endi2: // same as endi but without the message
	// Return failure
	return 1;
}



