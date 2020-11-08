//
#ifndef __INCLUDED_HDDETECT_H__
#define __INCLUDED_HDDETECT_H__

// Unremark this line if size limitation is needed
//#define FORCE_BLOCKS	((1024*1024*4)/512)

//
#include <stdio.h>
#include "hd.h"

//
int GetHDParam(HD *h, int drive, WORD *hdp);
int hdDetect(HD *h);
int hdTryDevice(HD *h, int selection);
int StrFix(BYTE *s, int l);
int hdPrimaryDetect(HD *h);
void hdDetermineUnits(HD *h);


#endif



