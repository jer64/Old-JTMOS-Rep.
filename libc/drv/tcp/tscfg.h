//=======================================================
// TCP/IP STACK CONFIGURATION
//=======================================================
#ifndef __INCLUDED_TSCFG_H__
#define __INCLUDED_TSCFG_H__

// How many seconds to wait until attempting
// to resend the connect request?
#define TM_TCP_CONNECT_TIMEOUT_SECS	2

// Which packet device to use?
#define NET_DEVICE      "slip"

// HOST IP ADDRESS
#define HOSTIPAD0       ipad0
#define HOSTIPAD1       ipad1
#define HOSTIPAD2       ipad2
#define HOSTIPAD3       ipad3

// HOST IP MASK
#define HOSTIPMA0       ipma0
#define HOSTIPMA1       ipma1
#define HOSTIPMA2       ipma2
#define HOSTIPMA3       ipma3

//
extern int ipad0;
extern int ipad1;
extern int ipad2;
extern int ipad3;
extern int ipma0;
extern int ipma1;
extern int ipma2;
extern int ipma3;

#endif


