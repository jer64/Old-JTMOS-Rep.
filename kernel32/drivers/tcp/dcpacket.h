//
#ifndef __INCLUDED_DCPACKET_H__
#define __INCLUDED_DCPACKET_H__

//
// Parameters for standard DEVICE_CALL function:
//
#define DEVICE_CALL_PARS int n_call,\
                        int p1,int p2,int p3,int p4,\
                        void *po1,int *po2
#define DCP DEVICE_CALL_PARS

// SREQ structure
typedef struct
{
        int n_call;
        int p1,p2,p3,p4;
        void *po1,*po2;
}SREQ;

//
extern char *devCmdNames[];

// DEVICE COMMANDS
#define DEVICE_CMD_INIT                 0x01
#define DEVICE_CMD_SHUTDOWN             0x02
#define DEVICE_CMD_READBLOCK            0x03
#define DEVICE_CMD_WRITEBLOCK           0x04
#define DEVICE_CMD_GETSIZE              0x06
#define DEVICE_CMD_GETBLOCKSIZE         0x07
#define DEVICE_CMD_SEEK                 0x08
#define DEVICE_CMD_GETCH                0x09
#define DEVICE_CMD_PUTCH                0x0A
#define DEVICE_CMD_TELL                 0x0B
#define DEVICE_CMD_FLUSH                0x0C
#define DEVICE_CMD_MOUNT                0x0D
#define DEVICE_CMD_UNMOUNT              0x0E
#define DEVICE_CMD_RECEIVERAWPACKET	0x0F
#define DEVICE_CMD_TRANSMITRAWPACKET	0x10
#define DEVICE_CMD_OPENSOCKET		0x11
#define DEVICE_CMD_CLOSESOCKET		0x12
#define DEVICE_CMD_READSOCKET		0x13
#define DEVICE_CMD_WRITESOCKET		0x14
#define DEVICE_CMD_PING			0x15

// Specifies that the device is a block device,
// it inputs/outputs blocks with a linear
// offset selector to specify a read/write offset.
#define DEVICE_TYPE_BLOCK               0x0001
// Specifies that the device is only able
// to input/output chars with a linear offset
// selector to specify a read/write offset.
#define DEVICE_TYPE_CHAR                0x0002
// Specifies that the device driver does not
// use kernel memory space, and it is an
// independent [application] process.
#define DEVICE_TYPE_NONKERNELDRV        0x0004

// Device Call Packet
typedef struct
{
	// Device call function call # (command number)
	int n_function;
	// Parameters
	int par1,par2,par3,par4;
	// Pointers
	void *p1,*p2;
	// Pointer to an integer variable which
	// indicates when the operation is done.
	// (TRUE = operation completed,
	//  FALSE = operation not complete yet)
	int *done;
	// Pointer to return value variable
	int *rval;
}DCPACKET;

#endif



