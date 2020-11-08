#ifndef __INCLUDED_HDBLOCKRW_H__
#define __INCLUDED_HDBLOCKRW_H__

//
#include "hd.h"

//
int hdBlockRW(HD *h, BYTE *buf,int l_buf,
                DWORD blocknr,
                int drivenr,
                int rw);

#endif


