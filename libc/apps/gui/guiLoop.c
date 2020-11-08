// =====================================================
// guiStart.c
// =====================================================
#include <stdio.h>
#include "guiLoop.h"
#include "guiUpdate.h"
#include "guiApp.h"
#include "windowMoving.h"
#include "jtmos/mouse.h"

//
void guiGetInput(VMODE *v)
{
	//
	v->lx = v->x;
	v->ly = v->y;
	v->x = GetMouseX();
	v->y = GetMouseY();
	v->buttons = GetMouseButtons();

	//
	v->alt = GetALT();
	v->ctrl = GetCTRL();
}

//
void guiLoop(VMODE *v)
{
	int key;

	//
	while(1)
	{
		// -------------------------------
		guiServerPoll(v);
		//
		guiGetInput(v);
		//
		UndoOldArrow(v);
		//
		automaticallyRefreshTerminalWindows();

		// -------------------------------
		windowMoving(v);
		// -------------------------------
		guiApplicationCall(v);
		// -------------------------------
		guiWindowSystemCall(v);
		// -------------------------------
		// TASKBAR now exists as an application
	//	guiTaskbarCall(v);
		// -------------------------------

		// Store new arrow
		StoreNewArrow(v);
		// Update screen
		guiUpdate(v);

		// Input user
		key = getch1();
		if(key==27 && v->ctrl)break;
		v->keyPressed = key;
		v->cycle++;
	}
}


