////////////////////////////////////////////////////////////////////////////////////////////
// hdService.c
// IDE hard disk driver service.
// (C) 2004 Jari Tuominen.
////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "hd.h"
#include "hdService.h"

////////////////////////////////////////////////////////////////////////////////////////////
//
static char hdReady=FALSE;

//
// int hdDeviceCall(int n_call,
//        int p1,int p2,int p3,int p4,
//        void *po1,void *po2)

//
#ifdef JTMOSKERNEL
////////////////////////////////////////////////////////////////////////////////////////////
//
void hdDriverInit(void)
{
	//
	static THREAD HDThread;

	//
	hdReady = FALSE;

        //
        ThreadCreateStack(&HDThread,1024*16);
        HDThread.pid =
                create_thread(HDThread.stack,
                        HDThread.l_stack, hdDriverMain);

        // max. 31 characters.
        IdentifyThread(HDThread.pid, "KIDEHD");

	// Wait until the service is up and running.
	while(hdReady != TRUE)
		idle_moment();

	//
	if( driver_TestDevice(ND("hda")) )
		panic("device hda registration failure");
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
// Hard disk driver service process.
//
#ifdef JTMOSKERNEL
void hdDriverMain(void)
#else
int main(int argc, char **argv)
#endif
{
	BYTE *p;

	//
	if( getdevicenrbyname("hda")>0 )
	{
		//
		printf("IDE hard disk driver is already installed.\n");
		return 1;
	}

	// Test malloc
	p = malloc(1024*64);
	if(p==NULL)
	{
		//
		printf("Malloc isn't working, returns NULL pointer.\n");
		return 0;
	}
	free(p);

	//
	printf("IDE hard disk driver 2.0  (C) 2003-2004 by Jari Tuominen\n");
	hdServiceInit();
	hdServiceMain();
	hdServiceEnd();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
static void demoWrite(int lba)
{
	BYTE dat;
	int c,h,s,i,i2,i3,i4;
	BYTE tmp[512];

	// Generate buffer data.
	dat = lba&255;
	//
	memset(tmp,dat,512);

	// Write to HD.
	LinearToCHS(chd,lba,
		&c,&h,&s);
	//
	hdSectorRW(chd,
		tmp,512,
		c,h,s,
		chd->slave,
		WRITE,
		1);
		
}

////////////////////////////////////////////////////////////////////////////////////////////
static void demoVerify(int lba)
{
	BYTE dat;
	int c,h,s,i,i2,i3,i4;
	BYTE tmp[512];

	//
	dat = lba&255;

	//
	LinearToCHS(chd,lba,
		&c,&h,&s);
	//
	hdSectorRW(chd,
		tmp,512,
		c,h,s,
		chd->slave,
		READ,
		1);

	//
	for(i=0; i<512; i++)
	{
		if(tmp[i]!=dat)
		{
			printk("ER W %d/%d/%d, ",
				c,h,s);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void HD_DEMO(void)
{
	int c,h,s,i,i2,i3,i4;

	//
	for(i=0; i<1500; i++)
	{
		demoWrite(i);
	}

	//
	for(i=0; i<1000; i++)
	{
		demoVerify(i);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Init hard disk service
void hdServiceInit(void)
{
	// Start message box
	startMessageBox();

	// Detect HD
	if( hd_init() )
	{
		// Found HD:

		// Register device
		registerNewDevice("hda",
			NULL,0,
			GetCurrentThread(), DEVICE_TYPE_NONKERNELDRV,
			0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Hard disk service main
void hdServiceMain(void)
{
	static MSG m;
	DCPACKET *dc;
	int x,rval;
	int t,lt;

	//--------------------------------------------------
	// Get DCPACKET PTR
	dc = &m.buf;

	//
	hdReady = TRUE;

	//--------------------------------------------------
	// Loop
	//
	for(lt=GetSeconds(); ;)
	{
		//--------------------------------------------------
		// Autoflush cache
		//
		t = GetSeconds();
		if( (t-lt) >= 3 )
		{
			//
			lt = GetSeconds();
			hdFlushWriteBack();
		}

retry:
		//--------------------------------------------------
		// Probe for packet
		//
		if( receiveMessage(&m) )
		{
			//
			if(devser_debug)
				DumpDC(dc, __FUNCTION__);

			// Message received
			rval = hdDeviceCall(
				dc, m.sndPID,
				dc->n_function,
				dc->par1,dc->par2,dc->par3,dc->par4,
				dc->p1,dc->p2);

			// Store return value
			globalcopy(GetCurrentThread(),&rval,
				m.sndPID,dc->rval,
				4);

			// Flag as completed
			x = TRUE;
			globalcopy(GetCurrentThread(),&x,
				m.sndPID,dc->done,
				4);

			// Go back to client
			SwitchToThreadEx(m.sndPID);
			goto retry;
		}

		//--------------------------------------------------
		// Schedule
		//
		SwitchToThread();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// End hard disk service
void hdServiceEnd(void)
{
	//
}



