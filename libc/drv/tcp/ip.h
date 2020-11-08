#ifndef __INCLUDED_IP_H__
#define __INCLUDED_IP_H__

// Function prototypes
int MakeIPPacket(IPHDR *ip,
        int srcip, int dstip);
WORD ip_chksum16(WORD *wp, int words);
DWORD GetLocalHostIP(void);
WORD _htons(WORD n);
DWORD IPAD(int A1, int A2, int A3, int A4);
int IPPacketChecksumMeasurement(IPHDR *ip);

#endif


