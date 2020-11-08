#ifndef __JTMVGA_H__
#define __JTMVGA_H__
#include <dpmi.h>
void jtm_setpalette(unsigned char _color,
     unsigned char _rrr,unsigned char _ggg,unsigned char _bbb)
{
/* regs.x.ax=0x1010;
 regs.x.bx=_color;
 regs.x.cx=(_ggg>8)|_bbb;
 regs.x.dx=_rrr;
 __dpmi_int(0x10,&regs);*/
 outportb(0x3c8,_color);
 outportb(0x3c9,_rrr);
 outportb(0x3c9,_ggg);
 outportb(0x3c9,_bbb);
 jtmansi.palette[(_color<<2)+0]=_rrr;
 jtmansi.palette[(_color<<2)+1]=_ggg;
 jtmansi.palette[(_color<<2)+2]=_bbb;
}

void jtm_setpalette1(unsigned char _color,
     unsigned char _rrr,unsigned char _ggg,unsigned char _bbb)
{
/* regs.x.ax=0x1010;
 regs.x.bx=_color;
 regs.x.cx=(_ggg>8)|_bbb;
 regs.x.dx=_rrr;
 __dpmi_int(0x10,&regs);*/
 outportb(0x3c8,_color);
 outportb(0x3c9,_rrr);
 outportb(0x3c9,_ggg);
 outportb(0x3c9,_bbb);
}


void jtm_readpalette(char *p)
{
 int i;
 //
 for(i=0;i<256;i++)
 {
  outportb(0x3C7,i);
  p[0+(i*3)]=inportb(0x3C9) & 0x3F;
  p[1+(i*3)]=inportb(0x3C9) & 0x3F;
  p[2+(i*3)]=inportb(0x3C9) & 0x3F;
 }
}

void jtm_waitsd()
{
 unsigned char m1;
/* do
 {
  m1=inportb(0x3da);
 }while(m1&8!=0);*/
 do
 {
  m1=inportb(0x3da);
 }while((m1&8)==0);
}

// Notice: VESA required.
void jtmgl_setscreenstart(unsigned x,unsigned y)
{
 __dpmi_regs regs;
 regs.x.ax=0x4f07;
 regs.x.bx=0x80;   // 0x80=Wait vertical redraw, 0x00=Don't.
 regs.x.cx=x;
 regs.x.dx=y;
 __dpmi_int(0x10,&regs);
}

void jtm_setvmode1(unsigned _mode)
{
 __dpmi_regs regs;
 regs.x.ax=_mode;
 __dpmi_int(0x10,&regs);
}

void jtm_setvmode(unsigned _mode)
{
 __dpmi_regs regs;
 jtm_vmode=_mode;
 regs.x.ax=_mode;
 __dpmi_int(0x10,&regs);
}

void jtm_copyscreen(char *src,char *dst)
{
 memcpy(dst,src,64000);
}

void jtm_setvesa1(unsigned _vmode)
{
#ifdef __BORLANDC__
 asm
 {
  mov ax,0x4f02
  mov bx,word ptr _vmode
  int 0x10
 }
#else
 __dpmi_regs regs;
 if(!(jtm_vmode!=_vmode))goto skippi;
 regs.x.ax=0x4f02;
 regs.x.bx=_vmode;
 __dpmi_int(0x10,&regs);
 jtm_vmode=_vmode;
skippi:
#endif
 while(0!=0);
}

void jtm_setvesa(unsigned _vmode)
{
#ifdef __BORLANDC__
 asm
 {
  mov ax,0x4f02
  mov bx,word ptr _vmode
  int 0x10
 }
#else
 __dpmi_regs regs;
 jtm_vmode=_vmode;
 regs.x.ax=0x4f02;
 regs.x.bx=_vmode;
 __dpmi_int(0x10,&regs);
#endif
}

void jtm_vf(char *__videoframe)
{
 dosmemput(__videoframe, 0x10000, 0xa0000);
}

void jtm_videoframe(char *__videoframe)
{
 if(jtm_emode==0 && jtm_vmode==0x13)
 {
  if(jtm_waitsdALLWAYS==1)jtm_waitsd();
  dosmemput(__videoframe, 0x10000, 0xa0000);
  goto done;
 }
 if(jtm_emode==0x06 && jtm_vmode==0x13)
 {
  if(jtm_waitsdALLWAYS==1)jtm_waitsd();
  dosmemput(__videoframe, 0x10000, 0xb8000);
  goto done;
 }
done:
 while(0!=0);
}

void jtm_setvbank(unsigned _vbank) // Switch vesa bank(0=first 64kb,2=second...)
{
 __dpmi_regs regs;
 //
 if(jtm_vbank!=_vbank)
 {
  regs.x.ax=0x4f05;
  regs.x.bx=0x0000;
  regs.x.dx=_vbank;
  __dpmi_int(0x10,&regs);
//  outportb(0x03ce,0x09);
//  outportb(0x03cf,_vbank);
//  outportb(0x03d6,0x0b);
//  outportb(0x03d7,_vbank);
  jtm_vbank=_vbank;
 }
}
#endif
/** END OF JTMVGA_H **/
