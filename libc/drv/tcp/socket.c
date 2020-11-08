//===========================================================
// JTMOS TCP/IP stack
// socket.c - Socket system
// (C) 2003 by Jari Tuominen
//===========================================================

//
#include <stdio.h>
#include "ts.h"
#include "socket.h"
#include "ip.h"

// Socket database
static BYTE *big;
SOCKET *socket[N_MAX_SOCKETS]={NULL};
int n_sockets=0;

// Incoming / Outcoming port
// (port allocation database)
//
// Example:
// incomingPort[23] = 1;
// ^^ Will make the TCP/IP stack to
// accept incoming connection via TCP port 23.
char *incomingPort=NULL;
char *outcomingPort=NULL;

// Read from incoming socket stream
int ReadIncomingSocketStream(SOCKET *s, BYTE *buf, int max_len)
{
	int d1,i;

	//
	for(i=0; i<max_len; i++)
	{
		// Read byte
		if( (d1 = GetBuffer(&s->incomingStream))<0 )
			break;

		// Store byte
		buf[i] = d1;
	}
	return i;
}

// Read from incoming socket stream
int ReadOutgoingSocketStream(SOCKET *s, BYTE *buf, int max_len)
{
	int d1,i;

	//
	for(i=0; i<max_len; i++)
	{
		// Read byte
		if( (d1 = GetBuffer(&s->incomingStream))<0 )
			break;

		// Store byte
		buf[i] = d1;
	}
	return i;
}

// Write to incoming socket stream
void WriteIncomingSocketStream(SOCKET *s, BYTE *buf, int len)
{
	int i;

	//
	for(i=0; i<len; i++)
		PutBuffer(&s->incomingStream, buf[i]);
}

// Read from incoming socket stream
void WriteOutgoingSocketStream(SOCKET *s, BYTE *buf, int len)
{
	int i;

	//
	for(i=0; i<len; i++)
		PutBuffer(&s->outgoingStream, buf[i]);
}

// Removes incoming packet from the queue
void EjectIncomingPacket(SOCKET *s)
{
	//
	if(s->incbuf)
		free(s->incbuf);
	s->incbuf = NULL;
}

// Adds incoming packet in queue
int StoreIncomingPacket(SOCKET *s,  void *p, int len)
{
	// Allocate
repeat:
	if(s->incbuf==NULL)
	{
		s->incbuf = malloc(len);
		s->l_incbuf = len;
	}
#ifndef REUSE_SOCKET_INCBUF_ALWAYS
	else
	{
		//
		if(debug)
			printf("%s: cannot buffer incoming packet for socket %d, already got a packet.\n",
				__FUNCTION__, s->id);
		// Failure, already packet in queue
		return 1;
	}
#endif
	if(s->l_incbuf<len)
	{
		free(s->incbuf);
		s->incbuf = NULL;
		goto repeat;
	}

	// Copy
	memcpy(s->incbuf, p, len);
	return 0;
}

// Closes specified network socket
int CloseSocketNR(int sock)
{
	SOCKET *s;

	//-----------------------------------------------------------------
	// Check socket handle validity
	//
	if(sock<0 || sock>=n_sockets)
		return -1;

	//-----------------------------------------------------------------
	// Get PTR to socket structure
	//
	s = &socket[sock];

	//-----------------------------------------------------------------
	if(s->isfree)
		// Socket is already closed!
		return -2;

	//-----------------------------------------------------------------
	// TODO:
	// here we should close the TCP connection first.

	//

	//-----------------------------------------------------------------
	// Close socket
	//
	s->isfree = TRUE;
}

