#ifndef __INCLUDED_DHD_LOWLEVELRWACCESS_H__
#define __INCLUDED_DHD_LOWLEVELRWACCESS_H__

#include "kernel32.h"
#include "dhd.h"

int dhd_sectorRW2(HDSPEC *h, BYTE *buf,long headNr,long sectorNr,
                long cylinder,
                long driveNr, // 0=master,1=slave(I think)
                long rw, // 0=read, 1=write
                long sectorCount);

#endif

