// slipConfig.c
#include <stdio.h>
#include "slip.h"
#include "slipLogin.h"
#include "slipConfig.h"
#include "slipStack.h"

//
int debug=0;
char user[100]="slippi";
char passwd[100]="slippi";

// Return value:
// 1 = end of configuration
// 0 = processed parameter OK
int ProcessParameter(const char *par, int value, int line)
{
	// Debug ON/OFF
	if(!strcmp(par, "debug"))
	{
		debug = value;
		return 0;
	}

	// Packet capturing
	if(!strcmp(par, "packetcapture"))
	{
		DO_PACKET_CAPTURING = value;
		return 0;
	}

	// Define serial line speed
	if(!strcmp(par, "bps"))
	{
		slip.bps = value;
		return 0;
	}

	// Define which serial port to use
	if(!strcmp(par, "serialport"))
	{
		slip.port = value;
		return 0;
	}

	//
	if(!strcmp(par, "end"))
		return 1;

	//
	return 0;
}

// Read & parse slip configuration
void slipReadConfiguration(void)
{
	FILE *f;
	int i,value,line;
	static char str[256],par[256];

	//
	f = fopen(SLIP_CFG_FNAME, "rb");
	if(f==NULL)
	{
		printf("%s: cannot find configuration file '%s'\n",
			__FUNCTION__, SLIP_CFG_FNAME);
		abort();
	}
	printf("found configuration '%s'\n",
		SLIP_CFG_FNAME);

	//
	for(i=0,line=1; ; i++,line++)
	{
		//
		fgets(&str, 250, f);
		sscanf(str, "%s %d",
			&par, &value); 

		// User
		if(!strcmp(par, "user"))
		{
			sscanf(str, "%s %s",
				&par, &user);
			continue;
		}

		// Password
		if(!strcmp(par, "passwd"))
		{
			sscanf(str, "%s %s",
				&par, &passwd);
			continue;
		}

		//
		if( ProcessParameter(par, value, line) )
			// End of configuration
			break;
	}

	//
	fclose(f);
}



