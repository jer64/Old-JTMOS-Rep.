// ----------------------------------------------------------------
// mkfs.c - make a JTMOS file system
// (C) 2002-2003 by Jari Tuominen
// ----------------------------------------------------------------
#include <stdio.h>
#include <jtmos/filefind.h>

// Make JTMOS file system
int do_mkfs(const char *path)
{
	int r;

	//
	printf("Formatting drive %s: ... ", path);
	if( (r = formatDrive(path)) )
	{
		// Failure:
		printf("\rInvalid device specified: '%s:'        \n", path);
	}
	else
	{
		// Success:
		printf("\rFormatting of drive %s: completed.      \n", path);
	}	

	//
	return 0;
}

// Parse arguments
int do_args(int argc, char **argv)
{
	int i,i2,i3,i4;

	//
	for(i=1; i<(argc+1); i++)
	{
	}

	//
	return 0;
}

//
void do_help(void)
{
	//
	printf("Usage: mkfs device\n");
	printf("Examples:\n");
	printf("mkfs ram\n");
	printf("mkfs sys\n");
	printf("mkfs floppy\n");
	printf("mkfs hda\n");
}

//
int main(int argc, char **argv)
{
	int opt;

	//
	if(argc<2)
	{
		// When no arguments:
		do_help();
		return 0;
	}

	//
	switch( (opt=do_args(argc, argv)) )
	{
		// Regular
		case 0:
		break;

		// Show help page
		case 1:
		do_help();
		return 0;
	}

	//
	do_mkfs(argv[1]);
	return 0;
}

