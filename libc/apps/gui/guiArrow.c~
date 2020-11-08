// ============================================
// guiArrow.c
// ============================================
#include <stdio.h>
#include <math.h>
#include "graos.h"
#include "guiApp.h"
#include "guiArrow.h"

void UndoOldArrow(VMODE *v)
{
	if(v->cursorBehindActive)
	{
		guiWriteArea(v, v->lx+0,v->ly+0, v->lx+32,v->ly+32, 
				v->cursorBehind);
	}
}

void StoreNewArrow(VMODE *v)
{
	// 1) Indicate that we have got slice of original area
	v->cursorBehindActive=1;
	// 2) Read original area
	guiReadArea(v, v->x+0,v->y+0, v->x+32,v->y+32, 
			v->cursorBehind);
	// 3) Write new arrow area on it
	guiWriteArea1(v, v->x+0,v->y+0, v->x+32,v->y+32, 
			v->cursorRaw);
}

