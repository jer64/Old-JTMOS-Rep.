//
#ifndef __INCLUDED_VIDCOMP_H__
#define __INCLUDED_VIDCOMP_H__

//
typedef struct
{
	// Stepping, beginning frame
	int step,beg;

	// Video format
	int width,height,bpp;

	// Temporary frame buffer
	BYTE *tmp;

	// Last frame
	BYTE *last;

	// Misc. temporary storage 64K
	BYTE *t;
}VC;

//
int detectFrameWalking(const char *src);
int outputVideoHeader(FILE *f);
int determineBlockSize(int current, int top);
int getBlockNoiseLevel(BYTE *buf, int tx,int ty, int bs);
int outputVideoBlock(BYTE *buf, int xs,int ys,
	int bs, int step, FILE *f2);
int encodeFrame1(BYTE *buf, FILE *f2);
int encodeFrame(const char *src, FILE *f2);
int compress1(const char *src, const char *dst);
int compress(const char *src, const char *dst);
int initCompressor(void);

//
extern int QUALITY;
extern int VID_BLOCK_SIZE;
extern int MIN_NOISE_DET_LEVEL;
extern int QUALITY;
extern int keyFrameInterval;
extern int SIM_LEVEL;
extern int IGNORE_BITS;

#endif





