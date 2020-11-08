// editEditor.c - editor main loop
#include <stdio.h>
#include <conio.h>
#include "edit.h"
#include "editEditor.h"

//
void OrderRefresh(void)
{
	//
	ed.view.upd=1;
}

//----------------------------------------------------
// Editor main loop
//
int Editor(const char *fname)
{
	int key,i,i2;
	char str[256];
	int viewhelp;

	//
	viewhelp = 1;

	// ---------------------------------
	// Clear screen for a start
	clrscr();

	// ---------------------------------
	// Initialize the editor
	EditorInit();

	// Load file
	EditorLoad(fname);
/**	{
		// Error:
		printf("Error: file not found '%s'\n",
			fname);
		return 1;
	}**/

	// Update contents at the beginning
	OrderRefresh();

	//========================================================
	// Editor main loop
	while(1)
	{
		// ---------------------------------
		// Update edit box contents
		textcolor(COLOR1);
		textbackground(COLOR1B);
		UpdateEditBox();

		//
		gotoxy(1,1);
		textcolor(COLOR2);	
		textbackground(COLOR2B);
		sprintf(str,
			" File: %s  (%d,%d)  %s  %d lines  Press F1 for manual. ",
			fname,
			ed.cursor.x-CURSOR_X_MIN,
			(ed.cursor.y-CURSOR_Y_MIN)+ed.view.y,
			ed.insertMode ? "Insert On " : "Insert Off",
			ed.lines);
		for(i=strlen(str); i<80; i++)
			strcat(str," ");
		printf("%s", str);

		//
		if(viewhelp==1)
		{
			//
			viewhelp = 2;
			textcolor(YELLOW);
			textbackground(RED);
			// For this line of text, thanks to http://www.cnr.colostate.edu/computing/help/pc/elm.html ...
			gotoxy(1,23);
			printf(" To exit the editor press ^x (press and hold the control key and then press x). ");
			gotoxy(1,22);
			printf(" ^o : Saves current work.                                                       ");
			gotoxy(1,21);
			printf(" Cursor keys / page up&down : Moves cursor location.                            ");
			gotoxy(1,19);
			printf(" Helpful hint: ^ before a letter usually means to                               ");
			gotoxy(1,20);
			printf(" press control and that letter together.                                        ");
		}
		if(viewhelp==2)
		{
			//
			viewhelp = 3;
			OrderRefresh();
		}

		// ---------------------------------
		// Update cursor location
		textcolor(COLOR1);
		textbackground(COLOR1B);
		gotoxy(ed.cursor.x, ed.cursor.y);

		// ---------------------------------
		// Get key
		key = getch();
		// CTRL/ALT states
		ed.ctrl = GetCTRL();
		ed.alt = GetALT();

		// ---------------------------------
		// Handle extra keys

		// Cursor up
		if(key==JKEY_CURSOR_UP)
			CursorUp();
		// Cursor left
		if(key==JKEY_CURSOR_LEFT)
			CursorLeft();

		// View some help.
		if(key==JKEY_F1)
		{
			viewhelp = 1;
		}

		// PAGE UP
		if(key==JKEY_PAGE_UP)
		{
			//
			ed.cursor.x=CURSOR_X_MIN;
			ed.cursor.y=CURSOR_Y_MIN;
			ViewUp(20);
		}

		// PAGE DOWN
		if(key==JKEY_PAGE_DOWN)
		{
			//
			ed.cursor.x=CURSOR_X_MIN;
			ed.cursor.y=CURSOR_Y_MIN;
			ViewDown(20);
		}

		// Cursor down
		if(key==JKEY_CURSOR_DOWN)
			CursorDown();

		// Cursor right
		if(key==JKEY_CURSOR_RIGHT)
			CursorRight();

		// End -key
		if(key==JKEY_ESC)
		{
			ed.cursor.x = strlen( ed.buf[GetWritePointY()] )
					+ CURSOR_X_MIN;
		}

		// Home -key
		if(key==JKEY_HOME)
		{
			ed.cursor.x = CURSOR_X_MIN;
		}

		// Insert -key
		if(key==JKEY_INSERT)
		{
			ed.insertMode = 1-ed.insertMode;
		}

		// ---------------------------------
		// Handle actions
		//

		// CTRL+o or F2: quits
		if( (ed.ctrl && key=='o') || key==JKEY_F2 )
		{
			//
			gotoxy(1,1);
			textcolor(COLOR2);	
			textbackground(COLOR2B);
			printf("                                                      ");
			gotoxy(1,1);
			printf(" Saving file (%s) ...", fname);
			EditorSave(fname);
			printf("\r File saved (%s).                   \r",
				fname);
			sleep(1);
		}

		// CTRL+x or F10: quits
		if( (ed.ctrl && key=='x') || key==JKEY_F10 )
		{
			//
			key=0;
			if(ed.modified)
			{
				//
				gotoxy(1,1);
				textcolor(COLOR2);	
				textbackground(COLOR2B);
				printf("                                                      ");
				gotoxy(1,1);
				while(getch1()) { SwitchToThread(); }
					printf("Save modified buffer (y/n) ? ");
				if(getch()=='y')
					EditorSave(fname);
			}
			break;
		}

		// ---------------------------------
		// Handle Backspace
		//
		if(key=='\b' || key==JKEY_BACKSPACE) // backspace or delete
		{
			key=0X1000; // processed
			printf(" ");
			CursorLeft();
		}

		// Handle Enter
		if(key==0x0A)
		{
			key=0x1000; // processed
			CursorEnter();
		}

		// ---------------------------------
		// Handle other(store character)
		//
		if((key>>8)==0)
		{
			printf("%c", key);
			StoreChar(key);
			CursorRight();
		}
	}

	// Exit main loop
	return 0;
}
