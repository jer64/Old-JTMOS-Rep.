//-----------------------------------------------------------------------------------------------
// SendPacket.c
//-----------------------------------------------------------------------------------------------
#include <stdio.h>
#include "ts.h"
#include "SendPacket.h"

// Send TCP packet
void SendPacket(DWORD dstip,
	int srcport, int dstport,
	int flags,			// Send synchronize(start connection)
	int seqno, int ackno,		// SYNNO, ACKNO(0=none, SYN or ACK not set then)
	BYTE *dataptr, int datalen)	// DATA PTR, DATA LEN
{
	static BYTE buf[65536];
	PACKET *p;
	int l;

	//
	p = buf;

	//---------------------------------------------------
	// Produce IP header
	MakeIPPacket(&p->ip,
			GetLocalHostIP(),
			dstip);
	//---------------------------------------------------
	// Produce TCP header
	MakeTCPPacket(&p->tcp,
			srcport, dstport,
			seqno, ackno,
			flags);

	//---------------------------------------------------
	// MODIFY IP HEADER
	// Define correct length in IP header.
	l = sizeof(IPHDR)+sizeof(TCPHDR)+datalen;
	p->ip.len = htons(l);

	// Copy data in place.
	if(dataptr!=NULL)
		memcpy(buf+40,dataptr,datalen);

	//---------------------------------------------------
	// Measure checksums AFTER settings are set.
	IPPacketChecksumMeasurement(&p->ip);
	TCPPacketChecksumMeasurement(p);

	//
	//PacketDump(&p, l);

	// Transmit packet
	TransmitRawPacket(NETDNR(), p, l);
}


