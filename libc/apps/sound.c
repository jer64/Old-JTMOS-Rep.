// Audio Player Application
#define LOAD_AMOUNT_B		1440*1024
#define DEV_NAME "floppy"
#include <stdio.h>
#include <jtmos/directdisk.h>
#include <jtmos/video.h>

DWORD audioOffs=0;

void read_audio(BYTE *audio)
{
	int i,i2,i3,i4,devnr;
	float fa,fb,fc,fd;

	//
	printf("Calculating audio ... ");
	for(i=0,fb=1; i<(LOAD_AMOUNT_B); i++,fb+=.0001)
	{
		//
		fa = i;
		fa=fa/fb;
		i4=fa;

		//
		audio[i] = i4&255;
	}
	printf("\n");
}

void PlayMoreAudio(BYTE *audio)
{
	SendAudio(audio+audioOffs, 1024*32, 0, 11025);
	audioOffs+=1024*32;
	if( audioOffs>=LOAD_AMOUNT_B )audioOffs=0;
}

void WaitUser(BYTE *audio)
{
	int key;
	int pos;

	printf("Press 'm' to send more audio data\n");
	printf("Press 'z' to play from start\n");
	printf("Press ESC to quit\n");
	while(key!=27)
	{
		key=getch1();
		printf( "Audio position = 0x%x      \r", (pos=GetAudioPosition()) );
		if(key=='z')
		{
			audioOffs=0;
			PlayMoreAudio(audio);
		}
		if(key=='\n' || key=='m' || pos>=0xff00)
		{
			//printf("Sending more audio to play ...                \n");
			PlayMoreAudio(audio);
		}
	}
	printf("User pressed ESC key, quiting ...                  \n");
}

void play_audio(BYTE *audio)
{
	int i,i2,i3,i4;

	printf("Press any key to play audio \n");
	getch();

	PlayMoreAudio(audio);

	WaitUser(audio);
}

int main(int argc, char **argv)
{
	BYTE *audio;

	printf("JTMOS audio demo - Cheering up the user.\n");
	printf("Will read audio from floppy & play it.\n");
	printf("Format: 8-bit, mono, 11 Khz\n");

	audio=malloc(LOAD_AMOUNT_B);
	read_audio(audio);
	play_audio(audio);
	free(audio);

	printf("Exit.\n");

	return 0;
}

