// Hard disk driver service
// Contains main -function.
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "hd.h"
#include "hdService.h"

//
// int hdDeviceCall(int n_call,
//        int p1,int p2,int p3,int p4,
//        void *po1,void *po2)

// Init hard disk service
void hdServiceInit(void)
{
	// Start message box
	startMessageBox();

	// Detect HD
	if( hdInit() )
	{
		// Found HD:

		// Register device
		registerNewDevice("hda",
			NULL,0,
			GetCurrentThread(), DEVICE_TYPE_NONKERNELDRV,
			0);
	}
}

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
			// Report
		//	printf("%s: received message\n",
		//		__FUNCTION__);

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

// End hard disk service
void hdServiceEnd(void)
{
	//
}

// Start hard disk driver
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
	printf("IDE hard disk driver 1.0\n");
	printf("(C) 2003 by Jari Tuominen\n");
	hdServiceInit();
	hdServiceMain();
	hdServiceEnd();
	return 0;
}

//


