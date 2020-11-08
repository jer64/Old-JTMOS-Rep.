//--------------------------------------------------------------------------------------------
// chdir.c - Change Directory.
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include "formatapp.h"

// chdir function:
int chdir_app(int argc, char **argv)
{
	//
	if(argc<2)
	{
		printk("Usage: chdir [path]\n");
	}
	else
	{
		chdir(argv[1]);
	}
	return 0;
}

