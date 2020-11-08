// ==============================================================
// hdBlockRW.c
// (C) 2003 by Jari Tuominen
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

	// ---------------------------------------------------------------------
	// CONVERT LINEAR TO CHS
	sectornr = (blocknr % h->sectors)+1;
	headnr = (blocknr / h->sectors) % h->heads;
	cylindernr = (blocknr / h->sectors / h->heads);

	// ---------------------------------------------------------------------
#ifndef USE_HD_CACHE
	// --------------------------------------------------------
	// CACHE DISABLED
	if(rw==READ)
	{
		//
		retval=hdBufferedSectorRW(h,
			buf,l_buf,headnr,sectornr,cylindernr,drivenr,READ, 1);
	}
	else
	if(rw==WRITE)
	{
		//
		retval=hdBufferedSectorRW(h,
			buf,l_buf,headnr,sectornr,cylindernr,drivenr,WRITE, 1);
	}
	else	panic("CASE 1010");
	// --------------------------------------------------------
#else

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
	// --------------------------------------------------------
#endif
	return retval;
}



