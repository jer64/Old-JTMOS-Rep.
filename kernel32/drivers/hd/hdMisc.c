//====================================================================
// hdWait.c
// (C) 2003-2004 by Jari Tuominen
//====================================================================
#include <stdio.h>
#include "hd.h"
#include "hdMisc.h"

// Choose drive
int hdChooseDrive(int which)
{
	outportb(0x1F6, (inportb(0x1F6)&(0xFF-0x10)) | which<<4);
}

// Get HD status
int hdGetStatus(void)
{
	return inportb(0x01F7);
}

// Delay a bit
void hdDelay(void)
{
	int i;

	//
	for(i=0; i<100; i++)
	{
		inportb(0x80); nop(); nop(); nop(); nop();
	}
}

// Read error register
BYTE hdReadError(void)
{
	return inportb(0x01F1);
}

// Read status register
inline int hdReadStatus(void)
{
	return hdGetStatus();
}

void hdWaitIDEIRQ(int idenr)
{
	idle_moment();
	while(!ideirqtick[idenr])
	{
		idle_moment();
	}
	ideirqtick[idenr]=0;
}

//
int hdIsDriveBusy(void)
{
	// 0x80 0x40 0x10
	int v;
	v=inportb(0x01F7);
	if(
	   (v&0x80)==0x80 ||
	   (v&0x40)!=0x40 ||
	   (v&0x10)!=0x10
	  )
	  return 1;
	else
	  return 0;
}

// Tells whether the last command ended in error
int hdIsError(void)
{
	return (inportb(0x01F7)&0x01);
}

// Wait until HD has completed it's
// operation and is ready for action.
int hdWait(HD *h)
{
	static int t;

	//
	if(h==NULL)return 1;

	// Choose drive
	hdChooseDrive(h->slave);

	// Reset timeout timer & wait until ready
	for(t=GetSeconds(); hdIsDriveBusy(); )
	{
		//
		if(hdGetStatus()==0)return 2;

		// timeout handler(somewhat long period for timeout)
		if( (GetSeconds()-t)>=30 ) 
		{
			printk( "hdWait: timeout error(status reads=0x%x)\n",
				hdGetStatus() );
			//hdGetStatus(h);
			return 1;
		}

		//
		idle_moment();
	}
	return 0;
}

//
int hdErrorCheck(HD *h,
	int rw, int drivenr, int headnr, int sectornr, int cylinder)
{
        //
        if( rw==READ && hdIsError() )
        {
		return 2;
        }

        // Write fault when wrote?
        if( rw==WRITE && ((hdGetStatus()&32)==32 || hdIsError()) )
        {
		printk("\n%s: chs %d/%d/%d, WRITE FAILURE!\n",
				__FUNCTION__,
				cylinder,
				headnr,
			sectornr);
		sleep(2);
		printk("\n");
		return 1;
        }
	return 0;
}

//
void hdCommand(HD *h, int which)
{
	outportb(0x1F7, which);
}

//
void hdRecalibrate(HD *h)
{
	//
	HDDEBUG
	hdChooseDrive(h->slave);
	hdReset(h);
	hdWait(h);
	hdCommand(h, 0x10);
	hdWait(h);
}

//



