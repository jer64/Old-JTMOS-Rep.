#ifndef __INCLUDED_GUIFUNC_H__
#define __INCLUDED_GUIFUNC_H__

#include <stdio.h>
#include "graos.h"

void DrawMonoBitmapEx(VMODE *v,
        BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
        long target_x,    long target_y,
        long wanted_width,long wanted_height,
        long source_x,    long source_y,
        int tr);

void DrawMonoBitmap(VMODE *v,
                                BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
                                long target_x,    long target_y,
                                long wanted_width,long wanted_height,
                                long source_x,    long source_y);
void DrawTransparentMonoBitmap(VMODE *v,
                                BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
                                long target_x,    long target_y,
                                long wanted_width,long wanted_height,
                                long source_x,    long source_y);

void gui_DrawBitmap(VMODE *v,
                        BYTE *src, int swidth, int sheight, BYTE *pal,
                        int xp, int yp,
                        int lenx,int leny,
                        int Xread,int Yread);

void gui_ClearScreen(VMODE *v);
int waitkey(void);
int LoadBin(const char *fname, BYTE *buf, int limit);
int follow(int what, int dest);
void DrawMonoBitmapColor(VMODE *v,
        BYTE *srcbuf,long s_width,long s_height,BYTE *pal,
        long target_x,    long target_y,
        long wanted_width,long wanted_height,
        long source_x,    long source_y,
        int color,
        char drawbackground);

#endif
