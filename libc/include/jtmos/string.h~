#ifndef __INCLUDED_STRING_H__
#define __INCLUDED_STRING_H__

//
#include "dataproc.h"
#include "printf.h"
#include "sprintf.h"
#include "ctypefamily.h"
#include "limits.h"
#include "sscanf.h"
#include "strchr.h"
#include "strncat.h"
#include "modf.h"
#include "float.h"

#ifndef __COMPAR_FN_T
#define __COMPAR_FN_T
typedef int (*__compar_fn_t)(const void *, const void *);
#endif

#ifdef	__USE_GNU
#define	comparison_fn_t	__compar_fn_t
#endif

//
char*
ecvt(double arg, int ndigits, int *decpt, int *sign);

// memory/memmove.c
void *memmove(void *dest, const void *src, int n);
//
extern void *memcpy(void *o,void *s,DWORD length);
extern int strlen(const char *);
extern int strcpy(char *,const char *);
extern int strcmp(const char *,const char *);
extern void memset(char *,char,long);
void sdec32(char *s, int value);
void sudec32(char *s, int value);
void _sdec32(char *s, DWORD value);
void sdec32z(char *s,DWORD value);
void shex8(char *s,char value);
void shex16(char *s,WORD value);
void shex32(char *s,DWORD value);
void shex32z(char *s,DWORD value);
DWORD str2int(const char *s);
DWORD GetRunningNumber(void);
char *strcat(char *dest,const char *src);
int isValidString(const char *s);

// split string and get specified section - sptstr
// sptstr(str,"dog cat fireman judge",' ',1,0);
// ^^ copies cat to str
//
extern void sptstr(char *dst,const char *src,char split_mark,long nr_item,long max_length);

// FROM OTHER EXTERNAL OBJECTS:
char *strncpy(char *dest, const char *src, int n);
int strncmp(const char *s1, const char *s2, int n);
void readStringEx(char *output, char *source, const char *stops);
char *seekTowardEnd(char *ptr, char *stops);
char *skipSpaces(char *ptr);
void StretchPrint(const char *s, int len);

//
int atoi(const char *ptr);
double atof(const char *ptr);

//
void stringfloat(float fval, char *s);
void printfloat(float fval);

//
double strtod(const char *s1, char *s2);
void pdouble(double d);
void bzero(void* s, int l);
void bcopy(const void *src, void *dest, int n);

#endif
