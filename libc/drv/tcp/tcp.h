#ifndef __INCLUDED_TCP_H__
#define __INCLUDED_TCP_H__

//
#include "tcpCOnfig.h"

// Maximum size of a TCP segment
// (e.g. packet size)
#define TCP_MAX_SEGSIZE		800

//
int MakeTCPPacket(TCPHDR *t,
        WORD srcport, WORD dstport,
        int seqno, int ackno,
        BYTE flags);

//
int conv16(int x);

//
void TCPPacketChecksumMeasurement(PACKET *p);

#endif


