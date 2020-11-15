#ifndef __INCLUDED_READCHAIN_H__
#define __INCLUDED_READCHAIN_H__

//
#include "jtmfsAccess.h"

int jtmfs_ReadFileChain(BYTE *buf, int dnr, int _d,
                        int preciseLength,
                        int offset);

#endif


