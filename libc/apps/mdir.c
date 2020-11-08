//===================================================
// MS-DOS DIRECTORY DISPLAYER
// (C) 2003 by Jari Tuominen
//===================================================
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "msdos.h"

// Check whether if device has FAT FS
int isFATFS(const char *src)
{
	char id[10];
	FILE *f;

	//
	f=fopen(src, "rb");
	if(f==NULL)
		return FALSE;

	// Pick up the ID
	fseek(f, 0x36, SEEK_SET);
	fread(&id, 6, 1, f);
	id[5]=0;

	// Check ID
//	printf("ID = '%s'\n", id);

	// Supported FAT12
	if(!strcmp(id, "FAT12"))
		return TRUE;

	// FAT16 not supported
	if(!strcmp(id, "FAT16"))
		return FALSE;

	// FAT32 not supported
	if(!strcmp(id, "FAT32"))
		return FALSE;

	//
	return FALSE;
}

// MS-DOS directory displayer
int MSDOSDir(const char *src, const char *path)
{
	FILE *f;
	FFE *e;
	int i,i2,i3,i4,files,dirs;

	//
	files = 0;
	dirs = 0;

	//-------------------------------------------------
	// Check file system
	//
	if(isFATFS(src)==FALSE)
		return 1;

	//-------------------------------------------------
	// Open device
	//
	f=fopen(src, "rb");
	if(f==NULL)
		return 2;

	//-------------------------------------------------
	// Seek to root directory
	// (only root directory supported by now)
	fseek(f, 0x2600, SEEK_SET);

	//-------------------------------------------------
	// Display entry by entry

	//
	e = malloc(32);

	//
	for(i=0; ; i++)
	{
		//-------------------------------------
		// Ready single entry
		fread(e, 32, 1, f);

		// End?
		if(e->name[0]==0)
			break;

		// Skip deleted entries
		if(e->name[0]==0xE5)
			continue;

		//-------------------------------------
		// Display entry:

		switch(e->attr)
		{
			// Drive label
			case 0x28:
			printf(" Volume in drive [%s] is ",
				src);
			for(i2=0; i2<8; i2++)
				printf("%c", e->name[i2]);
			printf("\n\n Directory of %s\n",
				path);
			break;

			// Display file name
			default:
			for(i2=0; i2<8; i2++)
				printf("%c", e->name[i2]);
	
			// Display extension
			printf(".");
			for(i2=0; i2<3; i2++)
				printf("%c", e->ext[i2]);
	
			// Display size and attribute
			printf("  ATTR=%x  %d bytes",
				e->attr, e->size);
			files++;
			break;
		}

		//
		printf("\n");
	}

	//
	printf("        %d File(s)\n",	files);
	printf("        %d Dir(s)\n",	dirs);

	//
	free(e);

	//-------------------------------------------------
	fclose(f);

	//
	return 0;
}

// Main function
int main(int argc, char **argv)
{
	int err;
	char *errors[]={
		"no error",
		"drive is not MS-DOS formatted",
		"unable to access device"
			};

	//----------------------------------------------
	// Check argument count
	if(argc<2)
	{
		//
		printf("Usage: %s [drive]\n",
			argv[0]);
		printf("example #1: %s floppy\n", argv[0]);
		printf("example #2: %s dos.img\n", argv[0]);
		return 0;
	}

	//----------------------------------------------
	// Get directory
	err = MSDOSDir(argv[1], "/");

	//----------------------------------------------
	if(err)
		printf("%s: %s\n",
			argv[0], errors[err]);

	//----------------------------------------------
	return 0;
}
