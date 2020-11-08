//=====================================================================
// ts.c - TCP STACK
// (C) 2003 by Jari Tuominen
//=====================================================================
#include <stdio.h>
#include "ts.h"

//
int netDNR=-1;
int debug=0;

// Convert big-endian DWORD to little-endian DWORD
DWORD ntohl(DWORD x)
{
	return	((x&255)<<24) |
		((x>>8&255)<<16) |
		((x>>16&255)<<8) |
		(x>>24&255);
}

//
int NETDNR(void)
{
	int dnr;

	//
	dnr = getdevicenrbyname(NET_DEVICE);
	if(dnr<=0)
	{
		//
		if(debug)
			if(debug)printf("NETDNR: %s is not a valid network device.\n",
			NET_DEVICE);
		abort();
	}
	return dnr;
}

// Handle a socket that is directed as "outgoing"
// (client connection)
int SocketEngineHandleOutgoingSocket(SOCKET *s)
{
	//-----------------------------------------------------------------------
	// Handle time-outs for the connect
	//
	if(
		(s->state==TCP_STATE_WAITHANDSHAKE1
		|| s->state==TCP_STATE_WAITHANDSHAKE2)
		&& s->incbuf==NULL
		)
	{
		//
		if( (GetSeconds() - s->time) >= TM_TCP_CONNECT_TIMEOUT_SECS )
		{
			//
			if( (s->retry--) )
			{
				// Resend connect packet.
				s->state = TCP_STATE_CONNECT;
				// Reinitialize timer.
				s->time = GetSeconds();
			}
			else
			{
				// Out of retries, closing connection:

				// Send "connection failed" response
				// to the application.
				tcpServiceRespondClient(
					s->clientPID, &s->dc, -1);
				// Free socket.
				s->isfree = TRUE;
				// Continue with other sockets
				return 1;
			}
		}
	}

	//-----------------------------------------------------------------------
	// (2) We need to check for handshake1
	if(s->state==TCP_STATE_WAITHANDSHAKE1 && s->incbuf!=NULL)
	{
		// Check for SYN+ACK flags
		// (must be set)
		if(s->incbuf->tcp.syn && s->incbuf->tcp.ack)
		{
			//
			if(debug)
			printf("Sending ACK packet (I received SYN+ACK from server) ...\n");

			// Send acknowledgement
			SendPacket(s->dstip,
				s->srcport, s->dstport,
				TCP_ACK,		// Send ACK
				ntohl(s->incbuf->tcp.ackno),
				ntohl(s->incbuf->tcp.seqno)+1,// SEQ, ACK
				NULL, 0);		// DATA PTR, DATA LEN

			// Connection is now ready
			s->state = TCP_STATE_OUTGOING_OPEN;

			// Unallocate & remove incpacket
			EjectIncomingPacket(s);

			// Tell application that we're connected,
			// deliver socket handle to the application:
			tcpServiceRespondClient(
				s->clientPID, &s->dc, s->id);
		}
	}

	// (1) We need to fire up a connection?
	if(s->state==TCP_STATE_CONNECT)
	{
		// Next time we get a packet, we should get ACK=101
		s->seq.ack = 101;
		// Last sent SEQNO is 100 at the beginning
		s->seq.send = 100;

		//
		if(debug)printf("%s: sending TCP CONNECT packet\n",
			__FUNCTION__);
		// Let's connect
		SendPacket(s->dstip,
			s->srcport, s->dstport,
			TCP_SYN,	// Send synchronize(start connection)
			s->seq.send, 0,	// SYNNO, ACKNO(0=just ignore it)
			NULL, 0);	// DATA PTR, DATA LEN

		// We are waiting for reply (SYN+ACK)
		s->state = TCP_STATE_WAITHANDSHAKE1;
	}

	//-----------------------------------------------------------------------------
	// HERE THE OUTGOING CONNECTION IS OPEN,
	// WE RECEIVE DATA HERE, AND ACK THE RECEIVED PACKETS.
	//
	if(s->state==TCP_STATE_OUTGOING_OPEN && s->incbuf!=NULL &&
		s->incbuf->tcp.ack &&
		s->incbuf->tcp.rst==0 && 
		s->incbuf->tcp.syn==0 &&
		htons(s->incbuf->ip.len) > 40)
	{
		// Incoming data packet
		if(debug)
		printf("nowopen/outgoing: data packet received, answering with ACK\n");
		
		// Ack the packet
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send ACK (acks data packet)
			TCP_ACK,
			// our seq, ack it's packet
			htons(s->incbuf->tcp.ackno), ntohl(s->incbuf->tcp.seqno),
			// DATA PTR, DATA LEN
			NULL, 0);

		// Transmit data to the incoming socket stream
		if(htons(s->incbuf->ip.len) > 40)
			WriteIncomingSocketStream(s, ((void*)s->incbuf)+40,s->l_incbuf-40);

		// We've already processed this packet, so let's eject it
		EjectIncomingPacket(s);
		return;
	}

	//-----------------------------------------------------------------------------
	// Connection close request?
	//
	if(s->state==TCP_STATE_OUTGOING_OPEN && s->incbuf!=NULL &&
		s->incbuf->tcp.fin)
	{
		// Send response with FIN+ACK
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send SYN+ACK (accepts connection)
			TCP_FIN|TCP_ACK,
			// our seq, ack it's packet
			htons(s->incbuf->tcp.ackno), ntohl(s->incbuf->tcp.seqno),
			// DATA PTR, DATA LEN
			NULL, 0);

		//
		if(debug)
		printf("nowopen/outgoing: FIN(close connection) received, responding with FIN+ACK and closing socket.\n");
		// We've already processed this packet, so let's eject it
		EjectIncomingPacket(s);
		// Connection close request
		CloseSocketNR(s->id);
		return;
	}
}

