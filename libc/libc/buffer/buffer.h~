#ifndef __INCLUDED_BUFFER_H__
#define __INCLUDED_BUFFER_H__

//
typedef struct
{
	BYTE *buf;
	int l_buf;
	int head,tail;
	int count;
	DWORD magic; // 0xFCE2E37B
	int total; // total buffers travelled through the buffer
}FBUF;


//
int PushBufferW(FBUF *b, WORD w);
int PopBufferW(FBUF *b);
int PushBuffer(FBUF *b, BYTE byte1);
int PopBuffer(FBUF *b);

//
int GetBuffer(FBUF *b);
int PutBuffer(FBUF *b, int d1);


#endif


