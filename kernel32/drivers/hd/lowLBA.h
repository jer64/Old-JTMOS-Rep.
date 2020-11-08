//
#ifndef __INCLUDED_DHD_LOWLEVELRWACCESS_H__
#define __INCLUDED_DHD_LOWLEVELRWACCESS_H__

//
#include <stdio.h>
#include "hd.h"
#include "dhd.h"

//
void hdNewOrders_LBA(HD *h,
			DWORD lba,
                        int rw, int sectorCount);
//
int hdSectorRW_LBA(HD *h,
                BYTE *buf,int l_buf,
		DWORD lba,
                int driveNr, // 0=master,1=slave(I think)
                int rw, // 0=read, 1=write
                int sectorCount);
void HD_SetLBAMode(char sw);

#endif


