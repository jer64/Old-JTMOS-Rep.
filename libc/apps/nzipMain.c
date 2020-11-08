///////////////////////////////////////////////////////////////////////////////////////
// nzipMain.c
///////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "nzip.h"

// Main function
int main(int argc, char **argv)
{
#ifdef JTMOS
	FFBLK ff;
#endif
	int fd,i,i2,l;
	char nn[256];

#ifndef UNCOMPRESS_ALL
	//
	if(argc<2)
	{
		printf("Usage: %s [target file]\n",
			argv[0]);
		printf("pack: nzip [file]\n");
		printf("unpack: nzip -d [file]\n");
		return 0;
	}
#endif

	//
//////////////////////////////////////////////////////////////////////
// unz:
#ifdef UNCOMPRESS_ALL
	fd = findfirst("*", &ff);
	if(fd<=0)
	{
		printf("%s: Unable to get directory listing.\n",
			__FUNCTION__);
		goto past;
	}

	// Loop through all files, search for *.nz files
	while(1)
	{
		// Uncompress all files
		if( ff.ff_fsize==-1 ) goto past2;

		// Check for *.nz files
	//	printf("%s ", ff.ff_name);
		l = strlen(ff.ff_name);
		if(	ff.ff_name[l-3]=='.' &&
			ff.ff_name[l-2]=='n' &&
			ff.ff_name[l-1]=='z')
		{
			// Uncompress file
			uncompress(ff.ff_name);

			// Image?
			if(strstr(ff.ff_name, ".img"))
			{
				// Remove .nz type
				unNutFname(nn, ff.ff_name);

				// Extract
				seloffs = 0;
				printf("%s\n", nn);
				extract_archive(nn, "");
			}
		}

past2:
		// Next file or end of dir
		if( findnext(fd, &ff) )break;
	}

	// If install.img exists, extract it
repeat0:
	sleep(1);
past:

#else
//////////////////////////////////////////////////////////////////////
// nzip:
	//
	if(!strcmp(argv[1],"-d"))
	{
		// Uncompress a single file
		uncompress(argv[2]);
	}
	else
	{
		//
		compress(argv[1]);
	}
#endif
	return 0;
}


