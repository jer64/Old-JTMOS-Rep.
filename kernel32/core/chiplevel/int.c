//--------------------------------------------------------------------------
/*
 * System Interrupt Handling Code / Highlevel Section
 *
 * int.c
 *
 * (C) 2001-2007 by Jari Tuominen(jari@vunet.org).
 *
 * This code is really nasty stuff,
 * I wonder why this crap always
 * crashes my computer... Well, I hope
 * I make it work atleast somehow
 * properly sooner or later.
 *
 * (Update: it works pretty well now, but still got somethings to work!)
 */
//--------------------------------------------------------------------------
//
// NOTE: assembly language based low level code that
// this C-language based code is dependent on,
// is located at llint.asm -assembly language file.
// (coding standard is NASM)

//
#define IRQDEBUGGER

//
#include "kernel32.h" // FATALEXCEPTION
#include "term.h"
#include "int.h"
#include "cpu.h"
#include "sysglvar.h"
#include "delay.h"
#include "threads.h"

//--------------------------------------------------------------------------
// GDT table will be allocated in 0x100000 memory region.
GDTTAB *gdt_table = NULL;
TASKS *tasks = NULL;
WORD irq_mask = 0xFFFF; // All IRQs disabled initially.

//--------------------------------------------------------------------------
// Give out pointer to the new IDT table.
// The old one is the one which the boot loader setupped.
void *GetIDTPTR(void)
{
	//
	return 0x1F0000;
	//	return OFFSET_TO_IDT_TABLE;
}

//
void init_gdt(void)
{
	DWORD i,i2,i3,i4;

	//
	gdt_table = malloc(sizeof(GDTTAB));
	memset(gdt_table, 0, sizeof(GDTTAB));

	// create gdt table
	// segment selectors creation,
	// 0x00, 0x08, 0x10, 0x18, 0x20, 0x28 (6 total,
	// 			the first one is unusable dummy)
	// 				limit,baseLO,baseHI,FLAGS,ACCESS,baseHI2
	set_cdseg(&gdt_table->dummy,	0xFFFF,0,0,0x9A,0xCF,0);
	set_cdseg(&gdt_table->code32,	0xFFFF,0,0,0x9A,0xCF,0);
	set_cdseg(&gdt_table->data32,	0xFFFF,0,0,0x92,0xCF,0);
	set_cdseg(&gdt_table->core32,	0xFFFF,0,0,0x92,0xCF,0);
	set_cdseg(&gdt_table->code16,	0xFFFF,0,0,0x9A,0,0);
	set_cdseg(&gdt_table->data16,	0xFFFF,0,0,0x92,0,0);
 
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	/* <For general purpose>
	 * This is the base of how the memory is mapped,
	 * the mapping is created by defining the segment
	 * registers above, f.e. code/data/etc.
	 */
	SETUPGDT((DWORD)gdt_table);
}

void dump_gdt_info(void)
{
	//
	print("(0x00)  dummy   = 0x"); p32(&gdt_table->dummy); 	 print("(* dummy)\n");
	print("(0x08)  code32  = 0x"); p32(&gdt_table->code32);  print("\n");
	print("(0x10)  data32  = 0x"); p32(&gdt_table->data32);  print("\n");
	print("(0x18)  core32  = 0x"); p32(&gdt_table->core32);  print("\n");
	print("(0x20)  code16  = 0x"); p32(&gdt_table->code16);	 print("\n");
	print("(0x28)  data16  = 0x"); p32(&gdt_table->data16);	 print("\n");
}

// This function redirects idt starting at the predefined address
// and the table itself so will start at offset+6 ....
// (Max. size of GDT table is 128K!
//  This limit is based on the memory planning.)
void int_redirectidt(void)
{
	int i,i2,i3,i4,ad,ad2,ad3,ad4;
	WORD *p,cs_current;

	//
	asm volatile("movw %%cs,%0":"=g"(cs_current));

	//
	disable();

	// Point at beginning of the IDT table.
	p=GetIDTPTR();
	printk("IDTPTR = %x\n", p);
	//
	ad4 = GetIDTPTR()+6;
	p[0]=0x800; // IDT size = 0x800 (256 interrupts)
	p[1]=(ad4) & 0xFFFF; // ---> offset+6
	p[2]=(ad4 >> 16) & 0xFFFF; // -/

	// Point at beginning of the interrupt offsets.
	p += 3;
	// Write all 256 interrupts to point at nullinterrupt handler.
	for(i=0; i<256; i++)
	{
		ad=(unsigned long int)nullinterrupt;
		p[0+i*4]=ad&0xFFFF;
		p[1+i*4]=0x08; // segment -- cs_current
		p[2+i*4]=0xEE00;
		p[3+i*4]=(ad>>16)&0xFFFF;
	}
  
	// setup 17 standard interrupts
	for(i=0; i<17; i++)
	{
		//
		setint(i,nullinterrupt2, SEG_CODE32, 0x8E00);
	}
 
	// Activate new IDT.
	setupidt( GetIDTPTR() );
}

