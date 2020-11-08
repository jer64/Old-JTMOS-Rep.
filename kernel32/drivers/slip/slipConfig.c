// slipConfig.c
#include <stdio.h>
#include "slip.h"
#include "slipLogin.h"
#include "slipConfig.h"
#include "slipStack.h"

//
int slip_debug = TRUE;
char user[100]="slip";
char passwd[100]="slip";

// Read & parse slip configuration
void slipReadConfiguration(void)
{
	FILE *f;
	int i,value,line;
	static char str[256],par[256];

	//
	slip.bps = 115200;
	slip.port = 0;

	//
}



