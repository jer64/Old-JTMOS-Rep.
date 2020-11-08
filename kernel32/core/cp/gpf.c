/**********************************************************************
 * JTMOS General Purpose Exception Handler.
 * (C) 1999-2006 by Jari Tuominen.
 *
 * GPF/Etc. handler functions. Isolated from kernel32.c.
 * Check out FATALEXCEPTION.
 *
 * See exception.code for exception code.
 *
 **********************************************************************/

// Required includes.
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "sb.h"
#include "syssh.h"
#include "ega.h"
#include "driver_sys.h"
#include "zero.h"
#include "null.h"
#include "pci.h"
#include "vga.h"
#include "threads.h"
#include "syssh_applications2.h"
#include "statusline.h" // statusline_terminate trigger
#include "panic.h"

//
int EXCEPTION_HANDLER_ACTIVE=FALSE;

// from corner.mac
char irqActive=FALSE;

//------------------------------------------------------------------------------------------------------
//
inline void TellAtB8000(char *s)
{
	static char str[100];
	static int i,i2,l;
	static DWORD *ptr;
        static TSSEG *ts;
	static char *excact = "Exception caught";
	static char *excdea = "                ", *p;

	// Tell about exception.
	p = 0xB8000;
	
	// Wipe off some space.
	for(i=0; i<(80*2*5); i+=2)
	{
		p[i+0] = ' ';
		p[i+1] = 0x0F;
	}

	//
	l = strlen(excact);
	for(i=0,i2=0; i<l; i++,i2+=2)
	{
		p[i2+0] = excact[i];
		p[i2+1] = 0x0F;
	}
	l = strlen(s); p+=160;
	for(i=0,i2=0; i<l; i++,i2+=2)
	{
		p[i2+0] = s[i];
		p[i2+1] = 0x0F;
	}

	//
	sprintf(str, "PID=%d, Last IRQ ID(irqActive)=%d, taskSwitchNr=%d",
		nr_curthread, irqActive,
		taskSwitchNr);
	p+=160;
	for(i=0,i2=0; str[i]!=0; i++,i2+=2)
	{
		p[i2+0] = str[i];
		p[i2+1] = 0x0F;
	}

	//
	sprintf(str, "Press ESC to continue.");
	p+=160;
	for(i=0,i2=0; str[i]!=0; i++,i2+=2)
	{
		p[i2+0] = str[i];
		p[i2+1] = 0x0F;
	}
	DESCLOCK
}

/* 
 * ------------------------------------------
 * FATALEXCEPTION - Default exception handler
 * ------------------------------------------
 */
//
// Update: terminates thread and calls SwitchToThread();
//
// Exception handler process (TSS task).
//
__attribute__ ((__noreturn__)) void FATALEXCEPTION(const char *s)
{
	static char str[100];
	static int i,i2,l;
	static DWORD *b,*ptr;
        static TSSEG *ts;

	//
	while(1)
	{
		//----------------------------------------------------------------------
		// REPORT AND ANALYZE EXCEPTION :
		//
		//
		printk("\n");

		// Get thread TSS
		//
		ts = &tasks->tss[GetCurrentThread()];
	
		// Tell TSS handler that our TSS entry has to be unbusied.
		// (See tss.c for more info...)
		EXCEPTION_HANDLER_ACTIVE = TRUE;

		// Get pointer to the exeception location.
		ptr = ts->eip;

		//////////////////////////////////////////////////////////////////////////////
		//
		b = &gdt_table->tssdes;
	
		// If not text mode, setup it.
		if( dispadmin_getcurrentmode()!=3 && vesa_frame_buf==NULL )
		{
			setvmode(3);
		}

		//
		printk("PID %d/%d(%s): %s  taskSwitchNr=%d\n", nr_curthread, nr_threads-1,
			thread_name[RPID(GetCurrentThread())], s,
			taskSwitchNr);

		// Get thread TSS.
		ts = &tasks->tss[RPID(GetCurrentThread())];
		// Dump process registers from TSS.
		tss_dump(ts);
		//
		AnalyzeFault(ts, exception.code);
		// Bring up backtrace.
//		STACKWALK(ts->esp);
		// Stack dump.
//		STACKDUMP(ts->esp);
		// Machine language dump.
//		PRGDUMP();

		//
		if(!ts->cs || !ts->ds || !ts->es || !ts->fs || !ts->gs)
		{
			printk("Check the segment selector registers, something odd there.\n");
		}
//		Disassemble(ptr);

		//----------------------------------------------------------------------
		// HANDLE EXCEPTION :
		//

		// Fix for PID0:
		if(GetCurrentThread()==0)
		{
			//
			printk("Exception at PID zero. System halted.\n");
			while(1) { disable(); }
		}

		//
		printk("Calling KillThreadEx ...\n");

		// Terminate the current thread.
		KillThreadEx(GetCurrentThread(), KT_NO_THREAD_SWITCH);

		// Make switch to another task.
		SwitchToThread();

		//
		while(1) { }
	}
}

