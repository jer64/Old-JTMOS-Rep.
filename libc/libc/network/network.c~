//==================================================================
// NETWORK FUNCTIONS
// (C) 2003-2005.
//==================================================================
#include <stdio.h>
#include <dcpacket.h>
#include <dpi.h>
#include "network.h"

// Convert from big-endian WORD to little-endian WORD
// (same as htons)
int htons(int x)
{
        return (((x&255)<<8) | ((x>>8)&255));
}

//------------------------------------------------------------------------------
// Return value:
// - values below zero mean error.
// - values equal to zero and greater indicate a Socket Handle
int OpenSocket(DWORD ipaddr, int port)
{
	//
	return scall(SYS_opensocket, ipaddr,port,0, 0,0,0);
}

//------------------------------------------------------------------------------
int CloseSocket(int handle)
{
	//
	return scall(SYS_closesocket, handle,0,0, 0,0,0);
}

//------------------------------------------------------------------------------
int ReadSocket(int handle, BYTE *buf)
{
	//
	return scall(SYS_opensocket, handle,buf,0, 0,0,0);
}

//------------------------------------------------------------------------------
int WriteSocket(int handle, BYTE *buf, int len)
{
	//
	return scall(SYS_opensocket, handle,buf,len, 0,0,0);
}

//------------------------------------------------------------------------------
// Transmit RAW packet
int TransmitRawPacket(int dnr, BYTE *buf, int len)
{
	//
	return DeviceCall(dnr,
		DEVICE_CMD_TRANSMITRAWPACKET,
		len,0,0,0,
		buf,0);
}

//------------------------------------------------------------------------------
// Receive RAW packet
int ReceiveRawPacket(int dnr, BYTE *buf)
{
	//
	return DeviceCall(dnr,
		DEVICE_CMD_RECEIVERAWPACKET,
		0,0,0,0,
		buf,0);
}



