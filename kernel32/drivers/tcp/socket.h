#ifndef __INCLUDED_SOCKET_H__
#define __INCLUDED_SOCKET_H__

//
#include <jtmos/dcpacket.h>
#include "packet.h"

// Allow socket->incbuf to automatically reused
// on new packet arrival.
// All those non-processed packets will
// not be store anywhere, besides this
// should not bring any problems.
#define REUSE_SOCKET_INCBUF_ALWAYS

// Max. number of sockets
#define N_MAX_SOCKETS           100
// Define socket stream buffer size
#define SZ_SOCKET_STREAM_BUFFER	1024

//    SYN-SENT STATE
//    SYN-RECEIVED STATE
//    ESTABLISHED STATE
//    FIN-WAIT-1 STATE
//    FIN-WAIT-2 STATE
//    CLOSE-WAIT STATE
//    CLOSING STATE
//    LAST-ACK STATE
//    TIME-WAIT STATE

// OUTGOING CONNECTIONS: TCP connection states
#define TCP_STATE_DISCONNECTED		0
#define TCP_STATE_CONNECT		10
#define TCP_STATE_WAITHANDSHAKE1	1
#define TCP_STATE_WAITHANDSHAKE2	2
#define TCP_STATE_WAITHANDSHAKE3	3
#define TCP_STATE_WAIT_SYNACK		10
#define TCP_STATE_SENDREQUEST		4
#define TCP_STATE_RECVDATA		5
#define TCP_STATE_OUTGOING_OPEN		6

//
#define TCP_STATE_CLOSING1		11
#define TCP_STATE_CLOSING2		12
#define TCP_STATE_CLOSING3		13
#define TCP_STATE_CLOSING4		14
#define TCP_STATE_CLOSING5		15

// INCOMING CONNECTIONS
#define TCP_STATE_INCOMINGCONNECTION	7
#define TCP_STATE_INCOMINGWAITACK	8
#define TCP_STATE_INCOMING_OPEN		9

// Socket stream
#define SOCKET_TYPE_STREAM		1
// Packet queue socket (not implemented yet!)
#define SOCKET_TYPE_PACKET_QUEUE	2
// Service connection <-
#define SOCKET_TYPE_INCOMING		0x4000
// Client connection ->
#define SOCKET_TYPE_OUTGOING		0x8000

// Socket structure(socket = a connection)
typedef struct
{
	// Is free: TRUE/FALSE
	int isfree;

	// Socket type
	int type;

	// socket ID
	int id;

	// How many retries left?
	char retry;

	// Start time for time-out
	int time;

	// Device call packet
	// (for sending response to the application)
	DCPACKET dc;
	// For responding
	int clientPID;

	// Connection state
	int state;

	// IP address of the host we are connected at
	// and also our IP
	// (+ ports)
	int srcip,srcport;
	int dstip,dstport;

	// Sequence numbers
	struct
	{
		// The sequence number expected to receive next(0 if none).
		int recv;
		// The sequence number that was last sent by us.
		int send;
		// The sequence number that should be
		// ACKed by next ACK from peer
		int ack;
	}seq;

	// Maximum segment size for this connection(socket)
	int mss;

	// How many times we've retransmitted the packet
	int retransmit;

	// Incoming data stream
	FBUF incomingStream;
	FBUF outgoingStream;

	// Incoming packet for this socket
	PACKET *incbuf;
	int l_incbuf;
}SOCKET;

//
SOCKET *GetFreeSocket(void);
void SocketInit(void);
void FreeSocket(SOCKET *s);

// Port allocation database
extern char *incomingPort;
extern char *outcomingPort;
// Sockets
extern SOCKET *socket[N_MAX_SOCKETS];
extern int n_sockets;

// Port allocation functions
int GetFreePort(char *portsec, int beg);
void AllocatePort(char *portsec, int which);
void FreePort(char *portsec, int which);

//
SOCKET *OpenIncomingConnection(int clientIP, int clientPort,
        int ourIP, int ourPort);
int CloseSocketNR(int sock);
int StoreIncomingPacket(SOCKET *s,  void *p, int len);
void EjectIncomingPacket(SOCKET *s);

//
void WriteIncomingSocketStream(SOCKET *s, BYTE *buf, int len);
void WriteOutgoingSocketStream(SOCKET *s, BYTE *buf, int len);
int ReadOutgoingSocketStream(SOCKET *s, BYTE *buf, int max_len);
int ReadIncomingSocketStream(SOCKET *s, BYTE *buf, int max_len);

#endif


