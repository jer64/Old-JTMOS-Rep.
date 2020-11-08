//=======================================================
// init.c - JTMOS LIBC INIT.
// (C) 2002-2005 by Jari Tuominen.
//=======================================================
#include <stdio.h>
#include "basdef.h"
#include "console.h"
#include "init.h"
#include "file.h" // stdin,stdout,stderr
#include "malloc.h"
#include "process.h"
#include "XXidentification.h"
#include "modal.h"

//
int _libc_argc=0;
char **_libc_argv=NULL;

//-------------------------------------------------------------------------
void libc_receiveargs(void)
{
	int i;
	static char str[256];

	//
	_libc_argc = scall(SYS_GETARGC, 0,0,0, 0,0,0);

	//
	_libc_argv = malloc(_libc_argc*8);

	//
	for(i=0; i<_libc_argc; i++)
	{
		scall(SYS_GETARGV, i,str,0, 0,0,0);
		_libc_argv[i] = malloc(strlen(str)+5);
		strcpy(_libc_argv[i], str);
	}
}

//-------------------------------------------------------------------------
void libc_allocate_standard_streams(void)
{
	// First NULL these.
	stdin=NULL;
	stdout=NULL;
	stderr=NULL;

	// Create standard INPUT, OUTPUT and ERROR streams.
	stdin =		__fnewstream();
	stdout =	__fnewstream();
	stderr =	__fnewstream();

	// todo: here we would be specifying the buffers, etc.
	//
	//
}

//-------------------------------------------------------------------------
void init_libc(void)
{
	static char **argv;
	static int argc,x;
	int retval;

	//
//	printf("---- LIBC init reached .... ----\n");

	// -----------------------------------------
	// Check second string validity
	//
	if( XXidentificationString[0]!='[' )
	{
		printf("\nLIBC: Corrupted EXE detected.\n");
		abort();
	}

	// -----------------------------------------
	// Initialize malloc.
	//
	malloc_init( GetProgramEnd(), GetMemorySize()-GetProgramEnd() );

	// -----------------------------------------
	// Resolve arguments
	//
	argc=0;
	argv=NULL;

	//
	libc_allocate_standard_streams();

	// -----------------------------------------
	// Receive argc/argv(s).
	libc_receiveargs();

	// -----------------------------------------
	// Call main function
	retval = main(_libc_argc,_libc_argv);

	// Run the command before exit
	run_atexit();

	//
	exit(retval);
}

//

