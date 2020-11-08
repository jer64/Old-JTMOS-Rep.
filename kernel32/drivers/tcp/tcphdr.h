#ifndef __INCLUDED_TCPHDR_H__
#define __INCLUDED_TCPHDR_H__

// Maximum size of a TCP segment (e.g. packet size).
#define TCP_MAX_SEGSIZE		800

// TCP flags.
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20

// TCP header.
typedef struct
{
	// Source port.
	WORD srcport;
	// Destination port.
	WORD dstport;

	// Jarjestysluku.
	DWORD seqno;
	// Kuittausluku.
	DWORD ackno;

	// Important:
	// "doff" variable description:
	// (TCP HEADER LENGTH, IN 32-BIT DWORDs)
	// The number of 32-bit words in the TCP Header.  This indicates where
	// the data begins.  The TCP header (even one including options) is an
	// integral number of 32-bits long.

	//
	BYTE 	res1:4,
		doff:4;
	BYTE	fin:1,
		syn:1,
		rst:1,
		psh:1,
		ack:1,
		urg:1,
		res2:2;

	// Window size (16,384)
	WORD window;

	// TCP header checksum
	WORD chksum;

	// Urgent data pointer
	WORD urgp;

	// Options
	BYTE optdata[0];
}TCPHDR;

#endif


