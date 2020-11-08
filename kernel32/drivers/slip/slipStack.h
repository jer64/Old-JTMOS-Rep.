//
#ifndef __INCLUDED_SLIPSTACK_H__
#define __INCLUDED_SLIPSTACK_H__

// Time-out value for packets
#define SLIP_PACKET_TIMEOUT			20

// Max. number of packets queued at once
#define N_MAX_QUEUED_SLIP_PACKETS		40

//
typedef struct
{
	//
	char isfree;
	//
	BYTE *buf;
	int l_buf;
	int len;
	// Packet arrival time
	// (this time will be used to
	// determine whether if the packet
	// should be dismissed due to expiration)
	int time;
}SLPACKET;
extern SLPACKET sp;

//
typedef struct
{
	// Queued received packets
	SLPACKET rec[N_MAX_QUEUED_SLIP_PACKETS];
	// Queued transmittable packets
	SLPACKET tra[N_MAX_QUEUED_SLIP_PACKETS];
}SLDB;

//
SLPACKET *slipStack_ReceivePacket(void);
void slipStack_SendPacket(SLPACKET *p);
void slipStack(void);
extern int DO_PACKET_CAPTURING;
extern int debug;

#endif



