//
#ifndef __INCLUDED_TCPDEV_H__
#define __INCLUDED_TCPDEV_H__

//
#define TCP_DEV_NAME		"network"

//
#include <stdio.h>
#include <jtmos/dcpacket.h>

//
int tcp_register_device(void);
int tcpDevCall(DCPACKET *dc,
        int sndPID, int n_function, int v1, int v2, int v3, int v4,
        void *p1, void *p2);

#endif



