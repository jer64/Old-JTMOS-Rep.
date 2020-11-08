//=====================================================================
// Slip Service Function
// (C) 2003-2005 by Jari Tuominen(jari@vunet.org).
//=====================================================================
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "slip.h"
#include "slipService.h"

// A client waiting for a packet receive.
WAITINGCLIENT *waitingClient=NULL;

// Send response to the client.
void SlipRespondClient(int clientPID, DCPACKET *dc, int rval)
{
	int x;

	// Store return value.
	globalcopy(
		GetCurrentThread(),&rval, // SRC
		clientPID,dc->rval,	  // DST
		4);

	// Flag as completed.
	x = TRUE;
	globalcopy(GetCurrentThread(),&x,
		clientPID,dc->done,
		4);

	// Go back to client (quick switch).
	SwitchToThreadEx(clientPID);
}

// Slip service main.
void slipServiceMain(void)
{
	static MSG m;
	DCPACKET *dc;
	int x,rval;

	//--------------------------------------------------
	// Get DCPACKET PTR.
	dc = &m.buf;

	//--------------------------------------------------
	// Loop
	//
	while(1)
	{
		// General stack call.
		slipStackCall();

		//--------------------------------------------------
		// Probe for packet.
		//
		if( receiveMessage(&m) )
		{
			//
		//	printf("%s: msgbox/devcall packet received\n",
		//		__FUNCTION__);

			// Message received.
			rval = slipDevCall(
				dc, m.sndPID,
				dc->n_function,
				dc->par1,dc->par2,dc->par3,dc->par4,
				dc->p1,dc->p2);

			//
			if(rval!=-100)
			{
				//
				SlipRespondClient(m.sndPID, dc, rval);
			}
			else
			{
				// Client is queued to wait for receiving packet.
			}
		}

		//--------------------------------------------------
		// Check for client wait time-outs.
		//
		if(waitingClient!=NULL)
		{
			//
			if( (GetSeconds() - waitingClient->time) >= TMOUT_SLIP_WAITING_CLIENT )
			{
				// Response that no packet was received at the time being
				SlipRespondClient(waitingClient->clientPID,
					&waitingClient->dc, 0);
				// Remove client's PTR from waiting place
				waitingClient = NULL;
			}
		}

		//--------------------------------------------------
		// Schedule.
		//
		SwitchToThread();
	}
}

//


