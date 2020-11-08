#ifndef __INCLUDED_WINDOWFUNC_H__
#define __INCLUDED_WINDOWFUNC_H__

// for getWindowUnderCursorEx:
#define AREA_CAPTION	1
#define AREA_WINDOW	0

//
#include <stdio.h>
#include "graos.h"
#include "window.h"

//
void WindowClickQuit(int wid);
void SetQuickMessage(int wid, const char *msg);
int PopUpWindow(VMODE *v, const char *caption, const char *msg);
void HideDrawWindow(VMODE *v, WINDOW *w);
void guiCloseWindow(VMODE *v, WINDOW *w);
void onButtonClick(VMODE *v, WINDOW *w, BUTTON *b);
void PostQuitToWindowOwner(WINDOW *w);

//
int pointInsideRect(int x,int y,
                int x1,int y1,int x2,int y2);
int rectInsideRect(int x1,int y1,int x2,int y2,
                int xx1,int yy1,int xx2,int yy2);
WINDOW *getWindowUnderCursor(VMODE *v);
WINDOW *getWindowUnderCursorEx(VMODE *v, int headline);
BUTTON *guiCreateButton(WINDOW *w,
        int x1,int y1,int x2,int y2,
        int type,
        const char *caption);
BUTTON *getNewButton(WINDOW *w);
void unpressAllButtons(VMODE *v);
void unpressAllButtons1(WINDOW *w);

#endif
