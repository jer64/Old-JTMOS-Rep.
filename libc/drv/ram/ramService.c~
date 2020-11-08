// Hard disk driver service
// Contains main -function.
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "ram.h"
#include "ramService.h"

//
// int ramDeviceCall(int n_call,
//        int p1,int p2,int p3,int p4,
//        void *po1,void *po2)

// Init hard disk service
void ramServiceInit(void)
{
	//
	rd_init();

	// Start message box
	startMessageBox();
}

// Hard disk service main
void ramServiceMain(void)
{
	static MSG m;
	DCPACKET *dc;
	int x,rval;

	//--------------------------------------------------
	// Get DCPACKET PTR
	dc = &m.buf;

	//--------------------------------------------------
	// Loop
	//
	while(1)
	{
		//--------------------------------------------------
		// Probe for packet
		//
		if( receiveMessage(&m) )
		{
			// Report
		//	printf("%s: message received\n",
		//		__FUNCTION__);

			// Message received.
			rval = rdDevCall(
				dc, m.sndPID,
				dc->n_function,
				dc->par1,dc->par2,dc->par3,dc->par4,
				dc->p1,dc->p2);

			// Store return value.
			globalcopy(GetCurrentThread(),&rval,
				m.sndPID,dc->rval,
				4);

			// Flag as completed.
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

// Start hard disk driver
int main(int argc, char **argv)
{
	BYTE *p;

	//
	if( getdevicenrbyname(RAM_DEV_NAME) > 0 )
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
	else
		//
		printf("%s: allocation test OK\n",
			__FUNCTION__);

	//
	printf("RAM disk driver 1.0\n");
	printf("(C) 2003 by Jari Tuominen\n");
	ramServiceInit();
	ramServiceMain();
	ramServiceEnd();
	return 0;
}

//


