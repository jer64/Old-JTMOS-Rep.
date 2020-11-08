#ifndef __INCLUDED_AUDIO_H__
#define __INCLUDED_AUDIO_H__

//
#include "kernel32.h"
#include "buffer.h" // FBUF

//
typedef struct
{
	int length;
	int frequency;
}AUDIO;
extern AUDIO audio;
extern FBUF aud;

//
void SendAudio(BYTE *buf, int len, int freq);
void audio_init(void);

#endif
