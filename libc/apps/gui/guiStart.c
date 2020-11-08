// =====================================================
// guiStart.c - GRAOS Windowing System
// (C) 2002-2003 by Jari Tuominen
// =====================================================
#include <stdio.h>
#include "graos.h"
#include "guiStart.h"

//
int gui_LoadSystemFont(VMODE *v)
{
	// Regular
	LoadFont("guifont.raw", &v->font[0], 0);
	// Inverted
	LoadFont("guifont.raw", &v->font[1], 1);
}

//
void guiVideoModeAlloc(VMODE *v, int width,int height, int bpp)
{
	// Video frame buffer
	v->buf =	malloc(width*height);
	// Temporary buffer for directBlit conversions
	v->tmp =	malloc(width*height);
	// Geometrics
	v->width =	width;
	v->height =	height;
	v->bpp =	bpp;
	v->l_buf =	width*height;
}

//
void guiPrecalculate(VMODE *v)
{
	int i;

	//
	for(i=0; i<v->height; i++)
	{
		//
		v->mtab[i] = i*v->width;
	}
}

// Return value:
// Zero =	No error
// Non-zero =	Error
int guiVideoMode(VMODE *v, int width,int height, int bpp)
{
	static char str[256];

	//
	sprintf(str, "%s: GUI video mode = %dx%d %d BPP\n",
			__FUNCTION__,
			width, height, bpp); DebugMessage(str);

	// 640x480 256c emulation with 640x480 16c VGA
	if(width==640 && height==480 && bpp==8)
	{
		setmode(0x12);
		guiVideoModeAlloc(v, 640,480,8);
		guiPrecalculate(v);
		gui_FastClearScreen();
		return 0;
	}

	// 640x480 monochrome
	if(width==640 && height==480 && bpp==1)
	{
		setmode(0x11);
		guiVideoModeAlloc(v, 640,480,1);
		guiPrecalculate(v);
		gui_FastClearScreen();
		return 0;
	}

	// Failed to setup the video mode?
	printf("guiVideoMode: Unknown video mode '%dx%d %d BPP'\n",
		width,height, bpp);
	return 1;
}

//
void InitVMODEStruct(VMODE *v)
{
	//
	memset(v, 0, sizeof(VMODE));
}

// Start GUI
int guiStart(void)
{
	int i;
	VMODE *v;

	// Open debug stream
	OpenDebugStream("graos.log");

	//---------------------------------------------------------------------
	// Init general
	v = malloc( sizeof(VMODE) );
	InitVMODEStruct(v);
	v->cycle = 0;
	// Init window database
	wdbInit();

	//---------------------------------------------------------------------
	if( (i=guiVideoMode(v, 640,480,8)) )	return NULL;
	gui_LoadSystemFont(v);
	LoadBin("mousecursor.raw",	v->cursorRaw, 32*32);
	if(v->bpp==8)
		LoadBin("taskbar8.raw",		v->taskbarRaw, 640*30);
		else
		LoadBin("taskbar.raw",		v->taskbarRaw, 640*30);
	v->change=1;

	//---------------------------------------------------------------------
	// Load background(if any)
	if(0)
	{
nullbk:
		printf("Background image = NULL\n");
		v->background = NULL;
		v->l_background = 0;
	}
	else
	{
		printf("Loading background image\n");
		v->l_background = 640*480;
		v->background = malloc(v->l_background);
		if(LoadBin("background.raw", v->background, 640*480))
			goto nullbk;
	}

	//---------------------------------------------------------------------
	// Clear screen after background image is available
	gui_ClearScreen(v);

	//---------------------------------------------------------------------
	// Start GUI server
	//
	guiServerInit();

	//---------------------------------------------------------------------
	// Start taskbar application
	//
	if( bgexec("taskbar", "/bin")==0 )
	{
		PopUpWindow(v, "Error",
		"Error while trying to run taskbar program.");
	}

	//---------------------------------------------------------------------
	// Pop up some demo windows
	//
//	PopUpWindow(v, "System installation",
//	"This is just a test version of the installer.\nClick window to close it.");


	//
	return v;
}




