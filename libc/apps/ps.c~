// ----------------------------------------------------------------
// ps.c - list running processes
// (C) 2002 by Jari Tuominen
// ----------------------------------------------------------------
#include <stdio.h>
#include <jtmos/filefind.h>

// ps - report process status
//
void do_ps(void)
{
	//
	int i,i2,i3,i4,mem,unipid;
	char str[255],str2[255],name[255];

	//
	printf("Process list\n");
	//
	StretchPrint("PID       ",	10);
	StretchPrint("CPU spend ",	10);
	StretchPrint("Priority  ",	10);
	StretchPrint("Process name        ",	20);
	StretchPrint("Memory usage        ",	20);
	printf("\n");

	// Go through all threads in order
	for(i=0; i<GetThreadCount(); i++)
	{
		// Get unique PID
		unipid = GetThreadUniquePID(i);

		// Non-existing task? => Skip
		if(!GetThreadPriority(unipid))
			continue;

		//
		sprintf(str, "%d", unipid); StretchPrint(str, 10);
		sprintf(str, "%d", GetThreadSpending(unipid)); StretchPrint(str, 10);
		sprintf(str, "%d", GetThreadPriority(unipid)); StretchPrint(str, 10);
		GetThreadName(unipid, name);
		if(strlen(name))
		{
			sprintf(str, "%s", name);
		}
		else
		{
			strcpy(str, "[none]");
		}
		StretchPrint(str, 20);

		//
		if( (mem=GetThreadMemoryUsage(unipid)) )
		{
			sprintf(str, "%dK application", mem/1024);
		}
		else
		{
			sprintf(str, "kernel");
		}
		StretchPrint(str, 20);


		//
		printf("\n");

		//
	}
}

// Parse arguments
int do_args(int argc, char **argv)
{
	//
	int i,i2,i3,i4;

	//
	for(i=1; i<(argc+1); i++)
	{
	}

	return 0;
}

void do_help(void)
{
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

	//
	do_ps();
	return 0;
}

