// winshell.c - terminal window shell
#include <stdio.h>
#include <conio.h>
#include <jtmos/graos.h>

// Use logging to file? (otherwise logs to stderr)
#define LOG_TO_FILE

// Request 256K of RAM from the init process.
SYSMEMREQ(APPRAM_256K)

//
FILE *log;

//
void terminalEmulationWindow(int wid, int terID)
{
	static MSG m;
	GUICMD *cmd;
	BYTE *buf;
	int frame;

	// Get GUICMD PTR
	cmd = &m.buf;

	// Add EGA terminal screen
	addFrameBuffer(wid,
		NULL, 0,0,
		80,25, 8,
		TYPE_TERMINAL_EMULATION,
		terID); // terminal ID

	// Send refresh request
	refreshWindow(wid);

	// Loop until get exit command
	for(frame=0; ; frame++)
	{
		// Get new message
		if( receiveMessage(&m) )
		{
			// Got new message
			switch(cmd->id)
			{
				// Exit now?
				case GSID_EXIT:
				// We've been ordered to exit
				goto exitti;

				// Keyboard input
				case GSID_KEYPRESSED:
				OutputTerminalKeyboardBuffer(terID, cmd->v[0]);
				break;
			}
		}

		// Spare some CPU time
		SwitchToThread();
	}

exitti:
	// Close window
	closeWindow(wid);
}

// Test window system
int main(int argc, char **argv)
{
	int wid,terID,pid;
	char fname[256];

	//-------------------------------------------------------------
	// Get connected to graos (creates msgbox, etc.)
	connectGraos();

	//
	pid=0;

	//-------------------------------------------------------------
	// Open log file
	//
	sprintf(fname, "winshell%d.log", GetCurrentThread());
#ifdef LOG_TO_FILE
	log = fopen(fname, "wb");
#else
	log = stderr;
#endif

	//-------------------------------------------------------------
	// Report
	//
	fprintf(log, "WINSHELL TERMINAL EMULATION WINDOW LAUNCHPAD 1.0\n");

	//-------------------------------------------------------------
	// Will require one argument atleast
	if(argc<2)
	{
		//
		fprintf(log, "Usage: winshell [shell program to launch in window]\n");
		goto endi;
	}

	//-------------------------------------------------------------
	// Create a new terminal window
	//
	terID = CreateTerminal();
//	SetThreadTerminal(GetCurrentThread(), terID);

	// Run program there
	fprintf(log, "Running %s into background\n", argv[1]);
	printf("Running %s into background\n", argv[1]);
	pid = bgexecEx(argv[1], "/", terID);

	// Program not found?
	if(pid<=0)
	{
		// Program not found
		fprintf(log, "Program not found: %s\nbgexecEx returned = %d (PID value)\n",
			argv[1], pid);
		printf("Program not found: %s\nbgexecEx returned = %d (PID value)\n",
			argv[1], pid);
		goto endi;
	}

	// Create window
	wid = createWindow(-1,-1, 640,260-41, argv[1], // -1,-1 means autoposition
		STANDARD_WINDOW & (~WINDOW_AUTOCLEAR));

	//-------------------------------------------------------------
	// Call main loop
	fprintf(log, "Launching terminal emulation ...\n");
	terminalEmulationWindow(wid, terID);

endi:
	// Exit
	fprintf(log, "winshell exiting ...\n");
#ifdef LOG_TO_FILE
	fclose(log);
#endif

	// Terminate program(if any) on window close
	if(pid)		KillThread(pid);
	printf("Program end.\n");
	return 0;
}

//


