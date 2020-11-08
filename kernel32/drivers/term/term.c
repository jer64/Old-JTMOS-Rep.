//------------------------------------------------------------
// term.c - TERMINAL EMULATION / VIRTUAL TERMINAL CODE
//
// (C) 2000-2007-2020 by Jari Tuominen(jari.t.tuominen@gmail.com).
//
// Note: Terminal code is featured in, f.e. putch.
// The code will be seperated to own terminal.c
// or something similiar later on.
//
// Terminal capture might not be reliable enough - TODO?
//-----------------------------------------------------------
#include "kernel32.h"
#include "ega.h"
#include "term.h"
#include "sysglvar.h"
#include "macrodef.h"
#include "cpu.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "printf.h"
#include "string.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h"
#include "terminal.h"
#include "ega_ansi.h" // ansi code support

//
#define IF_VISIBLE	if( thread_terminal[RPID(GetCurrentThread())]==visible_terminal )

// Dump all putcs to COM1.
char dump_all_to_com1=FALSE;
// 0=no terminal lock
// other: only the specified PID can access
// terminal. Used by f.e. shell.
int terminalLockPID=0;
int puts_busy=0;
// Determines the number of terminal
// that is visible at the moment.
// Valid values are 0-3.
int visible_terminal=0;
int saved_terminal=-1;
//
SCREEN screens[N_MAX_SCREENS];

//
void PanicOnNULL(void *ptr_allocated_memory, const char *msg_txt)
{
	//
	printk("Halting System: Internal System Error.\n");
	printk("Debug Message: \"%s\"\n", msg_txt);
	printk("Returned Malloc PTR Address Is 0x%x.\n", ptr_allocated_memory);
}

// Virtual terminal system initialization.
void VirtualTerminalInit(void)
{
	int i;

	// Initialize screens structure
	memset(&screens, 0, sizeof(screens));

	// Init screens
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_1]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_2]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_3]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_4]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_5]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_6]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_7]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_8]);
	InitScreen(&screens[ID_VIRTUAL_TERMINAL_9]);

	// Redefine screen EGA HW dependent locations(copy)
	screens[ID_VIRTUAL_TERMINAL_1].copy = (DWORD)0xB8000;
	screens[ID_VIRTUAL_TERMINAL_2].copy = (DWORD)0xB9000;
	screens[ID_VIRTUAL_TERMINAL_3].copy = (DWORD)0xBA000;
	screens[ID_VIRTUAL_TERMINAL_4].copy = (DWORD)0xBB000;
	screens[ID_VIRTUAL_TERMINAL_5].copy = (DWORD)0xBC000;
	screens[ID_VIRTUAL_TERMINAL_6].copy = (DWORD)0xBD000;
	screens[ID_VIRTUAL_TERMINAL_7].copy = (DWORD)0xBE000;
	screens[ID_VIRTUAL_TERMINAL_8].copy = (DWORD)0xBF000;
	screens[ID_VIRTUAL_TERMINAL_9].copy = (DWORD)0xC0000; // obsolete

	// Define the real buffers(buf)
	screens[ID_VIRTUAL_TERMINAL_1].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_1].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 1 Buffer Is NULL.");
	screens[ID_VIRTUAL_TERMINAL_2].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_2].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 2 Buffer Is NULL");
	screens[ID_VIRTUAL_TERMINAL_3].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_3].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 3 Buffer Is NULL");
	screens[ID_VIRTUAL_TERMINAL_4].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_4].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 4 Buffer Is NULL");
	screens[ID_VIRTUAL_TERMINAL_5].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_5].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 5 Buffer Is NULL");
	screens[ID_VIRTUAL_TERMINAL_6].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_6].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 6 Buffer Is NULL");
	screens[ID_VIRTUAL_TERMINAL_7].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_7].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 7 Buffer Is NULL");
	screens[ID_VIRTUAL_TERMINAL_8].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_8].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 8 Buffer Is NULL");
	screens[ID_VIRTUAL_TERMINAL_9].buf = malloc(80*60*2);
	PanicOnNULL(screens[ID_VIRTUAL_TERMINAL_9].buf, "VirtualTerminalInit Failed On malloc!: Terminal Number 9 Buffer Is NULL");

	// First define all screens as free
	for(i=0; i<256; i++)
	{
		//
		screens[i].isFree = TRUE;
	}

	// Define first 10 as reserved(0-9)
	for(i=0; i<10; i++)
	{
		//
		screens[i].isFree = FALSE;
	}

	// Clear terminals
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_1]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_2]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_3]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_4]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_5]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_6]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_7]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_8]);
	sclrscr(&screens[ID_VIRTUAL_TERMINAL_9]);

	// Write something at the last terminals
	writetext(&screens[ID_VIRTUAL_TERMINAL_2], "This is terminal 2\n");
	writetext(&screens[ID_VIRTUAL_TERMINAL_3], "This is terminal 3\n");
	writetext(&screens[ID_VIRTUAL_TERMINAL_4], "This is terminal 4\n");
	writetext(&screens[ID_VIRTUAL_TERMINAL_5], "This is terminal 5\n");
	writetext(&screens[ID_VIRTUAL_TERMINAL_6], "This is terminal 6\n");
	writetext(&screens[ID_VIRTUAL_TERMINAL_7], "This is terminal 7\n");
	writetext(&screens[ID_VIRTUAL_TERMINAL_8], "This is terminal 8\n");
	writetext(&screens[ID_VIRTUAL_TERMINAL_9], "This is terminal 9\n");
	
	// Init done.
}

