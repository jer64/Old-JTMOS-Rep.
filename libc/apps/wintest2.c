// wintest2.c - the bitmap test
#include <stdio.h>
#include <conio.h>
#include <jtmos/graos.h>

// Request 256K of RAM from the init process.
SYSMEMREQ(APPRAM_256K)

//
void render(BYTE *buf, int width,int height,
	int frame)
{
	int x,y,ad,c,c2,c3,c4;
	BYTE pal[16]={
		0,8,7,14, 15,14,7,8,
		0,BLUE,GREEN,14, 15,14,GREEN,BLUE
		};

	//
	for(y=0; y<height; y++)
	{
		//
		ad = width*y;
	
		//
		for(x=0; x<width; x++)
		{
			//
			c = ((x+(frame<<4))&(y+(frame<<2)));
			if(c) c=4; else c=0;
			buf[ad+x] = pal[c+frame+(x*(x>>8)+y >> 8) & 15];
		}
	}
}

//
void mainLoop(int wid)
{
	static MSG m;
	static GUICMD *cmd;
	static BYTE *buf;
	static int frame;

	// Get GUICMD PTR
	cmd = &m.buf;

	//
	buf = malloc(320*240);
	//
	addFrameBuffer(wid,
		buf, 0,0,
		320,240, 8,
		TYPE_BITMAP_SCREEN, 0);	// type, terID
		// TYPE_TERMINAL_EMULATION

	// Loop until get exit command
	for(frame=0; ; frame++)
	{
		//
		render(buf, 320,240, frame);

		// Get new message
		if( receiveMessage(&m) )
		{
			// Got new message
			if(cmd->id==GSID_EXIT)
				// We've been ordered to exit
				break;
		}

		// Send refresh request
		refreshWindow(wid);
		// Spare some CPU time
		sleep(1);
	//	SwitchToThread();
	}

	//
	free(buf);
}

// Test window system
int main(int arg, char **argv)
{
	int wid;

	// Get connected to graos (creates msgbox, etc.)
	connectGraos();

	// Create window
	wid = createWindow(32,32, 400,300, "wintest2 (with frame buffer)",
		STANDARD_WINDOW);

	// Call main loop
	mainLoop(wid);

	// Exit
	return 0;
}

//


