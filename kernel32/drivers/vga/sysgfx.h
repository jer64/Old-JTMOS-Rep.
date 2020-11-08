/*** System Graphic Routines ***/
#ifndef _SYSGFX_H_
#define _SYSGFX_H_
#include "io.h"

extern void vga_xorpixel_m11(long x,long y);
extern void setvbank(int);
void setpaletteEx(int c, int r,int g,int b);
void setpalettemap(int offs, int amount, BYTE *paldata);
void vga_waitretrace(void);
void setvgaplane(int which);

#endif
