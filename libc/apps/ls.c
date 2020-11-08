// ----------------------------------------------------------------
// ls.c - List directory contents
// (C) 2002 by Jari Tuominen
// ----------------------------------------------------------------
#include <stdio.h>
#include <jtmos/filefind.h>

//
int MAX_LINES_TO_SHOW_AT_ONCE=22;
//#define DEBUGAPP
int dirPause=0;

// Dir function (ls)
// -----------------
//
// This function outputs directory from specified block(linear) to stdout.
int do_dir(void)
{
	//
	BYTE *buf,*p;
	WORD *wp;
	DWORD *dwp,i,i2,i3,i4,ii,ii2,ii3,ii4,l,sz;
	int k,t,key;
	DWORD ad;
	int current_block;
	char *str,*str2,*res1,*res2;
	int totalBytes,totalFiles,totalDirs;
	int lastone;
	FFBLK ff;
	int fd,last;

	//
	fd = findfirst("*", &ff);
	if(fd<=0)
	{
		printf("%s: Directory listing error. findfirst returned %d(0x%x)\n",
			__FUNCTION__, fd, fd);
		return 1;
	}
#ifdef DEBUGAPP
	printf("%s: I did findfirst\n", __FUNCTION__);
	printf("%s: fd = %d\n", __FUNCTION__, fd);
#endif

	// Warning, this must be freed afterwards:
	buf=malloc(16384);
	//
	str = malloc(256); str2 = malloc(256);
	res1 = malloc(256); res2 = malloc(256);
 
	//
	for(i=0,totalFiles=0,totalBytes=0,totalDirs=0,lastone=0;
			; i++)
	{
		// Pause when neccesary
		if(dirPause && (totalDirs+totalFiles)>=MAX_LINES_TO_SHOW_AT_ONCE)
		{
			i=0;
			printf("Press any key to continue ... ");
			key = getch();
			printf("\r                                 \r");
			if(key==27)return 0;
		}

		// Is it a ghost?->continue without notifying it (deleted file)
		// (ghost :  length == -1 )
		if( ff.ff_fsize==-1 ) goto emptyEntry;
  
		// --------- Start output string creation here: ----------
		strcpy(res1,""); strcpy(res2,"");

		// PRINT ENTRY NAME
		//
		// Note: I show filename so that all spaces show correctly,
		//       this requires putting the filename inside '"'s.

		//
		switch(ff.ff_attrib)
		{
			// DIRECTORY
			case JTMFS_ID_DIRECTORY:
			totalDirs++;
			sprintf(res1, "[%s]", ff.ff_name);
			break;
			// FILE
			default:
			sz = ff.ff_fsize;
			totalFiles++;
			totalBytes += sz;
			sprintf(res1, "%s", ff.ff_name);
			if(sz>=(1024*1024))
				sprintf(res2, "%dM", ff.ff_fsize/(1024*1024));
			if(sz>1023)
				sprintf(res2, "%dK", ff.ff_fsize/1024);
			else
				sprintf(res2, "%d bytes", ff.ff_fsize);
			break;
		}

		// Print strings in a style
		StretchPrint(res1, 30);
		StretchPrint(res2, 15);
		//
		printf("\n");

emptyEntry:
		// Receive next
		if( findnext(fd, &ff) )
			// End of directory
			break;
	}

	//
	StretchPrint("", 10);
	sprintf(res1, "%d File(s)",	totalFiles);
	StretchPrint(res1, 15);
	sprintf(res1, "%d bytes",	totalBytes);
	StretchPrint(res1, 15);
	printf("\n");
	StretchPrint("", 10);
	sprintf(res1, "%d Dir(s)",	totalDirs);
	StretchPrint(res1, 15);
	sprintf(res1, "%d bytes",	totalBytes);
	StretchPrint(res1, 15);
	printf("\n");
	//
	//printf("End of directory.\n");

	//
	free(buf); free(str); free(str2);
	free(res1); free(res2);

	//
	close(fd);
 
	//
	return 0;
}

// Parse arguments
int do_args(int argc, char **argv)
{
	//
	int i,i2,i3,i4;

	//
	for(i=1; i<(argc+1); i++)
	{
		// ENABLE PAUSE
		if(!strcmp(argv[i],"/p")
		|| !strcmp(argv[i],"-p")
		|| !strcmp(argv[i],"--pause"))
			dirPause=1;

		// HELP
		if(!strcmp(argv[i],"--help")
		|| !strcmp(argv[i],"-h")
		|| !strcmp(argv[i],"/?")
		|| !strcmp(argv[i],"/h"))
		{
			return 1;
		}
	}

	return 0;
}

void do_help(void)
{
	printf("Usage: ls [OPTIONS]... [FILE]...\n");
	printf("-p, --pause    Pauses after each screenful of information.\n");
	printf("-h, --help     Show this help page\n");
}

int main(int argc, char **argv)
{
	int opt;

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

	do_dir();
	return 0;
}

