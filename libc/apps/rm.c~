// ----------------------------------------------------------------
// rm.c - remove files or directories
// (C) 2002 by Jari Tuominen
// ----------------------------------------------------------------
#include <stdio.h>
#include <jtmos/filefind.h>

//
int MAX_LINES_TO_SHOW_AT_ONCE=22;
//#define DEBUGAPP
int dirPause=0;

// Remove files
int do_rm(const char *search)
{
	//
	BYTE *buf,*p;
	WORD *wp;
	DWORD *dwp,i,i2,i3,i4,ii,ii2,ii3,ii4,l;
	int k,Type,key;
	DWORD ad;
	int current_block;
	char *str,*str2,*res1,*res2;
	int totalBytes,totalFiles,totalDirs;
	int lastone;
	FFBLK ff;
	int fd,last;

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

	//
	for(i=0,totalFiles=0,totalBytes=0,totalDirs=0,lastone=0;
			; i++)
	{
		// Is it a ghost?->continue without notifying it (deleted file)
		// (ghost :  length == -1 )
		if( ff.ff_fsize==-1 ) goto dontRemoveThisOne;
  
		// Empty filename? -> End of dir.
		if( !strlen(ff.ff_name) )
		{
			break;
		}

		// When all files are assigned
		if(!strcmp(search, "*"))goto rmThisOne;

		// Check pattern match
		if( strstr(ff.ff_name, search)==NULL )
			// Pattern doesn't match
			goto dontRemoveThisOne;
rmThisOne:
		// Remove the file
		remove(ff.ff_name);
dontRemoveThisOne:

		//
		if(lastone)break;

		// Receive next
		if( findnext(fd, &ff) )lastone=1;
	}

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
	printf("Usage: rm [OPTIONS]... [FILE]...\n");
	printf("-f, --force         ignore nonexistent files, never prompt\n");
	printf("-r, --recursive     remove the contents of directories recursively\n");
}

int main(int argc, char **argv)
{
	int opt;

	if(argc<2)
	{
		// When no arguments:
		do_help();
		return 0;
	}

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
	do_rm(argv[1]);
	return 0;
}

