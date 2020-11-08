// guiServer.c
#include <stdio.h>
#include "graos.h"
#include "guiServer.h"

// Initialize GRAOS server communication system
void guiServerInit(void)
{
	// Correctly identify itself as Graos server
	IdentifyThread(GetCurrentThread(), GRAOS_SERVER_PID_NAME);

	// Enable our message box
	startMessageBox();
}

// Process a single client message
int guiServerProcessMessage(VMODE *v, MSG *m)
{
	GUICMD *cmd,*newcmd;
	static MSG newMessage;
	int wid;
	static char str[256];
	WINDOW *w;
	int rval,rwid;

	// Get CMD PTR
	cmd = &m->buf;

	// Report
	sprintf(str, "%s: packet received from PID%d\n",
		__FUNCTION__, m->sndPID); DebugMessage(str);

	// Set default return value
	rval = 0;

	// Process CMD ID
	switch(cmd->id)
	{
		//--------------------------------------------------
		// Create new frame buffer
		case GSID_ADDFRAMEBUFFER:

		//
		sprintf(str, "%s: I received ADD FRAME BUFFER -request\nwid=%d, buf=%x, x=%d,y=%d, width=%d,height=%d, bpp=%d\n",
			__FUNCTION__,
			cmd->v[0], cmd->ptr[0], cmd->v[1], cmd->v[2],
			cmd->v[3], cmd->v[4], cmd->v[5]);
		DebugMessage(str);

		//
		guiAddFrameBuffer(GetWindowPTR(cmd->v[0]),	// wid
			cmd->ptr[0], cmd->v[1], cmd->v[2],	// buf, x,y,
			cmd->v[3], cmd->v[4], cmd->v[5],	// width, height, bpp
			cmd->v[6], 				// type
			cmd->v[7]);				// terminal ID
		break;

		//--------------------------------------------------
		// Create window
		case GSID_OPENWINDOW:
		sprintf(str, "%s: I received OPEN WINDOW -request\n",
			__FUNCTION__); DebugMessage(str);
		wid = guiCreateWindow(v,
				cmd->v[0],cmd->v[1], 
				cmd->v[2],cmd->v[3],
			cmd->str,
			cmd->v[4]); // type
		// Define rval
		rval = wid;
		// Define owner PID
		w = getWindowPTR(wid);
		w->pid = m->sndPID;
		rwid = wid;
		break;

		//--------------------------------------------------
		// Close window
		case GSID_CLOSEWINDOW:
		// Report
		sprintf(str, "%s: I received CLOSE WINDOW -request\nWID to close = %d\n",
			__FUNCTION__, cmd->v[0]);
		DebugMessage(str);
		// Close window
		guiCloseWindow(v, &wdb.w[ cmd->v[0] ]);
		sprintf(str, "window's isfree variable after closing = %d\n",
			wdb.w[cmd->v[0]].isfree);
		DebugMessage(str);
		rwid = cmd->v[0];
		break;

		//--------------------------------------------------
		// Refresh window
		case GSID_REQUESTREFRESH:
		// Refresh window
		wdb.w[cmd->v[0]].refresh |= 1|2; // window + components
		rwid = cmd->v[0];
		break;

		//--------------------------------------------------
		// Create a button
		case GSID_CREATEBUTTON:
		guiCreateButton(&wdb.w[cmd->v[0]],
			cmd->v[1],cmd->v[2], cmd->v[3],cmd->v[4],
			cmd->v[5], // type
			cmd->str);
		/* BUTTON *guiCreateButton(WINDOW *w,
		        int x1,int y1,int x2,int y2,
		        int type,
		        const char *caption) */
		break;
	
		//--------------------------------------------------
		// Unknown cmd, request will be ignored,
		// but still an empty answer will be sent,
		// to prevent sender from jamming up.
		default:
		sprintf(str, "%s: I received unknown request(ID=%d), request ignored.\n",
			__FUNCTION__, cmd->id); DebugMessage(str);
		return 2;
	}

	// Send a response packet
	memset(&newMessage, 0, sizeof(MSG));
	newcmd = &newMessage.buf;
	newcmd->id = GSID_RESPONSE;
	newcmd->rval = rval;
	newcmd->wid = rwid;	// which window's message
	sprintf(str, "%s: sending response of open window to PID%d\n",
		__FUNCTION__, m->sndPID); DebugMessage(str);
	sendMessage(&newMessage, m->sndPID);

	//
	return 0;
}

//
void transmitInterfaceToApp(VMODE *v, WINDOW *w)
{
	GUICMD *cmd,*newcmd;
	static MSG newMessage;
	static MSG m;

	//--------------------------------------------------
	// Get CMD PTR
	cmd = &m.buf;

	//--------------------------------------------------
	// When key is hit
	// New: ignoring all key hits when CTRL or ALT is hit,
	// this makes it possible to system keys work properly.
	if(v->keyPressed>0 && !v->ctrl && !v->alt)
	{
		// Send keyboard output to the window
		cmd->id = GSID_KEYPRESSED;
		cmd->v[0] = v->keyPressed;
		sendMessage(&m, w->pid);
	}
}

// Polling function called by graos loop
void guiServerPoll(VMODE *v)
{
	static MSG m;

	// Send all proper messages to the application
	if(v->selwin)
		transmitInterfaceToApp(v, v->selwin);

	// Receive new message
	if( receiveMessage(&m) )
	{
		// Message received, process it:
		guiServerProcessMessage(v, &m);
	}
}

//


