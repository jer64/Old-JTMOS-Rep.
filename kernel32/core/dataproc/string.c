/*
 * JTMOS STRING FUNCTIONS
 * ----------------------
 *
 * (C) 2001-2005 by Jari Tuominen(jari@vunet.org).
 *
 * Comes with useful string manipulation functions.
 * Aims to emulate STL's standard STRING module.
 *
 * NOTE: CONTAINS FUNCTIONS LIKE MEMCPY & MEMSET
 * THESE FUNCTIONS MAYBE RELOCATED TO SOME
 * OTHER LIBRARY TO CLEAN UP THE ORDER.
 */
#include <stdio.h>
#include "string.h"

//
static DWORD RunningNumber=0;

/*
//
void *memcpy(void *dest, const void *src, size_t n)
{
	BYTE *s,*d;
	int i;

	//
	s = src;
	d = dest;

	//
	for(i=0; i<n; i++)
	{
		d[i] = s[i];
	}
	return dest;
}
*/

// Call this function to get a new value everytime,
// used in various applications.
//
DWORD GetRunningNumber(void)
{
	//
	RunningNumber++;

	//
	return RunningNumber;
}

/************ MISCELLANOUS STRING FUNCTIONS START **********************/

// split string and get specified section - sptstr
// sptstr(str,"dog cat fireman judge",' ',1,0);
// ^^ copies cat to str
//
// NOTE: if max_length is a zero, no limit is used.
//
// NOTE2: max_length should be used to secure the copies,
// 	  so the copies won't exceed the max. length of dst -string.
//
void sptstr(char *dst,const char *src,char split_mark,int nr_item,int max_length)
{
 int i,i2,i3,i4,l,l2,l3,l4,n_arg;

 //
 l=strlen(src);

 // i=read pointer, i2=write pointer
 for(i=0,i2=0,n_arg=0; i<l; i++)
 {
  if(src[i]==split_mark)
  {
   n_arg++;
   for(; src[i]==split_mark && i<l; i++);
   if(i>=l)break;
  }
  if(n_arg==nr_item)
  {
   dst[i2]=src[i]; i2++;
   if(max_length && i2>=max_length)break;
  }
 }

 //
 dst[i2]=0;

 //
}

//
int strlen(const char *s)
{
	int i,i2;

	// Check the seek max. here, set it to a reasonable value!
	for(i=0,i2=0; i<9999; i++)
	{
		//
		if(!s[i]){i2=i; break;}
	}
	return i2;
}

//
int strcpy(char *o,const char *s)
{
	int i,i2;

	//
	for(i=0,i2=0; i<9999; i++)
	{
		//
		o[i]=s[i];
		if(!s[i]){i2=i; break;}
	}
	return i2;
}

// strcmp
// zero 	= strings match
// nonzero 	= .. do not
int strcmp(char *o,const char *s)
{
 //
 int i,i2;

 //
 for(i=0,i2=0; i<9999; i++)
 {
  // End of string on both?
  if(!s[i] && !o[i])return 0;
  // End of string on just either one?
  if( (!s[i] && o[i]) || (s[i] && !o[i]) )return 1;
  // Compare
  if(o[i] != s[i])return 1;
 }
 return 0;
}

// Copyright (C) 1991 Free Software Foundation, Inc.
// This function is part of the GNU C Library.
/* Append SRC on the end of DEST.  */
char *strcat(char *dest,const char *src)
{
  register char *s1 = dest;
  register const char *s2 = src;
  char c;

  /* Find the end of the string.  */
  do
    c = *s1++;
  while (c != '\0');

  /* Make S1 point before the next character, so we can increment
     it while memory is read (wins on pipelined cpus).  */
  s1 -= 2;

  do
    {
      c = *s2++;
      *++s1 = c;
    }
  while (c != '\0');

  return dest;
}

/**************** NUMERICAL FUNCTIONS START *****************/

DWORD dec2int(const char *s)
{
	return str2int(s);
}

// DWORD str2int(char *s)
//
// string to integer
// this function converts a string with a decimal
// value to 32-Bit interger
//
// ALSO KNOWN AS dec2int
//
// ex. str2int("100");
//
DWORD str2int(const char *s)
{
	int i,i2,i3,i4,v;
	static DWORD kert[]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	//
	for(v=0,i=(strlen(s)-1),i3=0; i!=-1; i--,i3++)
	{
		// Not a number?
		if(s[i]<'0' || s[i]>'9')return 0;

		// Get single value
		i2=s[i]-'0';

		// Multiply
		v += i2*kert[i3];
	}

	//
	return v;
}

