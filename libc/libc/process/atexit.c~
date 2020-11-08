//
#include <stdio.h>
#include "atexit.h"

//
static void (*__atex)(void)=NULL;

//
void run_atexit(void)
{
//	if(__atex!=NULL)
//		__atex();
}

//
int atexit(void (*function)(void))
{
	if(__atex==NULL)
		__atex = function;
	return 0;
}


