//====================================================================
// JTMOS Audio Player Application
// (C) 2003 by Jari Tuominen
//====================================================================
#define LOAD_AMOUNT_B		(1440*1024)
#include <stdio.h>
#include <jtmos/directdisk.h>
#include <jtmos/video.h>
// How much of audio data to keep at the buffer?
#define KEEPATLEAST		(64*1024)

//
#ifdef JTMOS
// Request 2048K of RAM from the init process.
SYSMEMREQ(APPRAM_2048K)
#endif

//
DWORD audioOffs=0;
// Number of bytes loaded into buffer.
int l_data;

// Play audio from disk
int read_audio_file(const *fname, BYTE *audio)
{
	int i,i2,i3,i4,d;
	FILE *f;

	//
	memset(audio, 0x80, LOAD_AMOUNT_B);

	//
	f=fopen(fname, "rb");
	if(f==NULL)
	{
		//
		printf("File not found: %s\n",
			fname);
		return 1;
	}

	//
	printf("Reading audio data from %s . . ",
		fname);

	// Load until max. bytes received or end of file.
	for(i=0; i<LOAD_AMOUNT_B; i++)
	{
		// Read byte.
		if( (d = fgetc(f))==EOF )
			break;

		// Store data.
		audio[i] = d;
	}

	//
	l_data = i;

	//
	fclose(f);

	//
	printf("\n%d bytes of audio data loaded.\n",
		l_data);

	//
	return 0;
}

// Play more audio
void PlayMoreAudio(BYTE *audio)
{
	// Play more audio
	SendAudio(audio+audioOffs, 1024*32, 0, 11025);
	audioOffs+=1024*32;
	if( audioOffs>=l_data )audioOffs=0;
}

void WaitUser(BYTE *audio)
{
	int key;
	int pos;

	//
	printf("Press 'm' to send more audio data.\n");
	printf("Press 'z' to play from start.\n");
	printf("Press ESC to quit.\n");

	//
	do
	{
		// Input user keyboard
		key=getch1();

		// Report audio position
		pos = GetAudioPosition();
		printf("Audio buffer has %d bytes     \r", pos);

		// Keep atleast 64K of audio at the buffer
		if( pos<KEEPATLEAST )
		{
		//	printf("--- SENDING MORE AUDIO ---             \n");
			PlayMoreAudio(audio);
		}

		// Check key pressings
		if(key=='z')
		{
			audioOffs=0;
			PlayMoreAudio(audio);
		}
		if(key=='\n' || key=='m')
		{
			//printf("Sending more audio to play ...                \n");
			PlayMoreAudio(audio);
		}
	}while(key!=27);

	//
	printf("User pressed ESC key, quiting ...                  \n");
}

// Play audio
void play_audio(BYTE *audio)
{
	int i,i2,i3,i4;

	//
	PlayMoreAudio(audio);

	//
	WaitUser(audio);
}

// Main function.
int main(int argc, char **argv)
{
	BYTE *audio;
	char fname[256];

	// Empty file name
	strcpy(fname, "");

	// Check argument count
	if(argc<2)
	{
		printf("JTMOS audio demo\n");
		printf("source files must be 11025 Hz 8-bit mono PCM audio\n");
		printf("Usage: %s [audio file.raw or device]\n",
			argv[0]);
		return 0;
	}

	// Pick file name
	strcpy(fname, argv[1]);

	// Allocate audio buffer
	audio = malloc(LOAD_AMOUNT_B);

	// Try to read from device first
	if( read_audio_file(fname, audio) )
	{
		return 1;
	}

	// Let's play it
	play_audio(audio);

	// Free audio buffer
	free(audio);

	// Report
	printf("Exit.\n");

	// Exit
	return 0;
}