// Handle incoming socket
int SocketEngineHandleIncomingSocket(SOCKET *s)
{
	//
	if(s->incbuf && debug)
		printf("There is new data in incbuf on this socket (ID=%d, state=%d).\n",
			s->id, s->state);

	//-----------------------------------------------------------------------------
	// Here we wait for the ACK packet,
	// after we got it, the connection is made.
	//
	if(s->state==TCP_STATE_INCOMINGWAITACK && s->incbuf!=NULL)
	{
		// Check flags
		if(s->incbuf->tcp.ack)
		{
			//
			if(debug)
			printf("nowopen/incoming: ACK received, connection is now open.\n");

			// Change state
			if(debug)
			printf("INCOMING CONNECTION ESTABLISHED! (SYN, SYN+ACK(WE), ACK)\n");
			s->state = TCP_STATE_INCOMING_OPEN;

			// We've already processed this packet, so let's eject it
			EjectIncomingPacket(s);
			return;
		}
	}

	//-----------------------------------------------------------------------------
	// We're just receiving incoming connection to our service port?
	//
	if(s->state==TCP_STATE_INCOMINGCONNECTION && s->incbuf!=NULL)
	{
		// Let's send response
		// -------------------

		// Define our sequence number
		s->seq.send = 100;

		//
		if(debug)
		printf("incoming/service: service is sending SYN+ACK to the client and will wait for ACK\n");

		// Send reponse
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send SYN+ACK (accepts connection)
			TCP_SYN|TCP_ACK,
			// our seq, ack it's packet
			s->seq.send, ntohl(s->incbuf->tcp.seqno)+1,
			// DATA PTR, DATA LEN
			NULL, 0);

		// We change state to wait for the ACK from the host
		s->state = TCP_STATE_INCOMINGWAITACK;

		// We've already processed this packet, so let's eject it
		EjectIncomingPacket(s);

		//
		return;
	}

	//-----------------------------------------------------------------------------
	// Connection close request?
	//
	if(s->state==TCP_STATE_INCOMING_OPEN && s->incbuf!=NULL &&
		s->incbuf->tcp.fin)
	{
		// Send response with FIN+ACK
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send SYN+ACK (accepts connection)
			TCP_FIN|TCP_ACK,
			// our seq, ack it's packet
			htons(s->incbuf->tcp.ackno), ntohl(s->incbuf->tcp.seqno),
			// DATA PTR, DATA LEN
			NULL, 0);

		//
		if(debug)
		printf("nowopen/incoming: FIN(close connection) received, responding with FIN+ACK and closing socket.\n");
		// We've already processed this packet, so let's eject it
		EjectIncomingPacket(s);
		// Connection close request
		CloseSocketNR(s->id);
		return;
	}

	//-----------------------------------------------------------------------------
	// Here the connection is open, we ack the incoming packets, so that
	// the connection remains open and otherwise ok.
	//
	if(s->state==TCP_STATE_INCOMING_OPEN && s->incbuf!=NULL &&
		s->incbuf->tcp.ack && 
		s->incbuf->tcp.rst==0 && 
		s->incbuf->tcp.syn==0 &&
		htons(s->incbuf->ip.len) > 40)
	{
		// Incoming data packet
		if(debug)
		printf("nowopen: data packet received, answering with ACK\n");
		
		// Ack the packet
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send ACK (acks data packet)
			TCP_ACK,
			// our seq, ack it's packet
			htons(s->incbuf->tcp.ackno), ntohl(s->incbuf->tcp.seqno),
			// DATA PTR, DATA LEN
			NULL, 0);

		// Transmit data to the incoming socket stream
		if(htons(s->incbuf->ip.len) > 40)
			WriteIncomingSocketStream(s, ((void*)s->incbuf)+40,s->l_incbuf-40);

		// We've already processed this packet, so let's eject it
		EjectIncomingPacket(s);
		return;
	}
}