// This function's work is identical to
// phex32, except that this function does
// not output any zeros in front of any
// non-zero number.
// If the value equals to zero, it'll
// print plain zero.
void shex32z(char *s,DWORD value)
{
 //
 char str[50],first_non_zero;
 int i,i2;

 //
 shex32(str,value);

 // i=	read pointer
 // i2=	write pointerr
 for(i=0,i2=0,first_non_zero=0,s[0]=0; ; i++)
 {
  if(!str[i])break;
  if(str[i]!='0')first_non_zero=1;
  if(first_non_zero){s[i2]=str[i]; s[i2+1]=0; i2++;}
 }

 // If nothing yet there, we just output a zero at string buffer
 if(!strlen(s))strcpy(s,"0");

 // copy to result string
// strcpy(s,str);

 //
}

//
void shex32(char *s,DWORD value)
{
	char str[50],str2[50];

	//
	shex16(str,     value & 65535);
	shex16(str2,    (value>>16) & 65535);
	strcpy(s,str2);
	strcat(s,str);
}

//
void shex16(char *s,WORD value)
{
	char str[50],str2[50];

	//
	shex8(str,	value & 255);
	shex8(str2,	(value>>8) & 255);
	strcpy(s,str2);
	strcat(s,str);
}

// Generates a string with 8-Bit hexadecimal number
void shex8(char *s,char value)
{
	char *t="0123456789ABCDEF";
	char offs;

	//
	offs=0;
	s[offs]=t[(value>>4)&0xf]; offs++;
	s[offs]=t[value&0xf]; offs++;
	s[offs]=0; offs++;

	//
	return;
}

// PRINT DECIMAL NUMBER WITH ALL ZEROES IN FRONT
void sdec32z(char *s,DWORD value)
{
	char d[]={'0','1','2','3','4','5','6','7','8','9'};
	unsigned int i,i2,i3,i4,i5,i6,i7,i8,i9,i10;
	int offs;

	//
	offs=0;

	// 10x 0s at 32bit int
	i=  value/1000000000 % 10;
	i2= value/100000000  % 10;
	i3= value/10000000   % 10;
	i4= value/1000000    % 10;
	i5= value/100000     % 10;
	i6= value/10000      % 10;
	i7= value/1000       % 10;
	i8= value/100        % 10;
	i9= value/10         % 10;
	i10=value/1          % 10;

	// Print integer in zero fashion
	s[offs]=d[i ]; offs++;
	s[offs]=d[i2]; offs++;
	s[offs]=d[i3]; offs++;
	s[offs]=d[i4]; offs++;
	s[offs]=d[i5]; offs++;
	s[offs]=d[i6]; offs++;
	s[offs]=d[i7]; offs++;
	s[offs]=d[i8]; offs++;
	s[offs]=d[i9]; offs++;
	s[offs]=d[10]; offs++;
	s[offs]=0; offs++;
}

void sdec32(char *s, int value)
{
	// Add sign indicator character
	if(value<0)
		strcpy(s, "-");
		else
		strcpy(s, "");

	// Print according to the signing
	if(value<0)
		_sdec32(s+1, -value);
		else
		_sdec32(s, value);
}

void sudec32(char *s, int value)
{
	_sdec32(s, value);
}

// GENERATE ASCII DECIMAL INTEGER(Atmost 32bit)
// (Non-zero fashioned)
void _sdec32(char *s, DWORD value)
{
 int offs;
 char d[]={'0','1','2','3','4','5','6','7','8','9'};
 unsigned int i,i2,i3,i4,i5,i6,i7,i8,i9,i10,l;

 //
 offs=0;

 // 10x 0s at 32bit int
 i=  value/1000000000 % 10;
 i2= value/100000000  % 10;
 i3= value/10000000   % 10;
 i4= value/1000000    % 10;
 i5= value/100000     % 10;
 i6= value/10000      % 10;
 i7= value/1000       % 10;
 i8= value/100        % 10;
 i9= value/10         % 10;
 i10=value/1          % 10;

 //
 l=0;
 // Print integer in non-zero fashion
 if(i   || l)  { l=1; s[offs]=d[i ]; offs++; }
 if(i2  || l)  { l=1; s[offs]=d[i2]; offs++; }
 if(i3  || l)  { l=1; s[offs]=d[i3]; offs++; }
 if(i4  || l)  { l=1; s[offs]=d[i4]; offs++; }
 if(i5  || l)  { l=1; s[offs]=d[i5]; offs++; }
 if(i6  || l)  { l=1; s[offs]=d[i6]; offs++; }
 if(i7  || l)  { l=1; s[offs]=d[i7]; offs++; }
 if(i8  || l)  { l=1; s[offs]=d[i8]; offs++; }
 if(i9  || l)  { l=1; s[offs]=d[i9]; offs++; }
 // (Atleast print '0', or so)
 s[offs]=d[i10]; offs++;
 // zero at end
 s[offs]=0; offs++;
}

