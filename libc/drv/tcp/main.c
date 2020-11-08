//=======================================================================
// TCP/IP stack main
//=======================================================================
#include <stdio.h>
#include "ts.h"
#include "tcpDev.h"

// Request 2048K of RAM from the init process.
SYSMEMREQ(APPRAM_2048K)

// TCP/IP stack main function
int main(int argc, char **argv)
{
	static char tmp[8192];
	int l,i,tm;
	PACKET *p;

	//
	if( getdevicenrbyname(TCP_DEV_NAME) > 0 )
	{
		//
		printf("TCP/IP stack driver is already installed.\n");
		return 1;
	}

	// Report
	printf("TCP/IP stack version 1.0\n");

	// Read config
	tcpReadConfiguration();

	// Init TCP/IP stack
	NetStackInit();

	// Initialize "network" device
	if(debug)printf("%s: registering 'network' device.\n", __FUNCTION__);
	tcp_register_device();

	// Report
	if(debug)printf("%s: TCP/IP stack service up and running.\n", __FUNCTION__);

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
			if(debug)printf("%s: incoming packet, size=%d bytes(%s%s%s%s).\n",
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
	return 0;
}



