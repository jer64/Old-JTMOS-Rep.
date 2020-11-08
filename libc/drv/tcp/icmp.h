#ifndef __INCLUDED_ICMP_H__
#define __INCLUDED_ICMP_H__

//
#include "packet.h"

// ICMP types
#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO       8

// 8 bytes
typedef struct
{
	/* ICMP (echo) header. */
	BYTE type, icode;
	WORD chksum;
	WORD id, seqno;
	WORD nothing;
}ICMP;

//
WORD icmp_checksum(WORD *p_usBuffer, int words);
void GenICMPEchoReply(PACKET *p, int len, ICMP *i);

#endif


