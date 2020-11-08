//===================================================================
// sqa.c - SQUIRREL ARCHIVE EXTRACTER.
// (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
//===================================================================
#include <stdio.h>
#include "sqa.h"
#define SECAREA (1024*2+seloffs)
//#define SQDSK_USEFLOPPY

//
SQDSK sqdsk;

// Quiet mode
int quiet=0;
char specific[256]={0};
char dstdir[256]={0};
int seloffs=0;

// Process header part of the archive.
int read_header(int fd)
{
	int i;

	// skip past secarea
	lseek(fd, SECAREA, SEEK_SET);

	// Read identifier
	read(fd, &sqdsk.identifier, 8);
	sqdsk.identifier[8]=0;

	// Check type
	if( strcmp(sqdsk.identifier,"SQARC520") )
	{
		return 1;
	}
	else
	{
	//	printf("\n[Archive identifier is valid.]\n");
	}

	//
	read(fd, &sqdsk.nr_files, 2);

	//
	if(sqdsk.nr_files >= 1000 || sqdsk.nr_files<0)
	{
		printf("\nnr_files might be corrupted, sqdsk.nr_files = %d (0x%x)\n",
			sqdsk.nr_files,
			sqdsk.nr_files);
		abort();
	}

	//
	return 0;
}

// Read entry descriptors (directory of all files)
int read_entrydescs(int fd)
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

	// Read all entries
	for(i=0; i<sqdsk.nr_files; i++)
	{
		// Read single entry
		read(fd, sqdsk.fe[i], sizeof(FE));
	}

	// Convert WORD to int.
	i4 = sqdsk.nr_files;
//	printf("Found %d files.\n", i4);
	return 0;
}

// Extract entries according file descriptors
int extract_entries(int fd)
{
	static int i,i2,i3,i4,d,len,sum,rot,fd1,fd2;
	static BYTE *temp,*tmp;
	static FE *e;
	static char *anim="|(\~-~/)";

	//
/*	printf("\n%s: sqdsk.nr_files = %d\n",
		__FUNCTION__,
		sqdsk.nr_files);*/

	// Go through all files.
	for(i=0,rot=0; i<sqdsk.nr_files; i++)
	{
		// Get entry
		e = sqdsk.fe[i];

		//
		if(strlen(specific) && strcmp(e->fname, specific))
		{
			// Seek forward
			lseek(fd, e->length, SEEK_CUR);
			continue;
		}

		// Get Length
		len = e->length;

		//------------------------------------------------------
		// Create a new file.
		fd2 = open(e->fname, O_TRUNC | O_CREAT);
		if(fd2<0)
		{
			//
			printf("Write error on file '%s'.\n", e->fname);
			break;
		}

		// Report
		printf("%s (%dK) ",
			e->fname, len/1024);

		//
		tmp = malloc(len);
		read(fd, tmp, len);
		write(fd2, tmp, len);

		// Compare checksum values
		if( (sum=syschksum(tmp,len))!=e->chksum )
		{
			// Report on checksum mistach
			printf("(2) Checksum failure on EXTRACTED file %s: %x != %x\n",
				e->fname, sum,e->chksum);
		}

		//
		free(tmp);

		//
		printf("\r                                                    \r");
		// Close output stream
		close(fd2);

		//-------------------------------------------------------------
		// Read written file to memory & get check chksum
		//
		temp = malloc(len);
		printf("Touching file %s. ", e->fname);
		fd1 = open(e->fname, 0);
		if(fd1<0)
		{
			//
			printf("\rUnexpected error while trying read written file %s.\n",
				e->fname);
			break;
		}
		printf("\r                                     \r");
		read(fd1,temp,len);
		close(fd1);

		// Compare checksum values
		if( (sum=syschksum(temp,len))!=e->chksum )
		{
			// Report on checksum mistach
			printf("Checksum failure on WRITTEN file %s: %x != %x\n",
				e->fname, sum,e->chksum);
		}

		//-----------------------------------------------------
		// Free temp buffer
		free(temp);
nowrite:
		while(!1);
	}

	//
	if(!strlen(specific))
		printf("%d file(s) produced.\n", i);

	//
	return 0;
}

// Returns 0 if no error, non-zero if error.
int extract_archive(char *fname, char *spec)
{
	int fd;
	int rv;

	//
	strcpy(specific, spec);

	//
	memset(&sqdsk, 0, sizeof(SQDSK));

	//
	rv=0;

	//
	fd = open(fname, 0);
	if(fd<0)
	{
		printf("File not found '%s'.\n", fname);
		return 3;
	}
	if(read_header(fd))
	{
		printf("error rv=1\n");
		rv=1;
		goto skip;
	}

	//
	if(sqdsk.nr_files<=0)
	{
		printf("NO FILES FOUND (%d).\n",
			sqdsk.nr_files);
		rv=2;
		goto skip;
	}

	//
	read_entrydescs(fd);

	//
	extract_entries(fd);

skip:
	//
	close(fd);
	return rv;
}

