//=============================================================
// GRAOS CLIENT FUNCTIONS (Graos API).
// (C) 2003-2005 by Jari Tuominen.
// Gives access to the Graos GUI server.
//=============================================================
#include <stdio.h>
#include <msgbox.h>
#include <graosprotocol.h>
#include "graosClient.h"

//
static MSG m;
static GUICMD c;
static int graosPID=-1;

// Send window refresh request
int refreshWindow(int wid)
{
	GUICMD *cmd;
	int *d;

	// Get PID
	if(getGraosPID()<=0)
		return -1;

	// Get CMD PTR
	cmd = &m.buf;

	// Set CMD ID
	cmd->id = GSID_REQUESTREFRESH;

	// Set window ID
	cmd->v[0] = wid;

	// Get PID
	getGraosPID();
	// Send packet
	m.protocol = MB_PROTOCOL_GRAOS;
	sendMessage(&m, graosPID);

	// Get answer
	return waitGraosAnswer();
}

// Create button into a window
int createButton(int wid,
        int x1,int y1,int x2,int y2,
        int type,
        const char *caption)
{
	GUICMD *cmd;
	int *d;

	// Get PID
	if(getGraosPID()<=0)
		return -1;

	// Get CMD PTR
	cmd = &m.buf;

	// Set CMD ID
	cmd->id = GSID_CREATEBUTTON;

	// Get pointer to parameters(values)
	d = &cmd->v;
	// Set x,y
	*d++ = wid;	// 0
	*d++ = x1;	// 1
	*d++ = y1;	// 2
	*d++ = x2;	// 3
	*d++ = y2;	// 4
	*d++ = type;	// 5

	// Store caption
	strcpy(cmd->str, caption);

/**int createButton(int wid,
        int x1,int y1,int x2,int y2,
        int type,
        const char *caption)**/

	// Get PID
	getGraosPID();
	// Send packet
	m.protocol = MB_PROTOCOL_GRAOS;
	sendMessage(&m, graosPID);

	// Get answer
	return waitGraosAnswer();
}

// Add frame buffer to a window
int addFrameBuffer(int wid,
	BYTE *buf, int x, int y,
	int width, int height, int bpp,
	int type, int terID)
{
	GUICMD *cmd;
	int *d;

	// Get PID
	if(getGraosPID()<=0)
		return -1;

	// Get CMD PTR
	cmd = &m.buf;

	// Set CMD ID
	cmd->id = GSID_ADDFRAMEBUFFER;

	// Define PTR to the frame buffer
	cmd->ptr[0] = buf;

	// Get pointer to parameters(values)
	d = &cmd->v;
	// Set x,y
	*d++ = wid;	// 0
	*d++ = x;	// 1
	*d++ = y;	// 2
	*d++ = width;	// 3
	*d++ = height;	// 4
	*d++ = bpp;	// 5
	*d++ = type;	// 6
	*d++ = terID;	// 7

	// Get PID
	getGraosPID();
	// Send packet
	m.protocol = MB_PROTOCOL_GRAOS;
	sendMessage(&m, graosPID);

	// Get answer
	return waitGraosAnswer();
}

// This function is called before using any GUI functions
void connectGraos(void)
{
	// Check for existance
	graosCheck();

	// Create a msgbox for us,
	// this is needed so that
	// we can receive response
	// packets from the Graos server.
	startMessageBox();
}

// Check for existance of the Graos -process
void graosCheck(void)
{
	//
	if(getGraosPID()<=0)
	{
		//
		printf("This program requires Graos.\n");
		exit(1);
	}
}

// Get GRAOS server's PID
inline int getGraosPID(void)
{
	// Get Graos PID
	if(graosPID<=0)
		graosPID = GetThreadPID(GRAOS_SERVER_PID_NAME);

	// Return PID
	return graosPID;
}

// Wait until GRAOS sends an answer packet
// (with rval set to return value)
int waitGraosAnswer(void)
{
	GUICMD *cmd;

	// Wait until we get the answer
	while( receiveMessage(&m)==0 )
	{
		// Spare CPU time
		SwitchToThread();
	}

	// Get GUICMD packet
	cmd = &m.buf;

	// Return rval
	return cmd->rval;
}

// Close a window
int closeWindow(int wid)
{
	GUICMD *cmd;

	// Get PID
	if(getGraosPID()<=0)
		return -1;

	// Get CMD PTR
	cmd = &m.buf;

	// Set CMD ID
	cmd->id = GSID_CLOSEWINDOW;

	// Set WID
	cmd->v[0] = wid;

	// Get PID
	getGraosPID();
	// Send packet
	m.protocol = MB_PROTOCOL_GRAOS;
	sendMessage(&m, graosPID);

	// Get answer
	return waitGraosAnswer();
}

// Call GRAOS to create a new window
// Return value -1 means error,
// other values are valid window IDs.
int createWindow(int x,int y,
		int width,int height,
		const char *caption,
		int type)
{
	GUICMD *cmd;

	// Get PID
	if(getGraosPID()<=0)
		return -1;

	// Get GUICMD packet
	cmd = &m.buf;

	// Set CMD ID
	cmd->id = GSID_OPENWINDOW;

	// Set coordinate
	cmd->v[0] = x;
	cmd->v[1] = y;

	// Set size
	cmd->v[2] = width;
	cmd->v[3] = height;

	// Set type
	cmd->v[4] = type;

	// Store caption
	strcpy(cmd->str, caption);

	// Send packet
	m.protocol = MB_PROTOCOL_GRAOS;
	sendMessage(&m, graosPID);

	// Get answer
	return waitGraosAnswer();
}

//