// Creates clean/default IDT and exception handlers.
void init_idt(void)
{
	//
	int_redirectidt();
   
	//
	setup_exception_handlers();
}

// Creates safe IDT and exception handlers.
void init_safe_idt(void)
{
	int x;

	//
	int_redirectidt();
   
	//
	setup_llexception_handlers();

	//
	outportb(0x3c8,0);
	outportb(0x3c9,0x3F);
	outportb(0x3c9,0x1F);
	outportb(0x3c9,0x0F);
}

/*
 * setint
 * ------
 * This routine setups interrupt handlers.
 *
 * word handler_offs0_15
 * word segment_selector
 * word access
 * word handler_offs16_31
 */
void setint(	int intnum,
		void *handler,
		unsigned short int _segment,
		unsigned short int _access)
{
	WORD *p,*pz;
	DWORD ad,ad2,ad3,ad4,l,h;

	// Offset to IDT
	p = GetIDTPTR(); p+=3;

	//
	ad=(DWORD)handler;

	// 1x32bit->2x16bit
	l=ad & 0xFFFF;
	h=(ad>>16) & 0xFFFF;

	// 4 x 2 words
	// set offset to the handler
	p[(intnum * 4)+0]=l;
	p[(intnum * 4)+3]=h;
	// set segment
	p[(intnum * 4)+1]=_segment;
	// set access
	p[(intnum * 4)+2]=_access; // | (0x4000|0x2000); // |0x4000,0x2000
	return;
}

//
void *getint(long intnum)
{
	WORD *p;
	DWORD ad,ad2,ad3,ad4,l,h;

	// Offset to IDT
	p=0x00000000;

	//
	l=p[(intnum<<2)+0];
	h=p[(intnum<<2)+3]<<16;

	//
	ad=l|h;

	//
	return ad;
}

//
void Init8259(void)
{
	// Start 8259 initialization
	outportb(M_PIC, ICW1);
	outportb(S_PIC, ICW1);

	// Base interrupt vector
	outportb(M_PIC+1, M_VEC);
	outportb(S_PIC+1, S_VEC);

	// Bitmask for cascade on IRQ 2
	outportb(M_PIC+1, 1<<2);
	// Cascade on IRQ 2
	outportb(S_PIC+1, 2);

	// Finish 8259 initialization
	outportb(M_PIC+1, ICW4);
	outportb(S_PIC+1, ICW4);

	// Mask all interrupts
	outportb(M_IMR, 0xFF);
	outportb(S_IMR, 0xFF);
}

/* enables irq irq_no */
void enable_irq(unsigned short irq_no)
{
	//
	irq_mask &= ~(1 << irq_no);
	if(irq_no >= 8)
		irq_mask &= ~(1 << 2);

	//
	outportb(M_PIC+1, irq_mask & 0xFF);
	outportb(S_PIC+1, (irq_mask >> 8) & 0xFF);
}

/* disables irq irq_no */
void disable_irq(unsigned short irq_no)
{
	irq_mask |= (1 << irq_no);
	if((irq_mask & 0xFF00)==0xFF00)
		irq_mask |= (1 << 2);

	//
	outportb(M_PIC+1, irq_mask & 0xFF);
	outportb(S_PIC+1, (irq_mask >> 8) & 0xFF);
}

