//
#ifndef __INCLUDED_DRIVER_REMDEVCALL_H__
#define __INCLUDED_DRIVER_REMDEVCALL_H__

// Enable faster write block operations.
// (DOES NOT WORK !!)
//#define USE_WRITEBLOCK_QUEUE_ACCELERATION

//
#define N_MAX_DEVQ	400

//
typedef struct
{
	//
	int isFree;
	MSG msg;
	// Block buffer
	char buf[16384];
}DCALL;

//
int remoteDeviceCall(int dnr,
	int pid,
        int n_function,
        int par1,int par2,int par3,int par4,
        void *p1,void *p2);

#endif



