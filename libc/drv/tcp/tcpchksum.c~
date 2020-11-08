//==================================================================
// tcpchksum.c
// (C) 2003 by Jari Tuominen
//==================================================================
#include <stdio.h>
#include "ts.h"

//
#define chksum ip_chksum16

// 16-bit add with overflow check (ADC addition),
// suitable for checksum calculations.
WORD CusAdd(WORD a, WORD b)
{
	WORD sum;

	//
	if((sum = a + b) < b) {
		sum++;
	}
	return sum;
}

// Measure chksum for TCP packet
// (computes parts from IP header too)
WORD tcpchksum(PACKET *p)
{
	WORD hsum, sum;
	void *po;
	WORD data[20];

	//-------------------------------------------------------------------
	// Init checksum value
	//
	sum=0;

	//
	po = p;

	//-------------------------------------------------------------------
	// Compute the checksum of the TCP header.
	// ((1))
	hsum = chksum(po+20, 20>>1);
	//
	sum = CusAdd(sum, hsum);

	//-------------------------------------------------------------------
	// ((2))
	// Compute the checksum of the data in the TCP packet and add it to
	// the TCP header checksum.
	if( (htons(p->ip.len)-40) > 0 )
	{
	//	printf("Got data? Not possible! datalen = %d\n",
	//		htons(p->ip.len)-40);
	//	abort();
		hsum = chksum( po+20+20, (htons(p->ip.len) - 40)>>1 );
	}
	else
		hsum = 0;
	//
	sum = CusAdd(sum, hsum);

	//-------------------------------------------------------------------
	// ((3))
	// Collect things to add
	data[0] = p->ip.srcip;
	data[1] = p->ip.srcip>>16;
	data[2] = p->ip.dstip;
	data[3] = p->ip.dstip>>16;
	data[4] = htons(p->ip.proto);
	data[5] = htons(htons(p->ip.len)-20); // entire length of the TCP segment

	//
	sum = CusAdd(sum, data[0]);
	sum = CusAdd(sum, data[1]);
	sum = CusAdd(sum, data[2]);
	sum = CusAdd(sum, data[3]);
	sum = CusAdd(sum, data[4]);
	sum = CusAdd(sum, data[5]);

	//
	return sum;
}


