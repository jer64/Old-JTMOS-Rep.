//----------------------------------------------------
// MS-DOS editor/PICO clone for JTMOS
// (C) 2003 by Jari Tuominen
//
// CTRL+x	exit program
//		(prompts for save if needed)
// CTRL+o	write file
//
//----------------------------------------------------
#include <stdio.h>
#include <conio.h>
#include "edit.h"
#include "editEditor.h"

#ifdef JTMOS
// Request 1024K of RAM from the init process.
SYSMEMREQ(APPRAM_1024K)
#endif

//
EDITOR ed;

//
void AllocateLine(int y)
{
/*	if(y<MAX_LINES)
	{
		if(ed.lines<(y+1))
			ed.lines = y+1;
		if(ed.buf[y]==NULL)
			ed.buf[y] = malloc(100);
		EmptyLine(ed.buf[0]);
	}*/
}

//----------------------------------------------------
// VIEW MOVEMENT FUNCTIONS
//
void ViewUp(int much)
{
	if( (ed.view.y-much)>=0 )
	{
		ed.view.y-=much;
		ed.view.upd=1;
	}
	else
	{
		ed.view.y=0;
		ed.view.upd=1;
	}
}

//
void ViewDown(int much)
{
	if(ed.view.y<MAX_LINES)
	{
		ed.view.y+=much;
		ed.view.upd=1;
	}
}

//----------------------------------------------------
// CURSOR MOVEMENT FUNCTIONS
//
void CursorEnter(void)
{
	ed.cursor.x=CURSOR_X_MIN;
	CursorDown();
}

//
void CursorUp(void)
{
	if(ed.cursor.y<=CURSOR_Y_MIN)
	{
		ed.cursor.y+=8;
		ViewUp(8);
	}
		else
		ed.cursor.y--;
}

//
void CursorDown(void)
{
	if(ed.cursor.y<CURSOR_Y_MAX)
		ed.cursor.y++;
		else
	{
		ed.cursor.y-=8;
		ViewDown(8);
	}
}

//
void CursorLeft(void)
{
	if(ed.cursor.x>CURSOR_X_MIN)
		ed.cursor.x--;
		else
		{
			ed.cursor.x=CURSOR_X_MAX;
			CursorUp();
		}
}

//
void CursorRight(void)
{
	if(ed.cursor.x<CURSOR_X_MAX)
		ed.cursor.x++;
		else
		CursorEnter();
}

// Affect count of lines
void AffectLines(int x, int y)
{
	if((y+1)>ed.lines)
		ed.lines = y+1;
}

// Get Y-coordinate where cursor is currently pointing at
// (in text file space)
int GetWritePointY(void)
{
	int y;
	y = (ed.cursor.y-CURSOR_Y_MIN);
	// II
	y += ed.view.y;
	return y;
}

// Get X-coordinate where cursor is currently pointing at
// (in text file space)
int GetWritePointX(void)
{
	int x;
	// I
	x = (ed.cursor.x-CURSOR_X_MIN);
	return x;
}

//
void StoreChar(int ch)
{
	int i,i2,x,y,l;

	// Get write point
	x = GetWritePointX();
	y = GetWritePointY();

	//
	AffectLines(x, y);

	//
	if(ed.buf[y]==NULL)
		AllocateLine(y);

	//
	if(strlen(ed.buf[y])<=x)
	{
		for(i=strlen(ed.buf[y]); i<x; i++)
			ed.buf[y][i] = 32;
		ed.buf[y][x] = ch;
		ed.buf[y][x+1] = 0;
	}
	else
		//
		ed.buf[y][x] = ch;

	// Changes have been made
	ed.modified=1;
}

//----------------------------------------------------
void UpdateEditBox(void)
{
	int x,y,i,i2,xx,yy;
	BYTE *ptr;
	char empt[100];

	// Update if necessary
	if(ed.view.upd)
	{
		//
		ed.view.upd=0;

		//
	//	clrscr();

		//
		EmptyLine(empt);

		//
		for(y=CURSOR_Y_MIN,yy=0;
			y<(CURSOR_Y_MAX+1) && (yy+ed.view.y)<MAX_LINES; y++,yy++)
		{
			//
			gotoxy(CURSOR_X_MIN,y);
			printf("                                                                                ");
			gotoxy(CURSOR_X_MIN,y);
	
			//
			ptr = ed.buf[yy+ed.view.y];

			// If got content
			if(ptr!=NULL)
			{
				// Print contents
				printf("%s", ptr);
			}
			else
				// Print empty line
				printf("%s", empt);
		}
	}
}

//----------------------------------------------------
// Editor on close
//
void EditorOnClose(void)
{
	textcolor(7);
	textbackground(BLACK);
	clrscr();
}

//
void EmptyLine(char *s)
{
	int i,l;

	//
	strcpy(s,"");
}

//----------------------------------------------------
// Editor init
//
void EditorInit(void)
{
	int i,i2,offs;
	BYTE *big;

	// -----------------------------------
	// Init structure
	memset(&ed, 0, sizeof(ed));

	// -----------------------------------
	big = malloc(MAX_LINES*MAX_PER_LINE);
	for(i=0,offs=0; i<MAX_LINES; i++,offs+=MAX_PER_LINE)
	{
		ed.buf[i] = big+offs;
		strcpy(ed.buf[i],"");
	}

	// Init cursor location
	ed.cursor.x = CURSOR_X_MIN;
	ed.cursor.y = CURSOR_Y_MIN;

	// Default: set insert mode
	ed.insertMode=1;
}

//----------------------------------------------------
// Editor save file
//
int EditorSave(const char *fname)
{
	int i,i2,line,x,y,
		m1,m2,m3,m4;
	char str[256];
	FILE *f;

	//
	ed.modified=0;

	//
	f=fopen(fname, "wb");
	if(f==NULL)
		return 1;
	for(y=0; y<ed.lines; y++)
	{
		fprintf(f, "%s\n", ed.buf[y]);
	}
	fclose(f);
	return 0;
}

//----------------------------------------------------
// Editor load file
//
int EditorLoad(const char *fname)
{
	int i,i2,line,
		m1,m2,m3,m4;
	char str[256];
	FILE *f;

	//
	f=fopen(fname, "rb");
	if(f==NULL)
		return 1;

	// Reset line count
	ed.lines=0;

	//
	for(line=0; line<MAX_LINES; line++)
	{
		//
		strcpy(ed.buf[line], "");

		//
		for(i=0; ; )
		{
			//
			if(i>=(MAX_PER_LINE-1))
			{
				break;
			}

			// Get char
			m1 = fgetc(f);

			// Parse
			if(m1==EOF)
				goto eof;
			else
			if(m1==0)
				goto eof;
			else
			if(m1==0x0A || m1==0x0D)
			{
				break;
			}
			else
			if(m1=='\b')
			{
				// Expand to 8 chars
				for(i2=0; i2<8 && i<(MAX_PER_LINE-1); i2++)
					ed.buf[line][i++]=' ';
			}
			else
				// Character store
				ed.buf[line][i++]=m1;
		}

		// Mark end of line
		ed.buf[line][i]=0;
	}
eof:
	// Set line count
	AffectLines(0, line);

	//
	fclose(f);

	//
	return 0;
}

//----------------------------------------------------
// Program start up
//
int main(int argc, char **argv)
{
	char fname[256];

	// Determine file name
	if(argc<2)
		strcpy(fname, "Untitled");
		else
		strcpy(fname, argv[1]);

	// Launch editor
	if(!Editor(fname))
	{
		// Close editor
		EditorOnClose();
	}

	// Exit program.
	return 0;
}