//
WORD GetCursorLocation(int ter)
{
        SCREEN *s;

        // Get terminal
        s = &screens[ter];

        // Return location
        return (s->curx&255) | ((s->cury&255)<<8);
}

//
int CreateTerminal(void)
{
	int terID;

	//
        terID = NewTerminal();
        SetTerminalOwner(terID, GetCurrentThread());
	return terID;
}

// Free all terminals which are owned by specified process
void FreePIDTerminals(int pid)
{
	int i;
	SCREEN *s;

	//
	if(pid<=0)
	{
		//
		sound(1000);
		panic("FreePIDTerminals called with pid<=0");
	}

	// Screens 0-8 are reserved for hardware terminals(EGA)
	for(i=9; i<N_MAX_SCREENS; i++)
	{
		// Get screen PTR
		s = &screens[i];

		// Check
		if(s->pid==pid)
		{
			// Deallocate screen
			s->isFree = TRUE;
		}
	}

	//
	return 0;
}

//
void OutTermKey(int terID, int ch)
{
	//
	return OutputTerminalKeyboardBuffer(&screens[terID], ch);
}

// Output 16-bit key to terminal's keyboard buffer
int OutputTerminalKeyboardBuffer(SCREEN *s, int ch)
{
	//
	return PushBufferW(&s->key, ch);
}

// Input 16-bit key from terminal's keyboard buffer
int InputTerminalKeyboardBuffer(SCREEN *s)
{
	//
	return PopBufferW(&s->key);
}

// Set screen free
void FreeScreen(SCREEN *s)
{
	// Set free
	s->isFree = TRUE;
}

// Get new terminal by ID
int ReserveTerminal(void)
{
	int i;
	SCREEN *s;

	// Search for a free screen, and allocate it
	for(i=10; i<N_MAX_SCREENS; i++)
	{
		// Get screen PTR
		s = &screens[i];

		// Check
		if(s->isFree)
		{
			// Allocate it
			s->isFree = FALSE;

			// Return ID
			return i;
		}
	}

	// Not found
	return -1;
}

// Set terminal owner
int SetTerminalOwner(int id, int pid)
{
	SCREEN *s;

	// Get new terminal ID
	s = &screens[id];

	//
	s->pid = pid;
	return 0;
}

// Get terminal owner
int GetTerminalOwner(int id)
{
	SCREEN *s;

	// Get new terminal ID
	s = &screens[id];

	//
	return s->pid;
}

// Allocate screen data buffer
void AllocateScreenBuffer(SCREEN *s)
{
	//
	if(s->buf!=NULL)
		free(s->buf);
	//
	s->buf = malloc(s->width*s->height*2);
}