/********** NUMERICAL FUNCTIONS END *****************/

//
int isValidString(const char *s)
{
 //
 int i,ok;

 //
 for(i=0,ok=0; i<256; i++)
 {
        if(!s[i]){ ok=1; break; }
 }

 //
 return ok;
}

//
int CountWords(const char *s)
{
	int i,i2,i3,i4,l,lastSpace,words;

	if(!strlen(s))return 0;

	l=strlen(s);

	for(i=0,lastSpace=1,words=0; i<l; i++)
	{
		if( !s[i] )break;

		if(s[i]!=32 && lastSpace)
			words++;

		
		if(s[i]==32)	lastSpace=1;
			else
				lastSpace=0;
	}

	return words;
}

//
int strcpyarg(char *dst, const char *s, int which)
{
	int i,i2,i3,i4,l,lastSpace,words;

	strcpy(dst,"");

	if(!strlen(s))return 0;

	l=strlen(s);

	for(i=0,lastSpace=1,words=0; i<l; i++)
	{
		if( !s[i] )break;

		if(s[i]!=32 && lastSpace)
			words++;

		if(words==(which+1))
		{
			for(i2=0; i<l; i++,i2++)
			{
				if(!s[i])break;
				if(s[i]==' ')break;
				dst[i2]=s[i];
			}
			dst[i2]=0;
			return 0;
		}
		
		if(s[i]==32)	lastSpace=1;
			else
				lastSpace=0;
	}

	return words;
}

int asc2vt220(int c)
{
	if(c==0x0d) { return 0x0a; }
	if(c==0x0a) { return 0x0d; }
	return c;
}

int vt2202asc(int c)
{
	if(c==0x0d) { return 0x0a; }
	if(c==0x0a) { return 0x0d; }
	return c;
}

// Convert a single hex character to int
// (e.g. '0'-'F')
int hexchar2int(char ch)
{
	char *hs="0123456789abcdef";
	char *HS="0123456789ABCDEF";
	int i,i2,i3,i4;

	for(i=0; i<16; i++)
	{
		if( hs[i]==ch )
			return i;
		if( HS[i]==ch )
			return i;
	}
	return 0;
}

int hex2int(const char *s)
{
	int i,i2,i3,i4,k,kk,value,l;
	int potenssi[]={
			0x00000000,
			0x00000001,
                        0x00000010,
                        0x00000100,
                        0x00001000,
                        0x00010000,
                        0x00100000,
                        0x01000000,
                        0x10000000
			};

	l = strlen(s);
	for(i=l-1,value=0,k=0; i!=-1; i--,k++)
	{
		kk = 0x10*potenssi[k&7]; if(!kk)kk=1;
		value += hexchar2int(s[i]) * kk;
	}
	return value;
}

/* $Id: strncmp.c,v 1.2 2001/11/06 01:29:38 pavlovskii Exp $ */
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
int strncmp(const char *s1, const char *s2, int n)
{

  if (n == 0)
    return 0;
  do {
    if (*s1 != *s2++)
      return *(unsigned const char *)s1 - *(unsigned const char *)--s2;
    if (*s1++ == 0)
      break;
  } while (--n != 0);
  return 0;
}

/* $Id: strstr.c,v 1.2 2001/11/06 01:29:38 pavlovskii Exp $ */
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
char *strstr(const char *s, const char *find)
{
  char c, sc;
  int len;

  if ((c = *find++) != 0)
  {
    len = strlen(find);
    do {
      do {
	if ((sc = *s++) == 0)
	  return 0;
      } while (sc != c);
    } while (strncmp(s, find, len) != 0);
    s--;
  }
  return (char*) s;
}

//
void StretchPrint(const char *s, int len)
{
        int i,i2,i3,i4,l;

        l = strlen(s);
        for(i=0,i2=0; i<len; i++)
        {
                if(i>=l)
                        printf(" ");
                        else
                        printf("%c", s[i]);
        }
}

