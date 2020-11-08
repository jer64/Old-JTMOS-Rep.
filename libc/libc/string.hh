#ifndef __INCLUDED_STRING_H__
#define __INCLUDED_STRING_H__

//
#include "printf.h"
#include "sprintf.h"

//
#include "ctype.h"

//
extern void memcpy(void *,void *,DWORD);
extern long strlen(const char *);
extern long strcpy(char *,const char *);
extern long strcmp(char *,const char *);
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
int CountWords(const char *s);
int strcpyarg(char *dst, const char *s, int which);
int asc2vt220(int c);
int vt2202asc(int c);
int hexchar2int(char ch);
int hex2int(const char *s);
DWORD dec2int(const char *s);
int strncmp(const char *s1, const char *s2, int n);
char *strstr(const char *s, const char *find);

#endif
