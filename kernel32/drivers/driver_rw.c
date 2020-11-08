// =============================================================
// DEVAPI SYSTEM
// readblock writeblock
// New: readblocks, writeblocks
// (C) 2002-2017 by Jari Tuominen (jari.t.tuominen@gmail.com).
// =============================================================
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "driver_api.h"
#include "driver_sys.h"
#include "driver_lock.h"
#include "driver_rw.h"

// ------------------------------------------
int readblock(int dnr, int b, BYTE *buf)
{
	//
	DEBLINE

	//
	if(!isValidDevice(dnr))		return 1;
	if( !(b>=0 && b<getsize(dnr)) )
		return 2;
	driver_lockOn(0);
	_readblock(dnr, b, buf);
	driver_lockOff(0);
	return 0;
}

//
int writeblock(int dnr, int b, BYTE *buf)
{
	DEBLINE

	//
	if(!isValidDevice(dnr))		return 1;
	if( !(b>=0 && b<getsize(dnr)) )
		return 2;

	//
	driver_lockOn(0);
	_writeblock(dnr, b, buf);
	driver_lockOff(0);
	return 0;
}

// ------------------------------------------
int _writeblock(int dnr, int b, BYTE *buf)
{
	//
	DEBLINE

	//
	if(!isValidDevice(dnr))		return 1;

	//
	writeblock1(dnr, b, buf);
	return 0;
}

//
int _readblock(int dnr, int b, BYTE *buf)
{
	//
	DEBLINE

	//
	if(!isValidDevice(dnr))		return 1;

	//
	readblock1(dnr, b, buf);
	return 0;
}

// ====================================================================
// BEGINNING OF DIRECT ACCESS FUNCTIONS

// Reads a single block from any device specified by dnr
//
// Note: Same as readblock, but does not do buffering.
// Also used by the readblock function itself.
// Note2: Readblock depends on readblock1
//
int readblock1(long dnr,
                long block_nr,
                BYTE *buf)
{
	int i,i2,i3,i4, r;

	//
	if(!isValidDevice(dnr))		return 1;

	//
	driver_directAccessLockOn();

	//
	DEBLINE

	// ------------------------------------ LOGIC CHECK ------------------
	if(block_nr<0 || block_nr>=getsize(dnr))
	{
		return 1;
	}

	//
	HDLED(1);
	r = device_call(dnr,
			DEVICE_CMD_READBLOCK,
			block_nr,0,0,0,
			buf,NULL);
	HDLED(0);

	//
	driver_directAccessLockOff();
	return r;
}

// Direct write, used mostly by devapi itself.
int writeblock1(long dnr,
                long block_nr,
                BYTE *buf)
{
	int r;
	DEBLINE

	//
	if(!isValidDevice(dnr))		return 1;

	//
	driver_directAccessLockOn();

	//
	HDLED(2);
	//
	r = device_call(dnr,
	               DEVICE_CMD_WRITEBLOCK,
	               block_nr,0,0,0,
	               buf,NULL);
	//
	HDLED(0);

	//
	driver_directAccessLockOff();

	//
	return r;
}

// Write multiple blocks on a device.
int writeblocks(long dnr,
                long block_nr,
                BYTE *buf,
		int count)
{
	int i,i2;

	//
	for(i=block_nr,i2=0; i<count+block_nr; i++,i2++) {
		writeblock(dnr, i, buf);
	}
	return i2;
}

// Write multiple blocks on a device.
int readblocks(long dnr,
                long block_nr,
                BYTE *buf,
		int count)
{
	int i,i2;

	//
	for(i=block_nr,i2=0; i<count+block_nr; i++,i2++) {
		readblock(dnr, i, buf);
	}
	return i2;
}

// END OF DIRECT ACCESS FUNCTIONS
// ===================================================

//
void HDLED(int c)
{
#ifdef ENABLE_driver_HDLED
	char *p;
	int i,i2,i3,i4,col;

	//
	p=0xb8000;
	i=(80+79)*2;

	//
	switch(c)
	{
                case 1:
                col=0x22;
                break;
                case 2:
                col=0x44;
                break;
                default:
                col=0;
                break;
	}

	//
	p[i+1] = col;
#endif
}