// Create a new terminal
// (no hardware copy as default, e.g. EGA)
int NewTerminal(void)
{
	int i;
	SCREEN *s;

	//-----------------------------------------------
	// Get new terminal ID
	i = ReserveTerminal();
	s = &screens[i];

	//-----------------------------------------------
	// Init structure
	InitScreen(s);

	// Deactivate EGA copy
	// (copying not needed for non-HW screen)
	s->copy = NULL;

	//
	s->id = i;

	// Allocate buffer
	AllocateScreenBuffer(s);

	//-----------------------------------------------
	//
	sclrscr(s);

	//
//	sputs(s, "this is a test message\n");

	//
	return i;
}

// Just a clone
void attention(void)
{
	Attention();
}

// Get user's attention to the current thread's terminal
void Attention(void)
{
	//
	SetVisibleTerminal(GetCurrentTerminal());
}

// Emulates a TAB which jumps at 8 chars radius forward
void stab(SCREEN *s)
{
	int i,i2,i3,i4,x,x2;

	//
/*	x = swherex(s);
	x = x/TERM_TABSIZE; x++; x=x*TERM_TABSIZE;

	for(i=swherex(s); i<x; i++)
	{
		writemark(s,32,s->tc);
	}*/
}

// All go to terminal one (default terminal)
void defter(void)
{
	//
	SetVisibleTerminal(0);
	window(0,0, 79,24);
	thread_terminal[RPID(GetCurrentThread())] = 0;
}

// Get currently visible terminal #
int GetVisibleTerminal(void)
{
	//
	return visible_terminal;
}

// Set visible terminal
int SetVisibleTerminal(int which)
{
	// Set visible terminal
	visible_terminal = which;

	// EGA HW support
	if(which<9)
		EgaSelectWindow(which);

	//
	return 0;
}

// SetTerminalLock
//
// Return value:
// non-zero = error
// zero = success
//
int SetTerminalLock(int newLockPID)
{
	//
	if(terminalLockPID && terminalLockPID!=GetCurrentThread())
		return 1;
	return 0;
}

//
int GetTerminalLock(void)
{
	//
	return terminalLockPID;
}

//
void wait_system_pause(void)
{
	//
	if(glvar.system_pause)
	{
		enable();
		getch();
	}
}

//
int waitkey(void)
{
	int key;

	//
	printk("Press any key to continue ... ");
	key = getch();
	printk("\n");
	return key;
}

//** QUICK FUNCTIONS **

//---------------------------------------

// Fixed to return correct values
// according to the present window.
int wherex(void)
{
	return	swherex( &screens[MYTER] );
}

// Fixed to return correct values
// according to the present window.
int swherex(SCREEN *s)
{
	return s->curx - s->window.x1;
}

// Fixed to return correct values
// according to the present window.
int wherey(void)
{
	return	swherey( &screens[MYTER] );
}

// Fixed to return correct values
// according to the present window.
int swherey(SCREEN *s)
{
	return s->cury - s->window.y1;
}

//-----------------------------------

void gotoxy(int x,int y)
{
	//
	SCREEN *s;

	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
		  GetTerminalLock()!=GetCurrentThread())return;

	//
	s=&screens[MYTER];

	//
	sgotoxy(s,x,y);
}

// Locate cursor X
void gotox(int x)
{
	//
	SCREEN *s;
	int i,i2,i3,i4;

	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
			GetTerminalLock()!=GetCurrentThread())return;

	//
	s=&screens[MYTER];

	// Emulate with spaces if in CAP mode
	if(s->cap.length && s->cap.buf)
	{
		// FIX HERE
		for(i2=400,i=i2; i<x; i++)
		{
			printk(" ");
		}
	}
	else	sgotox(s,x);
}

// Locate cursor Y
void gotoy(int y)
{
	//
	SCREEN *s;

	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
		GetTerminalLock()!=GetCurrentThread())return;

	//
	s=&screens[MYTER];

	//
	sgotoy(s,y);
}

