#ifndef __INCLUDED_SCREEN_H__
#define __INCLUDED_SCREEN_H__

//
#include "buffer.h"

//
typedef struct
{
	long x1,y1,x2,y2,
		a1,a2,a3,a4;
}WINDOW;

//
typedef struct
{
	// Is this entry free?
	// Values: TRUE / FALSE
	int isFree;

	// Is this is a remote terminal screen?
	// Any value other than zero,
	// describes number of the COM port to
	// use as a source/output for the terminal screen.
	char isRemoteCOM;

	// CAPTURE OF THIS SCREEN'S I/O TO A BUFFER
	//
	struct
	{
		int length; // If zero, capture is inactive
		char *buf; // If NULL, no capture is being made
		int offs; // Can't be >=length
	}cap;

	//
	WINDOW window;
	char supallow,tc,updcur;
	long width,height,curx,cury;
	char *buf,*copy;

	// ANSI code handling variables
	char ac,HWCopy;
	char ansi[50];
	int ptr_ansi;

	// Keyboard buffer for the terminal
	FBUF key;

	// PID of the owner of this terminal
	int pid;

	// Screen ID.
	int id;
}SCREEN;

#endif


