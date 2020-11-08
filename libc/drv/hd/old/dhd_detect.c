// ------------------------------------------------------
// dhd_detect.c
// Drive detection code.
// ------------------------------------------------------
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

int StrFix(BYTE *s, int l)
{
	DWORD i,v1,v2;

	for(i=0; i<(l-1); i+=2)
	{
		v1=s[i+0]; v2=s[i+1];
		s[i+0]=v2; s[i+1]=v1;
	}
}

int GetHDParam(HDSPEC *h, int drive, WORD *hdp)
{
	WORD i,i2,w,w2,w3,w4;
	int t;
	HDPARAM *h;

	//
/*	dhd_waituntilready(h);
	dhd_resetHD(h);
	dhd_waituntilready(h);*/

	// Order to get drive information
	outportb(0x1F6, drive ? 0xB0:0xA0);
	outportb(0x1F7, 0xEC);

	// Wait until there's no data available
	i=0xFFFF;
	for(t=rtc_getseconds(); (inportb(0x1F7) != 0x58); )
	{
		if( (rtc_getseconds()-t)>3 )
		{
			printk("HDParamDetect: Failure - Time-out triggered.\n");
			return 1;
		}
		idle_moment();
	}

	//
	if(!i)	return 1;

	// Read the data.
	for(i2=0; i2<256; i2++)
	{
		if(hdp!=NULL)	hdp[i2] = inportw(0x1F0);
				else
					inportw(0x1F0);
	}

	// Fix names, those are written in weird way always
	h=hdp;
	StrFix(h->ModelNumber, 40);
	StrFix(h->FirmwareRev, 8);
	StrFix(h->SerialNumber, 20);

	// Completed OK!
	return 0;
}

