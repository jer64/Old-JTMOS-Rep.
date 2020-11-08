// =======================================================================
// sqsh.c - Squirrel Shell 1.0 for JTMOS
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
// =======================================================================
#include <stdio.h>
#include "sqsh.h"

// Request 256K of RAM from the init process.
SYSMEMREQ(256)

//
int sqsh_special=0;

//
struct
{
	//
	char *cmdline;
	char path[512];
	char dev[512];

	//
	char **cmdpar;

	//
	char **script;
	int lines_script;
	char *tmp;

	//
	char script_run;
	int line_script;
}sqsh;

// Load script
int loadscript(const char *fname)
{
	FILE *f;
	int i,i2,i3,i4,ch,bytes;

	// Open script file
	f=fopen(fname, "rb");
	if(f==NULL)
	{
		// Open error:
		return 1;
	}

	// Allocate script structure when neccesary
	if(sqsh.script==NULL)
		sqsh.script = malloc(SCRIPT_MAX_LINES*4);
	// Receive whole script
	for(i=0,bytes=0; ; )
	{
		// Allocate one string for the line when neccesary
		if(sqsh.script[i]==NULL)	sqsh.script[i] = malloc(200);

		// Get one line at once
		for(i2=0; i2<195; i2++)
		{
			bytes++;
			ch = fgetc(f);
			if(!ch || ch==EOF)break;
			if(ch==0xA)break;
			sqsh.script[i][i2] = ch;
		}
		sqsh.script[i][i2]=0;

		// Preprocess script line
		if( sqsh_parsecmd(sqsh.script[i], 1) )
		{
			// Parse error
			strcpy(sqsh.script[i], "# This line contained error(preprocessor).");
		}
		else
		{
			// Line was OK.
		}

		// End of script file?
		if(!ch || ch==EOF)break;

		//
		i++;
	}
eoffi:
	// Declare total amount of lines in the script.
	sqsh.lines_script = i;

	// Close script file stream.
	fclose(f);

//	printf("%d bytes processed.\n", bytes);
	// Request script to be ran now.
	sqsh.script_run=1;
	return 0;
}

//
void show_help_page(int argc, char **argv)
{
	printf("Squirrel Shell 1.0 for JTMOS\n");
	printf("    (C) 2002-2003 by Jari Tuominen\n");
	printf("\n");
	printf("Usage: %s [options] [files] [...]\n", argv[0]);
	printf("Options:\n");
	printf("          --help      shows help page(this)\n");
	printf("          --dir       specifies location to run at\n");
	printf("     e.g. --dir /\n");
}

//
void sqsh_allocate(void)
{
	int i;

	//
	sqsh.cmdline = malloc(1024*4);
	sqsh.cmdpar = malloc((SQSH_MAX_WORDS+10)*4);

	//
	for(i=0; i<(SQSH_MAX_WORDS+10); i++)
	{
		sqsh.cmdpar[i] = malloc(200);
	}
}

// Change directory
void sqsh_cd(void)
{
//	printf("cding to '%s'\n", sqsh.cmdpar[1]);
	chdir(sqsh.cmdpar[1]);
}

// Echo on terminal
void sqsh_echo(const char *cmdline)
{
	int i,i2,i3,i4, l;

	//
	l = strlen(cmdline);

	//
	for(i=0; i<l; i++)
	{
		if(cmdline[i]=='\"')
		{
			for(i2=i+1; i2<l; i2++)
			{
				if(cmdline[i2]=='\"')
				{
					printf("\n");
					break;
				}
				printf("%c", cmdline[i2]);
			}
			return;
		}
	}

	//
	printf("Usage: echo \"text inside here\"\n");
	printf("Quote -marks are required.\n");
}

//
void sqsh_list(void)
{
	int i;

	//
	printf("Script contains %d lines.\n", sqsh.lines_script);
	for(i=0; i<sqsh.lines_script; i++)
	{
		if( (i&15)==15 )	if(getch()==27)break;
		printf("Line %d: \"%s\"\n",  i, sqsh.script[i]);
	}
}

//
void sqsh_load(void)
{
}

//
void sqsh_save(void)
{
}

// 0	=	ok
// -1	=	exit request
// -2	=	invalid parameter
// -3	=	nothing to do
//
int sqsh_parsecmd(const char *s, int test)
{
	//
	int pid;

	// Detect bad arguments
	if(s==NULL)return -2;
	if(!strlen(s))return -3;

	// Make cmdpar argument split strings
	sqsh_process_cmdstring(s);

	// Detect remarked lines and skip these
	if( sqsh.cmdpar[0][0]=='#' )
	{
		// One remark line skipped :)
		return 0;
	}

	// List currently loaded script
	if( !strcmp(sqsh.cmdpar[0], "list") )
	{
		if(test==0)	sqsh_list();
		return 0;
	}

	// Load a script
	if( !strcmp(sqsh.cmdpar[0], "load") )
	{
		if(test==0)
		{
			printf("Loading script '%s'\n", sqsh.cmdpar[1]);
			loadscript(sqsh.cmdpar[1]);
		}
		return 0;
	}

	// Save a script
	if( !strcmp(sqsh.cmdpar[0], "save") )
	{
		if(test==0)
		{
			printf("Not implemented yet.\n");
		}
		return 0;
	}

	//
	if( !strcmp(sqsh.cmdpar[0], "cd") )
	{
		if(test==0)
		{
			sqsh_cd();
		}
		return 0;
	}

	//
	if( !strcmp(sqsh.cmdpar[0], "echo") )
	{
		if(test==0)
		{
			sqsh_echo(s);
		}
		return 0;
	}

	//
	if( !strcmp(sqsh.cmdpar[0], "ver")
		||
	    !strcmp(sqsh.cmdpar[0], "version") )
	{
		if(test==0)
		{
			sqsh_about();
		}
		return 0;
	}

	//
	if( !strcmp(sqsh.cmdpar[0], "exit") )
	{
		if(test==0)
			return -1;
			else
			return 0;
	}

	// Empty?
	if( !strcmp(sqsh.cmdpar[0], "")
		||
	    sqsh.cmdpar[0][0]==' ' )
		return -2;

	// Try external command
	if( cexec(s, sqsh.path)==0 )return -2;
	return 0;
}

