//-----------------------------------------------------------
// Hardware task switching support (TSS).
// (C) 2003-2005 by Jari Tuominen.
//-----------------------------------------------------------
#include "kernel32.h"
#include "int.h"
#include "threads.h"
#include "tss.h"
#include "scheduler.h"

//
static TSSEG backup_kertss[10];
static CDSEG backup_kertssdes[10];

// Current TSS entry being active
int n_curtss=0;

///////////////////////////////////////////////////////////////////////////////
//
// Initialize TSS system.
//
void InitTSS(void)
{
	int i;

	//
	printk("  - tasks = malloc(...)\n");
	tasks = malloc(sizeof(TASKS));
	printk("  - memset(tasks, 0, ....)\n");
	memset(tasks, 0, sizeof(tasks));

	///////////////////////////////////////////////////////////////////////
	// Set TSS descriptors on GDT table (0xNN, ... ->)
	//
	printk("  - Setting TSS descriptors on GDT table ...\n");
	for(i=0; i<256; i++)
	{
		SetSD(&gdt_table->tssdes[i], &tasks->tss[i], 0xFFFF); //
	}

	///////////////////////////////////////////////////////////////////////
	//
	printk("  - Setting up TSS exception handler ...\n");
	SetupTSSExceptionHandler();

	///////////////////////////////////////////////////////////////////////
	//

	//
}

//--------------------------------------------
// -- TSS functions for PIT based task switcher: --
//

///////////////////////////////////////////////////////////////////////////////
//
void SetTSSDESBusyBit(CDSEG *s, char val)
{
	//
	if(val)
		// Set the BUSY flag.
		s->flags |= 2;
	else
		// Unset the BUSY flag.
		s->flags &= ~2;
}

// Set current TSS as unbusy
void unbusyTSS(void)
{
	//
	if(EXCEPTION_HANDLER_ACTIVE)
	{
		//
		EXCEPTION_HANDLER_ACTIVE = FALSE;
		// This is a fix, to prevent TSS data to be written,
		// 0x48 will be used as a dump.
		LOADTR(0x48);
	}

	// 1) Fix for exception handler.
	SetTSSDESBusyBit(&gdt_table->kertssdes[0], 0);
	// 2) Fix for scall handler.
	SetTSSDESBusyBit(&gdt_table->kertssdes[1], 0);
	// Rest....
	SetTSSDESBusyBit(&gdt_table->kertssdes[2], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[3], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[4], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[5], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[6], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[7], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[8], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[9], 0);
	SetTSSDESBusyBit(&gdt_table->kertssdes[10], 0);

	// 3) Fix for threads.
	SetTSSDESBusyBit(&gdt_table->tssdes[nr_curthread], 0);

	//
	TaskSwitched();
}

///////////////////////////////////////////////////////////////////////////////
//
void tssNextThread(void)
{
	// Do what ever needed here:
	//
}

///////////////////////////////////////////////////////////////////////////////
//
void SetTaskGate(TSS_GATE *t,
	WORD selector)
{
	// First initialize the structure.
	memset(t, 0, sizeof(TSS_GATE));

	// Set selector.
	t->selector = selector;
	// Set type to "Task Gate" (from Intel manual).
	t->type = 1|4|32|64|128;
}

//--------------------------------------------
// -- TSS system: --
//


///////////////////////////////////////////////////////////////////////////////
//
void Bingo1(void)
{
	static int i,i2,i3,i4,l;
	BYTE *p;

	//
	p = 0xB8000;

	//
	l = 80*25*2;
	for(i=0; i<l; i++)
	{
		p[i] = i+i2;
	}
	i2++;
}

