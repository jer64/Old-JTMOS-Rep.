// =========================================================
// SQUIRREL DISK EXTRACTER APPLICATION (FOR KERNEL)
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
// =========================================================
#include <stdio.h>
#include <jtmos/directdisk.h> // readblock,writeblock,getdevnrbyname
#define SECAREA 1024*2
//#define SQDSK_USEFLOPPY

// 40 bytes description entry
typedef struct
{
	char fname[31];
	int offset,length;
}FE;

// Quiet mode
static int quiet=0;
static char specific[256];

//
struct
{
	int nr_files;
	FE **fe;
	char identifier[20];
}sqdsk;

// Area selection offset (A or B)
int seloffs=0;
//
int usingDevice=0;
int deviceNr=0;
char tmp[512]; // for block buffer
int blockNr=-1; // current block
int blockOffs=0; // current offset
int dataOffs;

//---------------------------------------------------------
//
void block_seek(int offs)
{
	// Set offset
	blockOffs = offs+seloffs;
	// Force re-read on next operation
	blockNr=-1;
}

//
void read_location(void)
{
	blockNr = blockOffs/512;
	readblock(deviceNr, blockNr, tmp);
}

//
int block_getchar(void)
{
	int chr;

	if( blockNr==-1 || blockNr!=(blockOffs/512) )
	{
		read_location();
	}

	chr = tmp[blockOffs & 0x1FF];
	blockOffs++;
	return chr;
}

//---------------------------------------------------------
//
int read_header(FILE *f)
{
	int i;

	// skip past secarea
	if(!usingDevice)
		fseek(f, SECAREA, SEEK_SET);
		else
		block_seek(SECAREA);

	if(usingDevice)
	{
		for(i=0; i<8; i++) sqdsk.identifier[i]=block_getchar();
	}
	else
		fread(sqdsk.identifier, 8,1, f);
	sqdsk.identifier[8]=0;

	if( strcmp(sqdsk.identifier,"SQARC520") )
	{
		//printf("Not a valid squirrel archive.\n");
		return 1;
	}
	else
	{
		//printf("Archive format identified correctly.\n");
	}

	if(usingDevice)
	{
		sqdsk.nr_files = block_getchar() | (block_getchar()<<8);
	}
	else
	{
		sqdsk.nr_files = fgetc(f) | (fgetc(f)<<8);
	}

	return 0;
}

//
int read_entrydescs(FILE *f)
{
	int i,i2,i3,i4;
	char *t;

	// Allocate & init
	sqdsk.fe = malloc(8*sqdsk.nr_files);
	for(i=0; i<sqdsk.nr_files; i++)
	{
		sqdsk.fe[i] = malloc(sizeof(FE));
		memset(sqdsk.fe[i], 0, sizeof(FE));
	}

	for(i=0; i<sqdsk.nr_files; i++)
	{
		if(usingDevice)
		{
			t = sqdsk.fe[i];
			for(i2=0; i2<sizeof(FE); i2++)
			{
				t[i2] = block_getchar();
			}
		}
		else
		{
			fread(sqdsk.fe[i], sizeof(FE),1, f);
		}
	}

	dataOffs = blockOffs;

	if(!quiet)
		printf("Found %d files.\n", sqdsk.nr_files);
	//printf("Figure: Data offset is at 0x%x.\n", dataOffs);
	return 0;
}

//---------------------------------------------------------
// Extract entries
int extract_entries(FILE *f)
{
	int i,i2,i3,i4,d,extractThis;
	FILE *f2;

	//
	for(i=0; i<sqdsk.nr_files; i++)
	{
		//
		f2=fopen(sqdsk.fe[i]->fname, "rb");
		if(f2)
		{
			//
			fclose(f2);
			goto overwrite;

			if(!quiet)
				printf("\nSkipping %s\n", sqdsk.fe[i]->fname);
			// Don't overwrite:
			for(i4=0; i4<sqdsk.fe[i]->length; i4++)
			{
				if(!usingDevice)d=fgetc(f);
					else d=block_getchar();
			}
			goto nowrite;
		}

overwrite:
		// Extract a specific file?
		if(strlen(specific))
		{
			// Is this the file?
			if(!strcmp(sqdsk.fe[i]->fname, specific))
				extractThis = TRUE;
				else
				extractThis = FALSE;
		}
		// No specific file [pattern] defined, extract all:
		else	extractThis = TRUE;

		// Extract this file ?
		if(extractThis)
		{
			//
			f2=fopen(sqdsk.fe[i]->fname, "wb");
			if(f2==NULL)
			{
				printf("Write error on file '%s'.\n", sqdsk.fe[i]->fname);
				break;
			}
		}
		else	f2=NULL;

		// Report only if requested
		if(quiet==FALSE && f2!=NULL)
			printf("Writing %s (%dK) ",
				sqdsk.fe[i]->fname,
				sqdsk.fe[i]->length/1024);
		/*	printf("Writing %s (size=%d bytes, offset=0x%x) ",
				sqdsk.fe[i]->fname,
				sqdsk.fe[i]->length, sqdsk.fe[i]->offset);*/

		// If output stream is open, then let's output to it
		if(f2)
		{
			for(i4=0; i4<sqdsk.fe[i]->length; i4++)
			{
				//
				if(!usingDevice)d=fgetc(f);
				else d=block_getchar();
	
				//
				if(!usingDevice && d==EOF)break;
				fputc(d,f2);
	
				//
				if(!quiet)
					if( !(i4&1023) ) printf(".");
			}

			// Report only when requested
			if(quiet==FALSE)
				printf("\n");
	
			// Close output stream
			fclose(f2);
	
			// *Temporary solution* Set byte fixed size.
			_fsetsize(sqdsk.fe[i]->fname, sqdsk.fe[i]->length);
		}

nowrite:
	}
}

