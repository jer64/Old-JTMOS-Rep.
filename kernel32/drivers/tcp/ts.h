//
#ifndef __INCLUDED_TS_H__
#define __INCLUDED_TS_H__

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
#include "tcpDev.h"

//
int PacketReceived(PACKET *p, int len);
int SocketEngine(void);
extern int netDNR;
extern int tcp_debug;
DWORD ntohl(DWORD x);

//
#include "SendPacket.h"
#include "ConnectHost.h"

#endif


