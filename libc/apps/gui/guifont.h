#ifndef __INCLUDED_GUIFONT_H__
#define __INCLUDED_GUIFONT_H__

// For font renderation function
#define NON_TRANSPARENT		0
#define TRANSPARENT		1
#define NON_THIN		2

//
#include <stdio.h>
#include "graos.h"

int LoadFont(const char *fname, FONT *fon, int invert);
void DrawText(VMODE *v, const char *s, int tx,int ty,
		int t, int color);
void DrawTextEx(VMODE *v, FONT *f, const char *s, int tx,int ty,
		int transparency, int color);
void DrawTextColorful(VMODE *v,
                FONT *f,
                const char *s, int tx,int ty,
                int transparency,
                char *colmap, int color);
void _DrawText(VMODE *v,
                const char *s,long x,long y,
                const char *font,long font_width,long font_height,
		int transparency,
                char *colmap, int color);

#endif
