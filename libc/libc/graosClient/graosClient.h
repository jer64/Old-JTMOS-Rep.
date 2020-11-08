#ifndef __INCLUDED_GRAOSCLIENT_H__
#define __INCLUDED_GRAOSCLIENT_H__

//
#define GRAOS_SERVER_PID_NAME	"guiserver"

//
void graosCheck(void);
inline int getGraosPID(void);
int waitGraosAnswer(void);
int createWindow(int x,int y,
                int width,int height,
                const char *caption,
		int type);
int closeWindow(int wid);
int refreshWindow(int wid);
int addFrameBuffer(int wid,
        BYTE *buf, int x, int y,
        int width, int height, int bpp,
	int type, int terID);
int createButton(int wid,
        int x1,int y1,int x2,int y2,
        int type,
        const char *caption);

#endif


