// ----------------------------------------------------------------
// top.c - display top CPU processes.
// (C) 2002-2005 by Jari Tuominen.
// 07/2003: Added UNIPID support.
// ----------------------------------------------------------------
#include <stdio.h>
#include <jtmos/filefind.h>

//
typedef struct
{
	int pid,spending;
}PR;
struct
{
	int count;
	PR *pr[256];
	PR *p[256];
}sorted;

//
int currentSpending[256];
int newSpending[256];
int oldSpending[256];

// Init top
void do_init(void)
{
	int i;

	//
	memset(currentSpending,0,256*4);
	memset(newSpending,0,256*4);
	memset(oldSpending,0,256*4);

	//
	memset(&sorted, 0, sizeof(sorted));

	//
	for(i=0; i<256; i++)
	{
		sorted.pr[i] = malloc(sizeof(PR));
		memset(sorted.pr[i], 0, sizeof(PR));
	}
}

// Get CPU load values for top.
void do_update(void)
{
	int i,i2;

	// Store old values
	for(i=0; i<GetThreadCount(); i++)
	{
		oldSpending[i] = newSpending[i];
	}

	// Get new values
	for(i=0; i<GetThreadCount(); i++)
	{
		newSpending[i] = GetThreadSpending(i);
	}

	// Get comparison values
	for(i=0; i<GetThreadCount(); i++)
	{
		currentSpending[i] = newSpending[i]-oldSpending[i];
	}
}

// Sort out a top list
void do_sort(void)
{
	int i,i2,i3,i4,a1;
	char *p1,*p2;

	//
	sorted.count = GetThreadCount();
	for(i=0; i<sorted.count; i++)
	{
		sorted.pr[i]->pid = i;
		sorted.pr[i]->spending = currentSpending[i];
		sorted.p[i] = sorted.pr[i];
	}

	// Sort
	for(a1=0; a1<sorted.count; a1++)
	{
		for(i=0; i<sorted.count; i++)
		{
			if(  sorted.p[i+1]->spending > sorted.p[i]->spending
				&&
			     sorted.p[i+1]->spending != sorted.p[i]->spending  )	
			{
				// Swap
				p1 = sorted.p[i+1];
				p2 = sorted.p[i+0];
				sorted.p[i+0] = p1;
				sorted.p[i+1] = p2;
			}
		}
	}
}

//
void do_list(void)
{
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

	//
	for(i2=0; i2<sorted.count && i2<20; i2++)
	{
		//
		i = sorted.p[i2]->pid;
		unipid = GetThreadUniquePID(i);

		// Non-existing task? => Skip
		if(!GetThreadPriority(unipid))
			continue;

		//
		sprintf(str, "%d", unipid); StretchPrint(str, 10);
		sprintf(str, "%d", sorted.p[i2]->spending); StretchPrint(str, 10);
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
	}
}

// top - display top CPU processes
//
void do_top(void)
{
	int i,i2;

	//
	while(1)
	{
		//
		clrscr();
		printf("(A)");
		do_update();
		printf("(B)");
		do_sort();
		printf("(C)");
		do_list();
		printf("(D)");
		sleep(2);
		printf("(E)");
		if(getch1())break;
	}
}

// Parse arguments
int do_args(int argc, char **argv)
{
	int i,i2,i3,i4;

	//
	for(i=1; i<(argc+1); i++)
	{
	}

	//
	return 0;
}

//
void do_help(void)
{
	//
	
}

int main(int argc, char **argv)
{
	int opt;

	//
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
	do_init();
	do_top();
	return 0;
}

