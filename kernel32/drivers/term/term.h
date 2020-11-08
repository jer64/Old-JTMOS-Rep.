/*** System EGA/Ansi Graphic Routines ***/
#ifndef __TERM_H__
#define __TERM_H__

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
extern int puts_busy;
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
extern int waitkey(void);
extern void putch(unsigned char);
extern void p8(char v);
extern void p16(int v);
extern void p32(long v);
extern void p32z(long v);
extern void pd32(int v);
extern void pud32(int v);
extern void pdz32(long v);
extern void print(const char *s);
extern void xputs(const char *s, char *fun);
extern void stextcolor(SCREEN *s,char c);
extern void stextbackground(SCREEN *s,char c);
extern void clearline(SCREEN *s,long y);
extern void copyline(SCREEN *s,long dy,long y);
extern void movesup(SCREEN *s);
extern void movesdown(SCREEN *s);
extern void movecdown(SCREEN *s);
extern void movecright(SCREEN *s);
extern void dropmark(SCREEN *s,char mrk,char color);
extern void senter(SCREEN *s);
extern void writemark1(SCREEN *s,char mrk,char color);
extern void writemark(SCREEN *s,char mrk,char color);
extern void dwrite(SCREEN *s,char *txt);
extern void xwritetext(SCREEN *s,char *txt, char *fun);
extern void scrpoke(SCREEN *s, long ad,char _data);
extern char scrpeek(SCREEN *s, long ad);
extern void sgotoxy(SCREEN *s,int x,int y);
extern void sgotox(SCREEN *s,int x);
extern void sgotoy(SCREEN *s,int y);
extern void sclrscr(SCREEN *s);
extern void initscreenstructure(SCREEN *scr_);
extern SCREEN *InitScreen(SCREEN *s);
extern void printhex8(SCREEN *s,BYTE value);
extern void printhex16(SCREEN *s,WORD value);
extern void printhex32(SCREEN *s,DWORD value);
extern void printdec32(SCREEN *s,unsigned long int value);
extern void printdec32z(SCREEN *s,unsigned long int value);
extern void locatetxtcursor(SCREEN *s);
int gettextcolor(void);
int gettextbackground(void);
extern void textcolor(int c);
extern void textbackground(int c);
extern void clrscr(void);
extern void pdh32(long);
extern void putch1(SCREEN *s,unsigned char _c);
extern void sysmsg(char *viesti);
extern void printhex32z(SCREEN *s,DWORD value);
extern void gotox(int x);
extern void gotoy(int y);
extern int swindow(SCREEN *s,
	long x1,long y1,long x2,long y2);
extern int window(long x1,long y1,long x2,long y2);
extern void sputch(SCREEN *s,char _c);
extern void CursorState(SCREEN *s,int _state);
int GetTerminalLock(void);
int SetTerminalLock(int);
extern int visible_terminal;
void ScrRemoteCom(SCREEN *s, char _c);
int swherey(SCREEN *s);
int swherex(SCREEN *s);
void FreeScreen(SCREEN *s);
int OutputTerminalKeyboardBuffer(SCREEN *s, int ch);
int InputTerminalKeyboardBuffer(SCREEN *s);
void SetTerminalScrolling(int terid, int what);

//
void SOutCap(SCREEN *s, int ch);
void SetTerCapture(int terID, char *buf, int length);
void StretchPrint(const char *s, int len);
int SetVisibleTerminal(int which);
void FreePIDTerminals(int pid);
void sputs(SCREEN *s, const char *str);
//
extern SCREEN screens[N_MAX_SCREENS];
//
void printudec32(SCREEN *s, int value);
void VisibleTerminals_SetHWCopy(int n);
void SetHWCopy(SCREEN *s, int what);
void BufToCopy(SCREEN *s);
void EGASafeMode(void);
void Attention(void);
//
void defter(void);
int NewTerminal(void);
int ReserveTerminal(void);
int SetTerminalOwner(int id, int pid);
int GetTerminalOwner(int id);
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

