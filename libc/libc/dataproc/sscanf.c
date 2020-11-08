// sscanf.c
#include <stdio.h>
#include "jtmos/sscanf.h"
#define SANE_LIMIT	10000

// int sscanf( const char *str, const char *format, ...);
int sscanf(char *s, const char *fmt, ...)
{
	va_list arg;
	int result;

	//
	va_start(arg, fmt);
	result=svscanf(s,fmt,arg);
	va_end(arg);
	return result;
}

//
int svscanf(char *ss, const char *fmt, va_list arg)
{
	char longIndicator;
	int l;
	char *s;
	char *p_str,charri;
	int num;
	char str[1000];
	int *p_ad;
	float *f_ad,f;
	double *d_ad,d;

	//
	for(l=0, s=ss;
		*fmt && l<SANE_LIMIT; fmt++,l++)
	{
loop:
		// End check
		if(l>=SANE_LIMIT || !*fmt)break;

		// Skip spaces
		if(*fmt == ' ')continue;

		// %
		if(*fmt != '%')
		{
			// Standard sscanf -function must
			// stop on case of unhandled chars.
///			goto stoppi;
		}

		//
		longIndicator=0;
nextPlease:
		// Get char, e.g. x,d,f (%x, %d, %f)
		fmt++;

                // Skip relevant but unsupported format chars:
		if( isdigit(*fmt) )     goto nextPlease;
		if(*fmt=='*')           goto nextPlease;
		if(*fmt=='#')           goto nextPlease;
		if(*fmt=='-')           goto nextPlease;
		if(*fmt=='+')           goto nextPlease;
		if(*fmt=='\'')          goto nextPlease;
		if(*fmt==' ')           goto nextPlease;
		if(*fmt=='.')           goto nextPlease;
		if(*fmt=='l'){longIndicator++; goto nextPlease;}
		if(*fmt=='L'){longIndicator++; goto nextPlease;}

		//
		switch(*fmt)
		{
			// End of string
			case 0:
			goto stoppi;

			// RECEIVE STRING
			case 's':
			// Read to p_str with " " as stop mark,
			// besides regular \0 stop mark.
			readStringEx(va_arg(arg,char *),
					s, " ");
			s = seekTowardEnd(s, " ");
			s = skipSpaces(s);
			continue;

			// RECEIVE CHAR
			case 'c':
			*va_arg(arg, char *) =
					*s;
			s++;
			if(!*s)goto stoppi;
			continue;

			// RECEIVE HEXADECIMAL NUMBER STRING
			case 'x':
			readStringEx(str, s, " ");
			*va_arg(arg, int *) =
				hex2int(str);
			s = seekTowardEnd(s, " ");
			s = skipSpaces(s);
			continue;

			// RECEIVE DECIMAL NUMBER STRING
			case 'd':
			readStringEx(str, s, " ");
			*va_arg(arg, int *) =
				dec2int(str);
			s = seekTowardEnd(s, " ");
			s = skipSpaces(s);
			continue;

			// RECEIVE UNSIGNED DECIMAL NUMBER STRING
			case 'u':
			readStringEx(str, s, " ");
			*va_arg(arg, DWORD *) =
				dec2int(str);
			s = seekTowardEnd(s, " ");
			s = skipSpaces(s);
			continue;

			// RECEIVE FLOAT
			case 'f':
			if(!longIndicator)
			{
				readStringEx(str, s, " ");
				*va_arg(arg, float *) =
					strtod(str, NULL);
				s = seekTowardEnd(s, " ");
				s = skipSpaces(s);
			}
			else
			{
				readStringEx(str, s, " ");
				*va_arg(arg, double *) =
					strtod(str, NULL);
				s = seekTowardEnd(s, " ");
				s = skipSpaces(s);
			}
			continue;

			// According to Linux Programmer's Manual
			// a sscanf -function must stop on
			// unknown format chars:
			default:
			goto stoppi;
		}
	}
stoppi:
	return 0;
}
