#ifndef __INCLUDED_DHD_BUFFER_H__
#define __INCLUDED_DHD_BUFFER_H__

#include "dhd.h"

int bufferPutBlock(HDSPEC *h,
                BYTE *buf, int headNr, int sectorNr,
                int cylinder, int driveNr, int rw);
int bufferGetBlock(HDSPEC *h,
                BYTE *buf, int headNr, int sectorNr,
                int cylinder, int driveNr, int rw);
int bufferCylinder(HDSPEC *h, int headNr, int sectorNr,
                int cylinder, int driveNr, int rw);
int forceGetFreeTU(HDSPEC *h);
int getFreeTU(HDSPEC *h);
int forceDeallocateSomeTU(HDSPEC *h);
int deallocateTU(HDSPEC *h, int id);
int flushTU(HDSPEC *h, int id);
int cylinderIsBuffered(HDSPEC *h, int cylinder, int headNr);
void dhd_bufferInit(HDSPEC *h);
void dhd_flush(HDSPEC *h);

#endif
