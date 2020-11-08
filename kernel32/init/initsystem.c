//////////////////////////////////////////////////////////////////////////////////
// InitSystem.c
// Memory initialization, etc. Very low level stuff.
// (C) 2002-2008,2017-2020 Jari Tuominen (jari.t.tuominen@gmail.com).
//////////////////////////////////////////////////////////////////////////////////
#include "kernel32.h"
#include "paging.h" // memory paging
#include "initsystem.h"
#ifdef GRAOS
#include "graos.h"
#endif

//
static *ega_scr = 0xB8000;
static BYTE *term_buf[(80*50*2)];

//---------------------------------------------------------------
void Bingoo(void)
{
	int i,ii;
	char *scr;

	//
	scr = 0xB8000;

	//
	for(ii=0; ; ii++)
	{
		for(i=0; i<(80*1*2); i++) { scr[i] = i+ii; }
	}
}

//---------------------------------------------------------------
// Initializes the system entirely (boot up).
// 0xD0000 bytes of static variables at 0x100000.
int InitSystem(void)
{
	unsigned short int *checksum,*measured_checksum;
	void *biosinfo;
	LDR *l;
	WORD *bootdrv;
	static char bootstr[20];
	DWORD ad,ad2;
	BYTE *vesainfo;
	static BYTE *vb,*p;
	DWORD *ptr;
	int i,i2,i3,i4,
		x,y;

	// A must do thing. Reset kernel static variables area.
	xmemset(0x100000, 0, 0xD0000);

	// Point at boot loader start in memory.
	l = 0x7C00;
	// Kernel checksum/original, kernel checksum/measured.
	// Kernel parameters reside at memory address 0x1000.
	checksum = 		0x1000;
	measured_checksum = 	0x1002;
	bootdrv =		0x1004;
	//
	screens[ID_VIRTUAL_TERMINAL_1].buf = term_buf;

	//
	init_safe_idt();

	//
	memset(term_buf,0,80*25*2);
	clrscr();
	printk("%s 1.0.%x(%s) \n\n",
		JTMOSBETANAME, BUILD_NR, BUILD_DATE);
	printk("\n");
        printk("   __ ______ _   _ _____ _____\n");
        printk("   __   __   __ __ __ __ __ __\n");
	printk("   __   __   _____ __ __ ___  \n");
	printk("__ __   __   __ __ __ __   ___\n");
	printk("__ __   __   __ __ __ __ __ __\n");
	printk("_____   __   __ __ _____ _____\n");
	printk("\n");
	printk("O P E R A T I N G  S Y S T E M\n");
	printk("\n");
	printk("(C) 1997-2020 by Jari Tuominen (jari.t.tuominen@gmail.com).\n");
	printk("\n");
	sleep(2);
	//

	// Store BIOS boot drive setting.
	SetBiosBootDrive(*bootdrv);

	// Make safe mode EGA usable first(for kernel panic messages).
	//VesaSetup();
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "EGASafeMode      ", 0,0);
#endif
	EGASafeMode(); // does not touch hardware
	SetVisibleTerminal(0);
	EGASAFE=1;

	//
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "Kernel info      ", 0,0);
#endif
	textcolor(WHITE); textbackground(BLACK);
	clrscr();
	printk("\n");
	printk("  Kernel (%x) initialization in progress. \n", BUILD_NR);
	printk("\n");
	textcolor(CYAN);

	// Check boot loader signature.
	if(l->signature!=0xAA55 && l->signature!=0x55AA)
	{
		//
		printk("Logic check failed:\n");
		printk("  Boot loader signature is invalid.\n");
		printk("  Proceed with caution.\n");
		sleep(10);
	}

	// IMPORTANT NOTE: I noticed that there is a huge need
	//		for initing malloc before most of the things,
	//		of course, before initing malloc here,
	//		we detect the amount of memory available..
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "scanmem      ", 0,0);
#endif
	printk("Probing Amount Of RAM Available (\"scanmem();\").\n");
	scanmem();

	// Dynamic memory region begins at 6 megabyte region
	// 000000-100000: kernel code/data
	// 100000-1E0000: kernel data
	// 1E0000-1F0000: zero process stack
	// 1F0000-1FF000: IDT table
	// 1FF000-200000: kernel page directory
	// 200000-600000: kernel page tables, etc. (4 MB)
	// 600000-9FFFFF: application page tables (4 MB)
	// A00000-XXXXXX: dynamic memory region

	//printk("Setup temporary safe IDT/exception handlers:\n");
	//printk("Done.\n");

	//
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "mmInit      ", 0,0);
#endif
	printk("mmInit:\n");
	ad = 0xA00000;
	mmInit(ad, ms.amount-ad); // paging and dynamic memory allocation will be enabled...

