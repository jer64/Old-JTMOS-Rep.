// ===================================================================
// chs.c Linear to CHS conversion
// (C) 2003 by Jari Tuominen
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



