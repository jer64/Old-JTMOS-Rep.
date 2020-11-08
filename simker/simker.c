//---------------------------------------------------------------------------------------
// simker.c - Simulated JTMOS Kernel - allows running parts of kernel on another host OS
// (C) 2008 by Jari Tuominen (jari.t.tuominen@gmail.com).
// Updated 21.09.2008.
//---------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simker.h"

//
struct simker_devices[]
{
	int size;
	int bs;
	BYTE *ramdisk;
}
=
{
	
};

//
static DWORD simker_device_size[SIMKER_MAX_DEV];

//
int getsize(int dnr)
{
	//
	if(dnr<0 || dnr>=simker_devices_registered)
	{
		return 0;
	}

	//
	return simker_device_size[dnr];
}

//
int getblocksize(int dnr)
{
	//
	if(dnr<0 || dnr>=simker_devices_registered)
	{
		return 0;
	}

	//
	return simker_device_block_size[dnr];
}

//
