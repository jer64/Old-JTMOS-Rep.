// ======================================================
// LIBC Audio Functions
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
// ======================================================
#include <stdio.h>

//
void InitAudio(void)
{
	static POSTMANCMD post;
	int ack;

	//
	post.id=PCMD_INITAUDIO;
	PostmanPost(&post);
}

// Returns:
// Amount of bytes left to play.
// Aka location reversed.
//
DWORD GetAudioPosition(void)
{
	static POSTMANCMD post;
	int ack;

	//
	post.id=PCMD_GETAUDIOPOSITION;
	return PostmanPost(&post);
}

// SendAudio
//
// Example:
//	SendAudio(myRawAudioData, 1024*32, 0, 11025);
//
void SendAudio(const char *audioBuf, int audioLength,
		int audioType, int audioFreq)
{
	static POSTMANCMD post;
	int ack;

	//
	post.id=PCMD_SENDAUDIO;
	post.par1=audioBuf;
	post.v1=audioLength;
	post.v2=audioType;
	post.v3=audioFreq;
	PostmanPost(&post);
}

//

