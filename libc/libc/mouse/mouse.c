// mouse.c
#include "basdef.h"
#include "console.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "postman.h"
#include "filebuf.h"
#include "mouse.h"

int SetMouseRect(int x1,int y1,int x2,int y2)
{
	return scall(SYS_setmouserect, x1,y1,x2,y2,0,0);
}

int GetMouseX(void)
{
	return scall(SYS_inputmouse, 0, 0,0,0,0,0);
}

int GetMouseY(void)
{
	return scall(SYS_inputmouse, 1, 0,0,0,0,0);
}

int GetMouseButtons(void)
{
	return scall(SYS_inputmouse, 2, 0,0,0,0,0);
}

