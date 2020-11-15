#ifndef __INCLUDED_FILEBUF_H__
#define __INCLUDED_FILEBUF_H__

//
#include "fsdos.h"
#include "filedef.h"

// Buffer setting
// big = fast for large files   (e.g. 64K)
// small = fast for small files (e.g. 8K)
#define PRE_BUF_AMOUNT_K        24

// Obsolete (just ignore this value)
#define BUFFER_MORE_K           64

//
void setbuf(FILE *stream, char *buf);
void setbuffer(FILE *stream, char *buf, size_t size);
void setlinebuf(FILE *stream);
int setvbuf(FILE *stream, char *buf, int mode , size_t size);
int fputc(int c, FILE *stream);
int fgetc(FILE *stream);
int __fenlargebuf(FILE *stream, int newsz);
//int frefresh_write(FILE *stream);
//int frefresh(FILE *stream);
int rbuf(FILE *stream);
int wbuf(FILE *stream);

#endif

