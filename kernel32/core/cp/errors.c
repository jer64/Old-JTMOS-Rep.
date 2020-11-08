// errors.c
#include "kernel32.h"
#include "errors.h"

//
void stack_violation(void)
{
	SetThreadTerminal(GetCurrentThread(), 0);
	window(0,0, 79,24);
	printf("PROCESS %d(%s) STACK VIOLATION - PROCESS TERMINATED.\n",
		GetCurrentThread(),
		thread_name[RPID(GetCurrentThread())]);
}
