//-------------------------------------------------------------------------------------
// performance.c - a system performance test program
//
#include <stdio.h>

//-------------------------------------------------------------------------------------
//
#define FAILURE if(1) { printf("Program failure in function %s, line %d, file %s",\
		__FUNCTION__, __LINE__, __FILE__);\
	abort();\
	}

//-------------------------------------------------------------------------------------
//
typedef struct
{
	//
	int test;
}RESULTS;
RESULTS results;

//-------------------------------------------------------------------------------------
//
void WaitChange(void)
{
	int t;

	//
	for(t=GetSeconds(); t!=GetSeconds(); )
		SwitchToThread();
}

//-------------------------------------------------------------------------------------
// test1:
// here we are going to measure how many times per a second it is possible to
// open and close a file.
//
void test1(RESULTS *r)
{
	FILE *f;
	int i,t;

	//------------------------------------------------------------------
	// Create a test file
	f = fopen("performance.tmp", "wb");
	if(f==NULL)
		FAILURE;
	for(i=0; i<65536; i++)
		fputc(0, f);
	fclose(f);

	//------------------------------------------------------------------
	// Wait until current second runs over
	//
	WaitChange();

	//------------------------------------------------------------------
	//
	printf("Analyzing performance PASS I ");

	// Run test for 10 seconds
	for(i=0, t = GetSeconds(); (GetSeconds()-t)<10; i++)
	{
		//
		f=fopen("performance.tmp", "rb");
		fclose(f);
	}

	//
	printf("\n");
	printf("File access performance = %d open/close actions per second.\n",
		i/10);

	//
	remove("performance.tmp");
}

//-------------------------------------------------------------------------------------
// test1:
// this is a test about how fast we can write data on the drive.
//
void test2(RESULTS *r)
{
	FILE *f;
	int i,i2,t,count,lim;

	//------------------------------------------------------------------
	// Wait until current second runs over
	//
	WaitChange();

	//------------------------------------------------------------------
	// Create a test file
	//
	printf("Analyzing performance PASS II ");
	lim = 1024*512;
	for(i=0,count=0,f=NULL,t=GetSeconds(); (GetSeconds()-t)<10; )
	{
		// Write a single block of data
		for(i2=0; i2<1024; i2++,i++,count++)
		{
repeat:
			//
			if(f==NULL)
			{
				f = fopen("performance.tmp", "wb");
				if(f==NULL)
					FAILURE;
			}

			// Loop at 512K boundary, so we won't
			// spend too much drive space.
			if(i>lim)
			{
				i=0;
				if(f!=NULL)	fclose(f);
				f=NULL;
				goto repeat;
			}

			//
			fputc(0, f);
		}

		//
		printf(".");
	}
	printf("\n");

	//
	printf("Total %d(%dK) bytes written durring 10 seconds of testing.\n",
		count, count/1024);
	printf("File write speed = %d(%dK) bytes per second.\n",
		count/10, (count/10) / 1024);

	//
	remove("performance.tmp");
}

//-------------------------------------------------------------------------------------
//
int main(int argc, char **argv)
{
	//
	test1(&results);
	test2(&results);
}


