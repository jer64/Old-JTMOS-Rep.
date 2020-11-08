//--------------------------------------------------------------------------
//
// printf.c
// printf & vprintf
// (C) 2002-2008 by Jari Tuominen.
//
//--------------------------------------------------------------------------
#include "kernel32.h" // preferences for kernel32(init)
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "sb.h"
#include "syssh.h"
#include "driver_sys.h"
#include "zero.h"
#include "null.h"
#include "printf.h"
#include "stdarg.h"
#include "dfunc.h"
#define SANE_LIMIT	10000

//--------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
//
int vprintf(const char *fmt, va_list arg)
{
	int l;
	char *str,charri;
	DWORD num;

	//
	for(l=0; *fmt && l<SANE_LIMIT; fmt++,l++)
	{
		//
		if(*fmt != '%')
		{
			//
			putch(*fmt);
			continue;
		}
  
		//
		fmt++;

		//
		switch(*fmt)
		{
			// print a string
			case 's':
			str = va_arg(arg,char *); 
			// Illegal pointer reference parser to a string "[NULL]".
			if( illegal(str) )
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
			num = va_arg(arg,DWORD);
			p32z(num);
			break;

			// pd32 => same as p32z but creates decimal(without extra zeroes)
			case 'd':
			num = va_arg(arg,DWORD);
			pd32(num);
			break;

			// TODO !!!!!!!!!!!!!!!!!!!!!!!!:
			case 'u':
			num = va_arg(arg,DWORD);
			pud32(num);
			break;

			//
			case 'f':
			break;

			//
			default:
			putch(*fmt);
			break;
		}

	}
	return 0;
}


