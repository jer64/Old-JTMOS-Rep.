// ===================================================================
// Direct HD Access
// (C) 2001,2002 by Jari Tuominen
// This is the IDE Hard Disk driver for The JTMOS Operating System.
// ===================================================================
#include "kernel32.h"
#include "int.h"
#include "cpu.h" // nop()
#include "sysglvar.h"
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
#include "dhd_lowLevelRWAccess.h"
#include "dhd_schedule.h"
#include "chs.h"
// Verbose debug print
//#define VERBOSE_DEBUG
//
#ifdef VERBOSE_DEBUG
#define VD_PRINT printk
#else
#define VD_PRINT //
#endif

//
void dhd_newOrders(HDSPEC *h,
			int cylinder,int head,int sector,
			int rw, int sectorCount)
{
		// Sector count port
		outportb(0x01F2, sectorCount);
		// Sector number port
		outportb(0x01F3, sector);
		// Cylinder low port
		outportb(0x01F4, cylinder&0xff);
		// Cylinder high port
		outportb(0x01F5, (cylinder>>8)&0xff);
		// Drive and head port, Drive, Head
		outportb(0x01F6,
			0xA0|(head&7)|((h->slave&1)<<4) );
		// Command
		if(rw==READ)outportb( 0x01F7,0x20 );
		else
		if(rw==WRITE)outportb( 0x01F7,0x30 );
}

// Lowest level HD sector R/W access
int dhd_sectorRW2(HDSPEC *h,
		BYTE *buf,long head,long sector,
		long cylinder,
		long driveNr, // 0=master,1=slave(I think)
		long rw, // 0=read, 1=write
		long sectorCount)
{
	WORD w;
        int i,lo,hi,stat,offs,t,t2,try,lsec;

	//
	VD_PRINT("%s: CHS %d/%d/%d (drive=%d, count=%d)\n",
			__FUNCTION__,
			cylinder, head, sector,
			driveNr, sectorCount);

	//
	if( !isValidCHS(h, cylinder,head,sector) )
	{
		printk("%s: Invalid CHS\n", __FUNCTION__);
		return 0;
	}

	// wait until the current operation has finished it's doing
	if(dhd_waitcompletion(h))
	{
		VD_PRINT("%s: error\n", __FUNCTION__);
		return 10;
	}

	//
	if(rw==READ || rw==WRITE) { }
	else
	{
		panic("dhd_blockrw: rw must be either READ or WRITE");
	}
   
	if(!sector)
		panic("ZERO SECTOR DETECTED! ILLEGAL!");

	//
	try=0;
again1:
	VD_PRINT("(1) ");
	dhd_waituntilready(h);
	VD_PRINT("(2) ");

	//
	dhd_newOrders(h, cylinder,head,sector, rw, sectorCount);
        for(i=0,t=rtc_getseconds(); ; i++)
 	{
		//
		if( (rtc_getseconds()-t)>=5 ) {
			printk("%s: Warning. IDE didn't receive our CMD??\n",
				__FUNCTION__);
			break;
		}

		//
		if( dhd_GetStatus()==0x58 )
			// OK we got it
			break;
		SCHEDULE();
	}

	// Command port, read with retry
	// WAIT STAGE
	// ----------
        // This means sector buffer requires servicing,
        // don't continue until sector buffer is ready.
	// 8(+16+64?)
        // wait seek complete+drive is ready+data arrived
        for(i=0,t=rtc_getseconds(),lsec=t; ; i++)
 	{
		//
		VD_PRINT("(4) secs=%d  ", rtc_getseconds());
		// Sec has changed?
		if(rtc_getseconds()!=lsec)
		{
			VD_PRINT("(3) ");
			printk("[HD CMD timed-out] ");
			dhd_resetHD();
			dhd_newOrders(h, cylinder,head,sector, rw, sectorCount);
			lsec = rtc_getseconds();
		}

		// Time-out check
		if( (rtc_getseconds()-t)>=5 ) {
						VD_PRINT("(8)\n");
						dhd_recalibrate(h);
						VD_PRINT("IDE: Error #3(TIME-OUT)\n");
						return 2;
					}

		//
		stat=dhd_GetStatus();
		// Got data?
		if( (stat&8) )break;
		// error?
		if( (stat&1) )	{
					VD_PRINT("(6)\n");
					VD_PRINT("%s: error\n", __FUNCTION__);
					return 20;
				}

		//
		SCHEDULE();
	}
	VD_PRINT("(5) ");

	// R/W STAGE
	// ---------
	if(buf==NULL)
	{
		for(i=0,offs=0; i<sectorCount;  i++, offs+=h->blocksize)
		{
			if(dhd_rw(h, NULL, rw))break;
			// error?
			if( (stat&1) )	{ VD_PRINT("IDE: Error #1(R/W ERROR)\n"); return 1; }
		}
	}
	else
	{
		for(i=0,offs=0; i<sectorCount;  i++, offs+=h->blocksize)
		{
			if(dhd_rw(h, buf+offs, rw))break;
			// error?
			if( (stat&1) )	{ VD_PRINT("IDE: Error #1(R/W ERROR)\n"); return 1; }
		}
	}

	VD_PRINT("(6) ");

	//
	return 
		dhd_errorcheck(h, rw,driveNr,head,sector,cylinder);
}


