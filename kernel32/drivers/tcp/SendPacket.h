//
#ifndef __INCLUDED_SENDPACKET_H__
#define __INCLUDED_SENDPACKET_H__

//
#include <stdio.h>
#include <jtmos/dcpacket.h>

// See tscfg.h for preferences!
//

//
#include "tscfg.h"
#include "ip.h"
#include "tcp.h"
#include "icmp.h"
#include "ts.h"
#include "socket.h"
#include "packet.h" // PACKET *
#include "tscfg.h"
#include "tcpConfig.h"

//
void SendPacket(DWORD dstip,
        int srcport, int dstport,
        int flags,                      // Send synchronize(start connection)
        int seqno, int ackno,           // SYNNO, ACKNO(0=just ignore it)
        BYTE *dataptr, int datalen);    // DATA PTR, DATA LEN

#endif


