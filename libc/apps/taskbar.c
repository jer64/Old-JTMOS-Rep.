//----------------------------------------------------------
// taskbar.c - the Graos taskbar application
//----------------------------------------------------------
#include <stdio.h>
#include <conio.h>
#include <jtmos/graos.h>
#include <jtmos/graosprotocol.h>

// Request 256K of RAM from the init process.
SYSMEMREQ(APPRAM_256K)

//
int RenderTaskBar(BYTE *buf, int width,int height, int frame)
{
	int x,y,ad,ad2,i,i2,fr;
	BYTE pal1[16]={
		0,8,7,14, 15,14,7,8,
		0,BLUE,CYAN,10, 15,10,CYAN,BLUE
		};

	//
	fr = frame;

	// Draw two scrolling fades
	for(i=0; i<2; i++)
	{
		//
		for(y=i; y<height; y+=2)
		{
			//
			ad = width*y;
	
			//
			for(x=0; x<width; x++)
			{
				//
				buf[ad+x] = pal1[( (((x>>(i<<4))+y)>>(2) )+fr & 7) + (i<<3)]; 
			}
		}
	}

	//
	return 0;
}

//
void mainLoop(int wid)
{
	static MSG m;
	static GUICMD *cmd;
	static BYTE *buf;
	static int frame;

	//------------------------------------------------------------
	// Get GUICMD PTR
	cmd = &m.buf;

	//------------------------------------------------------------
        // Allocate memory for bitmap region
        buf = malloc(640*30);

        // Create bitmap region next to start button on right
        addFrameBuffer(wid,
                buf, 72,0,
                640-72,30, 8,
                TYPE_BITMAP_SCREEN, 0); // type, terID

	// Create start button
	createButton(wid,
		0,0,72,29,
		0,
		"Start");

	// Render taskbar
	RenderTaskBar(buf, 640-72, 30, 0);
	// Send refresh request
	refreshWindow(wid);

	//------------------------------------------------------------
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
				break;
			}
		}

		// Render taskbar
		RenderTaskBar(buf, 640-72, 30, frame);
		// Send refresh request
		refreshWindow(wid);
		// Spare some CPU time
		sleep(4);
		SwitchToThread();
	}

	//
	free(buf);
}

// Taskbar application main function
int main(int arg, char **argv)
{
	int wid;

	// Get connected to graos (creates msgbox, etc.)
	connectGraos();

	// Create window
	wid = createWindow(0,480-30, 640,30, "", // taskbar has no caption
		TASKBAR_WINDOW | WINDOW_AUTOCLEAR);

	// Call main loop
	mainLoop(wid);

	// Exit
	return 0;
}

//


