////////////////////////////////////////////////////////////////////////////////////////////
// ramser.c
// RAM disk driver service.
// (C) 2004 by Jari Tuominen.
////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "ramdisk.h"
#include "ramser.h"

//
static char ramser_started=FALSE;

//////////////////////////////////////////////////////////////////////////////////
//
// DRIVER MAIN INIT
//
void ramDriverInit(void)
{
	//
	static THREAD RAMThread;

        //
	ramser_started=FALSE;
        ThreadCreateStack(&RAMThread,1024*16);
        RAMThread.pid =
                create_thread(RAMThread.stack,
                        RAMThread.l_stack, ramDriverMain);

        // max. 31 characters.
        IdentifyThread(RAMThread.pid, "KRAMDISK");

	//
	if( driver_TestDevice(ND("ram")) )
		print("device ram registration failure");
	else
	{
		//
		chdir("/ram");
		mkfs("");
	}

	//
}

//#define DEVICE_CALL_PARS int n_call,\
//                        int p1,int p2,int p3,int p4,\
//                        void *po1,int *po2
//#define DCP DEVICE_CALL_PARS
//
int ramDeviceCall(
        DCPACKET *dc,
        int clientPID, int n_call,
        int p1,int p2,int p3,int p4,
        void *po1,void *po2)
{
	//
	return rdDevCall(n_call,p1,p2,p3,p4,po1,po2);
}

//
// int ramDeviceCall(int n_call,
//        int p1,int p2,int p3,int p4,
//        void *po1,void *po2)

// Init hard disk service
void ramServiceInit(void)
{
	// Start message box
	startMessageBox();

	//
	rd_createdefaultramdisk();

	// Found RAM disk:

	// Register device
	registerNewDevice("ram",
		NULL,0,
		GetCurrentThread(), DEVICE_TYPE_NONKERNELDRV,
		0);
}

// Hard disk service main
void ramServiceMain(void)
{
	static MSG m;
	DCPACKET *dc;
	int x,rval;
	int t,lt;
	DWORD ad1,ad2;

	//--------------------------------------------------
	// Get DCPACKET PTR
	dc = &m.buf;

	//--------------------------------------------------
	// Loop
	//
	for(lt=GetSeconds(); ;)
	{
		//--------------------------------------------------
		// Probe for packet
		//
		if( receiveMessage(&m) )
		{
			// Report
			if(devser_debug)
				DumpDC(dc, __FUNCTION__);

			//----------------------------------------------------------
			// Message received
			rval = ramDeviceCall(
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
			continue;
		}

		//--------------------------------------------------
		// Schedule
		//
		SwitchToThread();
	}
}

// End hard disk service
void ramServiceEnd(void)
{
	//
}

//////////////////////////////////////////////////////////////////////////////////
// Start ram disk driver
void ramDriverMain(void)
{
	BYTE *p;

	//
	if( getdevicenrbyname("ram")>0 )
	{
		//
		printf("RAM disk driver is already installed.\n");
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
	printf("RAM disk driver 1.0\n");
	printf("(C) 2001-2004 by Jari Tuominen\n");
	ramServiceInit();
	ramser_started = TRUE;
	ramServiceMain();
	ramServiceEnd();
	return 0;
}

//


