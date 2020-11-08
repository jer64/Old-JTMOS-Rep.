//=========================================================
// driver_remdevcall.c - remote device call
// Remote device call is required when a non-kernel mode
// device driver is being accessed.
// Implements MSGBOX communication.
// (C) 2003-2004 by Jari Tuominen.
//=========================================================
#include "kernel32.h"
#include "driver_sys.h"
#include "driver_api.h"
#include "driver_remdevcall.h"

// Cached device block sizes
static int dbs[256]={0};
// Cache device sizes
static int ds[256]={0};

// Device call queue
int init_devq=1;
DCALL *devq[N_MAX_DEVQ];

////////////////////////////////////////////////////////////////////////////////////////////
//
int remoteDeviceCall(int dnr,
	int pid,	
	int n_function,
	int par1,int par2,int par3,int par4,	
	void *p1,void *p2)
{
	MSG *m;
	DCPACKET *dc;
	int done,rval,t,i,co,time;
	char str[50];

	////////////////////////////////////////////////////////////////////////////////////////////
	// Got a cached answer? (I)
	if(n_function==DEVICE_CMD_GETBLOCKSIZE && dbs[dnr])
	{
		*((int*)p1) = dbs[dnr];
		return 0;
	}

	// Got a cached answer? (II)
	if(n_function==DEVICE_CMD_GETSIZE && ds[dnr])
	{
		*((int*)p1) = ds[dnr];
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//
	dprintk("%s: dnr=%d, func=%d, v1=%d, v2=%d, v3=%d, v4=%d, p1=%x, p2=%x, pid=%d\n",
		__FUNCTION__, dnr, n_function,
		par1,par2,par3,par4,
		p1,p2, pid);

	//---------------------------------------------------
	// Initialize packet structure.
	//
	m = malloc(sizeof(MSG));
	memset(m, 0, sizeof(MSG));

	// Get PTR to DC packet capsule.
	dc = m->buf;

	//---------------------------------------------------
	// Construct Device Call packet.
	//

	// Define "done" as false
	done = FALSE;
	// Point done PTR at done variable
	dc->done = &done;
	// Point rval PTR at rval variable
	rval = -1;
	dc->rval = &rval;

	// Copy PTRs
	dc->p1 = p1;
	dc->p2 = p2;
	
	// Copy pars
	dc->par1 = par1;
	dc->par2 = par2;
	dc->par3 = par3;
	dc->par4 = par4;

	// Copy n_function
	dc->n_function = n_function;

	////////////////////////////////////////////////////////////////////////////////////////////
	// Send message to the device driver's service process
	//
	m->protocol = MB_PROTOCOL_DC;
	//
	sendMessage(m, pid);

	//
	co = GetSwitchCount();
	// Switch to the receiver PID.
	SwitchToThreadEx(pid);

	// Wait for completion (upto 5 seconds).
	for(i=0,time=GetSeconds(); done==FALSE && (GetSeconds()-time)<5; i++)
	{
		// Spare CPU
		SwitchToThread();
	}
//	dprintk("%s: I waited for %d schedule times. Meanwhile there was %d task switches.\n",
//		__FUNCTION__, i, GetSwitchCount()-co);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Store answer in cache, so we won't need to remote call next time,
	// the answer will be same anyways.
	if(n_function==DEVICE_CMD_GETBLOCKSIZE)
		dbs[dnr] = *((int*)p1);
	if(n_function==DEVICE_CMD_GETSIZE)
		ds[dnr] = *((int*)p1);

	////////////////////////////////////////////////////////////////////////////////////////////
	//
	free(m);

	////////////////////////////////////////////////////////////////////////////////////////////
	// Return the return value
	return rval;
}