//
void ScrRemoteCom(SCREEN *s, char _c)
{
	// Send to the remote terminal too.
	if(dump_all_to_com1)
	{
		serial_put(0, _c);
	}

	//
	if(s->isRemoteCOM)
	{
		// Send to the remote terminal too.
		serial_put(s->isRemoteCOM-1, asc2vt220(_c));
	}
}

// Output a string
void sputs(SCREEN *s, const char *str)
{
	int i,l;

	//
	l = strlen(str);
	for(i=0; i<l; i++)
		sputch(s, str[i]);
}

// Putch used by system calls
void sputch(SCREEN *s, char _c)
{
	char str[256];
	static int cnt=0;

	// Do we have a capture working ?
	if( s->cap.length && s->cap.buf )
	{
		// Capture is working:
		SOutCap(s, _c);
		//DirectDrawText(vesa_frame_buf,640,480, "capture", 0,16);
	}
	else
	{
		// No capture:

		// Send debug character
//		doch(_c);

		//sprintf(str, "character %d", cnt++);
		//DirectDrawText(vesa_frame_buf,640,480, str, 0,16);

		// Print on console
		putch1(s,_c);
		ScrRemoteCom(s, _c);
	}
}

// Non-system call putch(safe)
void _putch(unsigned char _c)
{
	SCREEN *s;
	int i;

	//
	s=&screens[MYTER];
	putch1(s, _c);
	ScrRemoteCom(s, _c);
	//serial_directput(0, _c);
}

// putch via system call
void scputch(unsigned char _c)
{
	//  eax ebx ...
	scall(SYS_PUTCH,_c, 0,0,0,0,0);
}

// Using system call to prevent collisions.
//
void putch(unsigned char _c)
{
	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
		GetTerminalLock()!=GetCurrentThread())return;

	//
	if(scallsIsWorking)
	{
		scputch(_c);
	}
	else
	{
		PUSHFLAGS;
		disable();
		_putch(_c);
		POPFLAGS;
	}
}

// putch1 subfunction
int AnsiCase(SCREEN *s, unsigned char _c)
{
	// Are we handling an ansi code?
	if( s->ac==123 )
	{

		// Handle odd situation(double 27s)
		if(_c==27){ s->ac=0; return 0; }

		// Store to buffer & increase ptr
		s->ansi[s->ptr_ansi] = _c;
		s->ptr_ansi++;

		// Last char? (=alphabet)
		if( (_c>='a' && _c<='z')
			||
		    (_c>='A' && _c<='Z') )
		{
			s->ac=0;
			s->ansi[s->ptr_ansi]=0;
			HandleAnsiCode(s);
			return 1;
		}


		// Handle overflowing
		if( s->ptr_ansi >= 20 )
		{
			// Overflow occured
			s->ac=0;
			return 0;
		}
		return 1;
	}

	// Malformed ansicode
	s->ac=0;
	return 0;
}

//----------------------------------------------------------------------------------
//
// The main screen outputting function.
//
void putch1(SCREEN *s, BYTE _c)
{
	int i,i2;

	//
	if(s==NULL)
		return;

	// Debug to COM1.
	if(Multitasking)
		serial_directput(0, _c);	
	//
	if(s->ac) { if( AnsiCase(s, _c) )goto past; }

	//
	switch(_c)
	{
		//
		case '\t':
		stab(s);
		break;

		//
		case '\b':
		if( s->curx > s->window.x1 )
		{
			s->curx--;
			dropmark(s, ' ', s->tc);
		}
		IF_VISIBLE locatetxtcursor(s);
		break;

		// Return?
		case '\n':
		writemark(s, '\n', s->tc);
		break;

		//
		case '\r':
		sgotox(s,1);
		break;

		// ANSI CODE(new support for terminals)
		case 27:
		s->ac=123;
		s->ptr_ansi=0;
		break;

		//
		default:
		writemark(s, _c, s->tc);
		break;
	}
past:
	while(!1);
}

//
void p8(char v)
{
	printhex8(&screens[MYTER],v);
}

//
void p16(int v)
{
	printhex16(&screens[MYTER],v);
}

//
void p32(long v)
{
	printhex32(&screens[MYTER],v);
}

//
void p32z(long v)
{
	printhex32z(&screens[MYTER],v);
}

