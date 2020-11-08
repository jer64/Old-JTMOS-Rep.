//=================================================
// icmp.c - Internal Control Message Protocol
// (C) 2003 by Jari Tuominen
//=================================================
#include <stdio.h>
#include "ts.h"

// Generate ICMP checksum
WORD icmp_checksum(WORD *p_usBuffer, int words)
{
	DWORD lCheckSum = 0;

	//
	while(words > 1)
	{
		lCheckSum += *p_usBuffer++;
		words -= sizeof(WORD);
	}

	//
	if(words)
		lCheckSum += 
			*(BYTE*)p_usBuffer; // + 0 entfällt, da überflüßig

	//
	lCheckSum = ((lCheckSum >> 16) + (lCheckSum & 0xFFFF)) + (lCheckSum >> 16);
	return (WORD)(~lCheckSum);
}

// Generate ICMP reply
void GenICMPEchoReply(PACKET *p, int len, ICMP *i)
{
	int tmp;

	// Echo reply
	i->type = ICMP_ECHO_REPLY;

	// Send back to sender
	tmp = p->ip.srcip;
	p->ip.srcip = p->ip.dstip;
	p->ip.dstip = tmp;

	// Recalculate checksum
	i->chksum = 0;
	i->chksum = ip_chksum16(&p->tcp, (len-sizeof(IPHDR))>>1) ^ 0xFFFF;
}

