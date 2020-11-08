////////////////////////////////////////////////////////////////////////////////////////////
// fdcser.c
// Floppy disk driver service.
// (C) 2004,2014 by Jari Tuominen.
////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "fdc.h"
#include "fdcser.h"

//
static char fdcser_started=FALSE;

//////////////////////////////////////////////////////////////////////////////////
//
// DRIVER MAIN INIT
//
void fdcDriverInit(void)
{
	//
	static THREAD FDCThread;

        //
	fdcser_started=FALSE;
        ThreadCreateStack(&FDCThread,1024*16);
        FDCThread.pid =
                create_thread(FDCThread.stack,
                        FDCThread.l_stack, fdcDriverMain);

        // max. 31 characters.
        IdentifyThread(FDCThread.pid, "KFDC");
	while(fdcser_started==FALSE)
		idle_moment();

	//
	if( driver_TestDevice(ND("floppy")) )
		panic("device floppy registration failure");
}

//#define DEVICE_CALL_PARS int n_call,\
//                        int p1,int p2,int p3,int p4,\
//                        void *po1,int *po2
//#define DCP DEVICE_CALL_PARS
//
int fdcDeviceCall(
        DCPACKET *dc,
        int clientPID, int n_call,
        int p1,int p2,int p3,int p4,
        void *po1,void *po2)
{
	//
	return floppy_device_call(n_call,p1,p2,p3,p4,po1,po2);
}

//
// int fdcDeviceCall(int n_call,
//        int p1,int p2,int p3,int p4,
//        void *po1,void *po2)

// Init hard disk service
void fdcServiceInit(void)
{
	// Init hw.
	fdc_init();

	// Start message box
	startMessageBox();

	// Register device
	registerNewDevice("floppy",
		NULL,0,
		GetCurrentThread(), DEVICE_TYPE_NONKERNELDRV,
		0);
}

// Hard disk service main
void fdcServiceMain(void)
{
	static MSG m;
	DCPACKET *dc;
	int x,rval;
	int t,lt;
	char str[256],str2[256];

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
			//
			if(devser_debug)
				DumpDC(dc, __FUNCTION__);

			//
			if(dc->done==NULL || dc->rval==NULL)
			{
				//
				IllegalPacket(dc);
				GetThreadName(m.sndPID, str2);
				sprintf(str, "done=%x, rval=%x, sender PID=%d(%s)",
					dc->done,dc->rval,
					m.sndPID, str2);
				panic(str);
			}

			// Message received
			rval = fdcDeviceCall(
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
		//	printk("%s: DevCall complete.\n", __FUNCTION__);
			continue;
		}

		//--------------------------------------------------
		// Schedule
		//
		SwitchToThread();
	}
}

// End hard disk service
void fdcServiceEnd(void)
{
	//
}

//////////////////////////////////////////////////////////////////////////////////
// Start fdc disk driver
void fdcDriverMain(void)
{
	BYTE *p;

	//
	if( getdevicenrbyname("floppy")>0 )
	{
		//
		printf("Floppy disk driver is already installed.\n");
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
	printf("Floppy disk driver 1.0.\n");
	printf("(C) 2001-2004 by Jari Tuominen.\n");
	fdcServiceInit();
	fdcser_started = TRUE;
	fdcServiceMain();
	fdcServiceEnd();
	return 0;
}

//


