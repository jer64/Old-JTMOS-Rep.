#ifndef __INCLUDED_WDB_H__
#define __INCLUDED_WDB_H__

//
#define DEF_BORDERWIDTH		1
#define N_MAX_WINDOWS		50

//
#include "window.h"

//
typedef struct
{
	// Window structures
	WINDOW w[N_MAX_WINDOWS];

	// Number of windows registered
	int n_windows;

	// Top Z-coordinate for a window
	int top_z;

	// Ordered windows list
	WINDOW *o[N_MAX_WINDOWS];
	int n_o;
}WDB;
extern WDB wdb;

// Window manipulation functions prototypes
void wdbInit(void);
int newWDB(void);
int emptyWDB(void);
int GetNWindows(void);
void *GetWindowPTR(int id);
int DeleteWindow(int id);
int guiCreateWindow(VMODE *v,
                int x,int y,
                        int width,int height,
                        const char *caption,
                        int type);
WINDOW *getWindowPTR(int wid);
WINDOW *getNextWindow(WINDOW *w);
WINDOW *getAnyWindow(int begID);
void SelectWindow(VMODE *v, WINDOW *W);
void autoGetWinPos(VMODE *v,
        int *x,int *y, int width,int height);

#endif


