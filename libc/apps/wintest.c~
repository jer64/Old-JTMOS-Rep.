// wintest.c - the simple test
#include <stdio.h>
#include <jtmos/graos.h>

// Request 256K of RAM from the init process.
SYSMEMREQ(APPRAM_256K)

//
void mainLoop(void)
{
	static MSG m;
	GUICMD *cmd;

	// Get GUICMD PTR
	cmd = &m.buf;

	// Loop until get exit command
	while(1)
	{
		// Get new message
		if( receiveMessage(&m) )
		{
			// Got new message
			if(cmd->id==GSID_EXIT)
				// We've been ordered to exit
				break;
		}

		// Spare some CPU time
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
	wid = createWindow(32,32, 400,300, "wintest",
		STANDARD_WINDOW);

	// Call main loop
	mainLoop();

	// Exit
	return 0;
}

//


