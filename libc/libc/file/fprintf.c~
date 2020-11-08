// fprintf.c
#include <stdio.h>
#include "fprintf.h"

//
int fprintf(FILE *fd, const char *fmt, ...)
{
	va_list args;

	//
	va_start(args, fmt);
	vfprintf(fd, fmt, args);
	va_end(args);
	return 0;
}

//
int vfprintf(FILE *fd, const char *fmt, va_list args)
{
	BYTE *tmp;

	//
	if(fd->readonly)
		return EOF;

	// Allocate temp.
	tmp=malloc(2048);

	// Build string
	_vsprintf(tmp,fmt,args, 2048);

	// Determine who gets the string.
	if(fd==stderr || fd==stdout)
	{
		// TODO: Currently just mirroring all the output to stdout.
		puts(tmp);
	}
	else
	{
		fputs(tmp,fd);
	}

	// Free temp.
	free(tmp);

	//
	return 0;
}

//





