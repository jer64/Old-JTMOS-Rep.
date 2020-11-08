// ================================================================================
// driver_flush.c (obsolete stuff mostly)
// (C) 2002-2004 by Jari Tuominen.
// ================================================================================
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
#include "driver_rw.h" // readblock...
#include "driver_flush.h"
//#include "FatFlush.h" // FlushFat
#include "driver_diskchange.h"

//-------------------------------------------------------------
//
void driver_TriggerCacheFlush1(void)
{
	int i;

	//
	DEBLINE

	//
	sync();
}

// Trigger FAT cache flush
void driver_TriggerCacheFlush(void)
{
	//
	DEBLINE

	sync();

/*	//
	while(FlushFat) { SwitchToThread(); }
	FlushFat = TRUE;
	while(FlushFat) { SwitchToThread(); }*/
}

//-------------------------------------------------------------
// FOR APPLICATIONS
//
void FlushAll(void)
{
	//
	driver_TriggerCacheFlush();
}

// ==========================================================
// FlushCache --- THE MAIN CACHE FLUSH FUNCTION -------------
//
// __FOR CACHEFLUSHER PROCESS ONLY__:
//
// This function is ONLY used by
// CacheFlusher kernel process.
//
// Flushes all known system caches,
// so far we only have one in driver API.
// ==========================================================
void _FlushCache(void)
{
	int i,i2,i3,i4;
	char str[50];

	//
	DEBLINE

	//
	if(jtmos_debug) { printk("PASS 1\n"); }

	//
	if(jtmos_debug) { printk("PASS 2\n"); }

	// ---- DEVICE DRIVERS CACHE FLUSH ------------------------
	// L1: Flush device drivers own internal caches
	//
	printk("driver/FlushCache: Flushing L1 cache: ");
	//
	for(i=0; i<devdb->n_devices; i++)
	{
		//if( !driver_getAlias(i) )
		driver_getdevicename(i, str);
		printk("%s: ", str);
		driver_FlushDrive(i);
	}
	//
	printk("\ndriver/FlushCache: Flush completed OK\n");
	// -------------------------------------------------------



	//
	if(jtmos_debug) { printk("PASS 3\n"); }

	//
	return;
}

//
void driver_FlushDrive(int dnr)
{
		DEBLINE

/*		device_call(dnr,
			DEVICE_CMD_FLUSH,
			0,0,0,0, NULL, NULL);
*/
}

