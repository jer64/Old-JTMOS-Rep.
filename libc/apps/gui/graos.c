// **********************************************
// *						*
// * JTMOS Window System Server.		*
// * (C) 2002-2005 by Jari Tuominen.		*
// *						*
// **********************************************
#include <stdio.h>
#include <jtmos/video.h>
#include <math.h>
#include "graos.h"
#include "guiStart.h"
#include "guiLoop.h"
#include "guiEnd.h"

// Request 1024K of static RAM.
SYSMEMREQ(APPRAM_1024K)

// Graos main function
int main(int argc, const char **argv)
{
	int i;
	VMODE *v;

	//
	if(GetThreadPID(GRAOS_SERVER_PID_NAME)!=-1)
	{
		//
		printf("Graos server is already running.\n");
		return 0;
	}

	//
	printf("Graos Windowing System Server.\n");
	printf("     Host OS: %s\n", HOST_OS);
	printf("     (C) 1997-2005 by Jari Tuominen\n");
	//waitkey();

	//
	if( !(v=guiStart()) )	return 1;
	guiLoop(v);
	guiEnd(v);
	return 0;
}

