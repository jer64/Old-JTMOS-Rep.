#ifndef __INCLUDED_EDIT_H__
#define __INCLUDED_EDIT_H__

//----------------------------------------------------
// File
#define MAX_LINES	5000
#define MAX_PER_LINE	80
// Edit area
#define CURSOR_X_MIN	1
#define CURSOR_Y_MIN	2
#define CURSOR_X_MAX	80
#define CURSOR_Y_MAX	23

// Editbox text
#define COLOR1		7
#define COLOR1B		1
// Headline
#define COLOR2		0
#define COLOR2B		7

//
typedef struct
{
	// CTRL and ALT states
	int ctrl,alt;

	// Text file contents
	BYTE *buf[MAX_LINES];

	// How many lines on the file?
	int lines;

	// Insert(1) or replace(0) mode
	int insertMode;

	// File has been modified?
	int modified;

	// Coordinate for cursor
	struct
	{
		int x,y;
	}cursor;

	// Coordinate for view
	struct
	{
		int x,y;
		int upd; // update trigger
	}view;
}EDITOR;

//
extern EDITOR ed;

#endif

