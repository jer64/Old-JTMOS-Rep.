#ifndef __INCLUDED_PACKET_H__
#define __INCLUDED_PACKET_H__

//
#include "ip.h"
#include "tcp.h"

//
typedef struct
{
	//
	IPHDR ip;
	TCPHDR tcp;
	int data[0];
}PACKET;

#endif


