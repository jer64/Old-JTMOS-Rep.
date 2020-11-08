/*===========================================================
	    SLIP DIAL-UP ROUTINES - AUTOLOGIN FUNCTIONS
		SLIP LOGIN PROGRAM FOR JTMOS
   (C) 2002-2007 by Jari Tuominen(jari@vunet.org).
  ===========================================================*/
#include <stdio.h>
#include "slipLogin.h"
#include "slip.h" // slipOnline
#include "serial.h"

//
SLIP slip;

// Slip autologin
int slipLogin(const char *opt)
{
	int manual;
	static char loginStr[50], passwdStr[50];
	static char str[512];

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
	serial_put(slip.port, '\n');
	serial_put(slip.port, '\n');
	serial_put(slip.port, '\n');
	serial_put(slip.port, '\n');
	sleep(2);
	printf("Login pass: ");
	// Query for login prompt
	if( WaitSerialString(slip.port, "login:") )	goto endi;
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
	serial_puts(slip.port, loginStr);
	//
	printf("OK\n");

	// Query for password prompt
	printf("Password pass: ");
	if( WaitSerialString(slip.port, "Password:") )	goto endi;
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
	serial_puts(slip.port, passwdStr);
	//
	printf("OK\n");

	//
	printf("Waiting for PASS #1\n");
	//
	if( WaitSerialString(slip.port, "Your address is ") )
	{
		printf("SLIP login failed, do you want to try again manually?\n");
		if( getch()!='y' )goto endi2;
		manual=1;
		printf("*** Starting manual SLIP login procedure ***\n");
		goto loginstart;
	}
	else
	{
		slip_gets(str, 510);
		printk("IP: %s\n", str);
	}

	//
	printf("Waiting for PASS #2\n");
	//
	if( WaitSerialString(slip.port, "SLiRP Ready ...") )
	{
		printf("Slirp not detected?\n");
		goto endi;
	}
	else
	{
		// If SLIRP found:
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
endi2:
	// Same as end, but without the message.
	// Return failure.
	return 1;
}