//
void Disassemble(BYTE *p)
{
	//
	if(p[0]==0xCD)
	{
		printk("Disassembly:    %x  INT 0x%x\n",
			p,p[1]);
	}
}

//
inline void ESCLOCK (void)
{
	// Wait until user releases ESC key..(If it is down now)
	while(inportb(0x60) == 1);
	// Wait until user presses ESC key..
	while(inportb(0x60) != 1);
}

//------------------------------------------------------------------------------
// Backtrace.
//
void STACKWALK(DWORD ptr)
{
	DWORD i, i2, unit,ad;
	BYTE *p;

	//
	p = ptr;

	//
	if(illegal(p))
	{
		return;
	}

	//
	printk("Backtrace: ");
	for(i=0; i<10; i++,p+=4)
	{
		ad = (p[3]<<24)|(p[2]<<16)|(p[1]<<8)|p[0];
		printk("%x ", ad);
	}
	printk("\n");
}

//------------------------------------------------------------------------------
// Displays a stack dump.
//
void STACKDUMP(DWORD whereAD)
{
	DWORD i, i2, unit;
	BYTE *p;

	// expecting that there is 80 marks per a line
	unit = 16;

	//
	p = A2K(whereAD);

	//
	printk("ESP=0x%x (fixed=0x%x)  ", whereAD, p);
	printk("Base=0x%x\n", GetCurrentThreadBase());

	//
	if(illegal(p))
	{
		printk("Stack pointer refers to an illegal memory region, won't make a stack dump.\n");
		PageDumpNear(p);
		return;
	}

	//
	printk("stack dump:\n");
	for(i=0; i<0x20; i+=unit)
	{
		//
		p32(p - i);
		printk(": ");
		for(i2 = 0; i2 < unit; i2++)
		{
			p8(p[i-i2-1]);
			printk(" ");
		}
		printk("\n");
	}
}

// Displays a program binary dump
void PRGDUMP(void)
{
	DWORD i, i2, unit;
	BYTE *p;

	// expecting that there is 80 marks per a line
	unit = 16;

	//
	p = A2K(tasks->tss[GetCurrentThread()].eip);

	//
	if(illegal(p))
	{
		printk("Program counter CS:EIP points to an illegal memory region, won't dump.\n");
		PageDumpNear(p);
		return;
	}

	//
	printk("executable binary dump:\n");
	for(i=0; i<0x20; i+=unit)
	{
		//
		p32(p - i);
		printk(": ");
		for(i2 = 0; i2 < unit; i2++)
		{
			p8(p[i+i2]);
			printk(" ");
		}
		printk("\n");
	}
}

//-----------------------------------------------------------------------------
//
void AnalyzeFault(TSSEG *s, int code)
{
	DWORD p1,p2;

	//
	p1 = GetThreadMemoryStart(GetCurrentThread());
	p2 = GetThreadMemoryEnd(GetCurrentThread());

	//
	if(code==14)
	{
		// Page fault:
		//------------------------------------------

		//
		printk("Page fault address=0x%x, PID%d memory area 0x%x-0x%x.\n",
			GETCR2(), GetCurrentThread(), p1,p2);
		//
		PageDumpNear(GETCR2());
	}
}

//-----------------------------------------------------------------------------
//
void tss_dump(TSSEG *s)
{
	// Print registers
	printk("EAX=%x  EBX=%x  ECX=%x  EDX=%x\n",
		s->eax, s->ebx, s->ecx, s->edx);
	printk("ESI=%x  EDI=%x  EBP=%x  SS:ESP=%x:%x\n",
		s->esi, s->edi, s->ebp, s->ss, s->esp);
	printk("CS=%x  DS=%x  ES=%x  FS=%x  GS=%x  SS=%x\n",
		s->cs, s->ds, s->es, s->fs, s->gs, s->ss);
	printk("CS:EIP=%x:%x  Link=%x  EFLAGS=%x  CR3=%x  CR2=%x\n",
		s->cs, s->eip, s->link, s->eflags, s->cr3, GETCR2());
	if( !(s->eflags&(1<<9)) )
	{
		printk("eflags: Interrupts were disabled.\n");
	}
	else
	{
		printk("eflags: Interrupts were enabled.\n");
	}
}

//
void PROCESSINFO (void)
{
  int i;

  //
  for (i = 0; i < nr_threads; i++)
    {
      printk
	("PID %d: state %d, ESP=%x, SS=%x, tick=%d, name=%s, base=%x, limit=%x\n",
	 i + 1, thread_states[i], thread_esps[i], thread_ss[i],
	 thread_tick[i], thread_name[i], GetThreadBase(i),
	 GetThreadLimit (i));
    }

  //
}

//
