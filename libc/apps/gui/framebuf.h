#ifndef __INCLUDED_FRAMEBUF_H__
#define __INCLUDED_FRAMEBUF_H__

//
FRAMEBUF *newFrameBuf(WINDOW *w);
int drawFB(VMODE *v, WINDOW *w, FRAMEBUF *fb);
int drawWindowFrameBuffers(VMODE *v, WINDOW *w);
int guiAddFrameBuffer(WINDOW *w,
        BYTE *buf, int x, int y,
        int width, int height, int bpp,
        int type,
        int terID);
DWORD chksum(BYTE *buf, int len);
int automaticallyRefreshTerminalWindows(VMODE *v);

#endif


