/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTMOS linux header file
 *
 * (C)1998 by Jari Tapio Tuominen, all rights reserved.
 *
 * This library simulates some of JTMDJGFX(GraOS) functions
 * and redirects the function calls to VGA library, and other
 * Linux based libraries. 
 */

void jtm_waitsd()
{
 vga_waitretrace();
}

void jtm_vf(char *buf)
{
 vga_drawscansegment(buf,0,0,64000); // 64000=320*200
}

int jgetchim()
{
 return vga_getkey();
}

void jtmgl_vgabootup()
{
 jtmscr.width=320; jtmscr.height=200; jtmscr.bpp=8;
 jtmscr.vbuffer=malloc(jtmscr.width*jtmscr.height);
 vga_setmode(5);
}

void jtm_setpalette(int c,int r,int g,int b)
{
 vga_setpalette(c,r,g,b);
}



