#ifndef __INCLUDED_SIMKER32_H__
#define __INCLUDED_SIMKER32_H__

//
#include "boot_bin.h"

//
#define SIMKER_MAX_DEV		1000

//
int getsize(int dnr);
int getblocksize(int dnr);

//
/*** Basic Definations ***/
#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef DWORD
#define DWORD unsigned long int
#endif

#ifndef WORD
#define WORD unsigned short int
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

//
#ifndef BOOL
#define BOOL BYTE
#endif

#endif
