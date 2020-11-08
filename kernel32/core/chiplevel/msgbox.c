//========================================================
// IPC: Inter Process Communication: Message Box Communication System
// (Process to Process Communication System, PPCS).
// (C) 2003-2005,2013-2014 by Jari Tuominen.
//========================================================
#include "kernel32.h"
#include "msgbox.h"

//
#define X(x) (((x)&255) | ((x)&255<<8) | ((x)&255<<16) | ((x)&255<<24))

//
static DWORD unique_number=0;

//
static DWORD un(void)
{
	return unique_number++;
}

//-------------------------------------------------------------------------------------
// Activate a thread's message box
//
int ActivateThreadMessageBox(int unipid)
{
        // Check that the process really exists
        UNIF(unipid) {} else return 1;

	// Does the thread already have an existing message box?
	if(thread_msgbox[RPID(unipid)]==NULL)
	{
		// Create a new message box:
		thread_msgbox[RPID(unipid)] = createMessageBox();
	}
	return 0;
}

//-------------------------------------------------------------------------------------
//
int VerifyMessage(MSG *msg)
{
	//
	return (msg->magic1 == MSGBOX_MAGIC1 && msg->magic2 == MSGBOX_MAGIC2);
}

//-------------------------------------------------------------------------------------
// Return value:
// 0 = Message delivered
// 1 = Message couldn't have been delivered
//
int sendMessage(MSG *deliver_this, int unipid)
{
	int i,i2;
	MSGBOX *mb;
	MSG *me;
	DWORD ad;
	DCPACKET *dc;
	static char str[256];

	//
	printf("%s/%s/line %d: deliver_this=%x, unipid=%d\n",
		__FILE__, __FUNCTION__, __LINE__, unipid);
	if(!unipid) {
		printf("%s/%s/line %d: unipid is zero (%d), can't deliver\n",
			__FILE__, __FUNCTION__, __LINE__, unipid);
		return 1;
	}

	//-----------------------------------------------------------
        // Check that the process really exists.
        UNIF(unipid) {} else {
		printf("%s/%s/line %d: process (%d) does not exist, can't deliver\n",
			__FILE__, __FUNCTION__, __LINE__, unipid);
		return 1;
	}

	// Check for NULL pointer
	if( illegal(deliver_this) )
	{
		//
		printf("%s/%s: deliver_this = ILLEGAL POINTER\n",
			__FILE__, __FUNCTION__);
		// Hey it's an illegal ptr!
		return 1;
	}

	//-----------------------------------------------------------
repeat:
	// Thread must have an active message box
	if( (mb=thread_msgbox[RPID(unipid)])==NULL )
	{
		//
		printf("%s/%s: thread_msgbox[%d] is NULL\n",
			__FILE__, __FUNCTION__, RPID(unipid));
		// Thread has no message box!
		// Let's create one for it.
		ActivateThreadMessageBox(unipid);
		goto repeat;
	}

	//--------------------------------------------------------------------
	// Make sure all required pointers in the MSG structure are valid.
	//

	// Verify a DC packet.
	switch(deliver_this->protocol)
	{
		//
		case MB_PROTOCOL_DC:
		dc = &deliver_this->buf;
		ad = dc->rval; ad += GetCurrentThread();

		//
		if( illegal(ad) )
		{
			//
			sprintf(str, "%s: Illegal DC packet from PID%d.\n",
				__FUNCTION__,
				GetCurrentThread());
			panic(str);
		}
		break;

		//
		case MB_PROTOCOL_POSTMAN:
		break;
		case MB_PROTOCOL_GRAOS:
		break;

		//
		default:
	/**	sprintf(str, "%s/%s: Packet with unknown protocol (%d).",
			__FILE__, __FUNCTION__,
			deliver_this->protocol); **/
		printk("\n");
		p32(deliver_this->protocol); printk("\n");
		printk("%s/%s line %d: msgbox unknown protocol value in deliver_this->protocol\n",
			__FUNCTION__,__FILE__,__LINE__);
		return 10;
	}

	//-----------------------------------------------------------
	// Set "to be sent" -message as reserved.
	deliver_this->isFree = MSG_RESERVED;

	// Define sender's and receiver's PID
	deliver_this->sndPID = GetCurrentThread();
	deliver_this->rcvPID = unipid;

	// Set magic numbers.
	deliver_this->magic1 = MSGBOX_MAGIC1;
	deliver_this->magic2 = MSGBOX_MAGIC2;

	// Define unique number
	deliver_this->un = un();

	//
	if( !VerifyMessage(deliver_this) )
		panic("message verify failed (2)");

	//------------------------------------------------------------------------
repeatSearch:
	// Search for EMPTY LOCKER to drop it at.
	//
	for(i=0,me=NULL; i<mb->n_q; i++)
	{
retry:
		//
		me = &mb->q[i];

		// Found a free message entry?
		if( me->isFree==MSG_FREE )
		{
			// We got a free entry:

			// Deliver message
			memcpy(me, deliver_this, sizeof(MSG));

			//
			if( !VerifyMessage(me) )
				panic("msgbox.c, line 181: message verify failed (1)");

			// Message posted successfully.
			return 0;
		}
	}

	//------------------------------------------------------------------------
	return 1;
}

