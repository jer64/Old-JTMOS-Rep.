//-------------------------------------------------------------------------
// GRAOS PROTOCOL STRUCTURE AND TYPE DEFINITIONS
//-------------------------------------------------------------------------
#ifndef __INCLUDED_GRAOSPROTOCOL_H__
#define __INCLUDED_GRAOSPROTOCOL_H__


//-----------------------------------------------------------------
// Window types
//

// Window type options
#define WINDOW_CLOSE_BUTTON     1
#define EXIT_ON_CLICK           2
#define WINDOW_HAS_CAPTION	4
#define WINDOW_HAS_BORDER	8
#define WINDOW_AUTOCLEAR	16

//
#define STANDARD_BUTTON		0

// Standard window type
#define STANDARD_WINDOW		WINDOW_HAS_CAPTION \
				| WINDOW_CLOSE_BUTTON \
				| WINDOW_HAS_BORDER \
				| WINDOW_AUTOCLEAR

// Taskbar window (stand-alone)
#define TASKBAR_WINDOW		0

//-----------------------------------------------------------------
// Frame buffer type
//
#define TYPE_TERMINAL_EMULATION 1
#define TYPE_BITMAP_SCREEN      2

//-----------------------------------------------------------------
// GUI server responds with this command
//
#define GSID_RESPONSE		1
// Application sent "open window" -command
#define GSID_OPENWINDOW		2
// Application sent "close window" -command
#define GSID_CLOSEWINDOW	3
// User commanded program to exit
// (this message is sent to application,
//  graos closes the window before sending it.)
#define GSID_EXIT		4
// Add a bitmap image buffer region
// within window's client area
#define GSID_ADDFRAMEBUFFER	5
// Window contents have changed,
// therefore this command will be
// sent to the GUI server.
#define GSID_REQUESTREFRESH	6
// When key is pressed on the selected
// window, this message is sent to
// the application.
#define GSID_KEYPRESSED		7
// Button creation.
#define GSID_CREATEBUTTON	8

//-----------------------------------------------------------------
// GUICMD packet structure
//
typedef struct
{
	// 0xFCE2E37B
	DWORD magic;

	// Which window
	int wid;

	// Command ID
	int id;

	// Return value (used for answer packets)
	DWORD rval;

	// Misc. string (e.g. caption)
	char str[256];

	// Values
	int v[20];

	// Pointers
	BYTE *ptr[20];
}GUICMD;

#endif

