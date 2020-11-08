// ===================================================
// 		GDT MANIPULATION
//	A GDT table entry allocation system.
//	Used when creating application mode
//	processes, kernel processes always use
//	same static code segment and memory space.
// 	(C) 2002-2004 by Jari Tuominen.
// ===================================================
//
#include "int.h"
#include "cpu.h"
#include "sysglvar.h"
#include "kernel32.h" // FATALEXCEPTION
#include "delay.h"
#include "threads.h"
#include "gdtmanipulation.h"

// Privilege level 0-3.
#define DEF_PRIV_LEVEL			0

//
GDTENTSYS *gdtentsys;
//
int kappCodeSeg,kappDataSeg;

//
void InitGDTEntryAllocationSystem(void)
{
	//
	gdtentsys = malloc(sizeof(GDTENTSYS));
	memset(gdtentsys, 0, sizeof(gdtentsys));

	// Get kernel application code/data segment
	// (for scheduling purposes).
	kappCodeSeg = NewCDSegment(0, 0xFFFF, DEF_PRIV_LEVEL);
	kappDataSeg = kappCodeSeg+8;
}

// Frees a single GDT entry.
void FreeGDTEntry(int which)
{
	int w;

	//
	which &= 0xFFFFFFF8;

	//
	w = (which-APPSEL_OFFS)/0x10;

	//
	if(w>(N_GDTENTS-10))	return;

	//
	gdtentsys->gdtent[w].isAllocated = 0;
}

// Allocates a new GDT entry.
int gdt_NewEntry(void)
{
	int i,i2,i3,i4,entryID,selector;

	//
	for(i=0,entryID=-1; i<gdtentsys->n_gdtEntries; i++)
	{
		if(!gdtentsys->gdtent[i].isAllocated)
		{
			// re-use entry
			entryID = i;
			break;
		}
	}

	//
	if(entryID==-1)
	{
		// Create new entry
		if(gdtentsys->n_gdtEntries>(N_GDTENTS-10))
		{
			// out of entries
			panic("Out of GDT entries.");
			return 0;
		}
		entryID = gdtentsys->n_gdtEntries;
		gdtentsys->n_gdtEntries++;
	}

	//
	gdtentsys->gdtent[entryID].isAllocated=TRUE;
	//
	selector = entryID*0x10+APPSEL_OFFS; 
	printk("selector = 0x%x\n",
		selector);

	//
	return selector;
}

// Creates a new Code and Data segment.
int NewCDSegment(DWORD offs, WORD limit,
	int prilev)
{
	int selector;

	//
	if(prilev<0 || prilev>3)
		panic("NewCDSegment: Illegal priority level parameter (variable int prilev).");

	// Get a new GDT entry.
	selector = gdt_NewEntry();

	//
	SetCDEntry(selector, offs, limit,  prilev);

	//
// printf("selector=%x, offs=%x, limit=%x\n", selector,offs,limit);
// ESCLOCK(); ESCLOCK();

	//
	return selector | prilev;
}

// Sets up a call gate in GDT table.
void SetCallGate(DWORD which,
                        DWORD offset,
                        WORD selector,
			BYTE access,
			BYTE options)
{
	BYTE *buf;

	//
	buf = gdt_table;

	// RENDER CALL GATE DESCRIPTOR
	// ---------------------------

	//
	buf[0 + which] = (offset)&255;
	buf[1 + which] = (offset>>8)&255;
	buf[6 + which] = (offset>>16)&255;
	buf[7 + which] = (offset>>24)&255;

	//
	buf[2 + which] = (selector)&255;
	buf[3 + which] = (selector>>8)&255;

	//
	buf[4 + which] = options;
	buf[5 + which] = access;
}

// Sets up code and data segment descriptor.
//
// Wrapper to SetCDEntryEx
void SetCDEntry(DWORD which,
	DWORD offset,
	WORD limit,
	int prilev)
{
	//				... flags, 		access.
	SetCDEntryEx(which,	offset,limit, 0x9A|(0x40|0x20),	0xCF,  prilev); // 0xCF
	SetCDEntryEx(which+8,	offset,limit, 0x92|(0x40|0x20),	0xCF,  prilev);
}

// Sets up a single CODE/DATA segment descriptor.
//
// For which use values like: 0x10,0x20,0x30..
// But not like 1,2,3..
// The values('which') must be factories of 8.
//
void SetCDEntryEx(DWORD which,
	DWORD offset, WORD limit,
	BYTE flags, BYTE access,
	int prilev)
{
	BYTE *buf,d;

	//
	buf = gdt_table;

	// RENDER SEGMENT DESCRIPTOR
	// -------------------------
	// Segment limit       (0-15 - 16 bits)
	buf[0 + which] = limit&255;
	buf[1 + which] = (limit>>8)&255;
	// Base                (0-15 - 16 bits)
	buf[2 + which] = (offset)&255;
	buf[3 + which] = (offset>>8)&255;
 	// Base II             (16-23 - 8 bits)
	buf[4 + which] = (offset>>16)&255;

	// Flags
	// (Bits 0-7)
	// Bits 5-6 = DPL, descriptor privilege level.
	// Bit 7 = present bit (always set).
	d=flags;
	// Define descriptor priority level.
	d&=~(0x40|0x20);
	d|=((prilev&3)<<5);
	// Set present bit.
	d|=0x80;
	// Store flags.
	buf[5 + which] = d;

	// Access
	buf[6 + which] = access;
	// Base III            (24-31 - 8 bits)
	buf[7 + which] = (offset>>24)&255;
}

//


