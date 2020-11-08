//==========================================================
// slipStack.c - slip packet receiver/sender
// (C) 2002-2007 by Jari Tuominen(jari@vunet.org).
//==========================================================
#include <stdio.h>
#include "slip.h"
#include "slipLogin.h"
#include "slipStack.h"
#include "slipConfig.h"
#include "slipService.h"

// Directory for packet capturing
#define PACKETS_DIRECTORY	"packets"
// Enable or disable packet capturing:
int DO_PACKET_CAPTURING=0;

// Slip packet database
SLDB sldb;
// Slip packet buffer
SLPACKET sp;
int n_packet=0;

//
void slipStackInit(void)
{
	int i;
	SLPACKET *p;

	if(DO_PACKET_CAPTURING)
	// Create packet capture directory
	mkdir(PACKETS_DIRECTORY, 0);

	// Set all packets free at the beginning
	for(i=0; i<N_MAX_QUEUED_SLIP_PACKETS; i++)
	{
		// Recs
		p = &sldb.rec[i];
		p->isfree = TRUE;

		// Tras
		p = &sldb.tra[i];
		p->isfree = TRUE;
	}
}

// Save packet to disk
void SavePacket(const char *name,
	BYTE *buf, int len)
{
	char fn[256];
	int fd;

	//
	sprintf(fn, "%s/%spacket%d.dat",
		PACKETS_DIRECTORY, name, n_packet);
	n_packet++;

	//
	fd = open(fn, O_CREAT|O_TRUNC);
	if(fd<0) { return; }

	//
	write(fd, buf, len);

	//
	close(fd);
}

// Get free REC packet entry
SLPACKET *slipGetFreeRecEntry(void)
{
	int i;
	SLPACKET *st;

	//
	for(i=0; i<N_MAX_QUEUED_SLIP_PACKETS; i++)
	{
		// Get PTR
		st = &sldb.rec[i];

		// Is free?
		if(st->isfree==TRUE)
		{
			// Allocate it
			st->isfree = FALSE;
			// Return PTR to it
			return st;
		}
	}

	//
	return NULL;
}

// Automatically searches for the oldest packet
// in the received packets queue and returns
// PTR to it.
SLPACKET *slipStack_ReceivePacket(void)
{
	int i,record;
	SLPACKET *st,*sel;

	//
	for(i=0,sel=NULL,record=GetSeconds(); i<N_MAX_QUEUED_SLIP_PACKETS; i++)
	{
		// Get PTR
		st = &sldb.rec[i];

		// Is queued?
		if(st->isfree==FALSE)
		{
			// Is this a new record?
			if(st->time < record || sel==NULL)
			{
				// Set new record time
				// (older = better record)
				record = st->time;
				// Assign this packet
				sel = st;
			}
		}
	}

	// Return PTR to packet (NULL if none found)
	return sel;
}

//
SLPACKET *slipGetFreeTraEntry(void)
{
	int i;
	SLPACKET *st;

	//
	for(i=0; i<N_MAX_QUEUED_SLIP_PACKETS; i++)
	{
		// Get PTR
		st = &sldb.tra[i];

		//
		if(st->isfree==TRUE)
		{
			//
			st->isfree = FALSE;
			return st;
		}
	}

	//
	return NULL;
}

// Add packet to "received packets queue"
// New: checks for a waiting client,
// if one exists, it'll pass the packet
// right over to it and set
// waitingClient PTR to NULL.
void slipStack_AddPacketToRecQueue(SLPACKET *p)
{
	int i;
	SLPACKET *st;
	WAITINGCLIENT *w;

	//
	w = waitingClient;

	//------------------------------------------------------------
	// Got a client waiting?
	//
	if(waitingClient)
	{
		// Report
		if(slip_debug)
		{
			printf("%s: a client(PID%d) is waiting for packet already, therefore we won't queue the packet, instead we pass it straight to the client.\n",
				__FILE__, w->clientPID);
		}

		// Deliver packet
                globalcopy(
			GetCurrentThread(),p->buf,	// SRC
			w->clientPID,w->dc.p1,		// DST
			p->len);

		// Send response to the client
		SlipRespondClient(w->clientPID, &w->dc, p->len);

		// Remove client from waiting PTR
		waitingClient = NULL;
		return;
	}

	//------------------------------------------------------------
	st = slipGetFreeRecEntry();
	if(st==NULL)
		return;

	//------------------------------------------------------------
	// Adjust buffer to match demands
	//

	// Allocate on demand
	if(st->buf==NULL)
	{
		//
		st->l_buf = p->len;
		st->buf = malloc(p->len);
	}

	// Reallocate bigger buffer on demand
	if(st->l_buf < p->len)
	{
		//
		free(st->buf);
		st->l_buf = p->len;
		st->buf = malloc(p->len);
	}

	//--------------------------------------
	// Set packet length
	st->len = p->len;

	// Store packet on buffer
	//
	memcpy(st->buf, p->buf, st->len);

	// Set receive time for time-out functionallity
	st->time = GetSeconds();
}

