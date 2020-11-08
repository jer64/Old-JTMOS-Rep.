//==================================================================
// parse.c
// (C) 2002-2005 by Jari Tuominen.
//==================================================================
#include "kernel32.h"
#include "parse.h"
#include "paging.h"
#include "slip.h"
#include "tcp.h"


// shell_runcmd
//
// Run a single command.
// Return value:
// non-zero =	didn't run anything
// zero =	ran something
int shell_runcmd(const char *s)
{
	int             resp;

	// Nothing to run, eh? Then just return :)
	if(s==NULL || !strlen(s))
		return 1;

	// Call parser.
	resp = shell_parsecmd(s);

	// Handle / Report problems/errors.
	switch (resp)
	{
		// Seems okay response.
		case -1:
		return 0;

		// That's no internal command.
		default:
		return 1;
	}

	// 
	return 0;
}

//
void parse_cmdline(const char *s)
{
	int i;

	//
	for(i=0; i<100; i++)
		strcpy(cmdpar[i], "");

	//
	for(i=0; i<100; i++)
	{
		// Get parameter #$(i)
		sptstr(cmdpar[i], s, ' ', i, 990);
		// Last parameter?
		if(!strlen(cmdpar[i]))
			break;
	}
}

// You can add your internal maintenance shell's commands here :)
// 
int shell_parsecmd(const char *st)
{
	char	*s;
	int	i,i2,i3,i4,l;

tryagain:
	// 
	s = cmdpar[0];
	
	//


	// 
	return 0;
}



