//=====================================================================
// ts.c - TCP STACK.
// (C) 2003-2005 by Jari Tuominen.
//=====================================================================
#include <stdio.h>
#include "ts.h"
#include "www_server.h"

//
int netDNR=-1;
int tcp_debug=0;
int ts_debug=10;

// Convert big-endian DWORD to little-endian DWORD.
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
		if(ts_debug)
			if(ts_debug)printf("NETDNR: %s is not a valid network device.\n",
			NET_DEVICE);
		abort();
	}
	return dnr;
}

//------------------------------------------------------------------------------------
//
// Handle a socket that is directed as "outgoing".
// Client connection.
// SYN (WE), SYN+ACK, ACK (WE)
//
int SocketEngineHandleOutgoingSocket(SOCKET *s)
{
	//-----------------------------------------------------------------------
	// Handle time-outs for the connect...
	//
	if(
		s->state==TCP_STATE_WAIT_SYNACK
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
				// to the application. -- TODO:
				printk("%s: Outgoing connection attempt failed.\n",
					__FUNCTION__);

				// Free socket.
				CloseSocketNR(s->id);
				// Continue with other sockets
				return 1;
			}
		}
	}

	//-----------------------------------------------------------------------
	// (2) We need to check for handshake1
	if(s->state==TCP_STATE_WAIT_SYNACK && s->incbuf!=NULL)
	{
		// Check for SYN+ACK flags
		// (must be set)
		if(s->incbuf->tcp.syn && s->incbuf->tcp.ack)
		{
			//
			if(ts_debug)
			printf("Sending ACK packet (I received SYN+ACK from server) ...\n");

			// Send acknowledgement
			SendPacket(s->dstip,
				s->srcport, s->dstport,
				TCP_ACK,		// Send ACK
				ntohl(s->incbuf->tcp.ackno),
				ntohl(s->incbuf->tcp.seqno)+1,// SEQ, ACK
				NULL, 0);		// DATA PTR, DATA LEN

			// Connection is now ready.
			s->state = TCP_STATE_OUTGOING_OPEN;

			// Unallocate & remove incpacket.
			EjectIncomingPacket(s);
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
		if(ts_debug)printf("%s: sending TCP CONNECT packet\n",
			__FUNCTION__);
		// Let's connect ...
		SendPacket(s->dstip,
			s->srcport, s->dstport,
			TCP_SYN,	// Send synchronize(start connection)
			s->seq.send, 0,	// SYNNO, ACKNO(0=just ignore it)
			NULL, 0);	// DATA PTR, DATA LEN

		// We are waiting for reply (SYN+ACK)
		s->state = TCP_STATE_WAIT_SYNACK;
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
		if(ts_debug)
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

		// We've already processed this packet, so let's eject it.
		EjectIncomingPacket(s);
		return 1;
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
			TCP_FIN|TCP_ACK|TCP_RST,
			// our seq, ack it's packet
			htons(s->incbuf->tcp.ackno), ntohl(s->incbuf->tcp.seqno)+1,
			// DATA PTR, DATA LEN
			NULL, 0);

		//
		s->state = TCP_STATE_DISCONNECTED;
		if(ts_debug)
		printf("nowopen/outgoing: FIN(close connection) received, responding with FIN+ACK and closing socket.\n");
		// We've already processed this packet, so let's eject it.
		EjectIncomingPacket(s);
		// Connection close request
		CloseSocketNR(s->id);
		return 1;
	}
}

