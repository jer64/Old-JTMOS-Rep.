//=====================================================================
// Save drive as an image file
// (C) 2003 by Jari Tuominen
//=====================================================================
#include <stdio.h>

//
void readfloppy(const char *devname, const char *fname)
{
	FILE *f,*f2;
	char *tmp;
	int i,devnr;

	// Get device number
	devnr = getdevnrbyname(devname);

	// Allocate block buffer
	tmp=malloc(1024*64);

	//
	f=fopen(fname, "wb");
	if(!f) { printf("%s: Write error.\n", __FUNCTION__); return; }

	//
	printf("Writing image on %s ...\n", fname);
	for(i=0; i<2880; i++)
	{
		printf("Block %d/2880        \r", i);
		readblock(devnr, i, tmp);
		fwrite(tmp, 512,1, f);
	}

	//
	free(tmp);
	fclose(f);
}

// Main function
int main(int argc, char **argv)
{
	//
	if(argc<3)
	{
		printf("readfloppy - reads 1.44M floppy disk image out of specified device\n");
		printf("(C) 2002-2003 by Jari Tuominen\n");
		printf("Usage: %s [source device] [output image file name]\n",
			argv[0]);
		printf("Example: readfloppy floppy floppy.img\n");
		return 0;
	}

	//
	readfloppy(argv[1],argv[2]);
	//
	printf("%s: done.            \n", argv[0]);

	//
	return 0;
}


