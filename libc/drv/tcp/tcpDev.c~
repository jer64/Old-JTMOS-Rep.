//=================================================================================
// TCP Device Function
//=================================================================================
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "ts.h"
#include "tcp.h"
#include "tcpService.h"
#include "tcpDev.h"

// Registers device 'network'
int tcp_register_device(void)
{
        // Register the device
        registerNewDevice(TCP_DEV_NAME,
                NULL,0,
                GetCurrentThread(), DEVICE_TYPE_NONKERNELDRV,
                0);
        return 0;
}

//
int tcpDevCall(DCPACKET *dc,
        int sndPID, int n_function, int v1, int v2, int v3, int v4,
        void *p1, void *p2)
{
	// Temporary packet
	static MSG m;
	static char tmp[2048];
	SOCKET *s;

        //
	switch(n_function)
	{
		//-------------------------------------------------------------
		case DEVICE_CMD_OPENSOCKET:
		// Connect host HOSTIP:HOSTPORT
		s = ConnectHost(v1,v2);
		// Copy DCPACKET
		memcpy(&s->dc, dc, sizeof(DCPACKET));
		// Define client's PID
		s->clientPID = sndPID;
		// Specifiy that we'll wait for respond
		return RVAL_WILL_WAIT;

		//-------------------------------------------------------------
		case DEVICE_CMD_CLOSESOCKET:
		return CloseSocketNR(v1);

		//-------------------------------------------------------------
		case DEVICE_CMD_READSOCKET:
		return 0;

		//-------------------------------------------------------------
		case DEVICE_CMD_WRITESOCKET:
		return 0;

		//-------------------------------------------------------------
		default:
		printf("%s: received unknown devcall, function=%d\n",
			__FUNCTION__, n_function);
		return -2;
	}

	//
	return 0;
}