//
char *strncpy(char *s1, const char *s2, int n)
{
  register char c;
  char *s = s1;

  --s1;

  if (n >= 4)
    {
      int n4 = n >> 2;

      for (;;)
	{
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  c = *s2++;
	  *++s1 = c;
	  if (c == '\0')
	    break;
	  if (--n4 == 0)
	    goto last_chars;
	}
      n = n - (s1 - s) - 1;
      if (n == 0)
	return s;
      goto zero_fill;
    }

 last_chars:
  n &= 3;
  if (n == 0)
    return s;

  do
    {
      c = *s2++;
      *++s1 = c;
      if (--n == 0)
	return s;
    }
  while (c != '\0');

 zero_fill:
  do
    *++s1 = '\0';
  while (--n > 0);

  return s;
}

// Reads string from "source" and outputs at "output".
// Same as strcpy but stops on one of the characters featured
// at "stops" -string.
void readStringEx(char *output, char *source, const char *stops)
{
	int i,i2,l,l2,wi;

	// Initialize output string
	strcpy(output, "");

	// Get source string length
	l = strlen(source);

	// Get stop chars string length
	l2 = strlen(stops);

	// Go through whole string
	for(i=0,wi=0; i<l; i++,wi++)
	{
		// Check for end of string
		if(!source[i])break;

		// Get length of pattern string
		l2 = strlen(stops);

		// Search for stop chars
		for(i2=0; i2<l2; i2++)
		{
			if(source[i]==stops[i2])goto stop;
		}
		output[wi] = source[i];
	}
stop:
	// Sign end
	output[wi] = 0;
}

char *seekTowardEnd(char *ptr, char *stops)
{
	int i,i2,i3,i4,l,l2;
	char *p;

	l = strlen(stops);
	for(p=ptr; *p; p++)
	{
		// Check for stop mark
		for(i2=0; i2<l; i2++)
		{
			if(*p==stops[i2])goto stopNow;
		}
	}
stopNow:
	return p;
}

char *skipSpaces(char *ptr)
{
	char *p;

	for(p=ptr; ; p++)
	{
		if(!*p)break;
		if(*p!=' ')break;
	}
	return p;
}

// convert a string into double
double atof(const char *ascfloatptr)
{
        return strtod(ascfloatptr, NULL);
}

// Convert float to string
void stringfloat(float fval, char *s)
{
}

// Print float on stdout
void printfloat(float fval)
{
        printf("%f", fval);
}

//
double strtod(const char *s1, char **s2)
{
        return _IO_strtod(s1,s2);
}

// I had problems with badly formatted strings.
// This is why JTMOS LIBC functions nowadays
// work so that these first checks the file names
// that are passed to them.
//
// Badly terminated strings lead into
// program termination.
//
int StringCheck(const char *s)
{
        int i;
        if(!isValidString(s))
        {
                LIBCERROR("LIBC failure: StringCheck s=non-null terminated string");
        }
        return 0;
}

#define RAD 80
// Print double number
void pdouble(double d)
{
	int a1,a2;
	char str[255];

	sdouble(str, d);
	puts(str);
	return;
}

void sdouble(char *s, double d)
{
	// decimal & sign
	int dec,sign;
	char str[100],str2[100],first[100],second[100];
	int i,i2;
	char *p,*beg;
	int decimalDigits=5;

	// Get the 
	strcpy(str, ecvt(d,RAD, &dec,&sign));

	// Start with sign
	if(sign)	strcpy(s,"-");
			else
			strcpy(s,"");

	// Add digit(s)
	strncat(s,str, dec);

	// Add decimals if needed
	if( strlen(str)-dec )
	{
		// Add "."
		strcat(s, ".");

		// Add decimal(s)
		strncat(s, str+dec, strlen(str)-dec);
	}

	// Remove additional zeroes from the end
	if(strlen(s))
	{
		for(p=s+(strlen(s)-1); p!=s; p--)
		{
			if(*p!='0' && *p!=' '){*p=0; break;}
		}
	}

	// We got a decimal point?
	if( (p=strchr(s,'.'))!=NULL )
	{
		// Cut to defined amount of decimal digits
		p++;
		for(i=0; *p && i<decimalDigits; i++,p++) { }
		// Cut there
		*p=0;
	}

	// >> Debug stuff
	sdec32(first, dec);
//	puts("("); puts("dec=");
//	puts(first); puts(")\n");
}
