#ifndef __INCLUDED_GDTMANIPULATION_H__
#define __INCLUDED_GDTMANIPULATION_H__


//
#define N_GDTENTS               2000

//
typedef struct
{
	char isAllocated;
}GDTENT;

// Beginning from 0x100 GDT
typedef struct
{
	//
	int n_gdtEntries;
	GDTENT gdtent[N_GDTENTS];
}GDTENTSYS;

//
void SetCDEntryEx(DWORD which,
        DWORD offset, WORD limit,
        BYTE flags, BYTE access,
        int prilev);
void SetCDEntry(DWORD which,
        DWORD offset,
        WORD limit,
        int prilev);
int NewCDSegment(DWORD offs, WORD limit,
        int prilev);
void InitGDTEntryAllocationSystem(void);
void FreeGDTEntry(int which);
int gdt_NewEntry(void);
void SetCallGate(DWORD which,
                        DWORD offset,
                        WORD selector,
                        BYTE access,
                        BYTE options);

//
extern int kappCodeSeg,kappDataSeg;

#endif
