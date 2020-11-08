#ifndef __INCLUDED_AUDIO_H__
#define __INCLUDED_AUDIO_H__

void InitAudio(void);
DWORD GetAudioPosition(void);
void SendAudio(const char *audioBuf, int audioLength,
                int audioType, int audioFreq);

#endif
