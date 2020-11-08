// ConnectHost.c
#include <stdio.h>
#include "ts.h"
#include "ConnectHost.h"

// Connect to TCP host:port
SOCKET *ConnectHost(DWORD dstip, int dstport)
{
	SOCKET *s;

	//----------------------------------------------------------------
	// Get new socket
	if( (s = GetFreeSocket())==NULL )
	{
		//
		if(tcp_debug)
			printf("%s/%s: WARNING, OUT OF SOCKETS\n",
				__FUNCTION__, __FILE__);
		// All sockets used
		return NULL;
	}

	//----------------------------------------------------------------
	// Init socket structure
	memset(s, 0, sizeof(SOCKET));

	// Get new outcoming port
	s->srcport = GetFreePort(outcomingPort, 1024);
	AllocatePort(outcomingPort, s->srcport);
	// Define destination port
	s->dstport = dstport;

	// Define destination IP
	s->dstip =	dstip;
	// Define source IP(our IP)
	s->srcip =	GetLocalHostIP();

	// Set to 100 as default
	s->seq.send = 100;

	// Define connection state
	s->state = TCP_STATE_CONNECT;

	// Last action time (used for time-out determining)
	s->time = GetSeconds();
	// How many retries left? (retries left to attempt to connect)
	s->retry = 3;

	// Define type
	s->type = SOCKET_TYPE_OUTGOING;

	//
	printf("New socket created, connecting to host %d.%d.%d.%d:%d ...\n",
		s->dstip&255,
		s->dstip>>8&255,
		s->dstip>>16&255,
		s->dstip>>24&255,
		s->dstport);

	// Return new socket PTR
	return s;
}



