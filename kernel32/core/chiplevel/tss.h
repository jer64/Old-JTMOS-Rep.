#ifndef __INCLUDED_TSS_H__
#define __INCLUDED_TSS_H__

//
#define SEG_NO_TSS	0x40

//
#include "basdef.h"

//
typedef struct
{
	//
	WORD blank1;
	WORD selector;
	BYTE blank2;
	BYTE type;
	WORD blank3;
}TSS_GATE;

// CODE/DATA SEGMENT STRUCTURE
typedef struct
{
        // Base / Limit
        WORD    seg0_15;        // 0
        WORD    base0_15;       // 2
	//
        BYTE    base16_23;      // 4
        BYTE    type;           // 5
        BYTE    avl;            // 6 avl(23),granurality(20),limit(19:16)
        BYTE    base24_31;      // 7
}TSS_SD;

//
typedef struct
{
	// 00
	WORD	link,	res1;
	// 04
	DWORD	esp0;
	// 08
	WORD	ss0,	res2;
	// 0C
	DWORD	esp1;
	// 10
	WORD	ss1,	res3;
	// 14
	DWORD	esp2;
	// 18
	WORD	ss2,	res4;
	// 1C
	DWORD 	cr3,eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi;
	// 48
	WORD	es,	res5;
	// 4C
	WORD	cs,	res6;
	// 50
	WORD	ss,	res7;
	// 54
	WORD	ds,	res8;
	// 58
	WORD	fs,	res9;
	// 5C
	WORD	gs,	res10;
	// 60
	WORD	res11,	ldtr;
	// 64
	WORD	res12,	iopb;
	// 68
}TSSEG;

//
extern void NewTSS(TSSEG *t, DWORD sysad, WORD link, int pid);
extern void NewTSSEx(TSSEG *t, DWORD sysad, WORD link, int pid,
                DWORD options);
void SetSD(CDSEG *s, DWORD base, WORD limit);
void InitTSS(void);
void restoreTSS(void);
void storeTSS(void);
void unbusyTSS(void);
void tssNextThread(void);
void Bingo(void);
void Bingo1(void);
void SetTSSDESBusyBit(CDSEG *s, char val);
void TSSJump(TSSEG *t,
        WORD seg, DWORD eip);
void SetupTSSExceptionHandler(void);

#endif


