// cp.c - Validate executable files
#include <stdio.h>
#include <jtmos/filefind.h>
#define TRACE //

// ------ OPTIONS ----------------------------------
// Verbose mode, explains what it does to the user
int verbose;
int autoPause;
// -------------------------------------------------

// Determine whether if the binary in buffer ptr is JTMOS EXE or not?
int isExeBinary(BYTE *buf, int l)
{
        static char *det="[JTMOS LIBC (C) 2002 Jari Tuominen]";
        static char *tail="[VALID JTMOS EXECUTABLE BINARY FILE FCE2E37BE38BA000]";
        static int i,i2,i3,i4,ok;

        // Find first string
        for(i=0,ok=0,i2=0; i<200; i++)
        {
                if(det[i2]==0){ok=1; break;}
                if(buf[i]==det[i2])
                {
                        i2++;
                }
                else i2=0;
        }
        if(!ok)
        {
                TRACE("%s: First EXE identification string not found.\n",
                        __FUNCTION__);
                return FALSE;
        }

        // Find second string(tail)
        for(i=0,i2=0,ok=0; i<(l+1); i++)
        {
                if(tail[i2]==0){ok=1; break;}
                if(buf[i]==tail[i2])
                {
                        i2++;
                }
                else i2=0;
        }
        if(!ok)
        {
                TRACE("%s: Second EXE identification string not found.\n",
                        __FUNCTION__);
                return FALSE;
        }

        // Return that it is EXE
        return TRUE;
}

// Validate file [source file]
int validateFile(const char *src)
{
	FILE *s,*o;
	int l;
	char *tmp;

	// Open source and output
	TRACE("A"); //
	s = fopen(src, "rb");
	if(s==NULL)	return 1;

	// Get size of source
	TRACE("C"); //
	l = fsizeof(src);

	// Perform data validation if needed
	if(l>0)
	{
		TRACE("D"); //
		tmp = malloc(l+1024*4);
		TRACE("E"); //
		fread(tmp, l,1, s);
	}

	// Check for EXE
	if( isExeBinary(tmp, l)==FALSE )
	{
		printf("%s: non-executable file\n", src);
	}
	else
	{
		printf("%s: valid executable file\n", src);
	}

	// Close both streams
	TRACE("G"); //
	fclose(s);

	TRACE("I"); //
	//
	if(l>0)
		free(tmp);

	//
	return 0;
}

// Perform validate operation
int validate_now(const char *src)
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
		if(verbose)	printf("validating %s ", name);
		else
		{
				SwitchToThread();
		}

		// Perform validate operation
		TRACE("4"); //
		if( validateFile(name) )	// source file => output
		{
			printf("\nError: Cannot validate file '%s'\n",
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
		loppu = findnext(fd, &ff);
		//
		if(loppu)break;
		TRACE("7"); //
		// Allow user to break validating with ESC
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
	if(argc<2)
	{
		printf("%s: missing file arguments\n", argv[0]);
	//	printf("Try `%s --help' for more information.\n", argv[0]);
		return 0;
	}

	// Perform operation
	return validate_now(argv[1]);
}

