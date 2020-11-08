// ===========================================================
// HARDWARE DETECTION PROCESS
// Tries different drives and searches for new hardware.
// (C) 2003-2020 by Jari Tuominen (jari.t.tuominen@gmail.com).
// ===========================================================
#include "kernel32.h"
#include "threads.h"
#include "hwdet.h"

//
int alreadyCalled=0;
DWORD hwdetCounter=0,hwdetDone=0;
//
THREAD hwdetThread;

// Hardware detection
void hwdet(void)
{
	//
	hwdetDone=0;

	//
	printk("HWDET ...\n");

	//
	disable();
	hwdetDone = FALSE;
	ThreadCreateStack(&hwdetThread, 1024*512);
	hwdetThread.pid = 
		create_thread(hwdetThread.stack,
			hwdetThread.l_stack, hwdetProgram);

	// Non-visible thread terminal
	////SetThreadTerminal(hwdetThread.pid, 1);
	// Max. 31 characters
	IdentifyThread(hwdetThread.pid, "KHWDET");
	enable();

	//
	while(hwdetDone==FALSE)
	{
		sleep(1);
	}
	printk("End of internal driver running.\n");
}

//
void hwdetProgram(void)
{
	//
	if(alreadyCalled)
	{
		//
		hwdetDone=TRUE;
		ExitThread(0);
	}
	else
		//
		alreadyCalled=TRUE;

	// === DETECT NEW HARDWARE =======================
	// ===============================================


	// ===========================
	// COMMUNICATION DEVICES INIT:

	//
	if(LPT_ENABLED)
		// LPT ports
		lpt_init();

	// (HINT: DO THIS BEFORE THE SERIAL MOUSE, MOST WISE THING.)
	if(SERIAL_ENABLED)
		// SERIAL ports
		serial_init();

	if(PCI_ENABLED)
		// PCI bus
		pci_init();

	//
	if(PS2MOUSE_ENABLED)
		ps2mouse_init();

	//
	if(SERMOUSE_ENABLED)
		SerialMouseInit();

	// ============================
	// OTHER HARDWARE DEVICES INIT:

	//
	// loading_logo();

	// =========================
	// BLOCK DEVICE DRIVERS INIT:

	// Generic audio device(GAD) init.
	// GAD is required for any audio drivers.
	if(AUDIO_ENABLED)
	{
		// Init GAD.
		audio_init();

		// Gravis Ultrasound init.
		if(GUS_ENABLED)
			gus_init();

		// Sound Blaster init.
		// 0=only checks briefly for SB, 1=hardscan, takes time.
		if(SB_ENABLED)
			sb_init(0);
	}

	// Flag that we are done here
	hwdetDone=1;

	// Exit.
	ExitThread(0);
}

//

