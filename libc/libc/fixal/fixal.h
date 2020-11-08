//
#ifndef __INCLUDED_FIXAL_H__
#define __INCLUDED_FIXAL_H__

//
#define FIXAL_GUARD		0xE3FCE200

//
#define fixal(x) fixal1(x, __FUNCTION__)

//
void *fixal1(int amount, const char *func);
void fixalFree(void *p);
int fixalCheck(void *pp, int amount);
void *fixalReallocateOnCorruption(void *buf, int l_buf);

#endif


