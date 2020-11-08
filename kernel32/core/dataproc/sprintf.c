// sprintf.c
#include "kernel32.h"
#include "stdarg.h"
#include "sprintf.h"
#define SANE_LIMIT	10000

//
int sprintf(char *s, const char *fmt, ...)
{
	va_list arg;
	int result;

	//
	va_start(arg, fmt);
	result=svprintf(s,fmt,arg);
	va_end(arg);
	return result;
}

//
int svprintf(char *s, const char *fmt, va_list arg)
{
	int l;
	char *p_str,charri;
	int num;
	char str[2000];

	//
	strcpy(s, "");

	//
	for(l=0; *fmt && l<SANE_LIMIT; fmt++,l++)
	{
		//
		if(*fmt != '%')
		{
			//
			str[0] = *fmt; str[1]=0;
			strcat(s,str);
			continue;
		}

		//
		fmt++;

		//
		switch(*fmt)
		{
			//
			case 's':
			p_str = va_arg(arg,char *);
			// Null pointer reference parser
			if(!p_str)
			{
				strcat(s,"[NULL]");
			}
			else
			{
				strcat(s,p_str);
			}
			break;

			//
			case 'c':
			charri = va_arg(arg,char);
			str[0] = charri; str[1]=0;
			strcat(s,str);
			break;

			//
			case 'x':
			num = va_arg(arg,DWORD);
			shex32z(str,num);
			strcat(s,str);
			break;

			//
			case 'd':
			num = va_arg(arg,DWORD);
			sdec32(str,num);
			strcat(s,str);
			break;

			// TODO !!!!!!!!!!!!!!:
			case 'u':
			num = va_arg(arg,DWORD);
			sudec32(str,num);
			strcat(s,str);
			break;

			//
			case 'f':
			break;

			//
			default:
			str[0] = *fmt; str[1]=0;
			strcat(s,str);
			break;
		}
	}
	return 0;
}

// With one extra parameter which defines max. size in bytes that
// the output buffer can hold at max.
int _vsprintf(char *s,const char *fmt, va_list arg, int max_len)
{
	char longIndicator;
	int l,len;
	char *p_str,charri;
	int num;
	char str[1000];
	float fnum; // float number
	double dnum; // double number

	//
	strcpy(s, "");

	//
	for(l=0; *fmt && l<SANE_LIMIT && strlen(s)<max_len; fmt++,l++)
	{
		//
		if(*fmt != '%')
		{
			str[0] = *fmt; str[1]=0;
			strcat(s,str);
			continue;
		}

		//
		longIndicator=0;
nextPlease:
		//
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
			//
			case 0:
			goto stoppi;

			//
			case 's':
			p_str = va_arg(arg,char *);
			// Null pointer reference parser
			if(!p_str)
			{
				strcat(s,"[NULL]");
			}
			else
			{
				strcat(s,p_str);
			}
			break;

			case 'c':
			charri = va_arg(arg,char);
			str[0] = charri; str[1]=0;
			strcat(s,str);
			break;

			case 'x':
			num = va_arg(arg,DWORD);
			shex32z(str,num);
			strcat(s,str);
			break;

			case 'd':
			num = va_arg(arg,DWORD);
			sdec32(str,num);
			strcat(s,str);
			break;

			// Print unsigned integer
			case 'u':
			num = va_arg(arg,DWORD);
			sudec32(str,num);
			strcat(s,str);
			break;

			// Print float - TODO
			case 'f':
		/*	if(!longIndicator)
			{
				fnum = va_arg(arg,float);
				dnum=fnum;
				sdouble(str, dnum);
				strcat(s, str);
			}
			else
			{*/
				dnum = va_arg(arg,double);
				sdouble(str, dnum);
				strcat(s, str);
		//	}
			break;

			default:
			str[0] = *fmt; str[1]=0;
			strcat(s,str);
			break;
		}
	}
stoppi:
	return 0;
}


