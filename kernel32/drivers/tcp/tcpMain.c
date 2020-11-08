//=======================================================================
// TCP/IP stack main process.
//=======================================================================
#include <stdio.h>
#include "slip.h"
#include "ts.h"
#include "tcpDev.h"
#include "tcpMain.h"

//
int tcpOnline=FALSE;

// TCP/IP stack main function.
int TcpProcess(void)
{
	static char tmp[8192];
	int l,i,tm;
	PACKET *p;

	//
	while(!slipOnline) { sleep(2); }
	printf("SLIP detected. Starting TCP/IP module ...\n");

	//
	if( getdevicenrbyname(TCP_DEV_NAME) > 0 )
	{
		//
		printf("TCP/IP stack driver is already installed.\n");
		ExitThread(1);
	}

	// Report
	printf("TCP/IP stack version 1.0\n");

	// Read config
	tcpReadConfiguration();

	// Init TCP/IP stack
	NetStackInit();

	// Initialize "network" device
	if(tcp_debug)printf("%s: registering 'network' device.\n", __FUNCTION__);
	tcp_register_device();

	// Report
	if(tcp_debug)printf("%s: TCP/IP stack service up and running.\n", __FUNCTION__);

	//
	tcpOnline = TRUE;

	//
	for(tm=0; ;)
	{
		// Probe for incoming packet
		if( (l=ReceiveRawPacket(NETDNR(), tmp))>=20 )
		{
			// Clear sleep mode trigger counter
			tm=0;

			//
			p = tmp;

			//
			if(tcp_debug)printf("%s: incoming packet, size=%d bytes(%s%s%s%s).\n",
				__FUNCTION__, l,
				p->tcp.syn ? "SYN " : "",
				p->tcp.ack ? "ACK " : "",
				p->tcp.rst ? "RST " : "",
				p->tcp.fin ? "FIN " : "");

			// PACKET RECEIVED
			if( (l=PacketReceived(tmp,l)) )
			{
				// Send response
				TransmitRawPacket(NETDNR(), tmp, l);
			}
		}
		else
		{
			// No packet
			tm++;
			if(tm>10)
			{
				//
				tm = 0;
				SwitchToThread();
			}
		}

		// Check received packet length validity
		if(l<0)
		{
			//
			printf("%s: Error while receiving packets from network device.\n", __FUNCTION__);
			printf("%s: Invalid network device?\n", __FUNCTION__);
			abort();
		}

		// Run socket engine
		SocketEngine();

		// Handle device calls
		tcpDeviceCallProbe();

		// Schedule
		SwitchToThread();
	}

	//
	ExitThread(0);
}



