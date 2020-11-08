// ===================================================================
// low.c - LOW LEVEL DISK ACCESS ROUTINES
// (C) 2001-2003 by Jari Tuominen
// ===================================================================
#include <stdio.h>
#include "hd.h"
// Verbose debug print
#define VERBOSE_DEBUG
//
#ifdef VERBOSE_DEBUG
#define VD_PRINT dprintk
#else
#define VD_PRINT //
#endif

#define SCHEDULE nop
//#define SCHEDULE SwitchToThread

//
void hdNewOrders(HD *h,
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
int hdSectorRW(HD *h,
		BYTE *buf,int l_buf,
		int head,int sector,int cylinder,
		int driveNr, // 0=master, 1=slave
		int rw, // 0=read, 1=write
		int sectorCount)
{
	static WORD w;
	static int i,lo,hi,stat,offs,t,t2,try,lsec;

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
	if(hdWait(h))
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

	//
	if(!sector)
		panic("ZERO SECTOR DETECTED! ILLEGAL!");

	//
	try=0;
again1:
	VD_PRINT("(1) ");
	hdWait(h);
	VD_PRINT("(2) ");

	//
	hdNewOrders(h, cylinder,head,sector, rw, sectorCount);
        for(i=0,t=GetSeconds(); ; i++)
 	{
		//
		if( (GetSeconds()-t)>=5 ) {
			printk("%s: Warning. IDE didn't receive our CMD??\n",
				__FUNCTION__);
			break;
		}

		//
		if( hdGetStatus()==0x58 )
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
        for(i=0,t=GetSeconds(),lsec=t; ; i++)
 	{
		//
		VD_PRINT("(4) secs=%d  ", GetSeconds());
		// Sec has changed?
		if(GetSeconds()!=lsec)
		{
			VD_PRINT("(3) ");
			printk("[HD CMD timed-out] ");
			hdReset(h);
			hdNewOrders(h, cylinder,head,sector, rw, sectorCount);
			lsec = GetSeconds();
		}

		// Time-out check
		if( (GetSeconds()-t)>=5 ) {
						VD_PRINT("(8)\n");
						hdRecalibrate(h);
						VD_PRINT("IDE: Error #3(TIME-OUT)\n");
						return 2;
					}

		//
		stat=hdGetStatus();
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
		//
		for(i=0,offs=0; i<sectorCount;  i++, offs+=h->blocksize)
		{
			//
			if(hdRW(h, NULL, rw))break;
			// error?
			if( (stat&1) )	{ VD_PRINT("IDE: Error #1(R/W ERROR)\n"); return 1; }
		}
	}
	else
	{
		//
		for(i=0,offs=0; i<sectorCount;  i++, offs+=h->blocksize)
		{
			// Do not exceed buffer limits
			if( (offs+h->blocksize)<=l_buf )
			{
				//
				if(hdRW(h, ((void*)(buf+offs)), rw))break;
				// error?
				if( (stat&1) )
				{
					//
					VD_PRINT("IDE: Error #1(R/W ERROR)\n");
					return 1;
				}
			}
		}
	}

	//
	VD_PRINT("(6) ");

	//
	return 
		hdErrorCheck(h, rw,driveNr,head,sector,cylinder);
}


