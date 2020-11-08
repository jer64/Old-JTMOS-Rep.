/** VGA **
 *
 * See vgareg.txt for list of VGA registers I know.
 *
 */
#ifndef __INCLUDED_SYSVGA_H__
#define __INCLUDED_SYSVGA_H__

//
#define SEQADDR		0x3c4
#define GRACONADDR	0x3ce
#define CRTCADDR	0x3d4

//
void outvgareg(long reg,char daatta);
void vga_640x480(void);
void setup_systemfont(void);
void setwplane(long plane);
void setrplane(long plane);
void sysvga_drawpalette(void);

#endif
