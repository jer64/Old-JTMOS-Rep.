#ifndef __INCLUDED_IPHDR_H__
#define __INCLUDED_IPHDR_H__

// Protocol types
#define IP_PROTO_ICMP   1
#define IP_PROTO_TCP    6

// IP packet header (5 x4 bytes=20 bytes)
typedef struct
{
	// Version & header length
	//	VERSION&15 << 4		(4)
	//	HEADER LENGTH & 15	(5*2, in words)
	char ihl:4, version:4;

	// Type of service(0)
	BYTE tos;

	// Packet length in bytes (header + data)
	WORD len;

	// Identification(PDU identification, set to 0)
	// 16-bit number which together
	// with the source address uniquely identifies this packet
	// - used during reassembly of fragmented datagrams.
	WORD ipid;

	// Segment offset(0)
	// Fragmentation Offset (a byte count from the start
	// of the original sent packet, set by any router which
	// performs IP router fragmentation)
	// - used during reassembly of fragmented datagrams
	WORD ipoffset;

	// Time to line		(64)
	BYTE ttl;

	// Prototype 	(6=TCP, 1=ICMP)
	BYTE proto;

	// Saadaan summaamalla 16-bittisinä lukuina
	// otsikon muut kentät yhteen.
	// Jos muistinumero tulee 17. bittiin,
	// se poistetaan ja lisätään ykkösen arvoisena.
	// Lopuksi summan kaikki bitit koplementoidaan
	// (exclusive or).
	WORD chksum;

	// IP addresses
	DWORD srcip;	// a.a.a.a
	DWORD dstip;	// b.b.b.b

	//
	BYTE data[0];
}IPHDR;

#endif


