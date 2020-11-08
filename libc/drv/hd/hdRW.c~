// ========================================================
// hdRW.c
// (C) 2003 by Jari Tuominen
// ========================================================
#include <stdio.h>
#include "hd.h"
#include "hdRW.h"

// Single sector R/W operation
int hdRW(HD *h, BYTE *buf, int rw)
{
	static WORD w;
	static int i,lo,hi,stat,t;

	//
	hdWait(h);

        // Read data from data port,
        // data comes in and out of here
	//
	// Reads/Writes two bytes at once.
        for(i=0; i<h->blocksize; i+=2)
        {
		//
		if(rw==READ)
		{
			w = inportw(0x01F0);
			lo=w&255;
			hi=(w>>8)&255;
			//
			if(buf!=NULL) {
	               		buf[i+0] = lo;
				buf[i+1] = hi;
			}
		}
		else
		if(rw==WRITE)
		{
			if(buf!=NULL)
			{
				outportw( 0x01F0,
					(buf[i+0]&255) + ((buf[i+1]&255)<<8) );
			}
			else	outportw(0x01F0, 0);
		}
		else panic("dhd_rw NOT READ OR WRITE DEFINED!!!");
        }

	//
	hdWait(h);

	//
	return 0;
}