// Do something colorful.
void Bingo(void)
{
	int i,i2,i3,i4,l;
	BYTE *p;

	//
	for(i2=0; ; i2++)
	{
		//
		Bingo1();
		if( inportb(0x60)==1 )
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
void TSSJump(TSSEG *t,
	WORD seg, DWORD eip)
{
	// Setup code segment
	t->cs   = seg; 
	// And the address where to start execution ...
	t->eip  = eip;
}

///////////////////////////////////////////////////////////////////////////////
// Creates selectors 0x40 and 0x48 on the GDT table.
//
void SetupTSSExceptionHandler(void)
{
	WORD *p;

	//
	disable();

	printk("  - GPF handler\n");
	// GPF HANDLER TASK SELECTOR 0x40
	// 13: Setup new GPF handler, that supports TSS multitasking (selector 0x40).
	NewTSS(&tasks->kertss[0], ASMexception_13, 0x100, -1); // 0x100 is just a mandatory link (ignore it)
	tasks->kertss[0].eflags &= ~(1<<9); // disable interrupts
	SetSD(&gdt_table->kertssdes[0], &tasks->kertss[0], 0xFFFF);

	printk("  - page fault handler\n");
	// PAGE FAULT HANDLER 0x50
	// 14: Setup new GPF handler, that supports TSS multitasking (selector 0x40).
	NewTSS(&tasks->kertss[2], ASMexception_14, 0x100, -1);
	tasks->kertss[2].eflags &= ~(1<<9); // disable interrupts
	SetSD(&gdt_table->kertssdes[2], &tasks->kertss[2], 0xFFFF);

	// SCHEDULER: IMMEDIATE TASK SWITCH 0x58
	printk("  - immediate task switch (scheduler)\n");
	NewTSS(&tasks->kertss[3], LLSchedule, 0x40, -1);
	tasks->kertss[3].eflags &= ~(1<<9); // disable interrupts
	SetSD(&gdt_table->kertssdes[3], &tasks->kertss[3], 0xFFFF);

	// SCHEDULER: PIT TASK SWITCH 0x60
	printk("  - PIT task switch\n");
	NewTSS(&tasks->kertss[4], TaskPitHandler, 0x40, -1);
	tasks->kertss[4].eflags &= ~(1<<9); // disable interrupts
	SetSD(&gdt_table->kertssdes[4], &tasks->kertss[4], 0xFFFF);

	// TEMPORARY SELECTOR 0x48
	// Create TSS entry for temporary use (selector 0x48).
	printk("  - temporary selector\n");
	NewTSS(&tasks->kertss[1], ASMexception_13, 0x78, -1);
	tasks->kertss[1].eflags &= ~(1<<9); // disable interrupts
	SetSD(&gdt_table->kertssdes[1], &tasks->kertss[1], 0xFFFF);

	//
	printk("  - setting up task gates\n");
	// Get IDT table PTR.
	p = GetIDTPTR();
	p += 3;
	// (13) Define new GPF fault handler by defining a task gate on IDT. 
	SetTaskGate(p+13*4, 0x40);
	// (14) Define new page fault handler....
	SetTaskGate(p+14*4, 0x50);

	// TSS based PIT handler.
	SetTaskGate(p+M_VEC*4, 0x60);
	// INT 0x7E = immediate task switch.
	SetTaskGate(p+0x7E*4, 0x58);
	// INT 0x7F = system call.
	// Not setupped here, see heart/scs.

/*	//
	printk("  - enabling PIT timer interrupt (interrupts still disabled)\n");
        // Enable PIT timer.
        enable_irq(0);
	printk("  OK.\n");
	printk("Test ...\n");
	sleep(10);	*/
}

///////////////////////////////////////////////////////////////////////////////
// Set [segment] descriptor.
//
void SetSD(CDSEG *s, DWORD base, WORD limit)
{
	// First clear the structure.
	memset(s, 0, sizeof(CDSEG));

	// Define limit & base first.
	s->seg0_15 = limit & 0xFFFF;
	s->base0_15 = base & 0xFFFF;
	s->base16_23 = (base>>16) & 0xFF;
	s->base24_31 = (base>>24) & 0xFF;

	// Present	0x80
	s->flags = 0x80 | (0x40|0x20); // ring3
	//	TYPE	DESCRIPTION
	//	9	386 TSS (available, JMP)
	//	11	386 TSS (busy, IRET)
	s->flags |= 9;

	// Access contains AVL, GRA, LIMIT 19:16.
	s->access = (1<<4);
}

///////////////////////////////////////////////////////////////////////////////
//
void NewTSS(TSSEG *_t, DWORD sysad, WORD link, int pid)
{
	TSSEG *t = _t;

	//
	NewTSSEx(t,sysad,link,pid,0);
}

// Create a new task segment with data.
void NewTSSEx(TSSEG *t, DWORD sysad, WORD link, int pid,
		DWORD options)
{
	DWORD stacksize;
	BYTE *stack1,*stack2,*stack3,*stack4;

	// First clear the structure
	memset(t, 0, sizeof(TSSEG));

	// Link to next process(segment selector)
	t->link = link;
 
	// Define size of the stack.
	// (those stacks should not be used for any big things...)
	stacksize = 1024*4;
 
	// Allocate stacks
	stack1=malloc(stacksize);
	stack2=malloc(stacksize);
	stack3=malloc(stacksize);
	stack4=malloc(stacksize);
	if(pid!=-1)
	{
		ChangeChunkOwner(stack1, pid);
		ChangeChunkOwner(stack2, pid);
		ChangeChunkOwner(stack3, pid);
		ChangeChunkOwner(stack4, pid);
	}

	// Define stack pointers
	// (end of the buffer is the beginning)
	t->ss0  = SEG_DATA32;
	t->esp0 = stack1+(stacksize-1);
	t->ss1  = SEG_DATA32;
	t->esp1 = stack2+(stacksize-1);
	t->ss2  = SEG_DATA32;
	t->esp2 = stack3+(stacksize-1);
	// Actually used one:
	t->ss   = SEG_DATA32;
	t->esp  = stack4+(stacksize-1);
 

	// Setup data segment(s)
	t->ds   = SEG_DATA32;
	t->es   = SEG_DATA32;
	t->fs   = SEG_DATA32;
	t->gs   = SEG_DATA32;

	// Setup code segment
	t->cs   = SEG_CODE32; 
	// And the address where to start execution ...
	t->eip  = sysad;
 
	// Local Descriptor Table Register(selector)
	// (we don't need one)
	t->ldtr  = 0;
 
	// I/O permission bitmap.
	// (when paging is enabled)
	t->iopb  = NULL;

	// No CR3 needed.
	t->cr3 = GetCR3();
 
	// Set eflags register, interrupts must be enabled,
	// no NT flag set for JMP.
	t->eflags = get_eflags() | (1<<9);

	// Enable BIT3, task switched (for the FPU).
	t->eflags |= (1<<3);
}

///////////////////////////////////////////////////////////////////////////////
//
void DumpTSS(TSSEG *t)
{
	//
	printk("TSS DUMP\n");
	printk("CS:EIP = %x:%x\n",
		t->cs, t->eip);
}

///////////////////////////////////////////////////////////////////////////////