//------------------------------------------------------------------------------------------------------------
//
// Handle incoming socket
//
int SocketEngineHandleIncomingSocket(SOCKET *s)
{
	static BYTE content[8192];
	int content_length;

	//
/*	if(s->incbuf && ts_debug)
		printf("There is new data in incbuf on this socket (ID=%d, state=%d).\n",
			s->id, s->state);*/

	//-----------------------------------------------------------------------------
	// INCOMING CONNECTION OPEN REQUEST
	// (1) We're just receiving incoming connection to our service port?
	//
	if(  s->incbuf!=NULL &&
	(s->state==TCP_STATE_INCOMINGCONNECTION || (s->incbuf->tcp.syn && !s->incbuf->tcp.ack))  )
	{
		// Let's send response
		// -------------------

		// Reject any other port than 80 (WWW).
		if( htons(s->incbuf->tcp.dstport)!=80)
		{
			EjectIncomingPacket(s);
			return;
		}

		// Define our sequence number.
		s->seq.send = 100;

		//
		printf("incoming/service: server is sending SYN+ACK to the client and will wait for ACK\n");

		// Send reponse.
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send SYN+ACK (accepts connection)
			TCP_SYN|TCP_ACK,
			// our seq, ack it's packet
			s->seq.send, ntohl(s->incbuf->tcp.seqno)+1,
			// DATA PTR, DATA LEN
			NULL, 0);
		//
		s->seq.send++;

		// We change state to wait for the ACK from the host.
		s->state = TCP_STATE_INCOMINGWAITACK;

		// We've already processed this packet, so let's eject it.
		EjectIncomingPacket(s);

		//
		return 1;
	}

	//-----------------------------------------------------------------------------
	// CONNECTION OPEN.
	// (2) Here we wait for the ACK packet,
	// after we got it, the connection is made.
	//
	if(s->state==TCP_STATE_INCOMINGWAITACK && s->incbuf!=NULL
		&& s->incbuf->tcp.ack)
	{
		//
		printf("Now Open, incoming: ACK received, connection is now open (len=%d).\n",
			htons(s->incbuf->ip.len));

		// Change state
		printf("  INCOMING CONNECTION ESTABLISHED! (SYN, SYN+ACK(WE), ACK)\n");
		s->state = TCP_STATE_INCOMING_OPEN;

		// We've already processed this packet, so let's eject it
		EjectIncomingPacket(s);
		return 1;
	}

	//-----------------------------------------------------------------------------
	// CONNECTION OPEN:
	// (3) Here the connection is open, we ack the incoming packets, so that
	// the connection remains open and otherwise ok.
	//
	if(s->state==TCP_STATE_INCOMING_OPEN && s->incbuf!=NULL &&
		s->incbuf->tcp.ack && 
		htons(s->incbuf->ip.len) > 40)
	{
		//
		if( htons(s->incbuf->tcp.dstport)==80 )
		{
			//
			printk("Received this:\n%s\n", s->incbuf->data);
			//
			content_length = www_server(s, s->incbuf->data, content);
			// Respond...
			SendPacket(s->incbuf->ip.srcip,
				htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
				// send ACK (acks data packet), FIN tells it's the last segment for data transmission
				TCP_ACK|TCP_FIN,
				// SEQ                       ACK
				s->seq.send, ntohl(s->incbuf->tcp.seqno)+strlen(s->incbuf->data),
				// DATA PTR, DATA LEN
				content, content_length);
			//
			s->seq.send++;
			//
			if(content_length)
				printk("Sending RESPOND with ACK %d:\n%s\n",
					htons(s->incbuf->tcp.seqno),
					content);
			EjectIncomingPacket(s);
			return 1;
		}

		// Incoming data packet
		if(ts_debug)
		printf("Now open: data packet received, answering with ACK\n");

		//
		printk("content:\n%s\n", s->incbuf->data);
		
		// Ack the packet...
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send ACK (acks data packet)
			TCP_ACK,
			// our seq, ack it's packet
			s->seq.send, ntohl(s->incbuf->tcp.seqno),
			// DATA PTR, DATA LEN
			NULL, 0);
		//
		s->seq.send++;

		// Transmit data to the incoming socket stream.
		if(htons(s->incbuf->ip.len) > 40)
			WriteIncomingSocketStream(s, ((void*)s->incbuf)+40,s->l_incbuf-40);

		// We've already processed this packet, so let's eject it
		EjectIncomingPacket(s);
		return 1;
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
			// send FIN+ACK
			TCP_FIN|TCP_ACK,
			// our seq, ack it's packet
			s->seq.send, ntohl(s->incbuf->tcp.seqno)+1,
			// DATA PTR, DATA LEN
			NULL, 0);
		//
		s->seq.send++;

		//
		s->state = TCP_STATE_CLOSING1;
		printf("nowopen/incoming: FIN(close connection) received, responding with FIN+ACK and closing socket.\n");

		//
		return 1;
	}

	//-----------------------------------------------------------------------------
	// 1. Closing in progress...
	//
	if(s->state==TCP_STATE_CLOSING1 && s->incbuf!=NULL &&
		s->incbuf->tcp.ack)
	{
		// Send response with FIN+ACK
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send FIN+ACK
			TCP_FIN|TCP_ACK,
			// our seq, ack it's packet
			s->seq.send, ntohl(s->incbuf->tcp.seqno)+1,
			// DATA PTR, DATA LEN
			NULL, 0);
		//
		s->seq.send++;

		//
		s->state = TCP_STATE_CLOSING2;
		printf("Closing 1.\n");

		//
		return 1;
	}

	//-----------------------------------------------------------------------------
	// 2. Closing in progress...
	//
	if(s->state==TCP_STATE_CLOSING2 && s->incbuf!=NULL &&
		s->incbuf->tcp.ack
		&& 
		s->incbuf->tcp.fin)
	{
		// Send response with FIN+ACK
		SendPacket(s->incbuf->ip.srcip,
			htons(s->incbuf->tcp.dstport), htons(s->incbuf->tcp.srcport),
			// send ACK
			TCP_ACK,
			// our seq, ack it's packet
			s->seq.send, ntohl(s->incbuf->tcp.seqno)+1,
			// DATA PTR, DATA LEN
			NULL, 0);
		//
		s->seq.send++;

		//
		s->state = TCP_STATE_DISCONNECTED;
		printf("Closed.\n");

		//
		EjectIncomingPacket(s);
		// Close socket ...
		CloseSocketNR(s->id);

		//
		return 1;
	}

	//
	return 0;
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
			if(ts_debug)
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
		if(ts_debug)
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
		if(ts_debug)printf("%s: discarding packet, illegal length(%d bytes).\n",
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
			if(ts_debug)printf("%s: received ICMP packet from %d.%d.%d.%d\n",
				__FUNCTION__,
				ip[0], ip[1], ip[2], ip[3]);
	
			// Generate ping reply
			GenICMPEchoReply(p, len, &p->tcp);
			return len;
		}
		else
		{
			// Report discarding
			if(ts_debug)printf("%s: discarding unknown ICMP packet(type=%d)\n",
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
		//PacketDump(p, len);
		return IncomingTcpPacket(p, len);
/*		if(ts_debug)printf("%s: received TCP packet from %d.%d.%d.%d\n",
			__FUNCTION__,
			ip[0], ip[1], ip[2], ip[3]);*/
	}

	//---------------------------------------------------------------
	//
	// HANDLE UNKNOWN PROTOCOLS
	//
/*	if(ts_debug)printf("%s: received UNKNOWN IP packet from %d.%d.%d.%d\n",
			__FUNCTION__,
			ip[0], ip[1], ip[2], ip[3]);*/

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
		if(ts_debug)printf("Network device '%s' not found.\n",
			NET_DEVICE);
		abort();
	}

	// Initialize socket system
	SocketInit();
}



