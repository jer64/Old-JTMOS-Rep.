//
#ifndef __INCLUDED_JTMLIBC_CONSOLE_H__
#define __INCLUDED_JTMLIBC_CONSOLE_H__

#include "basdef.h"

//
struct text_info
{
    BYTE winleft;
    BYTE wintop;
    BYTE winright;
    BYTE winbottom;
    BYTE attribute;
    BYTE normattr;
    BYTE currmode;
    BYTE screenheight;
    BYTE screenwidth;
    BYTE curx;
    BYTE cury;
};

//
void gettextinfo(struct text_info *_r);

//
int getch1(void);
void clrscr(void);
int puts(const char *s);
int putchar(int _mark1);
void delay(unsigned msec);
int getch(void);
int putch(int _mark1);
void gotoxy(int x,int y);
char *gets(char *s);
void textcolor(int col);
void textbackground(int col);
int wherex(void);
int wherey(void);
void ESCLOCK(void);
int GetCTRL(void);
int GetALT(void);

//
#endif

//
