// ===================================================================
// low.c - CHS LOW LEVEL DISK ACCESS ROUTINES
// (C) 2001-2004 by Jari Tuominen(jari@vunet.org)
// ===================================================================
#include <stdio.h>
#include "hd.h"

//
void hdNewOrders(HD *h,
			int cylinder,int head,int sector,
			int rw, int sectorCount)
{
		// Drive and head port, Drive, Head
		outportb(0x01F6,
			0xA0|(head&7)|((h->slave&1)<<4)|h->lbaBit );
		// Sector count port
		outportb(0x01F2, sectorCount);
		// Sector number port
		outportb(0x01F3, sector);
		// Cylinder low port
		outportb(0x01F4, cylinder&0xff);
		// Cylinder high port
		outportb(0x01F5, (cylinder>>8)&0xff);
		// Command
		if(rw==READ)outportb( 0x01F7,0x20 );
		else
		if(rw==WRITE)outportb( 0x01F7,0x30 );
}

// Lowest level HD sector R/W access
int hdSectorRW(HD *h,
		BYTE *buf,int l_buf,
		int C,int H,int S,	// CHS
		int driveNr, // 0=master, 1=slave
		int rw, // 0=read, 1=write
		int sectorCount)
{
	static WORD w;
	static int i,lo,hi,stat,offs,t,t2,try,lsec;

	//
	if(h->lbaMode)
	{
		panic("Attempted to access LBA mode drive via CHS functions.");
	}

	//
/*	printk("%s: CHS %d/%d/%d (drive=%d, count=%d)\n",
			__FUNCTION__,
			C, H, S,
			driveNr, sectorCount);*/
/*	if(rw==READ)
		printk("(R)");
	if(rw==WRITE)
		printk("(W)");
	printk(" CHS %d/%d/%d ",
			C, H, S);*/

	//
	if( !isValidCHS(h, C,H,S) )
	{
		printk("%s: Invalid CHS\n", __FUNCTION__);
		return 0;
	}

	// wait until the current operation has finished it's doing
	if(hdWait(h))
	{
		return 10;
	}

	//
	if(rw==READ || rw==WRITE) { }
	else
	{
		panic("dhd_blockrw: rw must be either READ or WRITE");
	}

	//
	if(!S)
		panic("ZERO SECTOR DETECTED! ILLEGAL!");

	//
	try=0;
again1:
	hdWait(h);

	//
	hdNewOrders(h, C,H,S, rw, sectorCount);
        for(i=0,t=GetSeconds(); ; i++)
 	{
		//
		if( (GetSeconds()-t)>=30 ) {
			printk("%s: Warning. IDE didn't receive our CMD??\n",
				__FUNCTION__);
			break;
		}

		//
		if( hdGetStatus()==(HD_STATUS_READY|HD_STATUS_SEEK_COMPLETE|HD_STATUS_DRQ) )
			// OK we got it
			break;
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
		// Sec has changed?
		if( (GetSeconds()-lsec)>=30 )
		{
			printk("[HD CMD timed-out] ");
			hdReset(h);
			hdNewOrders(h, C,H,S, rw, sectorCount);
			lsec = GetSeconds();
		}

		// Time-out check
		if( (GetSeconds()-t)>=60 ) {
						printk("(8)\n");
						hdRecalibrate(h);
						printk("IDE: Error #3(TIME-OUT)\n");
						return 2;
					}

		//
		stat=hdGetStatus();
		// Got data?
		if( (stat&8) )break;
		// error?
		if( (stat&1) )	{
					printk("(6)\n");
					printk("%s: error\n", __FUNCTION__);
					return 20;
				}

		//
	}

	// R/W STAGE
	// ---------
	if(buf==NULL)
	{
		//
		for(i=0,offs=0; i<sectorCount;  i++, offs+=h->blocksize)
		{
			//
			if(hdRW(h, NULL, rw))break;
		}
	}
	else
	{
		//
		for(i=0,offs=0; i<sectorCount;  i++, offs+=h->blocksize)
		{
			//
			while( hdGetStatus()!=(HD_STATUS_READY|HD_STATUS_SEEK_COMPLETE|HD_STATUS_DRQ) )
			{
				idle_moment();
			}

			// Do not exceed buffer limits
			if( (offs+h->blocksize)<=l_buf )
			{
				//
				if(hdRW(h, ((void*)(buf+offs)), rw))break;
			}
		}
	}

	// Not needed really...
	hdWait(h);

	//

	//
	return 
		hdErrorCheck(h, rw,driveNr,H,S,C);
}


