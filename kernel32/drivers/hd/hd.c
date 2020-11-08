// ==================================================
// IDE hard disk driver revision 3.0.
// (C) 2003-2004 by Jari Tuominen.
// ==================================================
#ifdef JTMOSKERNEL
#include <kernel32.h>
#else
#include <stdio.h>
#endif
#include "hd.h"

// We require some memory to provide good
// performance for hard disk access.
// Request 1024K(1M) of RAM from the init process.
//
// Note: If you edit this value below,
// then remember to edit cache/hdCacheConfig.h also.
#ifndef JTMOSKERNEL
SYSMEMREQ(APPRAM_1024K)
#endif

// Current HD in use
HD *chd=NULL;
// HD disk info structures
HD **hd=NULL;
// IRQ variables
int ideirqtick[10]={0};

#ifndef JTMOSKERNEL
// Debug message printer
int dprintk(const char *fmt, ...)
{
        va_list arg;
        int result;

	//
#ifdef ENABLE_HD_DEBUG_MESSAGES
        va_start(arg, fmt);
        result=vprintf(fmt,arg);
        va_end(arg);
#endif

	//
        return result;
}
#endif

// Test HD drive
void hdTestDrive(void)
{
	BYTE *tmp;
	int dnr;
	int i,b;
	DWORD *p;

	//
	dnr = getdevicenrbyname("hda");

	//
	tmp = malloc(32768);

	//
	printk("Testing hard disk ");
	for(i=0,p=0xF0000; i<100; i++,p++)
	{
		//
		b = (*p+i) % 1024;
		readblock(dnr, b, tmp);
		printk(".");
	}
	printk("\nTest completed.\n");

	//
}

//---------------------------------------------------------------------
// IDE HARD DISK DRIVER MASTER INIT FUNCTION
//---------------------------------------------------------------------
int hd_init(void)
{
	static int i,dnr;

	// 10 entries
	hd = malloc(8* 10);
	if(hd==NULL)
	{
		//
		panic("MALLOC FAILED 1");
	}

	// allocate 5 HDs
	for(i=0; i<5; i++)
	{
		//
		hd[i] = malloc(sizeof(HD));
		memset(hd[i], 0, sizeof(HD));
		if(hd[i]==NULL)
		{
			panic("MALLOC FAILED 2");
		}
	}

	// Assign drive structure
	chd = hd[0];
	if(chd==NULL)
		panic("hddrv: chd equals NULL - out of memory?");

	// Detect drive
	if( hdDetect(chd) )
	{
		// Found drive:
		//--------------

		// Define cylinder buffer size in bytes
		chd->l_buf = chd->sectors*chd->blocksize;

		// Initialize cache system.
		if(chd->lbaMode)
		{
			// Start LBA cache...
			// NOT YET IMPLEMENTED.
		}
		else
		{
			// Start CHS cache...
			hdCacheInit();
		}

		//
		//hdTestDrive();
		return TRUE;
	}
	else
	{
		// -- Drive not found
		return FALSE;
	}
}
//---------------------------------------------------------------------

//