// Add packet to "transmit packets queue"
void slipStack_SendPacket(SLPACKET *p)
{
	int i;
	SLPACKET *st;

	//
	st = slipGetFreeTraEntry();
	if(st==NULL)
		return;

	//--------------------------------------
	// Adjust buffer to match demands
	//

	// Allocate on demand
	if(st->buf==NULL)
	{
		//
		st->l_buf = p->len;
		st->buf = malloc(p->len);
	}

	// Reallocate bigger buffer on demand
	if(st->l_buf < p->len)
	{
		//
		free(st->buf);
		st->l_buf = p->len;
		st->buf = malloc(p->len);
	}

	//--------------------------------------
	// Store packet on buffer

	// Define length
	st->len = p->len;

	//
	memcpy(st->buf, p->buf, st->len);

	// Set receive time for time-out functionallity
	st->time = GetSeconds();
}

// Clean up queues
// (only received packets queue currently requires cleaning up)
void slipCleanUpQueues(void)
{
	int i;
	SLPACKET *st;

	//
	for(i=0; i<N_MAX_QUEUED_SLIP_PACKETS; i++)
	{
		// Get PTR
		st = &sldb.rec[i];

		//
		if(st->isfree==FALSE && (GetSeconds()-st->time)>=SLIP_PACKET_TIMEOUT)
		{
			//
			if(slip_debug)printf("%s: packet time-out: removing queued recpacket %d\n",
				__FILE__, i);

			// This packet has timed out, let's free it:
			// (nobody picked up so far for a long time,
			// and it's mostly taking up the space)
			st->isfree=TRUE;
		}
	}
}

// Receive new packets to "received packets queue"
// (if any available)
void slipRecNewPackets(void)
{
	//-----------------------------------------------
	// Receive packet
	sp.len = slip_recv_packet(sp.buf, 1006);

	// Debug report
	if(sp.len)
	{
		// Packet received:

		// Report
		if(slip_debug)printf("%s: packet received, size=%d bytes\n",
			__FILE__, sp.len);

		//
		if(DO_PACKET_CAPTURING)
			// Save received packet
			if(DO_PACKET_CAPTURING)SavePacket("rec", sp.buf, sp.len);

		// Add packet to received packets queue
		slipStack_AddPacketToRecQueue(&sp);
	}
}

// Returns a queued "transmit packet"
SLPACKET *slipGetTraPacket(void)
{
	int i;
	SLPACKET *st;

	//
	for(i=0; i<N_MAX_QUEUED_SLIP_PACKETS; i++)
	{
		// Get PTR
		st = &sldb.tra[i];

		//
		if(st->isfree==FALSE)
		{
			// Return the packet that needs to be sent
			return st;
		}
	}

	//
	return NULL;
}

// Transmit queued "transmit packets"
void slipTransmitPackets(void)
{
	int i;
	SLPACKET *t;

	// Transmit all queued packets
	while( (t=slipGetTraPacket())!=NULL )
	{
		//
		if(slip_debug)
			printf("%s: now transmitting RAW packet(%d bytes)\n",
				__FILE__, t->len);

		//
		if(t->len<=15)
		{
			printf("Oops: this packet is too short to be sent\n");
		}
		else
		{
			// Send packet
			slip_send_packet(t->buf, t->len);
		}

		// Save transmitted packet
		if(DO_PACKET_CAPTURING)SavePacket("tra",
			t->buf, t->len);
		// Free packet
		t->isfree = TRUE;
	}
}

// Handle all tasks of a SLIP stack
void slipStackCall(void)
{
	// Receive new packets
	slipRecNewPackets();

	// Transmit queued "transmit packets"
	slipTransmitPackets();

	// Clean up queues, get rid of timed out packets
	slipCleanUpQueues();
}

// Slip stack main loop
void slipStack(void)
{
	DWORD flags;
	int ti,initial_ti,virgin;
	BYTE i, arptimer;

	// Register slip device
	slip_register_device();

	//
	sp.l_buf = 2048;
	sp.buf = malloc(sp.len);

	//
	IdentifyThread(GetCurrentThread(), "slipd");

	//
	slipStackInit();

	//
	slipServiceMain();

	//
	return;
}

