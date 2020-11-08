#ifndef __INCLUDED_HDMISC_H__
#define __INCLUDED_HDMISC_H__


//
#include "hd.h"

//
int hdChooseDrive(int which);
int hdGetStatus(void);
void hdDelay(void);
BYTE hdReadError(void);
inline int hdReadStatus(void);
void hdWaitIDEIRQ(int idenr);
int hdIsDriveBusy(void);
int hdIsError(void);
int hdWait(HD *h);
int hdErrorCheck(HD *h,
	int rw, int drivenr, int headnr, int sectornr, int cylinder);
void hdRecalibrate(HD *h);
void hdCommand(HD *h, int which);


#endif



