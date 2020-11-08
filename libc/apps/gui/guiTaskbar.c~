// ============================================
// guiTaskbar.c - Taskbar Renderation Program
// (C) 2002-2003 by Jari Tuominen
// ============================================
#include <stdio.h>
#include <math.h>
#include <time.h> // DATE -structure & getdate -function
#include "graos.h"
#include "guiTaskbar.h"
#include "guiArrow.h"

//
int taskbarVirgin=1;

//
struct
{
	int update;
	int lt;
}taskbar;

//
void guiTaskbarCall(VMODE *v)
{
	int x,y,mx,my;
	char str[256];
	DATE d;

	// Init on first run
	if(taskbarVirgin)
	{
		//
		taskbar.update =	1;
		taskbarVirgin =		0;
		taskbar.lt =		GetSeconds();
	}

	// Trigger taskbar refresh
	if( (GetSeconds()-taskbar.lt)>=4 )
	{
		//
		taskbar.lt =		GetSeconds();
		taskbar.update = 	1;
	}

	// If refresh requested render taskbar
	if(taskbar.update)
	{
		//
		taskbar.update = 0;

		// Get date
		getdate(&d);

		// Draw taskbar
		guiWriteArea(v, 0,v->height-30, 640,v->height,
			v->taskbarRaw);

		// Render clock
	        sprintf(str, "%d:%d:%d",
			d.hour, d.minute, d.second);
		// Draw clock -string with inverted font
	        DrawTextEx(v, &v->font[0],
			str,
			v->width - (strlen(str)+1)*8, v->height-8-10,
			TRANSPARENT,
			0x07);
	}
}

//


