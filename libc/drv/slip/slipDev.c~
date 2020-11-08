//=================================================================================
// Slip Device Function
//=================================================================================
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "slip.h"
#include "slipStack.h"
#include "slipService.h"
#include "slipDev.h"

//
WAITINGCLIENT Client;

// Registers device 'slip'
int slip_register_device(void)
{
        // Register the device
        registerNewDevice(SLIP_DEV_NAME, // see slipDev.h
                NULL,0,
                GetCurrentThread(), DEVICE_TYPE_NONKERNELDRV,
                0);
        return 0;
}

//
int slipDevCall(DCPACKET *dc,
        int sndPID, int n_function, int v1, int v2, int v3, int v4,
        void *p1, void *p2)
{
	// Temporary packet
	static MSG m;
	SLPACKET tp,*sl;
	static char tmp[2048];

        //
	switch(n_function)
	{
		//-------------------------------------------------------------
		// Receive RAW packet
		//
		// p1 = PTR to buffer where the packet will be received
		// v1 = amount of bytes
		case DEVICE_CMD_RECEIVERAWPACKET:

		// Get packet from REC queue
		if( (sl = slipGetQueuedRecPacket())==NULL )
		{
			// No RAW packet available
		//	printf("receiverawpacket/slip: no queued recpackets, returning 0.\n");
			if(waitingClient==NULL)
			{
				//
			//	if(debug)
			//	printf("%s: placing client on waiting queue (waitingClient)\n",
			//		__FUNCTION__);

				// Setup a wait stage
				waitingClient = &Client;
				// Copy client's information for the waiting state
				memcpy(&waitingClient->dc,dc,sizeof(DCPACKET));
				// For time-out
				waitingClient->time = GetSeconds();
				//
				waitingClient->clientPID = sndPID;
				// -100 means "A CLIENT WILL STAY WAITING",
				// won't send response to the client until
				// time-out or new received packet.
				return -100;
			}
			else
				// Already waiting client, and no rec. packet on queue
				return -1;
		}

		//
		if(debug)
		printf("receiverawpacket/slip: delivering recpacket to the client\n");

		// Check packet length
		if(sl->len <= 0)
		{
			//
			if(debug)
			printf("Odd: packet length equals zero or equals below zero, discarding packet.\n");
			return 0;
		}

		// Copy to client's packet buffer
		globalcopy(
				GetCurrentThread(),sl->buf,	// SRC
				sndPID,p1,			// DST
				sl->len);

		// Free the packet from queue
		sl->isfree = TRUE;
		return sl->len;

		//-------------------------------------------------------------
		// Transmit RAW packet
		//
		// p1 = PTR to buffer where the packet will be received
		// v1 = amount of bytes
		case DEVICE_CMD_TRANSMITRAWPACKET:
		//
		if(debug)
		printf("%s: adding RAW packet to transmit queue\n",
			__FUNCTION__);
		// Check packet length
		if(v1<=15)
		{
			if(debug)
			printf("cannot transmit this packet: packet is too small\n",
				v1);
			return -3;
		}
		// Get the packet
		globalcopy(
				sndPID,p1,			// src
				GetCurrentThread(),&tmp,	// dst
				v1);
		// Define packet
		tp.buf = tmp;
		tp.len = v1;
		// Send to queue
		slipAddPacketToTraQueue(&tp);
		return 0;

		//
		default:
		if(debug)
		printf("%s: received unknown devcall, function=%d\n",
			__FUNCTION__, n_function);
		return -2;
	}

	//
	return 0;
}


