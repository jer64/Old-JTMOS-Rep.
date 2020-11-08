// (C) 2002 by Jari Tuominen(jari@vunet.org)
// ====================================================
#include "kernel32.h"
#include "ega_ansi.h"

//
void EraseScrFromUPToY(SCREEN *s, int ty)
{
	int x,y, ox,oy;

	ox=swherex(s); oy=swherey(s);

	for(y=1; y<(ty+1); y++)
	{
		sgotoxy(s,  1, y);
		for(x=0; x<80; x++) { sputch(s, ' '); }
	}

	sgotoxy(s,  ox, oy);
}

//
void EraseScrFromDOWNToY(SCREEN *s, int yy)
{
	int x,y, ox,oy;

	ox=swherex(s); oy=swherey(s);

	for(y=25; y>=yy; y--)
	{
		sgotoxy(s,  1, y);
		for(x=0; x<80; x++) { sputch(s, ' '); }
	}

	sgotoxy(s,  ox, oy);
}

// There is a bug in this function
void EraseToEndOfLine(SCREEN *s)
{
	int i,i2,i3,i4,x;

	for(x=swherex(s); x<(s->window.x2+1); x++)
	{
		sputch(s, ' ');
	}
}

void HandleAnsiCode(SCREEN *s)
{
	int cmd,cmd2;

	cmd =	s->ansi[strlen(s->ansi)-1];
	cmd2 =	s->ansi[strlen(s->ansi)-2];

	switch(cmd)
	{
		// Cursor home
		case 'H':
		switch(cmd2)
		{
			case '1':
			EraseToEndOfLine(s);
			break;

			case '2':
			break;

			default:
			break;
		}
		sgotoxy(s,  1, 1);
		break;

		// Move cursor up
		case 'A':
		sgotoxy(s,  1, wherey()-1);
		break;

		// Move cursor down
		case 'B':
		sgotoxy(s,  1, wherey()+1);
		break;

		// Move cursor forward
		case 'C':
		sgotoxy(s,  wherex()+1, 1);
		break;

		// Move cursor backward
		case 'D':
		sgotoxy(s,  wherex()-1, 1);
		break;

		//
		case 'J':
		// 1J = Erase from start of scr to current pos
		if(cmd2=='1')
		{
			EraseScrFromUPToY(s, swherey(s));
			break;
		}

		// 2J = Erase wholescreen
		if(cmd2=='2')
		{
			sclrscr(s);
			break;
		}

		EraseScrFromDOWNToY(s, swherey(s));
		break;

		// No handling
		default:
		break;
	}
}
