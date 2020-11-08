/**** JTMOS KERNEL - (C) 1999-2014,2017 by Jari Tuominen (jari.t.tuominen@gmail.com). ***
 *
 *  kernel32.c - KERNEL INIT PROCESS.
 *  The First Process Of The Operating System To Be Run.
 *
 * This software is distributed under GPL license
 * (see file 'COPYING').
 * See keropt.h for kernel options.
 *
 * New XML-based init function.
 */

//
#include <stdio.h>
// STATIC DATA ARRAYS, included once:
// Only include this once.
// This header includes array of
// complete boot sector binary,
// which'll be used when formatting
// a system disk.
#include "boot_bin.h"
// Hard disk driver.
#include "testdrv.h"
// System Maintenance Debug Shell
#include "sh.h"
//
#define CHECKDEBUG debugKey();

// Safe mode in the beginning of system start up
DWORD KernelInitPassed=FALSE;
int EGASAFE=TRUE;
int KINIT=0;
BIOS_DATA bios_data;

// DUMMY CODE:	PATH, CURRENT COMMAND STRING,
//		COMMAND PARAMETERS FOR /MINIMAL SYSTEM SHELL/
// Active device.
char str_device[255];
// f.e. "ls" "/" "-la"
// created using sptstr
char *cmdpar[100];
// this string will contain the entire command line
char *wholestring;

//---------------------------------------------------------------
void WaitSystemStart(void)
{
	//
	while(!systemStarted)
	{
		//
		SwitchToThread();
	}
}

//---------------------------------------------------------------
//
// Fix The EGA Memory (Text Mode)
// Nice and clean...
//
int FixEgaMemory(void)
{
	int i;
	char *p;

	p = 0xb8000;

	for(i=0; i<(80*25*2 * 4); i+=2)
	{
		p[i+0] = ' ';
		p[i+1] = 0x07;
	}
	
	return 0;
}

//---------------------------------------------------------------
// Start the SMSH shell process
void StartShellProcess(void)
{
	//
	disable();

	//
	ThreadCreateStack(&ShellThread, 1024*32);
	ShellThread.pid=create_thread(
		ShellThread.stack,ShellThread.l_stack,run_shell);
	// With start up
	SetThreadTerminal(ShellThread.pid, 0);
	IdentifyThread(ShellThread.pid, "shell");

	//
/*	ThreadCreateStack(&ShellThread2, 1024*32);
	ShellThread2.pid=create_thread(
		ShellThread2.stack,ShellThread2.l_stack,run_shell);
	// With start up
	SetThreadTerminal(ShellThread2.pid, 1);
	IdentifyThread(ShellThread2.pid, "shell");*/

	//
	enable();
	return;
}

//---------------------------------------------------------------
//
void OneMegCleanUp(void)
{
	BYTE *p;
	int i,i2;

	//
	for(i=0,p=0x100000; i<(1024*1024); i++)
	{
		p[i] = 0;
	}
}

//---------------------------------------------------------------
// **** SYSTEM INITIALIZATION CODE ****
void KernelInit(void)
{
	char *buf;
	int i,i2,i3,i4;

	// Make sure we cannot be recalled
	if(KernelInitPassed==0xFCE2E37B)
	{
		Bingo();
		ExitThread(0);
	}

	// Indicate that we've already been here.
	KernelInitPassed = 0xFCE2E37B;

	// Get the BIOS data, we need it besides the PCI scanner (f.e. serial UART ports).
	memcpy(&bios_data,0x400,sizeof(BIOS_DATA));

	// This initializes basic kernel system(safe).
	// Start threads
	InitSystem();

	//
	initSystemLogging();
	start_statusline();

	// Major reason for the runtime to be at kinit(pid0) is the memory allocation.
	// Memory allocation from the init functions must be permanent.
	fdc_init();
	ramdisk_init();
	hd_init();
	partition_table_support_init();
	hwdet();
	/*
	buf = malloc(65536);
	printk("Reading RAMDISK boot block ...\n");
	readblock(ND("ram"), 0, buf);
	printk("Looking boot block for an identifier ...\n");
	for(i=0,i2=0; i<getblocksize(ND("ram")); i++) {
		printk("%.2x ", buf[i]);
		if(i2==0 && buf[i]=='J') { i2++; } else { i2=0; }
		if(i2==1 && buf[i]=='T') { i2++; } else { i2=0; }
		if(i2==2 && buf[i]=='M') { i2++; } else { i2=0; }
		if(i2==3 && buf[i]=='F') { i2++; } else { i2=0; }
		if(i2==4 && buf[i]=='S') { i2++; }
		if(i2==5) { i2=0; printk("%s: ", __FUNCTION__); break; }
	}*/

	// All shells require rd,hd init to be done (ramdisk, hard disk driver must be enabled)
	//register_shell();
	StartShellProcess();

//	InitSystemPass2_XML(); // compiled from XML (kernel.xml)

	// Indicate a level 1 system start.
	// (init process will start functioning)
	// Level 2 indicates that system logs can be written
	// on the root file system, it also basically means
	// that the system is fully functional otherwise too.
	systemStarted |= 1;

	//
	if(vesa_enabled)
	{
#ifdef GRAOS
		_beginthread(GuiMain, 0, NULL);
#endif
	}

	//
	// ---------------------------------------------------
	// Stay at loop, spare all CPU time.
	// Execution will always stop here.
	while(1)
	{
		SwitchToThread();
	}
}

//---------------------------------------------------------------
void debugKey(void)
{
	// If space has been hit => Enable debug mode
	if(getch1()==27)	ENABLE_STARTUP=0;
	if(getch1()==32)	jtmos_debug=1;
}

//---------------------------------------------------------------
//
// plasmaInit - Color plasma
//
void plasmaInit(void)
{
	int x,y,c,i,i2,framenr,zoom;
	float fa;
	char cols[]={
		BLACK,BLUE,CYAN,YELLOW,
		15,YELLOW,CYAN,BLUE,
	};

	//
	for(framenr=0,fa=0; ; framenr++,fa+=.01)
	{
		zoom = 80+(49*fa);

		gotoxy(1,1);
		for(y=0; y<23; y++) {
			for(x=0; x<80; x++)
			{
				textcolor( cols[((x*y/zoom)+framenr)&7] );
				printk("%c", 219);
			}
		}
	}
}

//
