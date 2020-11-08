// =========================================================================
// hdDev.c - HD device access interface
// (C) 2002-2003 by Jari Tuominen
// =========================================================================
#include <stdio.h>
#include <jtmos/dcpacket.h>
#include "hd.h"
#include "hdDev.h"

//
int dhd_dev_mounted=1;
static int hdLock=0;

// Static temporary block buffer
static char *sttmp=NULL;

// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check devapi.h.
//
int hdDeviceCall(
	DCPACKET *dc,
	int clientPID, int n_call,
	int p1,int p2,int p3,int p4,
	void *po1,void *po2)
{
	DWORD *size;
	int flags,i,i2,sz;
	char *p;

        // If unmounted, we won't answer anything until mounted again.
        if(n_call!=DEVICE_CMD_MOUNT && !dhd_dev_mounted)
                goto past;

	// Allocate temporary buffer
	if(sttmp==NULL)
		sttmp = malloc(32768);

	//
	switch(n_call)
	{
		//
                case DEVICE_CMD_MOUNT:
                dhd_dev_mounted=1;
                break;
		//
                case DEVICE_CMD_UNMOUNT:
                dhd_dev_mounted=0;
                break;

		//
		case DEVICE_CMD_INIT:
		break;
		//
		case DEVICE_CMD_SHUTDOWN:
#ifdef USE_HD_CACHE
		hdFlushWriteBack();
#endif
		break;

		// ------------------------------------------------------------
		case DEVICE_CMD_READBLOCK:
		if(p1<0 || p1>=chd->n_blocks)	break;

		// p1 =		(integer)	which block
		// po1 = 	(PTR)		buffer
		for(i=0,i2=p1*chd->units,p=sttmp;
			i<chd->units; i++,p+=512,i2++)
		{
			//
			hdBlockRW(chd, p,chd->l_buf, i2,0, READ);
		}

		// Copy chunk to client's memory space
		globalcopy(GetCurrentThread(),sttmp,
			clientPID,po1,
			chd->units*512);

		//
		break;

		// ------------------------------------------------------------
		case DEVICE_CMD_WRITEBLOCK:
		// Validity check
		if(p1<0 || p1>=chd->n_blocks)	break;

		// Copy chunk from client's memory space
		globalcopy(clientPID,po1,
			GetCurrentThread(),sttmp,
			chd->units*512);

		// p1 =		(integer)	which block
		// po1 = 	(PTR)		buffer
		for(i=0,i2=p1*chd->units,p=sttmp;
			i<chd->units; i++,p+=512,i2++)
		{
			hdBlockRW(chd, p,chd->l_buf, i2,0,WRITE);
		}
		break;
		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		sz = chd->n_blocks / chd->units;
		globalcopy(GetCurrentThread(),&sz,
			clientPID,size,
			4);
		break;
		//
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		sz = chd->blocksize * chd->units;
		globalcopy(GetCurrentThread(),&sz,
			clientPID,size,
			4);
		break;
		//
		case DEVICE_CMD_SEEK:
		break;
		//
		case DEVICE_CMD_GETCH:
		break;
		//
		case DEVICE_CMD_PUTCH:
		break;
		//
		case DEVICE_CMD_TELL:
		break;
		//
		case DEVICE_CMD_FLUSH:
#ifdef USE_HD_CACHE
		hdFlushWriteBack();
#endif
		break;
		//
		default:
		break;
	}

past:
	// Unlock
	hdLock=0;
	return 0;
}

// Registers device 'hda'
int hdRegisterDevice(void)
{
	//
	return 0;
}


