// ================================================================
// Enhanced memory management system 0.4.0.
// for JTMOS Operating System.
//
// (C) 2002-2006 by Jari Tuominen.
// ================================================================
#include <stdio.h>
#include "mm.h"
#include "mmTest.h"

// Pointer to the allocation database.
void *aldb;
// Memory begins from first megabyte area
// PTR to the beginning of dyn.mem.region
char *MSTART,*phMSTART;
// Memory end PTR
char *MEND,*phMEND;
// Real end PTR
char *REND,*phREND;
// Main memory management structure
MM mm;
//
MM *p_mm = NULL;
// Malloc is initialized?
int mallocOK=FALSE;
// Debug output from mm calls?
// (causes debug output to flood, not recommended,
// although only when kernel debug messages are enabled.)
int mm_debug=FALSE;

// Customized panic
void MMPANIC(const char *msg)
{
	//
	printf("\n");
	printf("last function which called 'malloc' = '%s'\n", malloc_caller);
	printf("last function which called 'free' =   '%s'\n", free_caller);
	printf("%s: '%s'\n",
		__FUNCTION__, msg);
	printf("%s: %d bytes free\n",
		__FUNCTION__,
		malloc_getmemoryfree());
	printf("%s: %d entries allocated\n",	__FUNCTION__, mm.n_allocated);
	printf("%s: (max. = %d)\n",	__FUNCTION__, mm.max_allocated);
	panic("MMPANIC (description printed above)");
}

//
void mm_Diagnostics(void)
{
	// Self-diagnostics:
	printf("%s: phMSTART=0x%x, phMEND=0x%x, phREND=0x%x\n",
		__FUNCTION__,
		phMSTART, phMEND, phREND);
	printf("%s: MSTART=0x%x, MEND=0x%x, REND=0x%x\n",
		__FUNCTION__,
		MSTART, MEND, REND);
	printf("%s: mm.allocated=0x%x(max=%d), mm.bitmap=0x%x(len=%d)\n",
		__FUNCTION__,
		mm.allocated, mm.max_allocated, mm.bitmap, mm.l_bitmap);
}

//
void ShowPageMap(void)
{
	DWORD i,i2,i3,i4,ad,mask,key;

	//
	printk("%s\n", __FUNCTION__);
	for(i=0,ad=0; i<1024; i++,ad+=PAGESIZE)
	{
		if( (i&255)==0 ) { mask=0; }
		mask |= GetPage(i);
		if( (i&255)==255 )
		{
			printk("%x: mask=%x\n", ad/(1024*1024), mask);
		}
	}
}

