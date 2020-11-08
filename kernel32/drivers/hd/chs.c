// ===================================================================
// chs.c Linear to CHS conversion.
// (C) 2003-2004 by Jari Tuominen.
// ===================================================================
#include <stdio.h>
#include "hd.h"
#include "chs.h"

//
int isValidCHS(HD *h, int cylinder,int head,int sector)
{
        // CHS validity check
        if( !(sector>=1 && sector<=h->sectors
                &&
             head>=0 && head<h->heads
                &&
             cylinder>=0 && cylinder<h->cylinders) )
        {
                return 0;
        }
	else	return 1;
}

//
void LinearToCHS(HD *hd, int LBA,
	int *CYL,int *HEAD,int *SECT)
{
	int TEMP,SPT,HPC;

	//
	SPT = hd->sectors;
	HPC = hd->heads;

	//
	*CYL = LBA / (HPC * SPT);
	TEMP = LBA % (HPC * SPT);
	*HEAD = TEMP / SPT;
	*SECT = TEMP % SPT + 1;

	//
//	*s = (blocknr % hd->sectors)+1;
//	*h = (blocknr / hd->sectors) % hd->heads;
//	*c = (blocknr / hd->sectors / hd->heads);
}


