//
#ifndef __INCLUDED_NETWORK_H__
#define __INCLUDED_NETWORK_H__

//
#include <jtmos/basdef.h>
#include <jtmos/ip.h>
#include <jtmos/tcp.h>

//
int TransmitRawPacket(int dnr, BYTE *buf, int len);
int ReceiveRawPacket(int dnr, BYTE *buf);

//
int WriteSocket(int handle, BYTE *buf, int len);
int ReadSocket(int handle, BYTE *buf);
int CloseSocket(int handle);
int OpenSocket(DWORD ipaddr, int port);

//
int htons(int x);
#define ntohs(n) htons(n)

#endif


