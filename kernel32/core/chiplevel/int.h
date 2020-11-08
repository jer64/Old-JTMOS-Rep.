// * Functions at aslib *
#ifndef __INCLUDED_INT_H__
#define __INCLUDED_INT_H__

// Max. amount of threads with own TSS
#define N_MAX_TSS_TASKS 256

// Selector offsets in GDT table
// TSSDES for each thread
#define TSSSEL_OFFS	0x100
// application memory space CS/DS
#define APPSEL_OFFS	((N_MAX_TSS_TASKS*8)+TSSSEL_OFFS)

//
#include "macrodef.h"
#include "basdef.h"
#include "delay.h"
#include "exception.h"
#include "cpu.h"
#include "threadscfg.h" // N_MAX_THREADS
#include "segments.h"
#include "tss.h"

//
extern WORD tmpds,tmpes,tmpfs,tmpgs,tmpss;

// Useful assembly instructions
#define PUSHFLAGS	asm("pushf")
#define POPFLAGS	asm("popf")

// llint.asm's temporary registers
// which are being used by functions
// at threads.c, still it was more
// logical to define these here,
// because llint.asm is actually
// part of the interrupt system.
extern WORD tempds,tempes,tempfs,tempgs,
	// not used(might be in future):
	tempss;

// --- Some code borrowed from GazOS...
void Init8259();
void enable_irq(unsigned short irq_num);
void disable_irq(unsigned short irq_num);

//
#define D_LDT   0x200   // LDT segment
#define D_TASK  0x500   // Task gate
#define D_TSS   0x900   // TSS
#define D_CALL  0x0C00  // 386 call gate
#define D_INT   0x0E00  // 386 interrupt gate
#define D_TRAP  0x0F00  // 386 trap gate
#define D_DATA  0x1000  // Data segment
#define D_CODE  0x1800  // Code segment

#define M_PIC  0x20     // I/O for master PIC
#define M_IMR  0x21     // I/O for master IMR
#define S_PIC  0xA0     // I/O for slave PIC
#define S_IMR  0xA1     // I/O for slace IMR

#define EOI    0x20     // EOI command

#define ICW1   0x11     // Cascade, Edge triggered
                        // ICW2 is vector
                        // ICW3 is slave bitmap or number
#define ICW4   0x01     // 8088 mode

#define M_VEC  0x68     // Vector for master
#define S_VEC  0x70     // Vector for slave

#define OCW3_IRR  0x0A  // Read IRR
#define OCW3_ISR  0x0B  // Read ISR


#define D_DPL3         0x6000   // DPL3 or mask for DPL
#define D_DPL2         0x4000   // DPL2 or mask for DPL
#define D_DPL1         0x2000   // DPL1 or mask for DPL
#define D_PRESENT      0x8000   // Present
#define D_NOT_PRESENT  0x8000   // Not Present
 

#define D_ACC      0x100  // Accessed (Data or Code)

#define D_WRITE    0x200  // Writable (Data segments only)
#define D_READ     0x200  // Readable (Code segments only)
#define D_BUSY     0x200  // Busy (TSS only)

#define D_EXDOWN   0x400  // Expand down (Data segments only)
#define D_CONFORM  0x400  // Conforming (Code segments only)

#define D_BIG      0x40   // Default to 32 bit mode
#define D_BIG_LIM  0x80   // Limit is in 4K units


// - - - 

extern void setint(	int, 			// INT # NUMBER
			void *, 		// HANDLER
			unsigned short int,	// ..
			unsigned short int); 	// ..

extern void *getint(long intnum);



//
extern void nullinterrupt(void);
extern void nullinterrupt2(void);
extern void genint(long which);
extern void int_redirectidt(void);
extern void init_idt(void);
extern unsigned long int asm_inc(unsigned long int);

//
#define DEF_VALUES { 0xFFFF,0,0,0x9A,0xCF,0 }

//
#define SEG_DUMMY   0x000
#define SEG_CODE32  0x008
#define SEG_DATA32  0x010
#define SEG_CORE32  0x018
#define SEG_CODE16  0x020
#define SEG_DATA16  0x028
#define SEG_TSS0    0x030
#define SEG_TSS1    0x038
#define SEG_TSS2    0x040
#define SEG_LDT1    0x048
#define SEG_LDT2    0x050

//
typedef struct
{
	// KERNEL segment descriptors
	CDSEG dummy;  // 00
	CDSEG code32; // 08
	CDSEG data32; // 10
	CDSEG core32; // 18
	CDSEG code16; // 20
	CDSEG data16; // 28
	// APPLICATION segment descriptors
	CDSEG appcode32; // 30
	CDSEG appdata32; // 38
	// Kernel reserved TSS entries (for exception handler)
	CDSEG kertssdes[24]; // C0 bytes
	// 100: TSSs for X threads
	CDSEG tssdes[N_MAX_TSS_TASKS]; // 100,108,...
	// 900: CDSEGs for 256 app threads
	CDSEG appsel[256];
	

	// RESERVE SPACE FOR 5000 ENTRIES FOR THREADS
	//CDSEG rest32_a[5000]; // ...
	//CDSEG rest32_b[5000]; // ...
	CDSEG UserThreads[N_MAX_THREADS]; // Setting from threads.h
}GDTTAB;

//
typedef struct
{
	//
	TSSEG tss[N_MAX_TSS_TASKS];
	TSSEG kertss[24];
	BYTE alt; // Allow task switching TRUE/FALSE
	DWORD tasknr;
}TASKS;
//
extern TASKS *tasks;

//
extern GDTTAB *gdt_table;

// * Exter. functions at aslib *
extern void nullinterrupt(void);
extern void genint(long which);
extern unsigned long int SETUPGDT(unsigned long int);
extern void idle_moment(void);

//
extern void dump_critical(void);
extern void dump_gdt_info(void);

//
void set_cdseg(CDSEG *t,unsigned short int,unsigned short int,
	unsigned char,unsigned char,unsigned char,unsigned char);

//
extern DWORD tarkitus_ecx,tarkitus_esp;
extern WORD newcs,newds;
extern DWORD whereAD;

//
DWORD get_eflags(void);
void set_eflags(DWORD flags);
extern void SETUPTR(DWORD x);
extern void CLEARNT(void);
extern void NESTEDTASK(void);
extern DWORD GetCR3(void);

// aslib.asm:
extern void LOADTR(WORD x);
extern void TaskSwitched(void);
extern void HaltNow(void);
extern void setupidt(DWORD ptrad);
extern void tss_goto(DWORD sel);
extern WORD get_tr(void);
// llint.asm:
extern void pit_low(void);

//
void *GetIDTPTR(void);

//
extern DWORD taskSwitchNr;

//
void init_safe_idt(void);
extern void TripleFault(void);

//
#include "threads.h"
#include "intgdt.h"
#include "intdb.h"

#endif
