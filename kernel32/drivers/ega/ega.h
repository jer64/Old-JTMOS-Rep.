/*** System EGA/Ansi Graphic Routines ***/
#ifndef _EGA_H_
#define _EGA_H_

// How many virtual terminals at max.?
#define N_MAX_SCREENS		256

// 9 is enough for ALT 1-9
#define N_VIRTUAL_TERMINALS	9

//
#define MYTER GetCurrentTerminal()
// TAB size
#define TERM_TABSIZE	8

//
#include "basdef.h" // BYTE, WORD, DWORD
#include "sysglvar.h"
#include "system_call.h" // scallsIsWorking
#include "screen.h" // The screen structure.

//
extern int terminalLockPID;
int GetVisibleTerminal(void);

// 0-7
#define BLACK		0
#define BLUE		1
#define GREEN		2
#define CYAN		3
#define RED		4
#define MAGENTA		5
#define BROWN		6
#define LIGHTGRAY	7
// 8-15
#define DARKGRAY	8
#define LIGHTBLUE	9
#define LIGHTGREEN	10
#define LIGHTCYAN	11
#define LIGHTRED	12
#define LIGHTMAGENTA	13
#define YELLOW		14
#define WHITE		15

//
#define SCREENADDY 0xb8000
//
extern int ega_init(void);
extern void CursorState(SCREEN *s,int _state);
void EgaSetViewStart(WORD ad);
void EgaSelectWindow(int which);

//
void EGAVisibleTerminals_SetHWCopy(int n);
void EGASafeMode(void);
//
void attention(void);
extern char dump_all_to_com1;
int CreateTerminal(void);
WORD GetCursorLocation(int ter);
void OutTermKey(int terID, int ch);

//
extern int saved_terminal;

//
#define writetext(x,y) xwritetext(x,y,__FUNCTION__)
#define puts(x) xputs(x,__FUNCTION__)

#endif

