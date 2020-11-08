//
#ifndef __INCLUDED_SLIPSERVICE_H__
#define __INCLUDED_SLIPSERVICE_H__

// Time-out for client's request for a received packet to be ejected
// and response of "no packet available" sent.
#define TMOUT_SLIP_WAITING_CLIENT	2

//
typedef struct
{
	// Client's DCPACKET, used for sending back a response
	DCPACKET dc;
	// Client's PID
	int clientPID;
	// For time-out
	int time;
}WAITINGCLIENT;

//
extern WAITINGCLIENT *waitingClient;

#endif





