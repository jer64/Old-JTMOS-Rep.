// SendPacket.c
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
	PACKET p;
	int l;

	//---------------------------------------------------
	// Produce IP header
	MakeIPPacket(&p.ip,
			GetLocalHostIP(),
			dstip);
	//---------------------------------------------------
	// Produce TCP header
	MakeTCPPacket(&p.tcp,
			srcport, dstport,
			seqno, ackno,
			flags);

	//---------------------------------------------------
	// MODIFY IP HEADER
	// Define correct length in IP header.
	l = sizeof(IPHDR)+sizeof(TCPHDR)+datalen;
	p.ip.len = htons(l);

	// Measure checksums later on(must be done this way!)
	IPPacketChecksumMeasurement(&p.ip);
	TCPPacketChecksumMeasurement(&p);

	// Transmit packet
	TransmitRawPacket(NETDNR(), &p, l);
}


