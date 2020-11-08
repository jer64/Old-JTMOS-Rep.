// ==============================================================
// hdBlockRW.c
// (C) 2003-2004 by Jari Tuominen.
// ==============================================================
#include <stdio.h>
#include "hd.h"
#include "hdBlockRW.h"
#include "hdCache.h"

// dhd_blockrw function
//
// returns zero on success
int hdBlockRW(HD *h, BYTE *buf,int l_buf,
		DWORD blocknr,
		int drivenr,
		int rw)
{
	static int sectornr,headnr,cylindernr,retval,i,i2,try,try1,try2;

	//
	retval = 0;

	//
	if(rw==READ || rw==WRITE)
	{
		// it's okay
	}
	else panic("dhd_blockrw: rw must be either READ or WRITE");

	//
	if(h->lbaMode)
	{
                // ------------------------------------------------------------
                // int hdSectorRW_LBA(HD *h,
                //                BYTE *buf,int l_buf,
                //                DWORD lba,
                //                int driveNr, // 0=master, 1=slave
                //                int rw, // 0=read, 1=write
                //                int sectorCount)
                // ------------------------------------------------------------
		hdSectorRW_LBA(h,	// HD*
			buf,l_buf,	// buf, l_buf
			blocknr,	// LBA
			h->slave,	// master or slave
			rw,		// read or write
			1);		// amount
	}
	else
	{
		// ---------------------------------------------------------------------
		// CONVERT LINEAR TO CHS
		LinearToCHS(h, blocknr, &cylindernr,&headnr,&sectornr);
	
		// --------------------------------------------------------
		// CACHE ENABLED
		if(rw==READ)
		{
			//
			hdCacheRW(cylindernr,headnr,sectornr, READ, buf,l_buf);
		}
		else
		if(rw==WRITE)
		{
			//
			hdCacheRW(cylindernr,headnr,sectornr, WRITE, buf,l_buf);
		}
		else	panic("CASE 1011");
	}

	// --------------------------------------------------------
	return retval;
}



