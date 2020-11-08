#ifndef __INCLUDED_JTMLIBC_CONSOLETEXT_H__
#define __INCLUDED_JTMLIBC_CONSOLETEXT_H__

//
int gettext(int left,int top,int right,int bottom,
                void *destin);
int puttext(int left,int top,int right,int bottom,
                void *destin);
int movetext(int left, int top,
        int right, int bottom,
        int destleft, int desttop);

#endif
