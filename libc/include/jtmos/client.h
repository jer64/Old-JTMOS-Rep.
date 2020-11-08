#ifndef __INCLUDED_CLIENT_H__
#define __INCLUDED_CLIENT_H__

//
void graosCheck(void);
inline int getGraosPID(void);
int waitGraosAnswer(void);
int createWindow(int x,int y,
                int width,int height,
                const char *caption);
int closeWindow(int wid);

#endif


