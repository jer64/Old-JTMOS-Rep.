//
#ifndef __INCLUDED_TCPSERVICE_H__
#define __INCLUDED_TCPSERVICE_H__

//
#define RVAL_WILL_WAIT		-100

//
void tcpServiceRespondClient(int clientPID, DCPACKET *dc, int rval);
void tcpDeviceCallProbe(void);

#endif




