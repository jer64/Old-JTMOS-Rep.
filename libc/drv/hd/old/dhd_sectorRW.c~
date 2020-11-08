// ===================================================================
// Direct HD Access
// (C) 2001,2002 by Jari Tuominen
// This is the IDE Hard Disk driver for The JTMOS Operating System.
// ===================================================================
#include "int.h"
#include "cpu.h" // nop()
#include "sysglvar.h"
#include "kernel32.h"
#include "delay.h"
#include "io.h"
#include "dma.h"
#include "dhd.h"
#include "devsys.h"
#include "dhd_detect.h"
#include "dhd_dev.h"
#include "dhd_func.h"
#include "hdparam.h"
#include "dhd_sectorRW.h"
#include "chs.h"

// THE WRAPPER RW FUNCTION
int dhd_sectorRW(HDSPEC *h, BYTE *buf, long head, long sector,
                long cylinder,
                long driveNr, // 0=master,1=slave(I think)
                long rw)
{
	//
	if( !isValidCHS(h, cylinder,head,sector) )
	{
		printk("%s: Invalid CHS %d/%d/%d\n",
			__FUNCTION__,
			cylinder, head, sector);
		return 0;
	}

	//
	if(HD_BUFFERING_ENABLED)
		return dhd_sectorRWCached(h, buf, head, sector,
				cylinder, driveNr, rw);
		else
		return dhd_sectorRW1(h, buf, head, sector,
				cylinder, driveNr, rw,  1);
}

//
int dhd_sectorRWCached(HDSPEC *h, BYTE *buf, long head, long sector,
		long cylinder,
		long driveNr, // 0=master,1=slave(I think)
		long rw)
{
	int i,i2,i3,i4,try;

	try=0;
again:
	if( cylinderIsBuffered(h, cylinder, head)==-1 )
	{
		bufferCylinder(h,  head, sector,
				cylinder, driveNr, rw);
		try++;
		if(try>=3)
		{
			printk("\ndhd_sectorRW: FATAL ERROR\n");
			return;
		}
		goto again;
	}

	if(rw==READ)
		bufferGetBlock(h, buf, head, sector,
				cylinder, driveNr, rw);
	else
	if(rw==WRITE)
		bufferPutBlock(h, buf, head, sector,
				cylinder, driveNr, rw);
	return 0;
}

// The sector RW function which works past cache
int dhd_sectorRW1(HDSPEC *h, BYTE *buf,long head,long sector,
		long cylinder,
		long driveNr, // 0=master,1=slave(I think)
		long rw, // 0=read, 1=write
		long sectorCount)
{
	BYTE *tmp,try;
	char typ[50];
	int i,i2,rv,rv2,er;

	//
	tmp = genhd.tmp;

	//
	if(rw==READ)strcpy(typ,"read");
	else
	if(rw==WRITE)strcpy(typ,"write");
	else
	panic("rw value");

	//
	printk("%s: %s CHS %d/%d/%d\n",
		__FUNCTION__,
		typ,
		cylinder, head, sector);

	// HANDLE READ CASES SEPERATELY
	if(rw==READ)
	{
	//	return dhd_sectorRW2(h, buf, head, sector,
	//			cylinder, driveNr, rw, sectorCount);

		// ------ READ CASE: ----------------------
		try=0;
tryAgain:
		if( (try++)>=5 )
		{
			printk("dhd_sectorRW1: Read error #9\n");
			return 1;
		}

		rv = dhd_sectorRW2(h, buf, head, sector,
				cylinder, driveNr, rw, sectorCount);
		rv2 = dhd_sectorRW2(h, tmp, head, sector,
				cylinder, driveNr, rw, sectorCount);

		// Retry on read error
		if(rv || rv2)goto tryAgain;

		// Compare
		i2 = 512*sectorCount;
		for(i=0,er=0; i<i2; i++)
		{
			if(tmp[i]!=buf[i])er=1;
		}
		// Loop if error
		if(er) goto tryAgain;
okay:
		return 0;
	}

	// ------ WRITE CASE: ----------------------
	for(try=0; try<40; try++)
	{
		memset(tmp, 0, 512*sectorCount);
		if( dhd_sectorRW2(h, buf, head, sector,
			cylinder, driveNr, rw, sectorCount) )continue;
		if( dhd_sectorRW2(h, tmp, head, sector,
			cylinder, driveNr, READ, sectorCount) )continue;

		//
		i2 = 512*sectorCount;
		for(i=0; i<i2; i++)
		{
			if(tmp[i]!=buf[i])goto notSame;
		}
		goto niceDeal;
notSame:
	}

	// None of the tries worked.
	printk("IDE/sectorRW1: Write failure after %d tries\n", try);
	return 1;

niceDeal:
	return 0;

}