// Print Decimal Hex value
// (0x0100 -> 01.00);
void pdh32(long v)
{
	printdechex32(&screens[MYTER],v);
}

// print signed decimal (in non-zero fashioned mode)
void pd32(int v)
{
	printdec32(&screens[MYTER],v);
}

// print unsigned decimal (in non-zero fashioned mode)
void pud32(int v)
{
	printudec32(&screens[MYTER],v);
}

// print decimal (in zero fashioned mode)
void pdz32(long v)
{
	printdec32z(&screens[MYTER],v);
}

// 27.01.2002: added multithread handling
//
void xputs(const char *s, char *fun)
{
	char str[256];

	//
	if(vesa_frame_buf!=NULL)
	{
		//DirectDrawText(vesa_frame_buf,640,480, s, 0,0);
	}

	//
/*	if( illegal(s) )
	{
		sprintf(str, "puts zero page reference, called by function %s.",
			fun);
		panic(str);
	}*/

	//
	writetext(&screens[MYTER], s);
}

//
void print(const char *s)
{
	//
	wait_system_pause();

	//
	if(scallsIsWorking)
		scall(SYS_PUTS, s,0,0,0,0,0);
		else
		writetext(&screens[MYTER],s);
}


//
int gettextcolor(void)
{
	SCREEN *s;

	//
	s = &screens[MYTER];
	return s->tc&15;
}

//
int gettextbackground(void)
{
	SCREEN *s;

	//
	s = &screens[MYTER];
	return (s->tc>>4)&15;
}

//
void textcolor(int c)
{
	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
		  GetTerminalLock()!=GetCurrentThread())return;
	stextcolor(&screens[MYTER],c);
}

//
void textbackground(int c)
{
	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
	   GetTerminalLock()!=GetCurrentThread())return;

	//
	stextbackground(&screens[MYTER],c);
}

//
void _clrscr(void)
{
	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
	   GetTerminalLock()!=GetCurrentThread())return;

	//
	sclrscr(&screens[MYTER]);
}

//
void clrscr(void)
{
	// 27.04.02 featured TerminalLock
	if(GetTerminalLock() &&
	   GetTerminalLock()!=GetCurrentThread())return;

	//
	if(scallsIsWorking)
		scall(9, 0,0,0,0,0,0);
	else
		_clrscr();
}



/** FUNCTIONS **/
// Set Textcolor
void stextcolor(SCREEN *s,char c)
{
	//
	s->tc=(s->tc&0xf0)|c;
}

// Set Textbackground
void stextbackground(SCREEN *s,char c)
{
	//
	s->tc=(s->tc&0x0f)|((c&0xf)<<4);
}

//
// New: now limited to window area
// s->window.[x1..y1..x2..y2]
//
void clearline(SCREEN *s,long yy)
{
	int ad,ad2,x,y;

	//
	if( !(yy>-1 && yy<s->height) ) { return; }

	//
	y = yy;
	ad2=(y*s->width)<<1;
	for(ad=ad2,x=0,y=yy; ad<ad2+(s->width<<1); ad+=2,x++)
	{
		// We must be within window's X1 & X2:
		if( x<=s->window.x2 && x>=s->window.x1)
		{
			scrpoke(s, ad,' ');
			scrpoke(s, ad+1,s->tc);
		}
		else
		{
			// Out of limits:
			// *do nothing here*
		}
	}
}

// s,dy,y  (SCREEN,DESTINATION-LINE,SOURCE-LINE)
//
// new: now implements window parameters
//
void copyline(SCREEN *s,long dy,long y)
{
	int x,ad,ad2;

	//
	ad=(dy*s->width);
	ad2=(y*s->width);

	//
	for(x=0; x<s->width; x++)
	{
		// We must be within window's X1 & X2:
		if( x<=s->window.x2 && x>=s->window.x1)
		{
			scrpoke( s, ((ad+x)<<1)+0,scrpeek(s,((ad2+x)<<1)+0) );
			scrpoke( s, ((ad+x)<<1)+1,scrpeek(s,((ad2+x)<<1)+1) );
		}
	}
}

