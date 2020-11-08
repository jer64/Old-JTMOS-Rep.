//==================================================================
// plasma2.c
// Visual demonstration program.
// (C) 2003 by Jari Tuominen
//==================================================================
#include <stdio.h>

// Sinus table
static BYTE *sinus;
// Video buffer
BYTE *buf;
int l_buf;
// Current video frame #
int frame;

// Initialize program
void plasmaInit(void)
{
	int i,i2,i3,i4;

	// Generate a "sinus table"
	sinus = malloc(65536);
	for(i=0; i<65536; i++)
	{
		sinus[i] = (i*i)>>8;
	}

	// Allocate frame buffer
	l_buf = 64000;
	buf = malloc(l_buf);

	// Setup 320x200 video mode
	setvmode(0x13);

	// Set palette
	// Front fade
	for(i=0; i<128; i++)
	{
		setpalette(i,  i>>1, i>>1, i>>1);
	}
	// Back fade
	for(i=0; i<128; i++)
	{
		setpalette(i+128,  127-i>>1, 127-i>>1, 127-i>>1);
	}
}

//
void plasmaRender(void)
{
	int x,y,ad,i,i2,i3,i4;

	//
	for(y=0; y<200; y++)
	{
		//
		ad = y*320;

		//
		for(x=0; x<320; x++)
		{
			//
			buf[ad+x] = 
				sinus[ sinus[x+frame & 0xFFFF]+sinus[y+frame & 0xFFFF] + frame & 0xFFFF ];
		}
	}
}

//
void plasmaMain(void)
{
	int key;

	//
	for(frame=0; ; frame++)
	{
		// Render frame
		plasmaRender();

		// View frame
		drawframe(buf, 0, 64000);

		// Input user
		key = getch1();
		if(key==27)break;

		// Increase frame counter & schedule
		frame++;
		SwitchToThread();
	}
}

//
void plasmaEnd(void)
{
	// Restore video mode
	setvmode(0x03);

	// Greet user
	printf("Thank you for watching Plasma Demo II\n");
	printf("We'd like to have your suggestions about\n");
	printf("our software, please these to jari@vunet.org\n");
	printf("we respect your mail alot, as far as it is positive/progressive ;-)\n");
}

//
int main(int argc, char **argv)
{
	int key;

	// Walk past
	plasmaInit();
	plasmaMain();
	plasmaEnd();

	// Return to shell
	return 0;
}

