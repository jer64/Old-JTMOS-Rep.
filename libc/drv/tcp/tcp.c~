//===========================================================
// JTMOS TCP/IP stack
// tcp.c - Transport Control Protocol
// (C) 2003 by Jari Tuominen
//===========================================================
#include <stdio.h>
#include "socket.h"
#include "ip.h"
#include "tcp.h"

//
void TCPPacketChecksumMeasurement(PACKET *p)
{
	//
	p->tcp.chksum = tcpchksum(p)^0xFFFF;
}


// Construct TCP packet
// (returns length of constructed TCP header)
// (NOTE: DOES NOT MEASURE CHECKSUM)
int MakeTCPPacket(TCPHDR *t,
	WORD srcport, WORD dstport,
	int seqno, int ackno,
	BYTE flags)
{
	void *p;

	//--------------------------------------------
	// Clear TCP structure
	memset(t, 0, sizeof(TCPHDR));

	//--------------------------------------------
	// Define ports
	t->srcport = htons(srcport) & 0xFFFF;
	t->dstport = htons(dstport) & 0xFFFF;

	// Define seq/ack numbers
	t->seqno = ntohl(seqno);
	t->ackno = ntohl(ackno);

	// Define flags
	//
	if( (flags&TCP_SYN) )
		t->syn |= 1;
	if( (flags&TCP_RST) )
		t->rst |= 1;
	if( (flags&TCP_ACK) )
		t->ack |= 1;

	// Otsiko pituus DWORDeina.
	t->doff = sizeof(TCPHDR)/4;

	// Window size
	t->window = htons(16384);

	// We send out the TCP Maximum Segment
	// Size option.
/*	t->optdata[0] = 2;
	t->optdata[1] = 4;
	t->optdata[2] = (TCP_MAX_SEGSIZE) / 256;
	t->optdata[3] = (TCP_MAX_SEGSIZE) & 255;*/

	//--------------------------------------------
	// Calculate TCP header checksum
	//
	t->chksum = 0;
//	t->chksum = ip_chksum16(t, 6*2);

	// Return length of the TCP header in bytes
	return t->doff*4;
}





