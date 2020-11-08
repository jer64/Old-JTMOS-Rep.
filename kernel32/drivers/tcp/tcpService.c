//=====================================================================
// TCP Service Function
// (C) 2003 by Jari Tuominen
//=====================================================================
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "ts.h"
#include "tcp.h"
#include "tcpService.h"

// Send response to the client
void tcpServiceRespondClient(int clientPID, DCPACKET *dc, int rval)
{
	int x;

	// Store return value
	globalcopy(
		GetCurrentThread(),&rval, // SRC
		clientPID,dc->rval,	  // DST
		4);

	// Flag as completed
	x = TRUE;
	globalcopy(GetCurrentThread(),&x,
		clientPID,dc->done,
		4);

	// Go back to client (quick switch)
	SwitchToThreadEx(clientPID);
}

// TCP device call probe
// (handles incoming device calls)
void tcpDeviceCallProbe(void)
{
	static MSG m;
	DCPACKET *dc;
	int x,rval;

	//--------------------------------------------------
	// Get DCPACKET PTR
	dc = &m.buf;

	//--------------------------------------------------
	// Probe for packet
	//
	if( receiveMessage(&m) )
	{
		// Message received.
		rval = tcpDevCall(
			dc, m.sndPID,
			dc->n_function,
			dc->par1,dc->par2,dc->par3,dc->par4,
			dc->p1,dc->p2);

		//
		if(rval!=RVAL_WILL_WAIT)
		{
			//
			tcpServiceRespondClient(m.sndPID, dc, rval);
		}
		else
		{
			// Client is queued to wait
		}
	}
}

//