// Move screen up once. (This is what happends when screen is full and
// enter key is hit).
// New: now implements window parameters
//
void movesup(SCREEN *s)
{
	int x,y,ad,ad2,ad3,ad4;

	//
	for(y=s->window.y1; y<(s->window.y2); y++)
	{
		copyline(s, y, y+1);
	}

	// Clear last line
	clearline(s, s->window.y2);

	//
}

// Move screen down once. (This maybe useful function in some cases)
// New: now implements window parameters
//
void movesdown(SCREEN *s)
{
	int x,y,ad,ad2,ad3,ad4;

	//
	for(y=(s->window.y2); y!=(s->window.y1+1); y++)
	{
		copyline(s, y, y-1);
	}

	// Clear first line
	clearline(s, s->window.y1);
}

//
void movecdown(SCREEN *s)
{
	//
	s->cury++;
	if(s->cury > s->window.y2)
	{
		s->cury = s->window.y2;
		if(s->supallow)
			movesup(s);
	}
}

//
void movecright(SCREEN *s)
{
	//
	s->curx++;

	//
	if(s->curx > (s->window.x2))
	{
		//
		senter(s);
	}

	// Call gotoxy so that 'VIDEO' cursor moves also.
	sgotoxy(s, (s->curx - s->window.x1)+1, (s->cury - s->window.y1)+1);
}

// Drops one single mark at cursor's current position.
void dropmark(SCREEN *s,char mrk_char,char ega_color)
{
	//
	scrpoke(s, ((s->cury*80+s->curx)<<1)+0, mrk_char);
	scrpoke(s, ((s->cury*80+s->curx)<<1)+1, ega_color);
}

// Enter - Simulates enter key pressing.
void senter(SCREEN *s)
{
	//
	sgotox(s,1);
	movecdown(s);
	sgotox(s,1);
	IF_VISIBLE locatetxtcursor(s);
}

// Direct
void writemark1(SCREEN *s,char mrk_char,char ega_color)
{
	//
	//serial_directput(0, mrk_char);
	dropmark(s,mrk_char,ega_color);
	movecright(s);
}

// ASCII
void writemark(SCREEN *s,char mrk,char color)
{
	//
	switch(mrk)
	{
		// ENTER
		case '\n':
		senter(s);
		break;
		// Returns to the beginning of line.
		case '\r':
		senter(s);
		break;
		// Outputs character.
		default:
		writemark1(s,mrk,color);
		break;
	}
}

// Direct write string
void dwritetext(SCREEN *s, char *txt)
{
	int i,i2,i3,i4;

	//
	for(i=0; ; i++)
	{
		if(!txt[i])break;
		writemark(s,txt[i],s->tc);
	}
}

// Write string
void xwritetext(SCREEN *s, char *txt, char *fun)
{
	int i,i2,i3,i4;
	char str[256];

	//
/*	if(illegal(txt))
	{
		sprintf(str, "writetext zero page reference, called by function %s",
			fun);
		panic(str);
	}*/

	//
	for(i=0; ; i++)
	{
		if(!txt[i])break;
		sputch( s, txt[i] );
	}
}

// Direct poke screen memory
inline void scrpoke(SCREEN *s, long ad,char _data)
{
	char *p;
	int x,y;
	int rad;
	char str[256];

	//
	if((vesa_enabled && vesa_frame_buf!=NULL) && !(ad&1))
	{
		//
		if(s->id = GetVisibleTerminal())
		{
			rad = ad>>1;
			x = ad%80;
			y = ad/80;
			str[0] = _data;
			str[1] = 0;
		/*	DirectDrawText(vesa_frame_buf,640,480,
				str,
				8,8);			**/
		}
	}
	else
	{
		//
		p = s->buf;
		p[ad] = _data;
	
		// Copy to hardware EGA too? (visible HW terminal..)
		if(s->HWCopy)
		{
			//
			p = s->copy;
			p[ad] = _data;
		}
	}

	//
}

// Direct peek screen memory
inline char scrpeek(SCREEN *s,long ad)
{
	char *p;

	//
	p = s->buf;
	return p[ad];
}

