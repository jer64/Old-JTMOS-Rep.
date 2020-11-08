// ============================================
// guiApp.c
// Internal applications for GUI.
// (APPCALL -based)
// ============================================
#include <stdio.h>
#include <math.h>
#include "graos.h"
#include "guiApp.h"
#include "guiArrow.h"

int guiApp_virgin=1;
int wid,wid2, appCyc=0;

//
void app_move(VMODE *v)
{
	int i,i2,i3,i4;
	WINDOW *w;

	//
	for(i=0; i<64; i++)
	{
		// Movement (Make both windows follow
		// at mouse cursor's location)
		if( !((v->cycle>>3)&1) )
		{
		//	w = GetWindowPTR(wid);
		//	windowFollow(w, v->x, v->y);
		}

		// Movement II
		w = GetWindowPTR(wid2);
		windowFollow(w, v->x, v->y);
	}
}

//
void guiApplicationCall(VMODE *v)
{
	int x,y,mx,my, i,i2,i3,i4;
	char str[256];
	static char *prgs[]={
		"wintest0.bin",
		"wintest.bin",
		"wintest2.bin",
		"wintest3.bin",
		"wintest4.bin",
		"wintest5.bin",
		"wintest6.bin",
		"wintest7.bin",
		"wintest8.bin",
		"wintest9.bin",
		"wintest10.bin"
		};

	// Close window	
	// (ALT+F4)
	if(v->keyPressed==JKEY_F4
		&& v->alt)
	{
		// If currently selected window is not NULL
		if(v->selwin!=NULL)
		{
			//
			guiCloseWindow(v, v->selwin);
			v->selwin=NULL;
		}
	}

	// Allow switching to new window
	// (CTRL+TAB)
	if(v->keyPressed=='\t'
		&& v->alt)
	{
		//
		DebugMessage("********* switch to new window **********\n");
		// Switch to next window
		switchToNextWindow(v);
	}

	// Run test program 1
	if(v->keyPressed=='s'
		&& v->ctrl)
	{
		// Run test program
		if( bgexec("winshell sqsh", "/bin")==0 )
		{
			// Pop error window
			PopUpWindow(v, "cannot run winshell",
				"winshell binary corrupted or not found?");
		}
		else
		{
			// Pop success window
		//	PopUpWindow(v, "success",
		//		"winshell was launched successfully");
		}
	}
	else
	// Run test program 1
	if(v->keyPressed>='a' && v->keyPressed<='j'
		&& v->ctrl)
	{
		// Run test program
		if( bgexec(prgs[v->keyPressed-'a'], "/bin")==0 )
		{
			// Pop error window
			PopUpWindow(v, "run error",
				"cannot run wintestX.bin, file probably not found");
		}
	}

/*	//
	if(guiApp_virgin || (v->buttons&2 && (v->cycle&7)==0))
	{
		// Creation
		guiApp_virgin=0;
		//
		sprintf(str, "Window %d", GetSeconds());
		wid = guiCreateWindow(32,32, 160,120,
				str, STANDARD_WINDOW);
	}
	else
	{
	//	if( !(v->cycle&7) && v->buttons&1 )app_move(v);
	}*/

	//
	if((v->cycle&7)==0 && XON_SYSTEM_METERS)
	{
		// Report different meters
		sprintf(str, "x,y: %d,%d  tick: %d  cycle: %d  n_windows: %d  buttons: 0x%x\nlx,ly: %d,%d  ulines: %d  seconds: %d  underwin: %x  selwin: %x\nmovwin: %x  moving: %d",
			v->x,v->y, GetTickCount(), v->cycle,
			wdb.n_windows, v->buttons,
			v->lx,v->ly,
			v->ulines, GetSeconds(),
			v->underwin, v->selwin, v->movwin, v->movingWindow);
		DrawText(v,str, 0,0, NON_TRANSPARENT, 0x07);
	}
}

//
void guiApplicationCall3(VMODE *v)
{
	int x,y,mx,my;

	//
	my = v->cycle;

	//
	for(y=0; y<v->height; y+=2)
	{
		for(x=v->width>>2; x<v->width; x+=2)
		{
			DrawDot( v,  x,y,  (x>>2)&((y>>2) + (my>>3)) );
		}
	}
}

void guiApplicationCall2(VMODE *v)
{
	int x,y,mx,my,text_my;
	float fa;

	/************ CALCULATE MOVEMENT **************************/
	fa=GetTickCount(); fa=fa/100;
//	fa=v->cycle; fa=fa/10;
	my = (v->height>>1) + (((v->height>>1)-40)*sin(fa));
	text_my = (v->height>>1) + (((v->height>>2)-20)*sin(fa));

	/************ START DRAWING *******************************/
	gui_ClearScreen(v);

	/************ SOME PIXELS *********************************/
	for(y=0; y<v->height; y+=2)
	{
		for(x=v->width>>2; x<v->width; x+=2)
		{
			DrawDot(v,  x,y,  (x>>2)&((y>>2) + (my>>3)) );
		}
	}

	/************ MOVING TEXT *********************************/
	DrawText(v, "welcome to graos windowing system", 0,text_my,
		NON_TRANSPARENT, 0x07);
}

