// wintest3.c - the terminal test
#include <stdio.h>
#include <conio.h>
#include <jtmos/graos.h>

// Request 256K of RAM from the init process.
SYSMEMREQ(APPRAM_256K)

// Which terminal ID to use with this terminal program
#define TERID		0

//
void mainLoop(int wid)
{
	static MSG m;
	static GUICMD *cmd;
	static BYTE *buf;
	static int frame;

	// Get GUICMD PTR
	cmd = &m.buf;

	// Add EGA terminal screen
	addFrameBuffer(wid,
		NULL, 0,0,
		80,25, 8,
		TYPE_TERMINAL_EMULATION,
		TERID); // terminal ID

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
				break;

				// Keyboard input
				case GSID_KEYPRESSED:
				OutputTerminalKeyboardBuffer(TERID, cmd->v[0]);
				break;
			}
		}

		// Send refresh request
	//	refreshWindow(wid);
		// Spare some CPU time
	//	sleep(1);
		SwitchToThread();
	}
}

// Test window system
int main(int arg, char **argv)
{
	int wid;

	// Get connected to graos (creates msgbox, etc.)
	connectGraos();

	// Create window
	wid = createWindow(32,32, 400,260, "wintest3 (with terminal)",
		STANDARD_WINDOW);

	// Call main loop
	mainLoop(wid);

	// Exit
	return 0;
}

//


