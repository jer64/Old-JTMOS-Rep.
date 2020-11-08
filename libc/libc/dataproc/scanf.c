// scanf.c
#include <stdio.h>
#include "scanf.h"
#include "sscanf.h"

/*
	scanf,  fscanf,  sscanf,
	vscanf, vsscanf, vfscanf - input
	format conversion
*/
int scanf(const char *fmt, ...)
{
	char *str;
	va_list arg;
	int result;

	str = malloc(1024);
	gets(str);
	va_start(arg, fmt);
	result=svscanf(str,fmt,arg);
	va_end(arg);
	free(str);
	return result;
}
