//
#ifndef __INCLUDED_SLIPDEV_H__
#define __INCLUDED_SLIPDEV_H__

//
#define SLIP_DEV_NAME		"slip"

//
#include <stdio.h>
#include <jtmos/dcpacket.h>

//
int slip_register_device(void);
int slipDevCall(DCPACKET *dc,
        int sndPID, int n_function, int v1, int v2, int v3, int v4,
        void *p1, void *p2);

#endif



