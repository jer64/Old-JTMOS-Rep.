#ifndef __INCLUDED_DIRECTDISK_H__
#define __INCLUDED_DIRECTDISK_H__

#include "basdef.h"

//
DWORD getdevnrbyname(const char *name);
DWORD readblock(int devnr, int block, char *buf);
DWORD writeblock(int devnr, int block, char *buf);

#endif
