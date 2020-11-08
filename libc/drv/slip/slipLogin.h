#ifndef __INCLUDED_APPS_SLIP_H__
#define __INCLUDED_APPS_SLIP_H__

#include <stdio.h>

// E.g. a ping packet consists of following sections:
// [IP_PACKET]
// [ICMP_PACKET]
// [Data area / Rest]

//
typedef struct
{
	// 0
	BYTE VerIHL;
	BYTE TypeOfService;
	WORD TotalLength;
	// 4
	WORD Identification;
	WORD FragmentOffs_Flag; // Fragment offset + flag at 13-15 bits
	// 8
	BYTE Time;
	BYTE Protocol;
	WORD HeaderCheckSum;
	// 12
	DWORD SourceAddress;
	// 16
	DWORD DestinationAddress;
	// Rest is data: (not featured)
}IP_PACKET;

//
typedef struct
{
	//
	BYTE    type;
	BYTE    code;
	WORD   checksum;

	//
	union {
		struct {
			WORD id;
			WORD sequence;
		} echo;
		DWORD gateway;
		struct {
			WORD __unused;
			WORD mtu;
		} frag;
	} un;
}ICMP_PACKET;

//
typedef struct
{
	int port;
	int bps;
}SLIP;
extern SLIP slip;

//
int slipLogin(const char *arg);

#endif