// sgotoxy [screen structure] [x-coordinate] [y-coordinate]
// New: now restricted inside window region.
// Note: coordinates for sgotoxy's x and y begin from 1.
//
void sgotoxy(SCREEN *s,int x,int y)
{
	int fixed_x,fixed_y;

	// Invalid coordinates are always invalid:
	if(x<1 || y<1)return;

	// Calculate real/fixed X,Y
	fixed_x = s->window.x1+x-1;
	fixed_y = s->window.y1+y-1;

	// This is within window limits?
	if(fixed_x <= s->window.x2  &&  fixed_y <= s->window.y2)
	{
		s->curx = fixed_x;
		s->cury = fixed_y;

		IF_VISIBLE locatetxtcursor(s);
	}
	else
	{
		// If not valid coordinate:
		// *do nothing*
	}
}

// Goto X
void sgotox(SCREEN *s,int x)
{
	//
	sgotoxy(s, x, (s->cury - s->window.y1)+1);
}

// Goto Y
void sgotoy(SCREEN *s,int y)
{
	//
	sgotoxy(s, (s->curx - s->window.x1)+1, y);
}

// Clear screen
void sclrscr(SCREEN *s)
{
	int x,y,ad;

	//
	sgotoxy(s, 1, 1);

	//
	for(y=s->window.y1; y<(s->window.y2+1); y++)
	{
		for(x=s->window.x1; x<(s->window.x2+1); x++)
		{
			scrpoke(s, ((y*80+x)<<1)+0, ' ');
			scrpoke(s, ((y*80+x)<<1)+1, s->tc);
		}
	}
}

// what = TRUE or FALSE.
// TRUE = allow screen to scroll up.
// FALSE = disallow screen scrolling.
void SetTerminalScrolling(int terid, int what)
{
	SCREEN *s;

	//
	s = &screens[terid];

	//
	s->supallow=what;
}

// This function initializes screen structure.
void initscreenstructure(SCREEN *s)
{
	//
	memset(s, 0, sizeof(SCREEN));

	//
	s->ac=0; // No ANSI code processing currently at queue
	s->ptr_ansi=0; // Set ptr to 0
	s->updcur=1; // Enable HW cursor support
	s->width=80; // Screen Width
	s->height=25; // Screen Height
	s->supallow=1; // Screen can be scrolled up? 1=YES!
	s->tc=0x07; // Default screen color
	s->buf = 0xB8000;	// As default, use EGA screen.
	s->copy = 0xB8000;	// Also as default copy to the EGA screen
	// NOTE that s->copy is not used
	// until HWCopy is set to non-zero.
	s->isRemoteCOM = 0; // It is not a remote terminal screen.
	s->pid = 0;
	s->HWCopy=FALSE;

	// Define terminal output window region
	swindow(s, 0,0,79,24);

	// Create keyboard buffer for this terminal
	CreateBuffer(&s->key, 1024);
}

// Creates a SCREEN structure that
// will work as a gateway to the text console.
//
// By redirecting later on s->buf to other
// memory location, it is possible to
// create a virtual terminal.
//
// By using multiple screen structures,
// it is possible to type on screen from
// many tasks at once.
//
SCREEN *InitScreen(SCREEN *s)
{
	// Init screen structure
	initscreenstructure(s);

	// Define default window
	swindow(s,  0,0, s->width-1,s->height-1);

	// Set textcolor/background & clear screen
	stextcolor(s,7); stextbackground(s,0);

	// Screen 2 is connected to terminal on COM2.
//	screens[1].isRemoteCOM = 2;

	//
	return s;
}

//
void printhex8(SCREEN *s,BYTE value)
{
	char str[100];

	//
	shex8(str, value);
	writetext(s, str);
}

//
void printhex16(SCREEN *s,WORD value)
{
	//
	printhex8(s, value>>8);
	printhex8(s, value&0xff);
}

//
void printhex32(SCREEN *s,DWORD value)
{
	//
	printhex16(s, value>>16);
	printhex16(s, value&0xffff);
}

//
void printhex32z(SCREEN *s,DWORD value)
{
	char str[100];
	shex32z(str, value);
	writetext(s, str);
}