#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "VirtualTerminalInit      ", 0,0);
#endif
	//serial_init();
	printk("VirtualTerminalInit\n");
	// PRIMITIVE LEGACY SERIAL PORTS INIT
	// Define port addresses (not real serial init,
	// but makes possible to poke serial ports at primitive level,
	// presuming the default settings are there).
	VirtualTerminalInit();
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "EgaConsolelInit      ", 0,0);
#endif
	printk("EgaConsoleInit\n");
	EgaConsoleInit();
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "EgaConsolelInit OK   ", 0,0);
#endif
	printk("EgaConsoleInit OK\n");

	// Memory requirement test..
	printk("GetRamAmount\n");
	if( GetRamAmount()<(1024*1024*16) )
	{
		panic("Oops! JTMOS operating system requires atleast 16M of RAM. This may change in future versions of the operating system.");
	}

	//------------------------------------------------------------------------------------------------------
	// Init environmental variable database system.
	// (this is good to be done slightly after mm is initialized)
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480, "initEnvDataBase      ", 0,0);
#endif
	printk("initEnvDataBase\n");
	initEnvDatabase();

	// Start system log (memory based).
	printk("initSystemLog\n");
	initSystemLog(); // logging.c
	printk("initSystemLog OK\n");

	//--------------------------------------------------------
	// From here on the system log is functional ...
	//--------------------------------------------------------

	//
	biosinfo=(DWORD)0x1010;

	// Clear the global variables structure
	xmemset( ((void*)&glvar), 0, sizeof(GLVAR));
	// debug
	glvar.debugMsg = jtmos_debug;

	// Copy biosinfo
	memcpy(&glvar.biosinfo, biosinfo, 0x100);
 
	// -- Init text screen console
	EGASAFE=0;
	//printk("VesaSetup\n");
	//VesaSetup();
	//printk("VesaSetup OK\n");

	// -- Make cursor alive
	CursorState(&screens[0], 1);

	// -- Initialize VMODE module
	// This is a mandatory job to do.
	// Otherwise the display administration won't
	// get the current video mode.
	printk("screen reset\n");
	if(vesa_frame_buf==NULL)
	{
		FixEgaMemory();
		//setvmode(3);
	}
	clrscr();

	// Set textcolor/background & clear screen
	window(0,0, 79,24);
	textcolor(7); textbackground(BLUE); clrscr();
	printk("\n");
	printk("  %s 1.0.%x(%s) \n\n",
		JTMOSBETANAME, BUILD_NR, BUILD_DATE);

	printk("some info\n");

	// KERNEL CHECKSUM CHECK
	if(*bootdrv<=1)
	{
		printk("%x/%x: ", checksum[0], measured_checksum[0]);
		if( checksum[0]!=measured_checksum[0] )
		{
			if(*bootdrv<=1)
			{
				printk("Warning: Kernel checksum failure detected.\n");
				sleep(10);
			}
		}
		else
		{
			printk("Kernel checksum OK.\n");
		}
	}

	//
	printk("Initializing kernel ...\n");
	printk("BuildNR: %x(%s), 10000 - %x\n", BUILD_NR,BUILD_DATE,&kernelend);

	// We call devsys so devices can register in the system properly.
	driver_sys_init();
	// Here we init the buffering system, and api(not much to do, just init the structure).
	driver_init();
 
	// Register Devices!
	// Init the ega/vga(doesn't touch the video itself, just detection,
	// and device registration into the device management system).

	//
	printk("Initializing drivers :  ");
	// EGA text features support + terminal support
	printk("*EGA ");
	ega_init();
	// VGA init must be called only after EGA init
	printk("*VGA ");
	vga_init();
	// Init some other devices
	printk("*NULL ");
	null_init();
	printk("*ZERO ");
	zero_init();
	printk("\n");

	// Most of these functions require malloc, remember that.
	// Malloc must be inited before all this!
	//
	printk("Initializing Global Descriptor Table (GDT).\n");
	init_gdt();
	printk("Initializing Intel Programmable Interrupt Controller 8259.\n");
	Init8259();
	printk("Initializing Interrupt Decriptor Table (IDT).\n");
	init_idt();
	printk("System timer system init.\n");
	InitTimer();
	printk("Initializing dynamic Global Descriptor Table (GDT) allocation system.\n");
	InitGDTEntryAllocationSystem();
	printk("\n");
	// Remark this line if syscalls are causing problems(e.g. putch)
	printk("Initializing System Calls.\n");
	scall_init();
	// Init thread management (will init TSS too).
	printk("Initializing Multitasking (Threads).\n");
	init_threads();
	// Define this process as init
	printk("Naming current process as \"init\".\n");
	IdentifyThread(GetCurrentThread(), "init");

	//
	switch(GetBiosBootDrive())
	{
		case 0:
		strcpy(bootstr, "floppy");
		break;
		case 1:
		strcpy(bootstr, "floppy2");
		break;
		case 0x80:
		strcpy(bootstr, "hda");
		break;
		case 0x81:
		strcpy(bootstr, "hdb");
		break;
		case 0x82:
		strcpy(bootstr, "hdc");
		break;
		case 0x83:
		strcpy(bootstr, "hdd");
		break;
		default:
		strcpy(bootstr, "UNKNOWN");
		break;
	}
	printk("System is using boot drive %s:.\n",
		bootstr);

