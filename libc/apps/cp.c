// cp.c - Copy files
#include <stdio.h>
#include <jtmos/filefind.h>
#define TRACE //

//
#ifdef JTMOS
// Request 2048K of RAM from the init process.
SYSMEMREQ(APPRAM_2048K)
#endif

// ------ OPTIONS ----------------------------------
// Verbose mode, output more stuff ON/OFF
int verbose;
int autoPause;
// -------------------------------------------------

// Copy file from [source file] to [destination path]
int copyFile(const char *src, const char *dstpath)
{
	FILE *s,*o;
	int l,i;
	char *tmp;
	static char dstname[256];

	// Build up a destination path+name string
	// e.g. /usr/bin/lala.bin 
	// from
	// (dst)"/usr/bin" (src)"lala.bin"
	sprintf(dstname, "%s/%s", dstpath, src);

	// Open source and output
	TRACE("A"); //
	s = fopen(src, "rb");
	if(s==NULL)	return 1;
	TRACE("B"); //
	o = fopen(dstname, "wb");
	if(o==NULL)	{	fclose(s); return 1;	}

	// Get size of source
	TRACE("C"); //
	l = fsizeof(src);

	// Perform data copy if needed
	if(l>0)
	{
		//
		for(i=0; i<l; i++)
			fputc(fgetc(s), o);
	}

	// Close both streams
	TRACE("G"); //
	fclose(s);
	TRACE("H"); //
	fclose(o);

	TRACE("I"); //
	//
	if(l>0)
		free(tmp);

	//
	return 0;
}

// Perform copy operation
int copy_now(const char *src, const char *dst)
{
	static FFBLK ff;
	int fd,i,loppu,ex,copied;
	static char srcPath[256];
	static char name[256];

	// Get current work directory
	getcwd(srcPath, 250);

	//
	fd=findfirst(src, &ff);
	if(!fd)
	{
		printf("No files found(%s).\n", src);
		return;
	}

	for(i=1,loppu=0,copied=0; ; i++)
	{
		TRACE("1"); //
		// Get name 
		strcpy(name, ff.ff_name);

		TRACE("2"); //
		// Determine what it is?
		ex = fexist(name);
		if( ex==2 || !ex || ex==-1 )
		{
			// Skip directories, deleted files and others:
			goto skip;
		}

		//
		TRACE("3"); //
		if(verbose)	printf("copying %s ", name);
		else
		{
				SwitchToThread();
		}

		// Perform copy operation
		TRACE("4"); //
		if( copyFile(name, dst) )	// source file => output
		{
			printf("\nError: Cannot copy file '%s'\n",
				name);
		}
		else copied++;

		//
		TRACE("5"); //
		if(verbose)	printf("\n");
		else
				SwitchToThread();

		// ----------------------------------
skip:
		//
		TRACE("6"); //

		// Get next file
		if( findnext(fd, &ff) )
			// End of directory
			break;

		//
		TRACE("7"); //
		// Allow user to break copying with ESC
		if(getch1()==27)break;
	}

	//
	if(verbose)	printf("%d File(s)\n", i);
	return 0;
}

// Parse arguments
int do_args(int argc, char **argv)
{
	int i,i2,i3,i4;

	//
	for(i=1; i<(argc+1); i++)
	{
		// ENABLE VERBOSE
		if(!strcmp(argv[i],"/v")
		|| !strcmp(argv[i],"-v")
		|| !strcmp(argv[i],"--verbose"))
			verbose=1;

		// ENABLE PAUSE
		if(!strcmp(argv[i],"/p")
		|| !strcmp(argv[i],"-p")
		|| !strcmp(argv[i],"--pause"))
			autoPause=1;

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


int main(int argc, char **argv)
{
	// OPTIONS
	verbose =	0;
	autoPause =	0;

	//
	do_args(argc, argv);

	// Check argument count
	if(argc<3)
	{
		printf("%s: missing file arguments\n", argv[0]);
	//	printf("Try `%s --help' for more information.\n", argv[0]);
		return 0;
	}

	// Perform operation
	return copy_now(argv[1], argv[2]);
}

