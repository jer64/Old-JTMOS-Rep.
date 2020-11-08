//--------------------------------------------------------------------------------------------
// debugapp.c - Debug Mode Toggler.
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include "debugapp.h"

// Debug mode toggler
// ------------------
//
// Debug mode toggler.
int debug_app(int argc, char **argv)
{
	//
	jtmos_debug	^= 1;
	debug2 		^= 1;
	
	//
	if(jtmos_debug)
	{
		printf("Debugging is enabled.\n");
	}
	else
	{
		printf("Debugging is disabled.\n");
	}

	//
	return 0;
}

