//--------------------------------------------------------------------------------
// scheduler.c - Main scheduler.
// (C) 2004-2005 by Jari Tuominen(jari@vunet.org).
//
// A TSS based task switching system.
// See threadSwitch.c for the switching logic.
//--------------------------------------------------------------------------------
#include "kernel32.h"
#include "system_call.h" // scallsIsWorking
#include "scheduler.h"

//--------------------------------------------------------------------------------
// Calls int 0x7E.
//
void _idle_moment(void)
{
	//
	if(scallsIsWorking && Multitasking)
		asm("int $0x7E");
}

//--------------------------------------------------------------------------------
//
// 0x8000 0000 = will have the application memory space, if any.
//
void MapApplicationMemory(void)
{
	DWORD *pd;
	void *process_page_tables;
	int i,i2,i3,i4;
	DWORD ad;

	//
	pd = GetKernelPageDirPTR();
	//
	process_page_tables = 0x600000;
	process_page_tables += nr_curthread*1024*16; // each process has 16 KB page tables (4x4 = 16)

	//
	for(i=0; i<4; i++,process_page_tables += 1024*4)
	{
		ad = process_page_tables;
	 	pd[512+i] = ad|3;
	}
}

//--------------------------------------------------------------------------------
//
void LLSchedule(void)
{
	TSSEG *ts;
	int sel;

	//
	taskSwitchNr++;

	//
	forceSwitch();
	unbusyTSS();

	//
	MapApplicationMemory();

	//
	LOADTR(0x48);
	sel = nr_curthread*8 + TSSSEL_OFFS;
		//	ps();
		//	printk("Jump to 0x%x.\n", sel);
	ts = &tasks->tss[nr_curthread];
		//	tss_dump(ts);
		//	printk("Current TR=%x.\n", get_tr());
		//	ESCLOCK();
		//	printk("Task switch:\n");
	tss_goto(sel);
}

//--------------------------------------------------------------------------------
//
// PIT timer handler TSS task.
//
void TaskPitHandler(void)
{
	//
	pit_handler();

	// We're done with the interrupt.
	outportb(0x20, 0x20);

	// Immediate switch to next thread.
	LLSchedule();
}

//--------------------------------------------------------------------------------


