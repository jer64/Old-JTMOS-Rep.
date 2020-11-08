#ifndef __INCLUDED_MOUSE_H__
#define __INCLUDED_MOUSE_H__

//
typedef struct
{
        int x1,y1, x2,y2;
}FRAME;

//
typedef struct
{
        int button1, button2, button3;
        int nCom; // 0=COM1, 1=COM2
        int signal;
        int inbyte;
        int dx,dy,x,y,div; // div=100
        int bytenum;
        BYTE combytes[100];
}SERMOUSE;

//
typedef struct
{
        //
        FRAME frame;
        // This determines whetever should the arrow
        // being drawn or not.
        int isVisible;
        //
        char ch;
        char mx, my;
        int buttons, x,y, lx,ly, rate, resolution,
                sm_flags, save_char,
                x_move, y_move, x_max, x_min, y_max, y_min,
                user_subrourtine, user_mask, temp_mask;

        // Serial mouse structures for each of serial COM ports.
        SERMOUSE serial[4];
}MOUSE;

//
int mouse_isVisible(void);
void mouse_setframe(long x1,long y1,long x2,long y2);
void mouse_setposition(long x,long y);
void mouse_modeChange(int vmode);
void mouse_showmouse(void);
void mouse_hidemouse(void);
void mouse_hidecursor(void);
void mouse_showcursor(void);

//
MOUSE mouse;
//
unsigned char mouse_arrow[];

#endif