//---------------------------------------------------------
//
int extract_archive(const char *fname, const char *dstdir)
{
	FILE *f;
	int rv;

	//
	rv=0;

	// Change to output directory if any specified
	if(strcmp(dstdir,""))
		chdir(dstdir);

	//
	if(!usingDevice)
	{
		f=fopen(fname,"rb");
		if(!f)
		{
			printf("File not found '%s'.\n", fname);
			if(strlen(dstdir))
			{
				printf("WARNING: option '-o' disables 'current path' -setting for source files\n");
				printf("Specify source file(if any) with FULL PATH.\n");
			}
			return 1;
		}
	}

	// Read archive header
	if(read_header(f))
	{
		// Error?
		rv=1;
		goto skip;
	}

	// Check file count
	if(sqdsk.nr_files<=0)
	{
		// No files
		printf("No files found, invalid archive?\n");
		rv=2;
		goto skip;
	}

	// Read entry descriptions
	read_entrydescs(f);

	// Extract all files
	extract_entries(f);

skip:
	// Close stream if any used
	if(!usingDevice) fclose(f);
	return rv;
}

//---------------------------------------------------------
// SQDSK main functiono
//
int main(int argc,char **argv)
{
	int err,i,i2,i3,i4;
	char *s;
	static char *devs[]={"floppy:", "hda:", "ram:"};
	static int n_devs=1;
	static char dstdir[256];

	// No destination specified as default
	strcpy(dstdir, "");

	// No specific file to extract as default (or file pattern)
	strcpy(specific, "");

	// Search for options I
	for(i=0; i<(argc+1); i++)
	{
		// Quiet mode
		if(!strcmp(argv[i], "-q"))
			quiet = 1;
	}

	// Search for options II
	for(i=0; i<argc; i++)
	{
		// Choose output directory
		if(!strcmp(argv[i], "-o"))
		{
			strcpy(dstdir, argv[i+1]);
		}

		// Choose specific file to extract
		if(!strcmp(argv[i], "-u"))
		{
			strcpy(specific, argv[i+1]);
		}
	}

	//
#ifndef SQDSK_USEFLOPPY
	if(argc<2)
	{
		printf("Squirrel Archive Extractor 1.0\n");
		printf("(C) 2002-2003 by Jari Tuominen(jari@vunet.org)\n");
		printf("Usage: sqdsk [source archive name or source device] [options]\n");
		printf("Examples:\n");
		printf("  sqdsk ramdisk:\n");
		printf("  sqdsk floppy:\n");
		printf("  sqdsk sqdsk.sqa\n");
		printf("Options:\n");
		printf("  -q  quiet mode, no screen output\n");
		printf("  -o  define destination directory\n");
		printf("      e.g. -o /tmp\n");
		return 0;
	}
#endif

	i=0;
loop:
	// Get device string
#ifdef SQDSK_USEFLOPPY
	s = devs[i];
	if(!quiet)
		printf("%s\n", s);
#else
	s=argv[1];
#endif
	// Is it a device?
	if( s[strlen(s)-1]==':' )
	{
		// Select device
		s[strlen(s)-1]=0;
		deviceNr = getdevnrbyname(s);
		blockNr=-1;
		blockOffs=0;
		usingDevice=1;
	}
	else
	{
		// No device
		usingDevice=0;
	}

	// Offset to 0
	seloffs=0;
	// Call extracting function
	err=extract_archive(argv[1], dstdir);
	// Error?
	if(!err)goto endi;
	// Accepting a device?
	if(usingDevice)
	{
		// Let's go to 640K offset
		seloffs=1024*640;
		// Call extract function
		err=extract_archive(argv[1], dstdir);
		// If error, try next device on list
		if(err) { i++; if(i>=n_devs)goto endi; goto loop; }
	}

endi:
	// Error reporting
	if(err)
	{
		// Report error
		printf("Error %d.\n", err);
	}
	
	return 0;
}