//---------------------------------------------------------------------------------
// Fires up new TCP/IP connections
// (incoming / outgoing)
//
int SocketEngine(void)
{
	int i,l;
	PACKET p;
	SOCKET *s;

	// Walk through all open connections and handle these
	for(i=0; i<n_sockets; i++)
	{
		// Get socket
		s = socket[i];

		// Skip non-allocated sockets
		if(s->isfree)
			continue;

		// OUTGOING SOCKET:
		if(s->type & SOCKET_TYPE_OUTGOING)
		{
			// Handle outgoing socket
			if( SocketEngineHandleOutgoingSocket(s) )
				continue;
		}

		// INCOMING SOCKET:
		if(s->type & SOCKET_TYPE_INCOMING)
		{
			//
			if( SocketEngineHandleIncomingSocket(s) )
				continue;
		}
	}

	//
	return 0;
}

//---------------------------------------------------------------
// **** Allow incoming connections ****
// Connections to our servers.
//
int serviceIncomingPacket(PACKET *p, int len)
{
	SOCKET *s;

	//------------------------------------------------------------
	// Is this a CONNECT request?
	//
	if(p->tcp.syn && !p->tcp.ack)
	{
		// Open socket for INCOMING CONNECT
		s = OpenIncomingConnection(
			p->ip.srcip, htons(p->tcp.srcport),
			p->ip.dstip, htons(p->tcp.dstport));

		// Store incoming packet into the socket
		if(s!=NULL)
			StoreIncomingPacket(s, p,len);
		else
		{
			if(debug)
			printf("%s: couldn't open incoming socket, cannot handle client connection.\n",
				__FUNCTION__);
		}

		//
		return 0;
	}

	//
	printf("%s: Packet from %d.%d.%d.%d to port %d couldn't have been delivered to any known socket.\n",
		__FUNCTION__,
		p->ip.srcip & 255,
		p->ip.srcip>>8 & 255,
		p->ip.srcip>>16 & 255,
		p->ip.srcip>>24 & 255,
		htons(p->tcp.dstport));

	//
	return 1;
}

// Handle incoming TCP packets
int IncomingTcpPacket(PACKET *p, int len)
{
	SOCKET *s;

	//--------------------------------------------------------------
	// (1)
	// Is this packet going to one of the open sockets?
	//

	// Look for any socket that might want the packet
	s = SocketLookFor(
		p->ip.srcip,
		htons(p->tcp.dstport), htons(p->tcp.srcport),
		ntohl(p->tcp.seqno), ntohl(p->tcp.ackno));

	// Got socket for it?
	if(s!=NULL)
	{
		//
		if(debug)
			printf("%s: INSERTING INCOMING PACKET IN SOCKET INCOMING BUFFER\n",
				__FUNCTION__);

		// Got socket
		StoreIncomingPacket(s, p, len);
		return 0;
	}

	//--------------------------------------------------------------
	// (2)
	// Handle incoming packets to local service(s)
	//

	// Get suitable socket
	return serviceIncomingPacket(p, len);
}

// Returns amount of bytes to resend
int PacketReceived(PACKET *p, int len)
{
	BYTE *ip;
	int tmp,l;
	ICMP *i;

	//
	if(len<=20)
	{
		//
		if(debug)printf("%s: discarding packet, illegal length(%d bytes).\n",
			__FUNCTION__, l);
		// Packet length is insufficient
		return 0;
	}

	//---------------------------------------------------------------
	// Get source IP for printf
	ip = &p->ip.srcip;

	//---------------------------------------------------------------
	//
	// HANDLE ICMP PROTOCOL
	//
	if(p->ip.proto==IP_PROTO_ICMP)
	{
		// Get ICMP packet
		i = &p->tcp;
		// We only handle ICMP echo request,
		// on other cases we just discard the packet.
		if(i->type == ICMP_ECHO)
		{
			// Report
			if(debug)printf("%s: received ICMP packet from %d.%d.%d.%d\n",
				__FUNCTION__,
				ip[0], ip[1], ip[2], ip[3]);
	
			// Generate ping reply
			GenICMPEchoReply(p, len, &p->tcp);
			return len;
		}
		else
		{
			// Report discarding
			if(debug)printf("%s: discarding unknown ICMP packet(type=%d)\n",
				__FUNCTION__, i->type);
			return 0;
		}
	}

	//---------------------------------------------------------------
	//
	// HANDLE TCP PROTOCOL
	//
	if(p->ip.proto==IP_PROTO_TCP)
	{
		//
		return IncomingTcpPacket(p, len);
/*		if(debug)printf("%s: received TCP packet from %d.%d.%d.%d\n",
			__FUNCTION__,
			ip[0], ip[1], ip[2], ip[3]);*/
	}

	//---------------------------------------------------------------
	//
	// HANDLE UNKNOWN PROTOCOLS
	//
	if(debug)printf("%s: received UNKNOWN IP packet from %d.%d.%d.%d\n",
			__FUNCTION__,
			ip[0], ip[1], ip[2], ip[3]);

	//
	return 0;
}

//
void NetStackInit(void)
{
	//
	netDNR = NETDNR();
	if(netDNR<=0)
	{
		//
		if(debug)printf("Network device '%s' not found.\n",
			NET_DEVICE);
		abort();
	}

	// Initialize socket system
	SocketInit();
}



