// vplay.c - Video player
// (DJGPP version)
#include <stdio.h>
#include "vplay.h"

//
int fastMode=0;

// Video player main
int main(int argc, char **argv)
{
	int i;

	//
	if(argc<2)
	{
		//
		printf("Usage: %s [video.jov] [options]\n",
			argv[0]);
		printf("Options:\n");
		printf(" --fast   play video incredibly fast\n");
		return 0;
	}

	//
	for(i=1; i<(argc+1); i++)
	{
		//
		if(!strcmp(argv[i],"--fast"))
			fastMode=1;
	}

	//
	startPlayer(argv[1]);

	//
	return 0;
}

