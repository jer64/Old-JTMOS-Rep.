//===========================================================
// JTMOS TCP/IP stack
// ip.c - Internet Protocol driver.
// (C) 2003-2005 by Jari Tuominen.
//===========================================================
#include <stdio.h>
#include "socket.h"
#include "ip.h"
#include "tcp.h"
#include "tscfg.h"
#include "icmp.h"

// Convert 4 digit IP address to 32-bit IP address
DWORD IPAD(int A1, int A2, int A3, int A4)
{
	DWORD v;
	BYTE *p;

	// Store to DWORD, byte by byte
	p=&v;
	*p++=A1;
	*p++=A2;
	*p++=A3;
	*p++=A4;
	return v;
}

// Get local host's IP address
DWORD GetLocalHostIP(void)
{
	return IPAD(HOSTIPAD0, HOSTIPAD1, HOSTIPAD2, HOSTIPAD3);
}

// Calculate 16-bit checksum
WORD ip_chksum16(WORD *wp, int words)
{
	int sum;

	// Calculate checksum
	for(sum=0; words!=0; words--,wp++)
	{
		sum += *wp;
		if(sum > 0xFFFF)
		{
			sum = ((sum+1)&0xFFFF);
		}
	}

	// Complementary operation(exclusive or)
	// (done by caler later on)
//	sum ^= 0xFFFF;

	// Return 16-bit checksum
	return sum & 0xFFFF;
}

//
int IPPacketChecksumMeasurement(IPHDR *ip)
{
	// determine checksum
	ip->chksum = ip_chksum16(ip, (5*4)>>1)^0xFFFF;
}

// Construct IP packet
// (DOES NOT MEASURE IP CHECKSUM)
// see:
// int TCPPacketChecksumMeasurement(IPHDR *ip)
int MakeIPPacket(IPHDR *ip,
	int srcip, int dstip)
{
	int i,i2,i3,i4,ii;
	WORD *wp;

	// Clear the structure first
	memset(ip, 0, sizeof(IPHDR));

	//---------------------------------------------------
	// Version 4 (ipv4)
	ip->version = 4;
	// Header length 5 (5x4 = 20 bytes)
	ip->ihl = 5;

	// Define packet length (header + data).
	// We default to size of IP+TCP headers.
	ip->len = htons( sizeof(IPHDR)+sizeof(TCPHDR) );

	// Time to live, we default to 64
	ip->ttl = 64;

	// Protocol type (6=TCP, 1=ICMP)
	ip->proto = 6;

	// Define IPs
	ip->srcip = srcip;
	ip->dstip = dstip;

	//---------------------------------------------------
	// As last thing we determine the checksum
	//

	// zero checksum
	ip->chksum = 0;

	// Make completed
	return 0;
}