// Look for socket connection with specific client IP and 
SOCKET *SocketLookFor(DWORD clientIP,
	WORD srcport, WORD dstport,
	int seqno, int ackno)
{
	int i;
	SOCKET *s;

	// Search for a free entry
	for(i=0; i<n_sockets; i++)
	{
		// Get socket PTR
		s = socket[i];

		// Skip freed sockets
		if(s->isfree==TRUE)
			continue;

		// Report
		if(debug>1)
		printf("socket %d: src=%d.%d.%d.%d:%d, dst=%d.%d.%d.%d:%d\n",
			i,
			s->srcip & 255,
			s->srcip>>8 & 255,
			s->srcip>>16 & 255,
			s->srcip>>24 & 6255,
			s->srcport,
			s->dstip & 255,
			s->dstip>>8 & 255,
			s->dstip>>16 & 255,
			s->dstip>>24 & 6255,
			s->dstport);

		//----------------------------------------------------------------
		// OUTGOING SOCKETS
		//
		if(s->type==SOCKET_TYPE_OUTGOING)
		{
			//
			if(debug>1)
			printf("%s: (OUTGOING) comparing clientIP %x<->%x, srcport %d<->%d, dstport %d<>%d\n",
				__FUNCTION__,
				s->dstip, clientIP,
				s->srcport, srcport,
				s->dstport, dstport);
		
			//
			if(s->dstip==clientIP &&
				s->srcport==srcport &&
				s->dstport==dstport) // TODO: add check for seqnos
			{
				// Found it:
				return s;
			}
		}

		//----------------------------------------------------------------
		// INCOMING SOCKETS
		//
		if(s->type==SOCKET_TYPE_INCOMING)
		{
			//
			if(debug>1)
			printf("%s: (INCOMING) comparing srcip %x<->%x, dstport %d<->%d, srcport %d<>%d\n",
				__FUNCTION__,
				s->srcip, clientIP,
				s->dstport, srcport,
				s->srcport, dstport);
		
			//
			if(s->srcip==clientIP &&
				s->dstport==srcport &&
				s->srcport==dstport) // TODO: add check for seqnos
			{
				// Found it:
				return s;
			}
		}
	}

	// Not found:
	return NULL;
}

//
SOCKET *OpenIncomingConnection(int clientIP, int clientPort,
	int ourIP, int ourPort)
{
	SOCKET *s;

	// Get new socket
	if( (s = GetFreeSocket())==NULL )
		return NULL;

	//
	if(debug)
		printf("%s: INCOMING connection to port %d: opening incoming socket for it\n",
			__FUNCTION__, ourPort);

	// Define as incoming connection
	s->type =	SOCKET_TYPE_INCOMING;
	s->state =	TCP_STATE_INCOMINGCONNECTION;

	// Define who is connecting to us
	s->srcip =	clientIP;
	s->srcport =	clientPort;
	// Define us
	s->dstip =	ourIP;
	s->dstport =	ourPort;

	//
	return s;
}

// Get a free socket entry
SOCKET *GetFreeSocket(void)
{
	int i;

repeat:
	// Search for a free entry
	for(i=0; i<n_sockets; i++)
	{
		if(socket[i]->isfree)
		{
			// Define as allocated
			socket[i]->isfree = FALSE;
			// Set ID
			socket[i]->id = i;
			// Return PTR
			return socket[i];
		}
	}

	// Already exceeded max. amount of sockets?
	if( n_sockets>=N_MAX_SOCKETS )
		return NULL;
		else
	{
		// Get one more free socket
		n_sockets++;
		goto repeat;
	}
}

// Frees a socket
void FreeSocket(SOCKET *s)
{
	s->isfree = 1;
}

// Allocate a port
void AllocatePort(char *portsec, int which)
{
	// Define as allocated
	portsec[which] = 1;
}

// Free a port
// Example: FreePort(incomingPort, 80);
void FreePort(char *portsec, int which)
{
	// Define as free
	portsec[which] = 0;
}

// Returns -1 if no free port found
// Example:
// port = GetFreePort(outcomingPort, 8000);
// AllocatePort(port);
int GetFreePort(char *portsec, int beg)
{
	int i;

	//
	for(i=beg&0xFFFF; i<65536; i++)
	{
		if(portsec[i]==0)
			return i;
	}
	return -1;
}

// INITIALIZE SYSTEM SOCKET SYSTEM
void SocketInit(void)
{
	void *p;
	int i;

	//---------------------------------------------------------
	//
	// Init socket system
	//

	// Init socket structure
	memset(socket, 0, sizeof(socket));

	// Allocate sockets
	big = malloc(N_MAX_SOCKETS * sizeof(SOCKET));

	// Define pointers
	for(p=big,i=0; i<N_MAX_SOCKETS; p+=sizeof(SOCKET),i++)
	{
		// Define PTR
		socket[i] = p;
		// Initialize contents
		memset(socket[i], 0, sizeof(SOCKET));
		// Set as free
		socket[i]->isfree = 1;

		// Create incoming stream buffer
		CreateBuffer(&socket[i]->incomingStream, SZ_SOCKET_STREAM_BUFFER);
		// Create outgoing stream buffer
		CreateBuffer(&socket[i]->outgoingStream, SZ_SOCKET_STREAM_BUFFER);
	}

	//---------------------------------------------------------
	//
	// Init port allocation database
	//

	// 65536 incoming port states
	incomingPort = malloc(65536);
	memset(incomingPort, 0, 65536);

	// 65536 outcoming port states
	outcomingPort = malloc(65536);
	memset(outcomingPort, 0, 65536);
}

//

