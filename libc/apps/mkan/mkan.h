#ifndef __INCLUDED_MKAN_H__
#define __INCLUDED_MKAN_H__

//
typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned long int DWORD;

//
#include "mkanImage.h"
#include "mkanStretch.h"
#include "mkanProcess.h"
#include "mkanUser.h"
#include "mkanPCX.h"
#include "mkanBMP.h"
#include "mkanVIC.h"

//
extern unsigned char *vb;
extern int extparam[100],ixl,iyl;
extern unsigned char *extpointer[100];

#endif
