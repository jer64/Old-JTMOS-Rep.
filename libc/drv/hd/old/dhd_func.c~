// dhd_func.c
// Useful functions needed by the driver.
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

//
int dhd_GetStatus(void)
{
	return inportb(0x01F7);
}

//
int dhd_waitcompletion(HDSPEC *h)
{
	return dhd_waituntilready(h);
}

void dhd_delay1(void)
{
	long i;
	for(i=0; i<100; i++)
	{ inportb(0x80); nop(); nop(); nop(); nop(); }
}

// read error register
BYTE dhd_readerror(void)
{
	return inportb(0x01F1);
}

// read status register
inline int dhd_readstatus(void)
{
	return dhd_GetStatus();
}

void dhd_waitideirq(int idenr)
{
	while(!ideirqtick[idenr]) { }
	ideirqtick[idenr]=0;
}

/**********************
 0x01 previous command ended in an error
 0x02 index - set to 1 each disk revolution
 0x04 disk data read successfully corrected
 0x08 sector buffer requires servicing
 0x10 seek complete
 0x20 write fault
 0x40 drive is ready
 0x80 controller is executing a commnad
 **********************/
// zero when drive is ready
// non-zero when it is busy
// in strcmp style
int dhd_isDriveBusy(void)
{
	// 0x80 0x40 0x10
	int v;
	v=inportb(0x01f7);
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
int dhd_isError(void)
{
	return (inportb(0x01F7)&0x01);
}

//
int dhd_waituntilready(HDSPEC *h)
{
	//
	int t;

	//
	if(h==NULL)return 1;

	// Choose drive
	dhd_chooseDrive(h->slave);

	// Reset timeout timer & wait until ready
	for(t=rtc_getseconds(); dhd_isDriveBusy(); )
	{
		//
		if(dhd_GetStatus()==0)return 2;

		// timeout handler(somewhat long period for timeout)
		if( (rtc_getseconds()-t)>=2 ) 
		{
			printk( "dhd_waituntilready: timeout error(status reads=0x%x)\n",
				dhd_GetStatus() );
			dhd_TellStatus(h);
			return 1;
		}
	}
	return 0;
}

//