// Dumps critical information.
// (GDT/IDT tables)
void dump_critical(void)
{
	WORD *gdt,*gdtt, *idt,*idtt;
	int i,i2,i3,i4;

	//
	gdt=(unsigned long int)GETGDTOFFS();
	idt=(unsigned long int)GETIDTOFFS();

	//
	print("> Doing IDT & GDT dump\n");

	// --- DUMP OF IDT ---
	// beginning
	print("Using IDT, which resides at the memory location 0x");
	p32( idt ); print("\n");
	//
	print("IDT size =   0x");
	p16( idt[0] );
	print("\n");
	//
	print("IDT table's offset = 0x");
	p32( (idt[1]+(idt[2]<<16)) );
	print("\n");

	//
	idtt=idt[1]+(idt[2]<<16);

	//
	print("Now dumping IDT table's contents :\n");

	//
	for(i=0; i<8; i++)
	{
		//
		p32( ((unsigned long int) (idtt))+(i*8) ); print("> ");
		// PRINT INTERRUPT #
		print("IDT entry #"); printhex8(&screens[0],i); print(": ");
		// PRINT OFFSET
		print("handler=0x");
		printhex32( &screens[0], idtt[0+i*4]+(idtt[3+i*4]<<16) );
		// PRINT SEGMENT
		print(", segment=0x"); printhex16( &screens[0], idtt[1+i*4] );
		// PRINT ACCESS
		print(", access=0x"); printhex16( &screens[0], idtt[2+i*4] );
		// new line !
		print("\n");
	}
	print("End of IDT dump.\n");
	waitpesc();

	//
	gdtt=(unsigned short int*)gdt_table;

	// --- DUMP OF GDT ---
	// beginning
	print("Using GDT, which resides at the memory location 0x");
	p32( gdt ); print("\n");
	//
	print("GDT size =   0x");
	p16( gdt[0]);
	print("\n");
	//
	print("GDT table's offset = 0x");
	p32( (unsigned long int)gdtt );
	print("\n");

	//
	print("Now dumping GDT table's contents :\n");

	// print # entries out of GDT table (gdtt)
	for(i=0; i<8; i++)
	{
		//
		p32( ((unsigned long int) (gdtt))+(i*8) ); print("> ");
		// PRINT GDT ENTRY #
		print("GDT entry #"); p8(i); print(": ");
  
		// PRINT SEGMENT
		print("length=0x"); p16(gdtt[i*4+0]); print(", ");
  
		// PRINT BASE(32bit, 24bit used) .. consists of one word, and two bytes.
		print("base=0x"); p16( gdtt[i*4+1] + ((gdtt[i*4+3]&0xff) << 16) + \
	                              ((gdtt[i*4+2]>>8) << 24) ); print(", ");
                              
		// PRINT FLAGS(8bit)
		print("flags=0x"); p8( gdtt[i*4+1]>>8 ); print(", ");
  
		// PRINT ACCESS
		print("access=0x"); p8( gdtt[i*4+3]&0xff );
  
		// Fin./Next
		print("\n");
	}
	print("End of GDT dump.\n");
	waitpesc();
}

//
void irqdebugger(void)
{
#ifdef IRQDEBUGGER
	//
	PUSHFLAGS;
	disable();

	//
	if( GetThreadBase(GetCurrentThread()) )
	{
		//
		printk("\n[Caught Application Thread Switch]\n");
		printk("PID %d/%d(%s), DS=%x ES=%x FS=%x GS=%x SS=%x, tick=%d, state=%d\n",
				GetCurrentThread(),nr_threads,
				thread_name[RPID(GetCurrentThread())],
			        thread_ds[RPID(GetCurrentThread())],
			        thread_es[RPID(GetCurrentThread())],
			        thread_fs[RPID(GetCurrentThread())],
			        thread_gs[RPID(GetCurrentThread())],
			        thread_ss[RPID(GetCurrentThread())],
				thread_tick[RPID(GetCurrentThread())],
				thread_states[RPID(GetCurrentThread())]
			);

		//
		printk("CODE SEG DESCRIPTOR: %x %x %x %x  %x %x %x %x\n",
			thread_codedescs[RPID(GetCurrentThread())*8+0],
			thread_codedescs[RPID(GetCurrentThread())*8+1],
			thread_codedescs[RPID(GetCurrentThread())*8+2],
			thread_codedescs[RPID(GetCurrentThread())*8+3],
			thread_codedescs[RPID(GetCurrentThread())*8+4],
			thread_codedescs[RPID(GetCurrentThread())*8+5],
			thread_codedescs[RPID(GetCurrentThread())*8+6],
 			thread_codedescs[RPID(GetCurrentThread())*8+7]);
		//
		printk("DATA SEG DESCRIPTOR: %x %x %x %x  %x %x %x %x\n",
			thread_datadescs[RPID(GetCurrentThread())*8+0],
			thread_datadescs[RPID(GetCurrentThread())*8+1],
			thread_datadescs[RPID(GetCurrentThread())*8+2],
			thread_datadescs[RPID(GetCurrentThread())*8+3],
			thread_datadescs[RPID(GetCurrentThread())*8+4],
			thread_datadescs[RPID(GetCurrentThread())*8+5],
			thread_datadescs[RPID(GetCurrentThread())*8+6],
			thread_datadescs[RPID(GetCurrentThread())*8+7]);

		//	
		//showappgdt();

		//
		printk("Execution freezed - Press [ESC] to unfreeze\n");

		//
		ESCLOCK();
	}

	//
	POPFLAGS;

	//
	return;
#endif
}

//
void set_cdseg(CDSEG *t,
	 	WORD	seg0_15,
 		WORD	base0_15,
		BYTE	base16_23,
		BYTE	flags,
		BYTE	access,
		BYTE	base24_31)
{
	//
	t->seg0_15=	seg0_15;
	t->base0_15=	base0_15;
	t->base16_23=	base16_23;
	t->flags=	flags;
	t->access=	access;
	t->base24_31=	base24_31;
}

//

