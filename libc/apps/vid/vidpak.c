// vidpak.c video compressor
#include <stdio.h>
#include "vidpak.h"
#include "vidcomp.h"

//
void checkOptions(int argc, char **argv)
{
	int i;
	char str[256];

	//
	for(i=0; i<(argc+1); i++)
	{
		//
		if(!strcmp(argv[i], "-q1"))	QUALITY = 1;
		if(!strcmp(argv[i], "-q2"))	QUALITY = 2;
		if(!strcmp(argv[i], "-q3"))	QUALITY = 3;
		// Ignorance level (0-7)
		if(!strncmp(argv[i], "-i=", 3))
		{
			strcpy(str, argv[i]);
			strncpy(str, "   ", 3);
			sscanf(str, "%d", &IGNORE_BITS);
		}
		// Similiarity level (1-20)
		if(!strncmp(argv[i], "-s=", 3))
		{
			strcpy(str, argv[i]);
			strncpy(str, "   ", 3);
			sscanf(str, "%d", &SIM_LEVEL);
		}
		// Key frame interval (1-100)
		if(!strncmp(argv[i], "-k=", 3))
		{
			strcpy(str, argv[i]);
			strncpy(str, "   ", 3);
			sscanf(str, "%d", &keyFrameInterval);
		}
	}
}

// Main function
int main(int argc, char **argv)
{
	//
	if(argc<3)
	{
		//
		printf("Usage: %s [source video name] [output file] [options]\n", argv[0]);
		printf("Note: only still .raw images are accepted.\n");
		printf("Program will attempt to detect frame walking\n");
		printf("format automatically.\n");
		printf("Options:\n");
		printf("-i=[LEVEL]    ignorance level   (0-7)  recommended=3\n");
		printf("    (higher = better compression)\n");
		printf("-s=[LEVEL]    similiarity level (1-20) recommended=3\n");
		printf("    (lower = better compression)\n");
		printf("-k=[KEY FR.]  key frame interval (1-100)  recommended=10\n");
		printf("    (higher = better quality for high motion pictures)\n");
	}
	else
	{
		//
		checkOptions(argc, argv);

		// Compress
		compress(argv[1], argv[2]);
	}

	//
	return 0;
}



