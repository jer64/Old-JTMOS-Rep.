//
#ifndef __INCLUDED_JTM_MEMCPY_H__
#define __INCLUDED_JTM_MEMCPY_H__

//
void *xmemset(char *buf,char ch,int length);
void *dmemset(char *buf,char ch,int length,
        char *whos_call);
void *dmemcpy(void *o,void *s,DWORD l,
        char *whos_call);
void *xmemcpy(void *o,void *s,DWORD length);
//
#define memcpy(o,s,l) dmemcpy(o,s,l, __FUNCTION__)
        // See memset.c:
#define memset(q1,q2,q3) dmemset(q1,q2,q3, __FUNCTION__)
//
int illegal(DWORD ad);

#endif




