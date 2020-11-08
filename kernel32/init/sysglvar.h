/*** System Global Variables Header File ***/
#ifndef __INCLUDED_SYSGLVAR_H__
#define __INCLUDED_SYSGLVAR_H__

//
#include "basdef.h"
#include "screen.h"

//
typedef struct
{
	//
	int timer[50];
	int dbtimer[50];
	char system_pause;
	// Send debug messages to console? 1=Yes, 0=No
	int debugMsg;
	// 1K of BIOS information
	BYTE biosinfo[1024];
	// CPU power identifier integer(larger=faster CPU)
	int cpuPower;
}GLVAR;

//
extern GLVAR glvar;
extern SCREEN screen;

#endif
