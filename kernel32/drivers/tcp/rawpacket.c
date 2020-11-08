//
#include "kernel32.h"
#include "tcp.h"
#include "rawpacket.h"

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

