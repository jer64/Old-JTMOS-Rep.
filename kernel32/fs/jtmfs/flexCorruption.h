//
#ifndef __INCLUDED_FLEXCORRUPTION_H__
#define __INCLUDED_FLEXCORRUPTION_H__

//
#include "flexFat.h"

//
void jtmfatInformation(void);
void jtmfatInformation1(FLEFAT *e, const char *name);
int checkFat(FLEFAT *e);
int ffCheck(const char *fn, const char *func, int line);

//
#define FFCHECK ffCheck(__FILE__, __FUNCTION__, __LINE__);

//
#define FLEXCHECK(x) if(1){ \
	if(checkFat( ((FLEFAT*)x) )==FALSE)\
	{ \
		SetThreadTerminal(GetCurrentThread(), 0);\
		Attention();\
		printk("Corruption detected: function %s, file %s, line %d\n",\
			__FUNCTION__, __FILE__, __LINE__);\
		panic("Corruption detected!"); \
	} } \

//
#define jtmfatChecker(); FFCHECK

#endif




