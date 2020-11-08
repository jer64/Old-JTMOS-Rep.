#ifndef __INCLUDED_BUFFER_H__
#define __INCLUDED_BUFFER_H__

//
#define FIFOBUF_MAGIC		0xFCE2E37B

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
/*int WriteBuffer(FBUF *b, BYTE *buf, int len);
int ReadBuffer(FBUF *b, BYTE *buf, int len);
int PopBuffer(FBUF *b)
int PushBuffer(FBUF *b, BYTE byte1);
int HeadForward(FBUF *b);
int TailForward(FBUF *b);
extern void CreateBuffer(FBUF *b, int len);*/
int PushBufferW(FBUF *b, WORD w);
int PopBufferW(FBUF *b);

//
int GetBuffer(FBUF *b);
int PutBuffer(FBUF *b, int d1);


#endif


