// Audio Player Application
#include <stdio.h>
#include <jtmos/directdisk.h>
#include <jtmos/video.h>

// Request 1024K of RAM from the init process.
SYSMEMREQ(APPRAM_1024K)

//
typedef struct
{
	int len;
}AUD;
AUD au;

//
DWORD audioOffs=0;

//
int read_audio(const char *fname, BYTE *audio)
{
	int i,i2,i3,i4,devnr,d;
	FILE *f;

	//
	printf("Reading %s\n", fname);

	//
	f=fopen(fname, "rb");
	if(!f)
	{
		//
		printf("File not found.\n");
		return 1;
	}

	//
	for(i=0; ; i++)
	{
		//
		if( !(i&1023) ) { printf("."); }
		if( (d = fgetc(f))==EOF )break;
		audio[i] = d;
	}

	//
	au.len = i;
	fclose(f);
	printf("\nRead done.\n");
	return 0;
}

//
void PlayMoreAudio(BYTE *audio)
{
	//
	SendAudio(audio+audioOffs, 1024*32, 0, 11025);
	audioOffs+=1024*32;
	if( audioOffs>=au.len )audioOffs=0;
}

//
void WaitUser(BYTE *audio)
{
	int key;
	WORD pos;

	//
	printf("Press 'm' to send more audio data\n");
	printf("Press 'r' to init audio device(may be needed)\n");
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
		if(key=='r')
		{
			printf("Commencing audio device init ...               \n");
			InitAudio();
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

	if(argc<2)
	{
		printf("JTMOS audio demo - Cheering up the user.\n");
		printf("Will read audio from floppy & play it.\n");
		printf("Format: 8-bit, mono, 11 Khz\n");
		printf("Usage: rawplay [audio.raw]\n");
		return 0;
	}

	printf("Requires 512K RAM ");
	audio=malloc(1024*512);
	if(!audio) { exit(2); }
	memset(audio, 0, 1024*512);
	printf("\r                       \r");
	InitAudio();
	if( read_audio(argv[1], audio) )
				exit(3);
	play_audio(audio);
	free(audio);

	printf("Exit.\n");
	return 0;
}

