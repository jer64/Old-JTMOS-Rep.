// syschksum.c - system checksum
#include <stdio.h>
#include "syschksum.h"

// Calculate 32-bit checksum
DWORD syschksum(BYTE *buf, int len)
{
        int i,val;

        //
        for(i=0,val=0; i<len; i++)
                val+=buf[i]+1; // count zeroes too

        //
        return val;
}



