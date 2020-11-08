//
#ifndef __INCLUDED_RAWPACKET_H__
#define __INCLUDED_RAWPACKET_H__

//
int TransmitRawPacket(int dnr, BYTE *buf, int len);
int ReceiveRawPacket(int dnr, BYTE *buf);

#endif