// Initialize memory management system
// mmInit(1024*1024, GetRamAmount()-1024*1024);
void mmInit(void *region, int length)
{
	//
	DWORD i,i2,i3,i4,l,pad,pad2,ad;
	char *p,*p2;

	//
	p_mm = &mm;

	// ---------- Init whole region first -------------------------------------

	//
//	memset(region, 0, length);

	//
	mm_debug = FALSE;


	// Init paging.
	printk("p_mm=0x%x\nESP = 0x%x\n", p_mm, GETESP());
	pg_init();

	// ---------- Initialize I ------------------------------------------------

	// First initialize the main structure
	memset(&mm, 0, sizeof(MM));

	//------------------------------------------------------------
	//
	// SPECIFY MSTART, MEND, REND
	//
	// Specify start of the dyn.mem.region
	// at the first megabyte area,
	// just after the kernel.
	phMSTART = region;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Specify end of the dyn.mem.region
	// so that enough memory remains
	// after it for the memory management
	// database system's data.
	phMEND = (region+length) - ALLOCATION_DATABASE_SIZE;
	// Specify the real end of all available memory.
	phREND = (region+length);

	//------------------------------------------------------------
	MSTART = MAPAD_MM_DB; // where to map to
	// Physical start, length, where to map at...
        //
	printk("pg_QuickMap: phst=%x, length=%x, mapad=%x\n",
		phMEND, ALLOCATION_DATABASE_SIZE, MSTART);
	pg_QuickMap(phMEND, ALLOCATION_DATABASE_SIZE, MSTART);
/*        while(1)
        {
                memset(vesa_frame_buf, YELLOW, 640*480);
                memset(vesa_frame_buf, RED, 640*480);
        }*/

	//
        MEND = MSTART;
        REND = MSTART + ALLOCATION_DATABASE_SIZE;

	// ---------- Initialize II -----------------------------------------------

	//	Identifier	|Description
	//	----------------+-------------------------------------
	//	0		|KERNEL FIRST MEGABYTE CRITICAL AREA
	//	MSTART		|USER MEMORY REGION
	//	MEND		|MM DATA BASE REGION
	//	......................................................(*)
	//	REND		|BEGINNING OF NO-RAM REGION
	//
	//	(*) All memory beginning from this
	//	identifier's location are not accessible.
	//

	// The allocation database memory region is devided in
	// following three different sectiona:
	// 1/3 = mm.allocated
	// 2/3 = mm.allocated[...]
	// 3/3 = mm.bitmap

	// A) Determine amount of entries we can have at max.
	// (Database size/3)/sizeof(MD) = number of entries we can have max.
	mm.max_allocated = (ALLOCATION_DATABASE_SIZE/3)/sizeof(MD);
	// Currently 0 entries are allocated
	mm.n_allocated = 0;
	// B) Specify PTR for the PTR list(DB region 1/3)
	mm.allocated = MEND;

	ShowPageMap();

	printk("%s info:\n", __FUNCTION__);
	printk("mm.bitmap = 0x%x\n",	mm.bitmap);
	printk("mm.allocated = 0x%x\n",	mm.allocated);
	printk("p:MEND = 0x%x\n",		&MEND);
	printk("p:REND = 0x%x\n",		&REND);
	printk("p:MSTART = 0x%x\n",		&MSTART);
	printk("MEND = 0x%x\n",		MEND);
	printk("REND = 0x%x\n",		REND);
	printk("MSTART = 0x%x\n",	MSTART);

	// C) Specify PTRs for each of the block info structures(DB region 2/3).
	for(i=0, p=MEND+((ALLOCATION_DATABASE_SIZE/3)*1);
		i<mm.max_allocated;  i++, p+=sizeof(MD))
	{
		// Init entry X
		mm.allocated[i] = p;
		memset(mm.allocated[i], 0, sizeof(MD));

		// Define it as "free".
		mm.allocated[i]->isFree = 1;
		// Set PID to none.
		mm.allocated[i]->PID = -1;
	}
	printk("Passed.\n");

	// ---------- Initialize III ----------------------------------------------

	// Define free memory bitmap(DB region 3/3)
	mm.l_bitmap =	1024*1024/8;
	mm.bitmap =	MEND + ((ALLOCATION_DATABASE_SIZE/3)*2);

	// ---------- Initialize IV -----------------------------------------------

	// Initialization of 4 gigabytes free memory bitmap.
	
	// Reserve all memory first(1024*1024 bit entries, 4K per bit),
	// in bytes this equals to 1024*1024/8)
	l = 1024*1024;
	for(i=0; i<l; i++)
	{
		BIT(i, 1);
	}

	// ---------- Initialize V ------------------------------------------------

	// Reserve kernel memory region
	mmReserveRegionAD(0, phMSTART);

	// Reserve MM DB memory region
	mmReserveRegionAD(phMEND, phREND);

	// Free user memory region
	mmFreeRegionAD(phMSTART, phMEND);

	// ---------- Initialize VI -----------------------------------------------
	mallocOK = TRUE;

	// ---------- End of inits ------------------------------------------------

	// ---------- Diagnostics -------------------------------------------------
	mm_Diagnostics();
	//
	printf("%s: %d bytes free\n",
		__FUNCTION__,
		malloc_getmemoryfree());
	printk("Passed 2.\n");

/*        if(vesa_frame_buf==0xF0000000)
        {
                system_reset1();
        }*/
}

#ifdef LIBCVERSION
void disable(void)
{
}
void enable(void)
{
}
void get_eflags(void)
{
}
void set_eflags(int flags)
{
}
#endif


#ifdef LIBCVERSION
void malloc_init(char *p, int l)
{
        mmInit(p, l);
}
#endif



