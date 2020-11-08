#ifndef __INCLUDED_SLIP_H__
#define __INCLUDED_SLIP_H__

#include <stdio.h>
#include "slipDev.h"
#include "slipConfig.h"

//
int slip_recv_packet(char *p,int len);
void slip_send_packet(char *p, int len);

// SPECIAL CHARACTER CODES
#define END		0300 // indicates end of packet
#define ESC		0333 // indicates byte stuffing
#define ESC_END		0334 // ESC ESC_END means END data byte
#define ESC_ESC		0335 // ESC ESC_ESC means ESC data byte

//
extern char slipOnline;

#endif
