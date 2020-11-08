#ifndef __INCLUDED_MKANPROCESS_H__
#define __INCLUDED_MKANPROCESS_H__

//
void processimages(int BGBYTE,int NST,int FORCESIZE,int MAXI,int DITHER,int type,char *fname,char *ofname,int begival,
	unsigned int XLEN,unsigned int YLEN,unsigned int CXPON,unsigned int CYPON);
void removefiletypefromfilename(char *fname);

#endif
