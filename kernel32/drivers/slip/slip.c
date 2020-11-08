/*** slip.c - SLIP DEVICE DRIVER ***
 * SERIAL LINE PROTOCOL DRIVER
 * (C) 2002-2005 by
 *	Jari Tuominen(jari@vunet.org).
 * Programmed according to RFC1055.
 * Feel free to spread and use this code.
 *
 * This device driver is dependent on two functions:
 *	- send_char
 *	- recv_char
 *
 * I programmed these functions to be simple wrappers
 * to the serial port driver's functions.
 * You can easily select the serial port number,
 * that is being used for slip connection by
 * changing the #define entry in slip.h.
 */

//
#include <stdio.h>
#include "slip.h"
#include "slipLogin.h"
#include "slipStack.h"

// SLIP has been dialed online yet?
// Default is 0(=offline).
char slipOnline = 0;

//
void slip_send_char(int c)
{
	serial_put(slip.port, c);
}

//
void slip_gets(char *buf, int l_buf)
{
	int i,ch;

	//
	for(; i<l_buf;)
	{
		ch = slip_recv_char();
		if(ch==-1) { continue; }
		if(!i && (ch=='\n' || ch=='\r')) { continue; }
		if(i && (ch=='\n' || ch=='\r')) { break; }
		buf[i++] = ch;
		buf[i] = 0;
	}
}

//
int slip_recv_char(void)
{
	int d1,i;

	//
	for(i=0; i<10; i++)
	{
		//
		d1=serial_get(slip.port);
		if(d1!=-1)
			return d1;
		SwitchToThread();
	}
	return -1;
}

//
void slip_send_packet(char *p, int len)
{
	BYTE *buf;
	int i,i2,i3,i4;

	// If not online, return...
	if(!slipOnline)return;

	// Send an initial END character to flush
	// out any data that may have accumulated
	// in the receiver due to line noise.
	slip_send_char(END);

	// For each byte in the packet, send
	// appropriate character sequence.
	for(buf=p,i=0; i<len; i++)
	{
		switch(buf[i])
		{
			// if it's the same code as an END character,
			// we send a special two character code so as
			// not to make the receiver think we sent an END.
			case END:
				slip_send_char(ESC);
				slip_send_char(ESC_END);
				break;
			// If it's the same code as an ESC character,
			// we send a special two character code so as not
			// to make the receiver think we sent an ESC.
			case ESC:
				slip_send_char(ESC);
				slip_send_char(ESC_ESC);
				break;
			// Otherwise, we just send the character.
			default:
				slip_send_char(buf[i]);
				break;
		}
	}

	// Tell the receiver that we're done sending
	// the packet.
	slip_send_char(END);
}

//
int slip_recv_packet(char *p, int len)
{
	int c,r;
	int received,ti;

	// If not online, return...
	if(!slipOnline)return 0;

	// Sit in a loop reading bytes until we
	// put together a whole packet.
	// Make sure not to copy them into packet
	// if we run out of room.
	for(r=0,received=0; received<len;)
	{
		// Get a character to process
		c = slip_recv_char();

		// Handle bytestuffing if necessary
		switch(c)
		{
			case -1:
			if(!received)
				return 0;
			// Keep time-out ticking -> continue:
			continue;

			// If it's an END character then
			// we're done with the packet
			case END:
				// a minor optimization: if there is no
				// data in the packet, ignore it.
				// This is meant to avoid bothering IP
				// with all the empty packets generated
				// the duplicate END characters which are in
				// turn sent to try to detect line noise.
				if(received)
					return received;
				else
					break;
			// If it's the same code as an
			// ESC character, wait and get
			// another character and then
			// figure out what to store in
			// the packet based on that.
			case ESC:
				c = slip_recv_char();

				// If "c" is not one of these two,
				// then we have a protocol violation.
				// The best bet seems to be to leave
				// the byte alone and just stuff it into
				// the packet.
				switch(c)
				{
					case ESC_END:
						c = END;
						break;
					case ESC_ESC:
						c = ESC;
						break;
				}

			// Here we fall into the default handler
			// and let it store the character for us.
			default:
				if(received < len)
				{
					p[received++] = c;
					break;
				}
				else return received;
		}
	}
	return received;
}

//
