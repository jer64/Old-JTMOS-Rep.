//
#include <stdio.h>
#define MAGIC 346346

#ifdef linux
//
int getch1(void)
{
	return 0;
}

//
void SwitchToThread(void)
{
	usleep(1);
}
#endif

//
int main(int argc, char **argv)
{
	float fa,fb,fc,fd,arvo;

	//
	printf("Now occupying floating point unit(FPU) with somethings.\n");
	printf("Press any key to cease using FPU and exit . . ");

	// Loop until a key is hit.
	for(arvo=MAGIC;getch1()<=0;)
	{
		// Do math things.
		fa = 43737535.4577;
		fb = 5745.5477;
		fc = 5474575.4363;
		fd = fa*fb/fc;
		fd = fa*fb/fc;
		fd = fa*fb/fc;
		fd = fa/fb/fc;
		fd = fa*fb/fc;
		fd = fa*fb/fc;
		fd = fa/fb/fc;

		// Spend some CPU time to system.
		SwitchToThread();

		//
		if(arvo!=MAGIC)
		{
			printf("\nApparent system FPU failure, scheduler not restoring FPU registers properly?\n");
			printf("MAGIC=%f, arvo=%f.\n",
				MAGIC, arvo);
			break;
		}
	}

	//
	printf("\n");
}

//



