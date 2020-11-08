//----------------------------------------
// Generic Audio Driver 1.0
// (C) 2003 by Jari Tuominen
//----------------------------------------
#include "kernel32.h"
#include "buffer.h" // FBUF
#include "audio.h"
#include "sb.h"
#include "gus.h"

//
FBUF aud;
AUDIO audio;

//
//	audio.length = len;
//	audio.frequency = 11025;
void SendAudio(BYTE *buf, int len, int freq)
{
	int i;

	// Output audio to the FIFO buffer
	for(i=0; i<len; i++)
		PutBuffer(&aud, buf[i]);
}

//
int audio_dev_call(void)
{
	return 0;
}

// Initialize generic audio driver.
void audio_init(void)
{
	//
	if(device("audio")>0)
	{
		printk("Cannot install generic audio driver: Driver already installed.");
		return;
	}

	//
	printk("Generic audio driver 1.0.\n");

	// 128K FIFO buffer to receive audio data
	CreateBuffer(&aud, SB_FIFO_BUFFER_SIZE);

	//
	driver_register_new_device("audio", audio_dev_call, DEVICE_TYPE_CHAR);
}

