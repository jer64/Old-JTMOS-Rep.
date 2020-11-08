// ====================================================
// Graphic Access Library - GAL
// (C) 2002 by Jari Tuominen(jari@vunet.org)
// ====================================================
#include "basdef.h"
#include "console.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "postman.h"
#include "filebuf.h"

//
int setpalettemap(int offs, int amount, BYTE *paldata)
{
        int r;
        static POSTMANCMD post;

        post.id=PCMD_SETPALETTEMAP;
	post.v1=offs;
	post.v2=amount;
        post.par1=paldata;
	return PostmanPost(&post);
}

//
int setpalette(int which, int r,int g,int b)
{
        static POSTMANCMD post;

        //
        post.id=PCMD_SETPALETTE;
        post.v1=which;
	post.v2=r;
	post.v3=g;
	post.v4=b;
	return PostmanPost(&post);
}

// Alias for setmode
int setvmode(int vmode)
{
	return setmode(vmode);
}

//
int setmode(int vmode)
{
	int r;
	static POSTMANCMD post;

	//
	post.id=PCMD_SETMODE;
	post.v1=vmode;
	return PostmanPost(&post);
}

//
int drawframe(const char *buf, int offs, int len)
{
	return scall(SYS_vgadrawframe, buf,offs,len, 0,0,0);
}

//
int drawframe1(const char *buf, int offs, int len)
{
	int r;
	static POSTMANCMD post;

	//
	post.id=	PCMD_DRAWFRAME;
	post.par1=	buf;
	post.v1=	offs;
	post.v2=	len;
	return PostmanPost(&post);
}

//
void waitretrace(void)
{
	scall(SYS_vgawaitretrace, 0,0,0, 0,0,0);
}

// VGA SELECT PLANE
int vgaselectplane(int which)
{
	return scall(SYS_vgaselectplane, which,0,0, 0,0,0);
}

//



