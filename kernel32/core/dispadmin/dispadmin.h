//
#ifndef __INCLUDED_DISPADMIN_H__
#define __INCLUDED_DISPADMIN_H__

//
typedef struct
{
	//
	int vmode;
	int width,height,bpp;
}DISPADMIN;

//
extern void dispadmin_reportmodechange(int vmode);
extern int dispadmin_getcurrentmode(void);
extern void dispadmin_xorpixel(int x,int y);


//
#endif