//
void sqsh_draw_prompt(void)
{
	//
	printf("[root@localhost %s]#", sqsh.path);
}

int sqsh_process_cmdstring(const char *s)
{
	int i;
	char *tmp;

	//
	for (i = 0; i < SQSH_MAX_WORDS; i++)
		strcpy (sqsh.cmdpar[i], "");

	//
	for (i = 0; i < SQSH_MAX_WORDS; i++)
	{
		// Get parameter #$(i)
		sptstr (sqsh.cmdpar[i], s, ' ', i, SQSH_MAX_WORDS-1);
		// Last parameter?
		if( !strlen(sqsh.cmdpar[i]) )
			break;
	}
}

// SQSH shell prompt
void sqsh_prompt(void)
{
	char str[512],str2[512];
	int rv,testbin;

	//
	for(;;)
	{
retryagain:
		//
		SwitchToThread();

		// Get current path
		getcwd(sqsh.path, 250);

		//
		if(sqsh.script_run==2)
		{
			break;
		}

		//
		if(sqsh.script_run==1)
		{
			strcpy(sqsh.cmdline, sqsh.script[sqsh.line_script]);
			sqsh.line_script++;
			if( sqsh.line_script>=sqsh.lines_script )
			{
				// Last line of the script reached
				//sqsh.script_run=2;
				sqsh.script_run=0;
			}
		}
		else
		if(sqsh.script_run==0)
		{
			sqsh_draw_prompt();
			gets(sqsh.cmdline);
		}

		// Sanity check
		if( !strlen(sqsh.cmdline) )goto retryagain;
		testbin=0;
		// Copy the name to the temp buffer
		strcpy(str2, sqsh.cmdline);
tryagain:
		rv = sqsh_parsecmd(str2, 0);
		switch(rv)
		{
			// End of session.
			case -1:
			goto outtahere;
			// File not found
			case -2:
			printf("Bad command or file name '%s'\n",
				str2);
			break;
			//
			default:
			break;
		}
line_processed:
	}
////////////////////////////////////////////////////////////////////////////////
outtahere:
}

// Initialize SQSH
void sqsh_init(void)
{
	//
	strcpy(sqsh.path, "/");

	//
	sqsh_allocate();
}

// Bring up some info about the shell
void sqsh_about(void)
{
	printf(" .    .     _ _   \n");
	printf(" |\\__/|  .~    ~. \n");
	printf(" /  o `./      .' JTMOS Squirrel Shell 1.0 \n");
	printf("{o__,   \\    {    \n");
	printf("  / .  . )    \\   (C) 2002-2003 by Jari Tuominen\n");
	printf("  `-` '-' \\    }  jari@vunet.org\n");
	printf(" .(   _(   )_.'  \n");
	printf("'---.~_ _ _|     \n");
}

// Start shell process itself
void sqsh_start(void)
{
	// Start prompt
	sqsh_prompt();
}

// Check sqsh start arguments for various options
int checkArgus(int argc, char **argv)
{
	if(argc>=2)
	{
		if(!strcmp(argv[1],"--dir"))
		{
			chdir(argv[2]);
			return -1;
		}

		if(!strcmp(argv[1],"--help"))
		{
			show_help_page(argc, argv);
			return 1;
		}
	}
	return 0;
}

// This is done in boot up sequence
void bootseq(void)
{
	if( !loadscript("/etc/boot.sh") )return;
	if( !loadscript("/bin/boot.sh") )return;
	if( !loadscript("/usr/etc/boot.sh") )return;
	if( !loadscript("/boot.sh") )return;
	printf("Cannot find boot.sh -automation script, entering manual start up ...\n");
}

//
int main(int argc, char **argv)
{
	//
	memset(&sqsh, 0, sizeof(sqsh));

	// Initialize
	printf("\rSquirrel Shell is now initialising ...                   \r");
	sqsh_init();
	printf("\r                                                         \r");

	//
	sqsh.script_run=0;
	sqsh.lines_script=0;

	//
	if( (sqsh_special=checkArgus(argc,argv))==1 )
	{
		// Shell quits
		return 0;
	}

	// Handle sqsh_special==-1 here
	// -1 = load start up sequence
	if(sqsh_special==-1)bootseq();

	// Load script if one defined
	if(argc>=2 && strstr(argv[1], "-")==NULL)
		loadscript(argv[1]);

	// Shell starts
	sqsh_start();

	//
	return 0;
}

