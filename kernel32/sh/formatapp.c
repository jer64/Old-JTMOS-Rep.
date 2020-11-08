//--------------------------------------------------------------------------------------------
// formatapp.c - Format A Device.
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include "formatapp.h"

// Dir function (ls)
// -----------------
//
// This function outputs directory from specified block(linear) to stdout.
int format_app(int argc, char **argv)
{
	//
        if(argc<2)
        {
                printk("Usage: format [device name]\n");
        }
        else
        {
                //
		mkfs(argv[1]);
	}
	return 0;
}