//===============================================================
	// From here on threads, terminals, etc. work:

//===============================================================
	//
	if(CMOS_ENABLED)
	{
		printk("Initializing real-time clock (CMOS).\n");
		cmos_init();
	}

	//
	if(KEYBOARD_ENABLED)
	{
		printk("Initializing keyboard.\n");
		InitKeyboard();
	}

//===============================================================

	//
	printk("Running Serial Port UART System Reset Before Interrupts Can Occur.\n");
	serial_init();
	printk("Done.\n");
	//
	printk("Enabling interrupts.\n");
	//	IRQStatusCritical();
	enable();
	printk("Interrupts enabled.\n");

	//
	printk("Enabling critical PIT timer interrupt.\n");
        enable_irq(0);
//===============================================================

	// Determine CPU power
//	if(CPU_SPEED_TEST_ENABLED)	CPUTest();
	        // -----------------------------------------------------------------
        // End of device detection and registration.

        // -----------------------------------------------------------------
        // BOOT UP TIME FILE SYSTEM INITIALIZATION.
        if(XON_FS)
        {
                //
                printk("Initialising file system service:\n");

                //
                printk("- File allocation system(JTMFAT) init: Calling jtmfat_init();\n");
                //getch();
                jtmfat_init();

                //
                printk("- General file system(JTMFS) init: Calling jtmfs_init();\n");
                //getch();
                jtmfs_init();

                //
                printk("- Directory cache(DIRDB) init: Calling dirdb_init();\n");
                //getch();
                dirdb_init();
                printk("Done.\n");
                //getch();
        }


	// Install generic audio driver.
	printk("Initializing audio.\n");
	audio_init();

	// -----------------------------------------------------------------
	// End of device detection and registration.
	//

	// -----------------------------------------------------------------
	// From now on we can normally access the devices:

	// Call CPU idler.....
//	if(XON_IDLE)
//		syssh_callappex(IdleThreadProgram, 0, GetCurrentTerminal());

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
	disable();
	textcolor(14);

	//
	printk("End of kernel init pass 1.\n");

/*	printk("KEY ... ");
	enable();
	getch();
	printk("Thanks ...\n");*/

	//
	return 0;
}

