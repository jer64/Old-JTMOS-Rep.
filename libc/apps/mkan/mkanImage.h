//
#ifndef __INCLUDED_MKANIMAGE_H__
#define __INCLUDED_MKANIMAGE_H__

//
int jtmgl_blurbuffer(unsigned char *buf, int w,int h);
void mkan_resizeframe(unsigned char *buf,int sx,int sy);
int mkan_decode_pcxto(unsigned char *_srcfname,char *_buffer,unsigned char _paletteadd);
int mkan_jtm_loadbmp(char *fname,char *frame);

#endif


