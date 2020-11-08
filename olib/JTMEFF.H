#ifndef __JTMEFF_H__
#define __JTMEFF_H__
// Random set backgronud color.
// (Possibilities are 256 colors in current palette)
void jtmscr_blinkbackground(void)
{
 jtm_setpalette(0,rand()&0x3f,rand()&0x3f,rand()&0x3f);
}

// Blink between Black'n'White.
void jtmscr_plinkbackground(void)
{
 jtmscr.plink=0x3f-jtmscr.plink;
 jtm_setpalette(0,jtmscr.plink,jtmscr.plink,jtmscr.plink);
}

jtmscr_noise()
{
 //
 char m1;

 //
 while(0==0)
 {
  m1=jgetchim(); if(m1==27)break;
  jtmscr_blinkbackground();
 }
}
#endif