// A thread can receive own messages
// through this function.
//
// Return value:
// Non-zero	message was received successfully
// Zero		no new messages available
//
int receiveMessage(MSG *user_packet_buffer)
{
	DWORD age;
	int i,i2,ind;
	MSGBOX *mailbox;
	MSG *message,*got;
	
	// Check pointer
	if(user_packet_buffer==NULL)
		return 0;

repeat:
	// Get box PTR.
	mailbox = thread_msgbox[RPID(GetCurrentThread())];
	if(mailbox==NULL)
	{
		// We have no message box!
		// So, let's create one.
		ActivateThreadMessageBox(GetCurrentThread());
		goto repeat;
	}

	// Search for [oldest] arrived message in the message box.
	for(i=0,age=0xFFFFFFFF,got=NULL; i<mailbox->n_q; i++)
	{
		//
		message = &mailbox->q[i];

		// Got [older] message?
		if(message->isFree==MSG_RESERVED && message->un<=age)
		{
			if(message->magic1==MSGBOX_MAGIC1 && message->magic2==MSGBOX_MAGIC2)
			{
				//
				got = message;
				age = message->un; // un = packet number
				ind = i;
			}
			else
			{
				// Wrong magic number(s)!
				// Free the packet, it's spoiled.
				printk("%s: Flushing corrupted packet #%d (%x,%x).\n",
					__FUNCTION__, i,
					message->magic1, message->magic2);
				message->isFree = MSG_FREE;
			}
		}
	}

	// If we eventually found a message ...
	if(got!=NULL)
	{
		// Got a message:
		// --------------

	//	printk("%s: ind=%d, packet number=%d\n", 
	//		__FUNCTION__, ind, age);

		// Copy message
		memcpy(user_packet_buffer, got, sizeof(MSG));

		// Flag it as received
		got->isFree = MSG_FREE;

		// Got message, return:
		return 1;
	}

	// No new message found
	return 0;
}

// Initialize a message box
void initMessageBox(MSGBOX *queue)
{
	int i;

	//
	memset(queue, 0x66, sizeof(MSGBOX));

	//
	queue->n_q = N_MAX_QUEUED_MSGS;

	//
	for(i=0; i<queue->n_q; i++)
		queue->q[i].isFree = MSG_FREE;
}

// Create a new message box
MSGBOX *createMessageBox(void)
{
	MSGBOX *box;

	// Allocate a message box
	box = malloc(sizeof(MSGBOX));

	// Initialize message box
	initMessageBox(box);

	// Return PTR to the new message box
	return box;
}

//


