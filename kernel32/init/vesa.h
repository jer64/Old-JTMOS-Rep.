#ifndef __INCLUDED_VESA_H__
#define __INCLUDED_VESA_H__

//
extern BYTE *vesa_frame_buf;
int VesaSetup(void);
extern int vesa_enabled;
extern unsigned char *vb,*vesainfo;
extern BYTE egapal[];

#endif

