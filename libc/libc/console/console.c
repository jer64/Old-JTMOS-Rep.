// =========================================================
// LIBC Console Functions
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
//
// Includes functions like f.e. puts.
// =========================================================
#define gets_bufmax 250

//
#include <stdio.h>

//
void ESCLOCK(void)
{
	//
	while(getch1()==27) { SwitchToThread(); }
	while(getch1()!=27) { SwitchToThread(); }
}

//
void gettextinfo(struct text_info *_r)
{
	// Screen rectangle
	_r->winleft =	1;
	_r->wintop =	1;
	_r->winright =	80;
	_r->winbottom =	25;

	// Geometrics
	_r->screenwidth =	80;
	_r->screenheight =	25;

	// Misc. things
	_r->currmode =	0;
	_r->attribute =	0;
	_r->normattr =	0;

	// Coordinates
	_r->curx =	0;
	_r->cury =	0;
}

//
void textcolor(int col)
{
	//
	scall(SYS_textcolor, col,0,0,0,0,0);
}

//
void textbackground(int col)
{
	//
	scall(SYS_textbackground, col,0,0,0,0,0);
}

//
int wherex(void)
{
	//
	return scall(SYS_WHEREX, 0,0,0,0,0,0);
}

//
int wherey(void)
{
	//
	return scall(SYS_WHEREY, 0,0,0,0,0,0);
}

// Delay specified amount of milliseconds.
void delay(unsigned msec)
{
	int i,t;

	//
	for(t=GetTickCount(); GetTickCount()<(t+msec); )
		SwitchToThread();
}

//
void clrscr(void)
{
	//
	scall(SYS_CLRSCR, 0,0,0,0,0,0);
}

// Get a key from keyboard (returns -1 on no key).
int getch1(void)
{
	int key;

	//
	SwitchToThread();
	key = scall(SYS_GETCH, &key,0,0,0,0,0);
	return key;
}

// Get a key from keyboard (wait until key is pressed).
// (Fixed to make use of wgetch function).
int getch(void)
{
	DWORD key;
	int i,i2;

	//
	return scall(SYS_wgetch, 0,0,0, 0,0,0);
}

// For compatibility.
int putch(int _mark1)
{
	//
	return putchar(_mark1);
}

// Outputs a character on the screen.
int putchar(int _mark1)
{
	//
	scall(SYS_PUTCH, _mark1,0,0,0,0,0);

	//
	return 0;
}

// Locate cursor.
void gotoxy(int x,int y)
{
	//
	scall(SYS_GOTOXY, x,y,0,0,0,0);

	//
}

// Prints a string on current terminal.
int puts(const char *s)
{
	//
	return scall(SYS_PUTS, s,0,0, 0,0,0);
}

// Get string from stdin.
char *gets(char *s)
{
	//
	int i,i2,i3,i4;
	int key;
	char str[256],tmp[20];

	//
	strcpy(s, "");

	//
	for(i=0; ; )
	{
		//
		key=getch();

		// ESC
		if(key==27)
		{
			printf("[ESC]");
		}
		// ENTER
		if(key=='\n')
		{
			putch(key);
			break;
		}
		// BACKSPACE
		if(key=='\b')
		{
			//
			if(i)
			{
				putch(key);
				i--;
				if(s[i]==27)
				{
					printf("\b\b\b\b");
				}
				s[i]=0;
			}
			continue;
		}
		// OTHER KEYS
		if( i < (gets_bufmax-1) )
		{
			if(key!=27)putch(key);
			s[i]=key; s[i+1]=0;
			i++;
			continue;
		}
	}

	//
	return s;
}

// Get CTRL key status.
int GetCTRL(void)
{
	//
	return scall(SYS_getctrl, 0,0,0,0,0,0);
}

// Get ALT key status.
int GetALT(void)
{
	//
	return scall(SYS_getalt, 0,0,0,0,0,0);
}

//

