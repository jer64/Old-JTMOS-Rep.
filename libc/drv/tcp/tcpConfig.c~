// tcpConfig.c
#include <stdio.h>
#include "ts.h"
#include "tcp.h"
#include "tcpConfig.h"

// IP address
int ipad0,ipad1,ipad2,ipad3;
// Mask
int ipma0,ipma1,ipma2,ipma3;

// Return value:
// 1 = end of configuration
// 0 = processed parameter OK
int ProcessParameter(const char *par, int value, int line)
{
	//
	if(!strcmp(par, "debug"))
	{
		// Enable debug messages
		debug = value;
		return 0;
	}

	//
	if(!strcmp(par, "end"))
		return 1;

	//
	return 0;
}

// Read & parse tcp configuration
void tcpReadConfiguration(void)
{
	FILE *f;
	int i,value,line;
	static char str[256],par[256];

	//
	f = fopen(TCP_CFG_FNAME, "rb");
	if(f==NULL)
	{
		printf("%s: cannot find configuration file '%s'\n",
			__FUNCTION__, TCP_CFG_FNAME);
		abort();
	}

	//
	printf("found configuration '%s'\n",
		TCP_CFG_FNAME);

	//
	for(i=0,line=1; ; i++,line++)
	{
		//
		fgets(&str, 250, f);
		sscanf(str, "%s",
			&par); 

		// IP address specification
		if( !strcmp(par, "ip") )
			sscanf(str, "%s %d %d %d %d",
				&par, &ipad0,&ipad1,&ipad2,&ipad3);
		else
		// IP mask specification
		if( !strcmp(par, "mask") )
			sscanf(str, "%s %d %d %d %d",
				&par, &ipma0,&ipma1,&ipma2,&ipma3);
		else
			sscanf(str, "%s %d",
				&par, &value);

		//
		if( ProcessParameter(par, value, line) )
			// End of configuration
			break;
	}

	//
	fclose(f);
}



