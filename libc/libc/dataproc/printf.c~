//--------------------------------------------------------------------------------------
// (C) 2002-2005 by Jari Tuominen.
//
// The functions which printf use SHOULD NOT
// call the printf -function itself.
// E.g. if needing to print something on screen,
// like e.g. pdouble -function has to,
// then better to use puts.
//--------------------------------------------------------------------------------------
#include "basdef.h"
#include "jtmos/stdarg.h"
#include "jtmos/string.h"

#define SANE_LIMIT	10000

//
int printf(const char *fmt, ...)
{
	va_list arg;
	int result;

	//
	va_start(arg, fmt);
	result=vprintf(fmt,arg);
	va_end(arg);

	//
	return result;
}

//
int vprintf(const char *fmt, va_list arg)
{
	char longIndicator;
	int l;
	char *str,charri;
	int num;
	float fnum; // float number
	double dnum; // double number

	//
	for(l=0; *fmt && l<SANE_LIMIT; fmt++,l++)
	{
		//
		if(*fmt != '%')
		{
			putch(*fmt);
			continue;
		}
  
		//
		longIndicator=0;
nextPlease:
		//
		fmt++;

		// Skip relevant but unsupported format chars:
		if( isdigit(*fmt) )	goto nextPlease;
		if(*fmt=='*')		goto nextPlease;
		if(*fmt=='#')		goto nextPlease;
		if(*fmt=='-')		goto nextPlease;
		if(*fmt=='+')		goto nextPlease;
		if(*fmt=='\'')		goto nextPlease;
		if(*fmt==' ')		goto nextPlease;
		if(*fmt=='.')		goto nextPlease;
		if(*fmt=='l'){longIndicator++; goto nextPlease;}
		if(*fmt=='L'){longIndicator++; goto nextPlease;}

		//
		switch(*fmt)
		{
			case 0:
			goto stoppi;

			// print a string
			case 's':
			str = va_arg(arg,char *); 
			// Null pointer reference parser to a string "[NULL]"
			if(!str)
			{
				puts("[NULL]");
			}
			else
			{
				puts(str);
			}
			break;

			// print a character
			case 'c':
			charri = va_arg(arg,char);
			putch(charri);
			break;

			// p32z => creates hexademical numerical string from an interger(32-Bit)
			case 'x':
			case 'X':
			num = va_arg(arg,DWORD);
			p32z(num);
			break;

			// pd32 => same as p32z but creates decimal(without extra zeroes)
			case 'd':
			case 'i':
			num = va_arg(arg,DWORD);
			pd32(num);
			break;

			// TODO !! !! !! !! !!:
			case 'u':
			num = va_arg(arg,DWORD);
			pd32(num);
		//	pud32(num);
			break;

			//
			case 'f':
		/*	if(!longIndicator)
			{
				fnum = va_arg(arg,float);
				dnum = fnum;
				pdouble(dnum);
			}
			else
			{*/
				dnum = va_arg(arg,double);
				pdouble(dnum);
		//	}
			break;

			//
			default:
			putch(*fmt);
			break;
		}

	}
stoppi:
	return 0;
}

