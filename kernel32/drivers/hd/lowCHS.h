//
#ifndef __INCLUDED_DHD_LOWLEVELRWACCESS_H__
#define __INCLUDED_DHD_LOWLEVELRWACCESS_H__

//
#include <stdio.h>
#include "hd.h"
#include "dhd.h"

//
void hdNewOrders(HD *h,
                        int cylinder,int head,int sector,
                        int rw, int sectorCount);
//
int hdSectorRW(HD *h,
                BYTE *buf,int l_buf, int head,int sector,
                int cylinder,
                int driveNr, // 0=master,1=slave(I think)
                int rw, // 0=read, 1=write
                int sectorCount);
void HD_SetLBAMode(char sw);

#endif


