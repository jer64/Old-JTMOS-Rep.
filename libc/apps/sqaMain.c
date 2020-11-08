// sqaMain.c
#include <stdio.h>
#include "sqa.h"

#ifdef JTMOS
// Request 1024K of RAM from the init process.
SYSMEMREQ(APPRAM_1024K)
#endif

#ifndef JTMOS
// Calculate 32-bit checksum
unsigned long int syschksum(BYTE *buf, int len)
{
        int i,val;

        //
        for(i=0,val=0; i<len; i++)
                val+=buf[i]+1; // count zeroes too

        //
        return val;
}
#endif

// Main function for sqa
int main(int argc,char **argv)
{
	int err,i,i2,i3,i4;
	char *s;
	static char *devs[]={"floppy:", "hda:", "ram:"};
	static int n_devs=1;

	//
	if(argc<2)
	{
		printf("Squirrel Archive Extractor 1.0\n");
		printf("(C) 2002-2003 by Jari Tuominen(jari@vunet.org)\n");
		printf("Usage: %s [source archive name or source device] [options]\n",
			argv[0]);
		printf("Examples:\n");
		printf("  %s ram\n", argv[0]);
		printf("  %s floppy\n", argv[0]);
		printf("  %s sqdsk.sqa\n", argv[0]);
		printf("Options:\n");
		printf("  -q         quiet mode\n");
		printf("  -u file    specify file to extract\n");
		return 0;
	}

	// No destination specified as default
	strcpy(dstdir, "");

	// No specific file to extract as default (or file pattern)
	strcpy(specific, "");

	// Search for options I
	for(i=0; i<(argc); i++)
	{
		// Quiet mode
		if(!strcmp(argv[i], "-q"))
			quiet = 1;
	}

	// Search for options II
	for(i=0; i<argc; i++)
	{
		// Choose output directory
		if(!strcmp(argv[i], "-o"))
		{
			strcpy(dstdir, argv[i+1]);
		}

		// Choose specific file to extract
		if(!strcmp(argv[i], "-u"))
		{
			strcpy(specific, argv[i+1]);
		}
	}


	//
	i=0;

loop:
	// Get device string
	s=argv[1];

	// Try extracting with current options
	seloffs=0;
	err=extract_archive(argv[1], "");

	// OK? => Skip 800K check
	if(!err)goto endi;

	// Check for SQARC at 800K offset
	// Extract archive from device
	seloffs=1024*640;
	err=extract_archive(argv[1], "");

	// If error:
	if(err)
	{
		// Go through all devices enlisted
		i++; if(i>=n_devs)goto endi; goto loop;
	}

endi:
	if(err)
	{
		printf("Error %d.\n", err);
		if(err==1)
			printf("(Archive not detected.)\n");
	}

	//
	return err;
}
