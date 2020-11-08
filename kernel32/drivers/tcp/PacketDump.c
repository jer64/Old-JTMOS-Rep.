//========================================================
// TCP/IP packet header dumper.	
// (C) 2003-2005 by Jari Tuominen
//========================================================
#include "kernel32.h"
#include "slip.h"
#include "tcp.h"
#include "iphdr.h"
#include "tcphdr.h"

//
#define p1(x) (x&255)
#define p2(x) (x>>8&255)
#define p3(x) (x>>16&255)
#define p4(x) (x>>24&255)

//
int conv16(int x)
{
	return (((x&255)<<8) | ((x>>8)&255));
}

// DUMP IP PACKET
void ShowIPPacket(IPHDR *p)
{
	//
	printf("-- IP packet header --\n");

	//
	printf("source IP: %d.%d.%d.%d\n",
		p1(p->srcip),
		p2(p->srcip),
		p3(p->srcip),
		p4(p->srcip));

	//
	printf("destination IP: %d.%d.%d.%d\n",
		p1(p->dstip),
		p2(p->dstip),
		p3(p->dstip),
		p4(p->dstip));

	//
	printf("length=%d, tos=%d, ipoffset=%d, ttl=%d, proto=%d\n",
		conv16(p->len), p->tos, p->ipoffset, p->ttl, p->proto);

	//
	printf("IP header checksum=0x%x, ihl=%x, ipid=%d, version=%d\n",
		p->chksum, p->ihl, p->ipid, p->version);
}

// DUMP TCP PACKET
void ShowTCPPacket(TCPHDR *p)
{
	//
	printf("-- TCP packet header --\n");

	//
	printf("source port=%d, destination port=%d\n",
		conv16(p->srcport), conv16(p->dstport));

	//
	printf("seqno=%d, ackno=%d, doff=%d\n",
		conv16(p->seqno),conv16(p->ackno),
		p->doff);

	//
	printf("window=%d bytes, TCP header checksum=0x%x, urgp=0x%x\n",
		conv16(p->window), p->chksum, conv16(p->urgp));

	//
	if(p->fin)
		printf("FIN flag is set\n");
	if(p->syn)
		printf("SYN flag is set\n");
	if(p->rst)
		printf("RST flag is set\n");
	if(p->psh)
		printf("PSH flag is set\n");
	if(p->ack)
		printf("ACK flag is set\n");
	if(p->urg)
		printf("URG flag is set\n");
}

//
int PacketDump(BYTE *buf, int l_buf)
{
	TCPHDR *tcp;
	IPHDR *ip;
	int type;

	// Define PTRs
	ip = buf;
	tcp = buf+sizeof(IPHDR);

	//
	ShowIPPacket(ip);

	//
	if(ip->proto==IP_PROTO_TCP)
		// Contains TCP packet
		ShowTCPPacket(tcp);

	//
/*	if(ip->proto==IP_PROTO_ICMP)
		// Contains ICMP packet
		ShowICMPPacket(tcp);
*/
}