// Prints decimal value as it was an internet with a decimal
// 12.34
// (0x1234)
void printdechex32(SCREEN *s,long value)
{
	printhex16(s, value>>16); print(".");
	printhex16(s, value&0xffff);
}

//
void printdec32(SCREEN *s,DWORD value)
{
	char str[100];
	sdec32(str,value);
	writetext(s,str);
}

//
void printudec32(SCREEN *s, int value)
{
	char str[100];
	sudec32(str,value);
	writetext(s,str);
}

//
void printdec32z(SCREEN *s,unsigned long int value)
{
	char str[100];

	//
	sdec32z(str,value);
	writetext(s,str);
}

// Updated:
// Now supports more than one screen.
// New: Now adds current visible screen offset
// to the cursor location.
//
void locatetxtcursor(SCREEN *s)
{
	int ad,ad2,ad3,ad4;

	//
	ad2=s->copy; ad2-=0xb8000;

	//
	if(s->updcur)
	{
		//
		ad = (s->width*s->cury + s->curx) + (ad2>>1);
		outvgareg(0x0e,ad>>8);
		outvgareg(0x0f,ad&0xff);
	}
}

//
void sysmsg(char *viesti)
{
	//
	int x,y;

	//
	x=wherex();
	y=wherey();

	//
	textcolor(YELLOW); textbackground(RED);
	gotoxy(1,3);
	printk("  %s  ",viesti);

	//
	textcolor(7); textbackground(BLACK);
	gotoxy(x,y);
}

//
int swindow(SCREEN *s,
	long x1,long y1,long x2,long y2)
{
	// Limit selections
	if(x1<0) x1=0;
	if(y1<0) y1=0;

	// Limit selections II
	if(x2 >= s->width)	x2=s->width-1;
	if(y2 >= s->height)	y2=s->height-1;

	// Last logic check
	if(x2<x1)return 1;
	if(y2<y1)return 2;

	//
	s->window.x1 = x1;
	s->window.y1 = y1;
	s->window.x2 = x2;
	s->window.y2 = y2;
	return 0;
}

//
int window(long x1,long y1,long x2,long y2)
{
	//
	return swindow(&screens[MYTER], x1,y1,x2,y2);
}

//
void CursorState(SCREEN *s,int _state)
{
	//
	s->updcur = _state;
}

// Set hardware copying option
// (1=copy to BYTE *copy too,
//  0=don't copy, just use buf only)
void SetHWCopy(SCREEN *s, int what)
{
	s->HWCopy = what;
}

void BufToCopy(SCREEN *s)
{
	BYTE *src,*dst;

	src = s->buf; dst = s->copy;
	memcpy(dst, src, 80*25*2);
}

// 0 =	Disables all output that is written at EGA screen memory
// 1 =	Enables screen output at EGA memory & refreshes the screen output
//	by copying the upto date information from buf to copy.
void VisibleTerminals_SetHWCopy(int n)
{
	// Restore selected window(video mode changer changes this on HW)
	SetVisibleTerminal(GetVisibleTerminal());

	// Enable copying
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_1], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_2], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_3], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_4], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_5], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_6], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_7], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_8], n);
	SetHWCopy(&screens[ID_VIRTUAL_TERMINAL_9], n);

	// Refresh
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_1]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_2]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_3]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_4]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_5]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_6]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_7]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_8]);
	BufToCopy(&screens[ID_VIRTUAL_TERMINAL_9]);
}

// (SCREEN) Output Captured Byte to a Capture Buffer
void SOutCap(SCREEN *s, int ch)
{
	if(s->cap.length && s->cap.buf)
	{
		if(  s->cap.offs < (s->cap.length-2)  )
		{
			s->cap.buf[s->cap.offs] = ch;
			s->cap.buf[s->cap.offs+1] = 0;
			s->cap.offs ++;
		}
	}
}

//
void SetTerCapture(int terID, char *buf, int length)
{
	SCREEN *s;

	//
	s = &screens[terID];

	//
	s->cap.length =	length;
	s->cap.buf =	buf;
	s->cap.offs =	0;
}

//

